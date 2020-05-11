#include "DanOS.h"
#include "Mdio.h"
#include "Mvl88E6321.h"
#include "Fpga.h"
#include "Gpio.h"
using namespace std;

// multiple chip mode, use indirect access
#define MVL88E6321_ADDR     0x02    // the addr is invert value of ADDR[4:0]
#define SMI_CMD_REG         0x0
#define SMI_DATA_REG        0x1
#define SMI_GLOBAL_1        0x1B

#define GLOBAL_2_ADDR       0x1C
#define SMI_PHY_CMD         0x18
#define SMI_PHY_DATA        0x19

#define VLAN_UNTAGGED       0x1
#define VLAN_TAGGED         0x2

void Mvl88E6321Dump();

bool SmiBusy(void)
{
    uint16_t val = 0;
    int timeout = 10;
    while (timeout--)
    {
        val = MiiRead(MVL88E6321_ADDR, SMI_CMD_REG);
        if ((val & 0x8000) == 0)
        {
            return false;
        }
        usleep(100*1000);
    }
    
    return true;
}

bool Mvl88E6321Write(uint16_t addr, uint16_t reg, uint16_t val)
{
    uint16_t cmd = (reg) | (addr << 5) | (0x1 << 10) | (0x1 << 12) | 0x8000;
    if (SmiBusy())
    {
        APP_LOG("Mvl88E6321Write not ready");
        return false;
    }
    MiiWrite(MVL88E6321_ADDR, SMI_DATA_REG, val);
    MiiWrite(MVL88E6321_ADDR, SMI_CMD_REG, cmd);
    if (SmiBusy())
    {
        APP_LOG("Mvl88E6321Write timeout");
        return false;
    }

    return true;
}

uint16_t Mvl88E6321Read(uint16_t addr, uint16_t reg)
{
    uint16_t cmd = (reg) | (addr << 5) | (0x2 << 10) | (0x1 << 12) | 0x8000;
    if (SmiBusy())
    {
        APP_LOG("Mvl88E6321Read not ready");
        return 0xffff;
    }
    MiiWrite(MVL88E6321_ADDR, SMI_CMD_REG, cmd);
    if (SmiBusy())
    {
        APP_LOG("Mvl88E6321Read timeout");
        return 0xffff;
    }

    return MiiRead(MVL88E6321_ADDR, SMI_DATA_REG);
}

#define GLOBAL_2_ADDR       0x1C
#define SMI_PHY_CMD         0x18
#define SMI_PHY_DATA        0x19

bool SerdesSmiBusy(void)
{
    uint16_t val = 0;
    int timeout = 10;
    while (timeout--)
    {
        val = Mvl88E6321Read(GLOBAL_2_ADDR, SMI_PHY_CMD);
        if ((val & 0x8000) == 0)
        {
            return false;
        }
        usleep(100*1000);
    }
    
    return true;
}

bool Mvl88E6321SerdesWrite(uint16_t addr, uint16_t reg, uint16_t val)
{
    uint16_t cmd = (reg) | (addr << 5) | (0x1 << 10) | (0x1 << 12) | 0x8000; 
    if (SerdesSmiBusy())
    {
        APP_LOG("Mvl88E6321SerdesWrite not ready");
        return false;
    }
    Mvl88E6321Write(GLOBAL_2_ADDR, SMI_PHY_DATA, val);
    Mvl88E6321Write(GLOBAL_2_ADDR, SMI_PHY_CMD, cmd);
    if (SerdesSmiBusy())
    {
        APP_LOG("Mvl88E6321SerdesWrite timeout");
        return false;
    }

    return true;
}

uint16_t Mvl88E6321SerdesRead(uint16_t addr, uint16_t reg)
{
    uint16_t cmd = (reg) | (addr << 5) | (0x2 << 10) | (0x1 << 12) | 0x8000;
    if (SerdesSmiBusy())
    {
        APP_LOG("Mvl88E6321SerdesRead not ready");
        return 0xffff;
    }
    Mvl88E6321Write(GLOBAL_2_ADDR, SMI_PHY_CMD, cmd);
    if (SerdesSmiBusy())
    {
        APP_LOG("Mvl88E6321SerdesRead timeout");
        return 0xffff;
    }

    return Mvl88E6321Read(GLOBAL_2_ADDR, SMI_PHY_DATA);
}

#define QD_REG_VTU_OPERATION        0x5
#define QD_REG_VTU_VID_REG          0x6
#define QD_REG_VTU_DATA1_REG        0x7
#define QD_REG_VTU_DATA2_REG        0x8


bool Mvl88E6321AddVtu(uint16_t vid, uint16_t *vlanTag)
{
	uint16_t val = 0;

    val = Mvl88E6321Read(SMI_GLOBAL_1, QD_REG_VTU_OPERATION);
    if (val & 0x8000)
    {
        APP_LOG("Mvl88E6321 VTU in busy");
        return false;
    }

    val = 0;
    for (int p = 0; p < 4; p++)
    {
        val |= (vlanTag[p] << (4 * p));
    }
    Mvl88E6321Write(SMI_GLOBAL_1, QD_REG_VTU_DATA1_REG, val);

    val = 0;
    for (int p = 4; p < 7; p++)
    {
        val |= (vlanTag[p] << (4 * (p - 4)));
    }
    Mvl88E6321Write(SMI_GLOBAL_1, QD_REG_VTU_DATA2_REG, val);

    val = (vid & 0xfff) | (1 << 12);
    Mvl88E6321Write(SMI_GLOBAL_1, QD_REG_VTU_VID_REG, val);

    val = (1 << 15) | (0x3 << 12);
    Mvl88E6321Write(SMI_GLOBAL_1, QD_REG_VTU_OPERATION, val);

	return true;
}

// P0	Triple Speed	OSC0
// P1	Triple Speed	OSC1
// P2	Digital Pins	FPGA
// P5	Digital Pins	CPU
void Mvl88E6321Init()
{    
    //uint16_t val = 0;
    //uint16_t vlanTag[7] = {0};
    APP_LOG("Mvl88E6321 Init");
    //FpgaWrite(0x141, 0x0);
    GpioSetPin(ENET_RESET, 0);
    usleep(10*1000);
    GpioSetPin(ENET_RESET, 1);
    usleep(10*1000);
#if 0
    // set P0 - OSC1 default PVID = 80
    val = Mvl88E6321Read(0x10, 0x07);
    val &= 0xf000;
    val |= 0x50;
    Mvl88E6321Write(0x10, 0x07, val);

    // set P1 - OSC2 default PVID = 81
    val = Mvl88E6321Read(0x11, 0x07);
    val &= 0xf000;
    val |= 0x51;
    Mvl88E6321Write(0x11, 0x07, val);

    // enable 802.1Q mode for all ports
    val = Mvl88E6321Read(0x10, 0x08);
    Mvl88E6321Write(0x10, 0x08, val | 0x0c00);
    val = Mvl88E6321Read(0x11, 0x08);
    Mvl88E6321Write(0x10, 0x08, val | 0x0c00);
    val = Mvl88E6321Read(0x15, 0x08);
    Mvl88E6321Write(0x10, 0x08, val | 0x0c00);

    // add vlan 80 for OSC1 + CPU
    for (int p = 0; p < 7; p++) vlanTag[p] = 0x3;
    vlanTag[0] = VLAN_UNTAGGED;
    vlanTag[5] = VLAN_UNTAGGED; //VLAN_TAGGED;
    Mvl88E6321AddVtu(80, vlanTag);

    // add vlan 81 for OSC2 + CPU
    for (int p = 0; p < 7; p++) vlanTag[p] = 0x3;
    vlanTag[1] = VLAN_UNTAGGED;
    vlanTag[5] = VLAN_UNTAGGED; //VLAN_TAGGED;
    Mvl88E6321AddVtu(81, vlanTag);
#endif
    if (!Mvl88E6321Write(0x10, 0x04, 0x7f)) return;
    if (!Mvl88E6321Write(0x11, 0x04, 0x7f)) return;
    if (!Mvl88E6321Write(0x12, 0x04, 0x7f)) return;
    if (!Mvl88E6321Write(0x15, 0x04, 0x7f)) return;

    if (!Mvl88E6321SerdesWrite(0x0c, 0x00, 0x1140)) return;
    if (!Mvl88E6321SerdesWrite(0x0d, 0x00, 0x1140)) return;

    if (!Mvl88E6321Write(0x16, 0x00, 0x00)) return;
    if (!Mvl88E6321Write(0x12, 0x00, 0x00)) return;

    if (!Mvl88E6321Write(0x12, 0x01, 0x083d)) return;
    if (!Mvl88E6321Write(0x15, 0x01, 0x083d)) return;

    // MacTableFlushAll();
}

void Mvl88E6321ClearAllPortCounter()
{
    uint16_t val = 0;

    val = (1<<15) | (1<<12) | (3<<10);
    Mvl88E6321Write(0x1b, 0x1d, val);

    while (val & (1<<15))
    {
        val = Mvl88E6321Read(0x1b, 0x1d);
        usleep(100*1000);
    }
}

uint32_t Mvl88E6321ReadPortCounter(uint8_t port, uint8_t type)
{
    uint32_t sum = 0;
    uint16_t val = 0, val1 = 0, val2 = 0;

    val = (1<<15) | (5<<12) | (3<<10) | ((port+1)<<5);
    Mvl88E6321Write(0x1b, 0x1d, val);

    while (val & (1<<15))
    {
        val = Mvl88E6321Read(0x1b, 0x1d);
        usleep(100*1000);
    }

    val = (1<<15) | (4<<12) | (3<<10) | type;
    Mvl88E6321Write(0x1b, 0x1d, val);

    while (val & (1<<15))
    {
        val = Mvl88E6321Read(0x1b, 0x1d);
        usleep(100*1000);
    }

    val1 = Mvl88E6321Read(0x1b, 0x1e);
    val2 = Mvl88E6321Read(0x1b, 0x1f);

    sum = val1;
    sum <<= 16;
    sum |= val2;

    return sum;    
}

void Mvl88E6321DumpPort(uint8_t port)
{
    APP_LOG("Port %d Counter:", (int)port);
    APP_LOG("\tRxUc: %08x", Mvl88E6321ReadPortCounter(port, 0x04));
    APP_LOG("\tRxBc: %08x", Mvl88E6321ReadPortCounter(port, 0x06));
    APP_LOG("\tRxMc: %08x", Mvl88E6321ReadPortCounter(port, 0x07));
    APP_LOG("\tTxUc: %08x", Mvl88E6321ReadPortCounter(port, 0x10));
    APP_LOG("\tTxBc: %08x", Mvl88E6321ReadPortCounter(port, 0x13));
    APP_LOG("\tTxMc: %08x", Mvl88E6321ReadPortCounter(port, 0x12));
}

void Mvl88E6321Dump(void)
{
    Mvl88E6321DumpPort(0);
    //Mvl88E6321DumpPort(1);
    //Mvl88E6321DumpPort(5);
}

void Mvl88E6321Menu(int argc, char *argv[])
{
    bool help = false;
    
    if (argc >= 1)
    {
        if (argc == 1 && argv[0][0] == 'i')
        {
            Mvl88E6321Init();
        }
        else if (argc == 1 && argv[0][0] == 'd')
        {
            Mvl88E6321Dump();
        }
        else if (argc == 1 && argv[0][0] == 'c')
        {
            Mvl88E6321ClearAllPortCounter();
        }
        else if (argc == 1 && argv[0][0] == 'v')
        {
            uint16_t val = 0;
            uint16_t vlanTag[7] = {0};

            // set P0 - OSC1 default PVID = 80
            val = Mvl88E6321Read(0x10, 0x07);
            val &= 0xf000;
            val |= 0x50;
            Mvl88E6321Write(0x10, 0x07, val);

            // set P1 - OSC2 default PVID = 81
            val = Mvl88E6321Read(0x11, 0x07);
            val &= 0xf000;
            val |= 0x51;
            Mvl88E6321Write(0x11, 0x07, val);

            // enable 802.1Q mode for all ports
            val = Mvl88E6321Read(0x10, 0x08);
            Mvl88E6321Write(0x10, 0x08, val | 0x0c00);
            val = Mvl88E6321Read(0x11, 0x08);
            Mvl88E6321Write(0x10, 0x08, val | 0x0c00);
            val = Mvl88E6321Read(0x15, 0x08);
            Mvl88E6321Write(0x10, 0x08, val | 0x0c00);

            // add vlan 80 for OSC1 + CPU
            for (int p = 0; p < 7; p++) vlanTag[p] = 0x3;
            vlanTag[0] = VLAN_UNTAGGED;
            vlanTag[5] = VLAN_UNTAGGED; //VLAN_TAGGED;
            Mvl88E6321AddVtu(80, vlanTag);

            // add vlan 81 for OSC2 + CPU
            for (int p = 0; p < 7; p++) vlanTag[p] = 0x3;
            vlanTag[1] = VLAN_UNTAGGED;
            vlanTag[5] = VLAN_UNTAGGED; //VLAN_TAGGED;
            Mvl88E6321AddVtu(81, vlanTag);

            // MacTableFlushAll();
        }
        else if (argc >= 3 && argv[0][0] == 'r')
        {
            int len = 1;
            long addr, reg;
            uint16_t val;
            if (argc == 4) len = atoi(argv[3]);
            addr = strtol(argv[1], NULL, 0);
            reg = strtol(argv[2], NULL, 0);
            for (int i = 0; i < len; i++)
            {
                val = Mvl88E6321Read((uint16_t)addr, (uint16_t)(reg + i));
                APP_LOG("[%04x:%04x]: 0x%04x", (uint16_t)addr, (uint16_t)(reg + i), val); 
            }
        }
        else if (argc == 4 && argv[0][0] == 'w')
        {
            long addr, reg, val;
            addr = strtol(argv[1], NULL, 0);
            reg = strtol(argv[2], NULL, 0);
            val = strtol(argv[3], NULL, 0);
            Mvl88E6321Write((uint16_t)addr, (uint16_t)(reg), (uint16_t)(val));
        }
        else if (argc >= 3 && !strcmp(argv[0], "sr"))
        {
            int len = 1;
            long addr, reg;
            uint16_t val;
            if (argc == 4) len = atoi(argv[3]);
            addr = strtol(argv[1], NULL, 0);
            reg = strtol(argv[2], NULL, 0);
            for (int i = 0; i < len; i++)
            {
                val = Mvl88E6321SerdesRead((uint16_t)addr, (uint16_t)(reg + i));
                APP_LOG("[%04x:%04x]: 0x%04x", (uint16_t)addr, (uint16_t)(reg + i), val); 
            }
        }
        else if (argc == 4 && !strcmp(argv[0], "sw"))
        {
            long addr, reg, val;
            addr = strtol(argv[1], NULL, 0);
            reg = strtol(argv[2], NULL, 0);
            val = strtol(argv[3], NULL, 0);
            Mvl88E6321SerdesWrite((uint16_t)addr, (uint16_t)(reg), (uint16_t)(val));
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
        APP_LOG("88e6321");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t c");
        APP_LOG("\t v - setup vlan table");
        APP_LOG("\t r <addr> <reg> [len]");
        APP_LOG("\t w <addr> <reg> <val>");
        APP_LOG("\t sr <addr> <reg> [len] - Serdes");
        APP_LOG("\t sw <addr> <reg> <val> - Serdes");
    }
}

