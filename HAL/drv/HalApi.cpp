#include "HalApi.h"
#include "DanOS.h"
#include "CThread.h"
#include "Gpio.h"
#include "Fpga.h"
#include "Immr.h"
#include "Spi.h"
#include "I2c.h"
#include "SpiFlash.h"
#include "Si5368.h"
#include "Pcie.h"
#include "Digi.h"
#include "M21036.h"
#include "MsaSfp.h"
#include "Mdio.h"
#include "MsaCfp.h"
#include "Eeprom.h"
#include "Max1668.h"
#include "PCA9535.h"
#include "ADT7470.h"
#include "Mvl88E6321.h"
#include "AlarmId.h"
#include "Alarm.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>

int isTranspondMode = 0;
static uint32_t eqpt_thread_count = 0;
static uint32_t fan_thread_count = 0;
static bool eqpt_running_flag = true;

int hal_to_digi_port(int id)
{
    if (isTranspondMode && id == 1)
    {
        return 12;
    }
    else if (isTranspondMode && id == 2)
    {
        return 11;
    }
    else if (id >= 1 && id <= 11)
    {
        return id;
    }
    return -1;
}

int hal_to_cfp_inst(int id)
{
    if (isTranspondMode && id == 1)
    {
        return 1;
    }
    else if (isTranspondMode && id == 2)
    {
        return 0;
    }
    else if (id == 11)
    {
        return 0;
    }
    return -1;
}

int hal_to_sfp_inst(int id)
{
    if (!isTranspondMode && id >= 1 && id <= 10)
    {
        return (id - 1);
    }
    return -1;
}

int digi_to_hal_port(int id)
{
    if (isTranspondMode && id == 12)
    {
        return 1;
    }
    else if (isTranspondMode && id == 11)
    {
        return 2;
    }
    else if (id >= 1 && id <= 11)
    {
        return id;
    }
    return -1;
}

void digi_callback(int id, AlarmId::Enum alm, bool active)
{
    int hal_id = digi_to_hal_port(id);
    
    if (alm >= AlarmId::ETH_LOS && alm <= AlarmId::ETH_RF)
    {
        HAL::GetInst().SendEthEvent(hal_id, alm, active);
    }
    else if (alm >= AlarmId::OTU_LOS && alm <= AlarmId::OTU_BERSD)
    {
        HAL::GetInst().SendOtuEvent(hal_id, alm, active);
    }
    else if (alm >= AlarmId::ODU_BERSD && alm <= AlarmId::OPU_CSF)
    {
        HAL::GetInst().SendOduEvent(hal_id, alm, active);
    }
    else if (alm >= AlarmId::STM_LOS && alm <= AlarmId::STM_RFI_L)
    {
        HAL::GetInst().SendStmEvent(hal_id, alm, active);
    }
}

bool MgmtGetLinkState()
{
    const char *dev = "/sys/class/net/eth0/carrier";
    char state;
    bool link = false;

    FILE *fp = fopen(dev, "r");
    if (fp == NULL) return false;

    if (1 == fread(&state, 1, 1, fp))
    {
        if (state == '1')
        {
            link = true;
        }
    }

    fclose(fp);
    return link;
}

void* EqptPoll(void *param)
{
		if(eqpt_running_flag == false)
		{
			sleep(1);
			return NULL;
		}
    if (isTranspondMode)
    {
        MsaCfp::GetInst(1).StateMachine();
    }
    MsaCfp::GetInst(0).StateMachine();

    if (!isTranspondMode)
    {
        for (int i = 0; i < 10; i++)
        {
            MsaSfp::GetInst(i).StateMachine();
        }
    }

    for (int i = 10; i < 12; i++)
    {
        MsaSfp::GetInst(i).StateMachine();
    }

    static bool FanFault = false;
    bool fan_fault = Adt7470GetFault();
    if (fan_fault != FanFault)
    {
        FanFault = fan_fault;
        HAL::GetInst().SendFanEvent(1, AlarmId::FAN_FAULT, fan_fault);
    }

    static int PwrFault[2] = {0, 0};
    int pwr_fault[2];
    FpgaGetPwrFault(&pwr_fault[0], &pwr_fault[1]);
    for (int i = 0; i < 2; i++)
    {
        if (PwrFault[i] != pwr_fault[i])
        {
            PwrFault[i] = pwr_fault[i];
            HAL::GetInst().SendPsuEvent(1 + i, AlarmId::PWR_FAULT, PwrFault[i] == 1);
        }
    }

    static bool MgmtLinkup = false;
    bool mgmt_link = MgmtGetLinkState();
    if (mgmt_link != MgmtLinkup)
    {
        APP_LOG("Mgmt Link %s", mgmt_link ? "Up" : "Down");
        int led = mgmt_link ? 0 : 1;
        MgmtLinkup = mgmt_link;
        Pca9535SetPin(1, 12, led);
        Pca9535SetPin(1, 13, led);
    }

    unsigned int cr, mj, mn;
    static unsigned int pcr = 0, pmj = 0, pmn = 0;
    ActiveAlarm::GetInst().GetAlarmCount(&cr, &mj, &mn);
    if (pcr != cr || pmj != mj || pmn != mn)
    {
        int led_mj = (cr + mj) > 0 ? 0 : 1;
        int led_mn = (mn) > 0 ? 0 : 1;
        Pca9535SetPin(2, 11, led_mj);
        Pca9535SetPin(2, 12, led_mn);
        pcr = cr;
        pmj = mj;
        pmn = mn;
    }
    
    sleep(1);

    eqpt_thread_count++;

    return NULL;
}

const int max_speed = 255;
const int min_speed = 100;
static bool full_speed = false;
static int target_speed = max_speed;
static int actual_speed = max_speed;
static int last_framer_loss = 0;
static bool fan_debug = false;
static int fan_interval = 5;
static int fan_target = 90;
static float fan_kp = 10;
static float fan_ki = 0.2;
void* FanCtrl(void *param)
{
    int framer, framer_loss = 0, offset = 0;
    
    // full speed condition
    if (full_speed == true)
    {
        target_speed = max_speed;
        goto fan_set;
    }

    if (Max1668GetTemp(0x02) > 45)  // air_in
    {
        target_speed = max_speed;
        goto fan_set;
    }

    framer = Max1668GetTemp(0x04);
    if (framer > 105)
    {
        target_speed = max_speed;
        goto fan_set;
    }
    
    if (MsaCfp::GetInst(0).GetPresent())
    {
        if (MsaCfp::GetInst(0).GetTemperature() > 70.0)
        {
            target_speed = max_speed;
            goto fan_set;
        }
    }

    if (MsaCfp::GetInst(1).GetPresent())
    {
        if (MsaCfp::GetInst(1).GetTemperature() > 70.0)
        {
            target_speed = max_speed;
            goto fan_set;
        }
    }

    if (!isTranspondMode)
    {
        for (int i = 0; i < 10; i++)
        {
            if (MsaSfp::GetInst(i).GetPresent())
            {
                if (MsaSfp::GetInst(i).GetTemperature() > 58.0) // CNMP requires SFP+ under 60 degree
                {
                    target_speed = max_speed;
                    goto fan_set;
                }
            }
        }
    }

    // auto fan control
    framer_loss = framer - fan_target;
    offset = (int)(fan_kp * (framer_loss - last_framer_loss) + fan_ki * framer_loss);
    last_framer_loss = framer_loss;
    target_speed = actual_speed + offset;
    
    if (target_speed > max_speed) target_speed = max_speed;
    if (target_speed < min_speed) target_speed = min_speed;

    if (fan_debug) APP_LOG("FanSpeed: %d, loss: %d, offset: %d\n", target_speed, framer_loss, offset);

fan_set:
    if (actual_speed != target_speed)
    {
        //APP_LOG("FanSpeed: %d, loss: %d, offset: %d\n", target_speed, framer_loss, offset);
        actual_speed = target_speed;
        Adt7470SetSpeed(actual_speed);
    }
    
    sleep(fan_interval);

    fan_thread_count++;

    return NULL;
}

bool HalApi::InitCold()
{
    ImmrInit();
    GpioInit();
    SpiInit();
    I2cInit();
    
    SetLampTest(true);
    
    if (IsFileExist("noinit"))
    {
        return true;
    }

    Adt7470SetSpeed(255);   // force fan full speed

    int retry = 5;
    bool fpgaAlive = false;
    FpgaInit();
    fpgaAlive = FpgaIsAlive();
    while (fpgaAlive == false && retry-- > 0)
    {
        APP_LOG("FPGA init retry = %d", retry);
        GpioSetPin(FPGA_PROGRAM, 0);
        sleep(1);
        GpioSetPin(FPGA_PROGRAM, 1);
        sleep(10);
        FpgaInit();
        fpgaAlive = FpgaIsAlive();
    }
    if (fpgaAlive == false)
    {
        APP_LOG("FPGA init failed");
        return false;
    }

    Si5368Init();
    PcieInit();

    if ((GpioGetAddress() & 0x01000100) != 0 ||
        MsaCfp::GetInst(1).GetPresent() == true)
    {
        isTranspondMode = 1;
    }

    Digi::GetInst().InitCold();
    M21036Init();
    Mvl88E6321Init();

    MsaCfp::GetInst(0);

    // resync eth0 port
    system("ethtool -s eth0 autoneg off");
    system("ethtool -s eth0 autoneg on");

    SetLampTest(false);
    Pca9535SetPin(2, 10, 0);
    Pca9535SetPin(2, 11, 1);
    Pca9535SetPin(2, 12, 1);

    Digi::GetInst().RegCallback(digi_callback);
    new CThread("eqpt", EqptPoll, NULL);
    new CThread("fan", FanCtrl, NULL);

    return true;
}

bool HalApi::HwMenu(int argc, char *argv[])
{
    bool help = false;

    if (argc >= 2)
    {
        if (strcmp(argv[1], "gpio") == 0)
        {
            GpioMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "fpga") == 0)
        {
            FpgaMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "immr") == 0)
        {
            ImmrMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "spi") == 0)
        {
            SpiMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "i2c") == 0)
        {
            I2cMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "spiflash") == 0)
        {
            SpiFlashMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "si5368") == 0)
        {
            Si5368Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "pcie") == 0)
        {
            PcieMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "digi") == 0)
        {
            Digi::GetInst().Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "m21036") == 0)
        {
            M21036Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "sfp") == 0 && argc >= 3)
        {
            int inst = atoi(argv[2]);
            if (inst >= 0 && inst <= 11)
            {
                MsaSfp::GetInst(inst).Menu(argc - 3, argv + 3);
            }
        }
        else if (strcmp(argv[1], "mdio") == 0)
        {
            MdioMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "cfp") == 0 && argc >= 3)
        {
            int inst = atoi(argv[2]);
            if (inst == 0 || inst == 1)
            {
                MsaCfp::GetInst(inst).Menu(argc - 3, argv + 3);
            }
        }
        else if (strcmp(argv[1], "eeprom") == 0)
        {
            EepromMenu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "max1668") == 0)
        {
            Max1668Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "pca9535") == 0)
        {
            Pca9535Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "adt7470") == 0)
        {
            Adt7470Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "88e6321") == 0)
        {
            Mvl88E6321Menu(argc - 2, argv + 2);
        }
        else if (strcmp(argv[1], "thread") == 0)
        {
            APP_LOG("Thread Count:");
            APP_LOG("\tEQPT = %u", eqpt_thread_count);
            APP_LOG("\t FAN = %u", fan_thread_count);
        }
        else if (strcmp(argv[1], "eqpt") == 0)
        {
            if (argc == 3)
            {
            	if(atoi(argv[2]) == 0)
        		{
        			eqpt_running_flag = false;
        		}
        		else
        		{
        		    eqpt_running_flag = true;
        		}
        		APP_LOG("eqpt_running_flag set to %d", eqpt_running_flag);
            }
            else
            {
                APP_LOG("eqpt_running_flag is %d", eqpt_running_flag);
            }
        }
        else if (strcmp(argv[1], "fan") == 0)
        {
            if (argc == 6)
            {
                fan_interval = atoi(argv[2]);
                fan_target = atoi(argv[3]);
                fan_kp = atof(argv[4]);
                fan_ki = atof(argv[5]);
            }
            else
            {
                fan_debug = true;
                APP_LOG("FanInv: %d, Target: %d, FanKp: %f, FanKi: %f", fan_interval, fan_target, fan_kp, fan_ki);
            }
        }
        else
        {
            help = true;
        }
    }
    else
    {
        help = true;
    }

    if (help)
    {
        APP_LOG("hw");
        APP_LOG("\t immr");
        APP_LOG("\t gpio");
        APP_LOG("\t spi");
        APP_LOG("\t i2c");
        APP_LOG("\t pcie");
        APP_LOG("\t spiflash");
        APP_LOG("\t si5368");
        APP_LOG("\t fpga");
        APP_LOG("\t digi");
        APP_LOG("\t m21036");
        APP_LOG("\t mdio");
        APP_LOG("\t sfp <0-11>");
        APP_LOG("\t cfp <0-1>");
        APP_LOG("\t eeprom");
        APP_LOG("\t max1668");
        APP_LOG("\t pca9535");
        APP_LOG("\t adt7470");
        APP_LOG("\t 88e6321");
        APP_LOG("\t thread");
        APP_LOG("\t eqpt 0/1");
        APP_LOG("\t fan [i:inv] [i:t] [f:kp] [f:ki]");
    }
    return true;
}

// System

typedef struct
{
    const char      *name;
    LedType::Enum   type;
    int             dev;
    int             pin;
    int             val;        // 0 - on, 1 - off
} LedMapEntry;

LedMapEntry LedMapTable[] = 
{
    {"port-1",   LedType::TX,    0,  11, 1},
    {"port-1",   LedType::RX,    0,  10, 1},
    {"port-2",   LedType::TX,    0,  0,  1},
    {"port-2",   LedType::RX,    0,  1,  1},
    {"port-3",   LedType::TX,    0,  13, 1},
    {"port-3",   LedType::RX,    0,  12, 1},
    {"port-4",   LedType::TX,    0,  2,  1},
    {"port-4",   LedType::RX,    0,  3,  1},
    {"port-5",   LedType::TX,    0,  15, 1},
    {"port-5",   LedType::RX,    0,  14, 1},
    {"port-6",   LedType::TX,    0,  4,  1},
    {"port-6",   LedType::RX,    0,  5,  1},
    {"port-7",   LedType::TX,    0,  17, 1},
    {"port-7",   LedType::RX,    0,  16, 1},
    {"port-8",   LedType::TX,    0,  6,  1},
    {"port-8",   LedType::RX,    0,  7,  1},

    {"port-9",   LedType::TX,    1,  3,  1},
    {"port-9",   LedType::RX,    1,  2,  1},
    {"port-10",  LedType::TX,    1,  0,  1},
    {"port-10",  LedType::RX,    1,  1,  1},
    {"osc-1",    LedType::TX,    1,  4,  1}, 
    {"osc-1",    LedType::RX,    1,  6,  1},
    {"osc-2",    LedType::TX,    1,  7,  1}, 
    {"osc-2",    LedType::RX,    1,  5,  1},
        
    {"test-1",   LedType::TX,    1, 10,  1},    // ?
    {"test-2",   LedType::TX,    1, 11,  1},
    {"mgmt-1",   LedType::LNK,   1, 12,  1},    // LAN yellow
    {"mgmt-1",   LedType::ACT,   1, 13,  1},    // LAN green
    {"test-5",   LedType::TX,    1, 14,  1},
    {"test-6",   LedType::TX,    1, 15,  1},
    {"test-7",   LedType::TX,    1, 16,  1},
    {"test-8",   LedType::TX,    1, 17,  1},    // ?
        
    {"port-11",  LedType::TX,    2,  0,  1},    // Line CFP
    {"port-11",  LedType::RX,    2,  1,  1},
    {"port-12",  LedType::TX,    2,  2,  1},    // Client CFP
    {"port-12",  LedType::RX,    2,  3,  1},

    {"test-1",   LedType::TX,    2,  4,  1},
    {"test-1",   LedType::TX,    2,  5,  1},
    {"test-1",   LedType::TX,    2,  6,  1},
    {"test-1",   LedType::TX,    2,  7,  1},
        
    {"card-1",   LedType::PWR,   2,  10, 1},
    {"card-1",   LedType::MAJ,   2,  11, 1},
    {"card-1",   LedType::MIN,   2,  12, 1},
        
    {"test-11",  LedType::TX,    2,  13, 1},
    {"test-12",  LedType::TX,    2,  14, 1},
    {"test-13",  LedType::TX,    2,  15, 1},
    {"test-14",  LedType::TX,    2,  16, 1},
    {"test-15",  LedType::TX,    2,  17, 1},    
};
size_t LedMapCount = sizeof(LedMapTable) / sizeof(LedMapEntry);

static int lamp_test_val = 1;   // default disabled

bool HalApi::SetLampTest(bool enable)
{
    int val = enable ? 0 : 1;
    
    if (val == lamp_test_val) return true;

    lamp_test_val = val;

    for (size_t i = 0; i < LedMapCount; i++)
    {
        if (lamp_test_val == 0)
        {
            Pca9535SetPin(LedMapTable[i].dev, LedMapTable[i].pin, 0);
        }
        else
        {
            Pca9535SetPin(LedMapTable[i].dev, LedMapTable[i].pin, LedMapTable[i].val);
        }
    }
    
#if 0    
    Pca9535SetPin(0, 0, val);
    Pca9535SetPin(0, 1, val);
    Pca9535SetPin(0, 2, val);
    Pca9535SetPin(0, 3, val);
    Pca9535SetPin(0, 4, val);
    Pca9535SetPin(0, 5, val);
    Pca9535SetPin(0, 6, val);
    Pca9535SetPin(0, 7, val);

    Pca9535SetPin(0, 10, val);
    Pca9535SetPin(0, 11, val);
    Pca9535SetPin(0, 12, val);
    Pca9535SetPin(0, 13, val);
    Pca9535SetPin(0, 14, val);
    Pca9535SetPin(0, 15, val);
    Pca9535SetPin(0, 16, val);
    Pca9535SetPin(0, 17, val);

    Pca9535SetPin(1, 0, val);
    Pca9535SetPin(1, 1, val);
    Pca9535SetPin(1, 2, val);
    Pca9535SetPin(1, 3, val);
    Pca9535SetPin(1, 4, val);
    Pca9535SetPin(1, 5, val);
    Pca9535SetPin(1, 6, val);
    Pca9535SetPin(1, 7, val);

    Pca9535SetPin(1, 10, val);
    Pca9535SetPin(1, 11, val);
    Pca9535SetPin(1, 12, val);
    Pca9535SetPin(1, 13, val);
    Pca9535SetPin(1, 14, val);
    Pca9535SetPin(1, 15, val);
    Pca9535SetPin(1, 16, val);
    Pca9535SetPin(1, 17, val);

    Pca9535SetPin(2, 0, val);
    Pca9535SetPin(2, 1, val);
    Pca9535SetPin(2, 2, val);
    Pca9535SetPin(2, 3, val);
    Pca9535SetPin(2, 4, val);
    Pca9535SetPin(2, 5, val);
    Pca9535SetPin(2, 6, val);
    Pca9535SetPin(2, 7, val);

    Pca9535SetPin(2, 10, val);
    Pca9535SetPin(2, 11, val);
    Pca9535SetPin(2, 12, val);
    Pca9535SetPin(2, 13, val);
    Pca9535SetPin(2, 14, val);
    Pca9535SetPin(2, 15, val);
    Pca9535SetPin(2, 16, val);
    Pca9535SetPin(2, 17, val);
#endif

    return true;
}

bool HalApi::SetLedStatus(Key &key, LedType::Enum type, LedState::Enum state, LedColor::Enum /*color*/)
{
    int val = (state == LedState::ON) ? 0 : 1;
    std::string name = key.name();

    if (lamp_test_val == 0) return false;

    if (isTranspondMode)
    {
        if (name == "port-1")
        {
            name = "port-12";
        }
        else if (name == "port-2")
        {
            name = "port-11";
        }
    }

    for (size_t i = 0; i < LedMapCount; i++)
    {
        if (LedMapTable[i].name == name && LedMapTable[i].type == type)
        {
            Pca9535SetPin(LedMapTable[i].dev, LedMapTable[i].pin, val);
            LedMapTable[i].val = val;
            return true;
        }
    }

    APP_LOG("Led Entry %s not found !!!", name.c_str());
    
    return false;
}

static void SetMacAddress(const char *intf, const char *ip)
{
    char cmdline[128];
    std::string imac = EepromGetTag('A');

    APP_LOG("set mac address %s %s %s", intf, ip, imac.c_str());

    if (strlen(intf) < 4 || strlen(ip) < 7) return;

    sprintf(cmdline, "ifconfig %s down", intf);
    system(cmdline);

    if (imac.size() == 17)
    {
        char jmac[18] = {0};
        strncpy(jmac, imac.c_str(), 17);
        if (intf[3] == '1') jmac[16]++;
        sprintf(cmdline, "ifconfig %s hw ether %s", intf, jmac);
    }
    else
    {
        int mac[4];
        sscanf(ip, "%d.%d.%d.%d", &mac[0], &mac[1], &mac[2], &mac[3]);
        sprintf(cmdline, "ifconfig %s hw ether 00:00:%02X:%02X:%02X:%02X", intf, mac[0], mac[1], mac[2], mac[3]);
    }
    system(cmdline);
    APP_LOG(cmdline);
}

// Network
bool HalApi::SetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    char cmdline[128];
    
    //sprintf(cmdline, "route del default");
    //APP_LOG(cmdline);
    //system(cmdline);
    
    SetMacAddress("eth0", ip.c_str());
    sprintf(cmdline, "ifconfig eth0 %s netmask %s up", ip.c_str(), mask.c_str());
    APP_LOG(cmdline);
    system(cmdline);
    
    if (gw.size() >= 7)
    {
        sprintf(cmdline, "route add default gw %s", gw.c_str());
        APP_LOG(cmdline);
        system(cmdline);

        sprintf(cmdline, "ping -c 1 %s > /dev/null &", gw.c_str());
        APP_LOG(cmdline);
        system(cmdline);
    }

    //sprintf(cmdline, "echo \"eth0,%s,%s,%s,end\" > /mnt/appfs/ifcfg.eth0; sync;", ip.c_str(), mask.c_str(), gw.c_str());
    //APP_LOG(cmdline);
    //system(cmdline);

    return true;
}

bool HalApi::GetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    return true;
}

bool HalApi::SetOscIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    char cmdline[128];
    
    //sprintf(cmdline, "route del default");
    //APP_LOG(cmdline);
    //system(cmdline);
    
    SetMacAddress("eth1", ip.c_str());
    sprintf(cmdline, "ifconfig eth1 %s netmask %s up", ip.c_str(), mask.c_str());
    APP_LOG(cmdline);
    system(cmdline);
    system("echo 1 > /proc/sys/net/ipv4/ip_forward");
    Mvl88E6321Init();

    if (gw.size() >= 7)
    {
        sprintf(cmdline, "route add default gw %s", gw.c_str());
        APP_LOG(cmdline);
        system(cmdline);

        sprintf(cmdline, "ping -c 1 %s > /dev/null &", gw.c_str());
        APP_LOG(cmdline);
        system(cmdline);
    }

    //sprintf(cmdline, "echo \"eth0,%s,%s,%s,end\" > /mnt/appfs/ifcfg.eth0; sync;", ip.c_str(), mask.c_str(), gw.c_str());
    //APP_LOG(cmdline);
    //system(cmdline);

    return true;
}

bool HalApi::GetOscIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    return true;
}

// Shelf
bool HalApi::GetShelfInletTemperature(int &degree)
{
    degree = Max1668GetTemp(0x02);
    return true;
}

bool HalApi::GetShelfOutletTemperature(int &degree)
{
    degree = Max1668GetTemp(0x01);
    return true;
}

// Card
bool HalApi::GetCardTemperature(int &degree)
{
    degree = Max1668GetTemp(0x00);
    return true;
}

bool HalApi::GetCardResetButton()
{
    return (GpioGetPin(RESET_BUTTON) == 0) ? true : false;
}

// Fan
bool HalApi::SetFanSpeed(int speed)
{
    full_speed = (speed == 0) ? false : true;
    if (full_speed) Adt7470SetSpeed(255);
    return true;
}

bool HalApi::GetFanSpeed(int &speed)
{
    speed = actual_speed * 100 / 255;
    return true;
}

// Optics
bool HalApi::GetPortOpticsInfo(int id, OpticsInfo_s &info)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                info.type = "CFP";
                info.laser = cfp.GetLaserOn();
                info.los = cfp.GetLos();
                info.txPower = cfp.GetTxPower();
                info.rxPower = cfp.GetRxPower();
                info.temp = cfp.GetTemperature();
            }
        }
    }
    else
    {
        int sfp_inst = hal_to_sfp_inst(id);
        if (sfp_inst >= 0)
        {
            MsaSfp &sfp = MsaSfp::GetInst(sfp_inst);
            if (sfp.GetPresent())
            {
                info.type = "SFP+";
                info.laser = sfp.GetLaserOn();
                info.los = sfp.GetLos();
                info.txPower = sfp.GetTxPower();
                info.rxPower = sfp.GetRxPower();
                info.temp = sfp.GetTemperature();
            }
        }
    }
    return true;
}

bool HalApi::GetOscOpticsInfo(int id, OpticsInfo_s &info)
{
    if (id == 1 || id == 2) id += 9;
    
    MsaSfp &sfp = MsaSfp::GetInst(id);
    if (sfp.GetPresent() && !sfp.GetEthTransceiver())
    {
        info.type = "SFP";
        info.laser = sfp.GetLaserOn();
        info.los = sfp.GetLos();
        info.txPower = sfp.GetTxPower();
        info.rxPower = sfp.GetRxPower();
        info.temp = sfp.GetTemperature();
    }
    
    return true;
}

bool HalApi::GetPortNum(int &num)
{
    num = isTranspondMode ? 2 : 11;
    return true;
}

bool HalApi::SetPortMode(int id, PortMode::Enum mode)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().SetSignalType(digi_port, mode);
    }
    
    int cfp_inst = hal_to_cfp_inst(id);
    if (cfp_inst >= 0)
    {
        MsaCfp::GetInst(cfp_inst).SetSignalType(mode);
    }
    
    return true;
}

bool HalApi::GetPortMode(int id, PortMode::Enum &mode)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        mode = Digi::GetInst().GetSignalType(digi_port);
    }
    
    return true;
}

bool HalApi::SetLaserEnable(int id, bool enable)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp::GetInst(cfp_inst).SetLaserOn(enable);
        }
    }
    else
    {
        int sfp_inst = hal_to_sfp_inst(id);
        if (sfp_inst >= 0)
        {
            MsaSfp::GetInst(sfp_inst).SetLaserOn(enable);
        }
    }
    return true;
}

bool HalApi::SetLoopbackType(int id, LoopbackType::Enum type)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().SetLoopbackType(digi_port, type);
    }
    
    return true;
}

bool HalApi::GetEthCounter(int id, EthCounter *cnt)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().UpdateCounter(digi_port);
        EthCounter *pCnt = Digi::GetInst().GetEthCnt(digi_port);
        memcpy(cnt, pCnt, sizeof(EthCounter));
    }
    
    return true;
}

bool HalApi::SetOtuFecMode(int id, FecType::Enum type)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        if((digi_port == 11)&&MsaCfp::GetInst(0).isMenaraCfp())
        {
            APP_LOG("SetOtuFecMode port %d FEC type force change from %d to %d for Menara CFP", digi_port, type, FecType::GFEC);
            Digi::GetInst().SetFecType(digi_port, FecType::GFEC);
        }
        else
        {
            Digi::GetInst().SetFecType(digi_port, type);
        }
    }
    
    return true;
}

//bool HalApi::SetOtuTimDefectMode(int id, TimDefectMode::Enum mode);
//bool HalApi::SetOtuTxSAPI(int id, std::string &sapi);
//bool HalApi::SetOtuTxDAPI(int id, std::string &dapi);
//bool HalApi::SetOtuTxOPER(int id, std::string &oper);
//bool HalApi::GetOtuRxSAPI(int id, std::string &sapi);
//bool HalApi::GetOtuRxDAPI(int id, std::string &dapi);
//bool HalApi::GetOtuRxOPER(int id, std::string &oper);
//bool HalApi::SetOtuExpSAPI(int id, std::string &sapi);
//bool HalApi::SetOtuExpDAPI(int id, std::string &dapi);
//bool HalApi::SetOtuExpOPER(int id, std::string &oper);

bool HalApi::SetOtuPrbsEnable(int id, bool enable)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().SetOtuPrbsEnable(digi_port, enable);
    }
    
    return true;
}

bool HalApi::GetOtuPrbsStatus(int id, PrbsStatus::Enum &status)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().GetOtuPrbsStatus(digi_port, status);
    }
    
    return true;
}

bool HalApi::GetOtuCounter(int id, OtuCounter *cnt)
{
    int digi_port = hal_to_digi_port(id);
    if (digi_port > 0)
    {
        Digi::GetInst().UpdateCounter(digi_port);
        OtuCounter *pCnt = Digi::GetInst().GetOtuCnt(digi_port);
        memcpy(cnt, pCnt, sizeof(OtuCounter));
    }
    
    return true;
}

bool HalApi::SetOchFecMode(int id, FecType::Enum type)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                cfp.SetFecMode(type);
            }
        }
    }

    return true;
}

bool HalApi::SetOchChan(int id, int chan)
{   
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                cfp.SetChan(chan);
            }
        }
    }

    return true;
}

bool HalApi::GetOchMinFreq(int id, std::string &freq)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                freq = cfp.GetMinFreq();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchMaxFreq(int id, std::string &freq)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                freq = cfp.GetMaxFreq();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchGridSpacing(int id, std::string &freq)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                freq = cfp.GetGridSpacing();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchMaxChanNum(int id, std::string &num)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                num = cfp.GetMaxChanNum();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchCurTxFreq(int id, std::string &freq)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                freq = cfp.GetCurTxFreq();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchCurRxFreq(int id, std::string &freq)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                freq = cfp.GetCurRxFreq();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchDGD(int id, std::string &dgd)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                dgd = cfp.GetDGD();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchCD(int id, std::string &cd)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                cd = cfp.GetCD();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchOSNR(int id, std::string &osnr)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                osnr = cfp.GetOSNR();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchQfactor(int id, std::string &qfactor)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            if (cfp.GetPresent())
            {
                qfactor = cfp.GetQfactor();
            }
        }
    }
    
    return true;
}

bool HalApi::GetOchCounter(int id, OchCounter *cnt)
{
    //freq = MsaCfp::GetInst(0).GetMinFreq();
    return true;
}

// Inventory
//bool HalApi::GetShelfInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
bool HalApi::GetCardInventory(int id, Inventory_s &inv)
{
    inv.present = true;
#if 0
    if (isTranspondMode == true)
    {
        inv.moduleType = "Tranponder";
        inv.partNumber = "MXP-100GT";
    }
    else
    {
        inv.moduleType = "Muxponder";
        inv.partNumber = "MXP-100GM";
    }
#endif
    inv.moduleType = EepromGetTag('M');
    inv.partNumber = EepromGetTag('P');
    inv.serialNumber = EepromGetTag('S');
    inv.hwVersion = EepromGetTag('H');
    inv.vendorName = EepromGetTag('N');
    inv.produceDate = EepromGetTag('D');
    inv.fwVersion = EepromGetTag('F');
    return true;
}

//bool HalApi::GetFanInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
//bool HalApi::GetPsuInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
bool HalApi::GetPortInventory(int id, Inventory_s &inv)
{
    if (isTranspondMode || id == 11)
    {
        int cfp_inst = hal_to_cfp_inst(id);
        if (cfp_inst >= 0)
        {
            MsaCfp &cfp = MsaCfp::GetInst(cfp_inst);
            inv.present = cfp.GetPresent();
            if (inv.present)
            {
                cfp.UpdateInventory();
                inv.moduleType = "CFP";
                inv.partNumber = cfp.GetPartNumber();
                inv.serialNumber = cfp.GetSerialNumber();
                inv.hwVersion = cfp.GetHwVersion();
                inv.vendorName = cfp.GetVendorName();
                inv.produceDate = cfp.GetDateCode();
                inv.fwVersion = cfp.GetFwVersion();
            }
        }
    }
    else
    {
        int sfp_inst = hal_to_sfp_inst(id);
        if (sfp_inst >= 0)
        {
            MsaSfp &sfp = MsaSfp::GetInst(sfp_inst);
            inv.present = sfp.GetPresent();
            if (inv.present)
            {
                sfp.UpdateInventory();
                inv.moduleType = "SFP+";
                inv.partNumber = sfp.GetPartNumber();
                inv.serialNumber = sfp.GetSerialNumber();
                inv.hwVersion = sfp.GetHwVersion();
                inv.vendorName = sfp.GetVendorName();
                inv.produceDate = sfp.GetDateCode();
                inv.fwVersion = "N/A";
            }
        }
    }
    return true;
}

bool HalApi::GetOscInventory(int id, Inventory_s &inv)
{
    if (id == 1 || id == 2) id += 9;
    
    MsaSfp &sfp = MsaSfp::GetInst(id);
    inv.present = sfp.GetPresent();
    if (inv.present)
    {
        sfp.UpdateInventory();
        inv.moduleType = "SFP+";
        inv.partNumber = sfp.GetPartNumber();
        inv.serialNumber = sfp.GetSerialNumber();
        inv.hwVersion = sfp.GetHwVersion();
        inv.vendorName = sfp.GetVendorName();
        inv.produceDate = sfp.GetDateCode();
        inv.fwVersion = "N/A";
    }
    return true;
}


bool HalApi::GetOptLos(int id, bool &los)
{
    if (id % 2 == 0)
        los = false;
    else
        los = true;
    return true;
}
    
