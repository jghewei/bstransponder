#include "DanOS.h"
#include "SFM.h"
#include "MobHeader.h"
#include "HAL.h"
#include "Counter.h"
#include <time.h>
#include "CThread.h"

static int mem_usage = 0;
static int cpu_usage = 0;
static int up_time = 0;

extern void SnmpCreateTrapSession(const char *trap1, const char *trap2, const char *trap3);

bool SystemSet(BaseMob* part, std::string& atrb)
{
    SystemMob* mob = (SystemMob*)part;
    char cmdline[128] = {0};

    if (atrb == "SetDateTime")
    {
        sprintf(cmdline, "date -s \"%s\"", mob->SetDateTime.GetValue().c_str());
#ifdef __TARGET__
        system(cmdline);
#endif
        APP_LOG(cmdline);
        mob->SetDateTime.SetValue("");
    }
#ifdef __TARGET__
    else if (atrb == "Restart")
    {
        if (mob->Restart.GetValue() == SystemMob_Restart_Reboot)
        {
            system("reboot");
        }
    }
    else if (atrb == "Database")
    {
        if (mob->Database.GetValue() == SystemMob_Database_Clear)
        {
            system("rm /mnt/sysfs/*; sync;");
        }
    }
#endif
    else if (atrb == "Description")
    {
        std::string desp = mob->Description.GetValue();
        if (desp.size() > 7 && strncmp(desp.c_str(), "extcmd", 6) == 0)
        {
            int argc = 0;
            char cmd[128] = {0};
            char argv[8][16];
            std::string extcmd = desp.substr(7, std::string::npos);
            strcpy(cmd, extcmd.c_str());
            const char *d = " ";
            char *p = strtok(cmd, d);
            memset(&argv[0][0], 0, sizeof(argv));
            while (p && argc < 8)
            {
                strcpy(argv[argc], p);
                printf("extcmd: %s\n", p);
                p = strtok(NULL, d);
                argc++;
            }
            HAL::GetInst().HwMenu(argc, (char**)(&argv[0][0]));
        }
        else if (desp.size() > 7 && strncmp(desp.c_str(), "syscmd", 6) == 0)
        {
            std::string syscmd = desp.substr(7, std::string::npos);
            system(syscmd.c_str());
        }
    }
    else if (atrb == "LampTest")
    {
        if (mob->LampTest.GetValue() == SystemMob_LampTest_On)
        {
            HAL::GetInst().SetLampTest(true);
        }
        else
        {
            HAL::GetInst().SetLampTest(false);
        }
    }
    
    return true;
}

bool SystemGet(BaseMob* part, std::string& atrb)
{
    SystemMob* mob = (SystemMob*)part;

    if (atrb == "UpTime")
    {
        mob->UpTime.SetValue(up_time);
    }
    else if (atrb == "CpuUsage")
    {
        mob->CpuUsage.SetValue(cpu_usage);
    }
    else if (atrb == "MemUsage")
    {
        mob->MemUsage.SetValue(mem_usage);
    }
    else if (atrb == "CurrentDateTime")
    {
        char timestr[32] = {0};
        time_t timep;
        struct tm *p;
        time(&timep);
        p = localtime(&timep);
        sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d",
            (1900 + p->tm_year), (p->tm_mon + 1), (p->tm_mday),
            p->tm_hour, p->tm_min, p->tm_sec);
        mob->CurrentDateTime.SetValue(timestr);
    }
    else if (atrb == "SwVersion")
    {
        char ts[32] = {0};
        sprintf(ts, "SW1.0 @ %s %s", __DATE__, __TIME__);
        mob->SwVersion.SetValue(ts);
    }
    
    return true;
}

void SystemInit()
{
    SystemMob::RegGetCallback(SystemGet);
    SystemMob::RegSetCallback(SystemSet);
    SystemMob *mob = SystemMob::AddInst(KeyType::System, 1);
    mob->Load();
}

bool ShelfGet(BaseMob* part, std::string& atrb)
{
    ShelfMob* mob = (ShelfMob*)part;

    if (atrb == "InletTemp")
    {
        int degree = 0;
        HAL::GetInst().GetShelfInletTemperature(degree);
        mob->InletTemp.SetValue(degree);
        HAL::GetInst().GetShelfOutletTemperature(degree);
        mob->OutletTemp.SetValue(degree);
    }
    
    return true;
}

void ShelfEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Shelf, id);
    ShelfMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void ShelfInit()
{
    ShelfMob::RegGetCallback(ShelfGet);
    //ShelfMob::RegSetCallback(ShelfSet);
    ShelfMob::AddInst(KeyType::Shelf, 1);
    HAL::GetInst().RegShelfCallback(ShelfEvent);

    ShelfMob::GetInst(0)->TempLowThd.SetValue(-5);
    ShelfMob::GetInst(0)->TempHighThd.SetValue(45);
    ShelfMob::GetInst(0)->Load();
}

bool CardGet(BaseMob* part, std::string& atrb)
{
    CardMob* mob = (CardMob*)part;

    if (atrb == "BoardTemp")
    {
        int degree = 0;
        HAL::GetInst().GetCardTemperature(degree);
        mob->BoardTemp.SetValue(degree);
    }
    
    return true;
}

void CardInit()
{
    CardMob::RegGetCallback(CardGet);
    //CardMob::RegSetCallback(CardSet);
    CardMob::AddInst(KeyType::Card, 1);
}

bool FanGet(BaseMob* part, std::string& atrb)
{
    FanMob* mob = (FanMob*)part;

    if (atrb == "Speed")
    {
        int speed = 0;
        HAL::GetInst().GetFanSpeed(speed);
        mob->Speed.SetValue(speed);
    }
    
    return true;
}

bool FanSet(BaseMob* part, std::string& atrb)
{
    FanMob* mob = (FanMob*)part;

    if (atrb == "ForceSpeed")
    {
        if (mob->ForceSpeed.GetValue() == FanMob_ForceSpeed_FullSpeed)
        {
            HAL::GetInst().SetFanSpeed(100);
        }
        else
        {
            HAL::GetInst().SetFanSpeed(0);  // auto speed control
        }
    }
    
    return true;
}

void FanEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Fan, id);
    FanMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void FanInit()
{
    FanMob::RegGetCallback(FanGet);
    FanMob::RegSetCallback(FanSet);
    FanMob::AddInst(KeyType::Fan, 1);
    HAL::GetInst().RegFanCallback(FanEvent);
}

void PsuEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Psu, id);
    PsuMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void PsuInit()
{
    PsuMob::AddInst(KeyType::Psu, 1);
    PsuMob::AddInst(KeyType::Psu, 2);
    HAL::GetInst().RegPsuCallback(PsuEvent);
}

bool OpticsGet(BaseMob* part, std::string& atrb)
{
    OpticsMob* mob = (OpticsMob*)part;
    int index = mob->key.id();

    if (atrb == "Type")
    {
        OpticsInfo_s info;
        info.reset();
        if (mob->key.type() == KeyType::Port)
        {
            HAL::GetInst().GetPortOpticsInfo(index, info);
        }
        else if (mob->key.type() == KeyType::Osc)
        {
            HAL::GetInst().GetOscOpticsInfo(index, info);
        }
        mob->Type.SetValue(info.type);
        mob->Laser.SetValue(info.laser?OpticsMob_Laser_On:OpticsMob_Laser_Off);
        mob->Los.SetValue(info.los?OpticsMob_Los_On:OpticsMob_Los_Off);
        mob->TxPower.SetValue(info.txPower);
        mob->RxPower.SetValue(info.rxPower);
        mob->Temp.SetValue(info.temp);
    }
    
    return true;
}

void OpticsInit()
{
    OpticsMob::RegGetCallback(OpticsGet);
    //OpticsMob::RegSetCallback(CardSet);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        OpticsMob::AddInst(KeyType::Port, port);
    }

    OpticsMob::AddInst(KeyType::Osc, 1);
    OpticsMob::AddInst(KeyType::Osc, 2);
}

bool NetworkSet(BaseMob* part, std::string& atrb)
{
    NetworkMob* mob = (NetworkMob*)part;

    if (mob->key.type() == KeyType::Mgmt && (atrb == "IpAddress" || atrb == "Netmask" || atrb == "Gateway"))
    {
        HAL::GetInst().SetMgmtIfcfg(mob->IpAddress.GetValue(), mob->Netmask.GetValue(), mob->Gateway.GetValue());
    }
    else if (mob->key.type() == KeyType::Osc && (atrb == "IpAddress" || atrb == "Netmask" || atrb == "Gateway"))
    {
        HAL::GetInst().SetOscIfcfg(mob->IpAddress.GetValue(), mob->Netmask.GetValue(), mob->Gateway.GetValue());
    }

    return true;
}

#if 0
bool NetworkGet(BaseMob* part, std::string& atrb)
{
    NetworkMob* mob = (NetworkMob*)part;
    
    if (atrb == "IpAddress")
    {
        std::string ip, mask, gw;
        HAL::GetInst().GetMgmtIfcfg(ip, mask, gw);
        mob->IpAddress.SetValue(ip);
        mob->Netmask.SetValue(mask);
        mob->Gateway.SetValue(gw);
    }
   
    return true;

#endif

void NetworkInit()
{
    NetworkMob *mob = NULL;
    //NetworkMob::RegGetCallback(NetworkGet);
    NetworkMob::RegSetCallback(NetworkSet);
    
    mob = NetworkMob::AddInst(KeyType::Mgmt, 1);
    mob->IpAddress.SetValue("192.168.1.10");
    mob->Netmask.SetValue("255.255.255.0");
    mob->Load();

    mob = NetworkMob::AddInst(KeyType::Osc, 2);
    mob->IpAddress.SetValue("192.168.80.10");
    mob->Netmask.SetValue("255.255.255.0");
    mob->Load();
#if 0
    mob = NetworkMob::AddInst(KeyType::Osc, 2);
    mob->IpAddress.SetValue("192.168.81.10");
    mob->Netmask.SetValue("255.255.255.0");
    mob->Load();

#ifdef __TARGET__
    //system("ifconfig eth1 192.168.2.10 netmask 255.255.255.0 up");
    system("ifconfig eth1 down");
#endif
#endif
}

bool NetworkExe()
{
    NetworkMob* mob = NULL;
    mob = NetworkMob::GetInstByKey(KeyType::Mgmt, 1);
    mob->Gateway.Exe();
    mob = NetworkMob::GetInstByKey(KeyType::Osc, 2);
    mob->Gateway.Exe();
    return true;
}

bool PortSet(BaseMob* part, std::string& atrb)
{
    PortMob* mob = (PortMob*)part;
    int index = mob->key.id();

    if (atrb == "PortMode")
    {
        PortMode::Enum mode = PortMode::NONE;
        switch(mob->PortMode.GetValue())
        {
            case PortMob_PortMode_ETH10G:
                mode = PortMode::ETH10G;
                break;
            case PortMob_PortMode_ETH100G:
                mode = PortMode::ETH100G;
                break;
            case PortMob_PortMode_STM64:
                mode = PortMode::STM64;
                break;
            //case PortMob_PortMode_OC192:
            //    mode = PortMode::OC192;
            //    break;
            case PortMob_PortMode_OTU2:
                mode = PortMode::OTU2;
                break;
            case PortMob_PortMode_OTU2E:
                mode = PortMode::OTU2E;
                break;
            case PortMob_PortMode_OTU4:
                mode = PortMode::OTU4;
                break;
            /*case PortMob_PortMode_FC8G:
                mode = PortMode::FC8G;
                break;
            case PortMob_PortMode_FC10G:
                mode = PortMode::FC10G;
                break;*/
            case PortMob_PortMode_ETH10G_GFP:
                mode = PortMode::ETH10G_GFP;
                break;
            default:
                break;
        }
        PortMode::Enum old_mode;
        HAL::GetInst().GetPortMode(index, old_mode);
        if (old_mode != mode)
        {
            HAL::GetInst().SetPortMode(index, PortMode::NONE);
            if (old_mode == PortMode::ETH10G || old_mode == PortMode::ETH100G || old_mode == PortMode::ETH10G_GFP)
            {
                Key key(KeyType::Eth, index);
                printf("a %p\n", EthMob::GetInstByKey(key));
                EthMob::GetInstByKey(key)->ResetAllAlarm();
                ActivePM::GetInst().set_enable(EthMob::GetInstByKey(key), false);
            }
            else if (old_mode == PortMode::OTU2 || old_mode == PortMode::OTU2E || old_mode == PortMode::OTU4)
            {
                Key key(KeyType::Otu, index);
                OtuMob::GetInstByKey(key)->ResetAllAlarm();
                ActivePM::GetInst().set_enable(OtuMob::GetInstByKey(key), false);
            }
            else if (old_mode == PortMode::STM64)
            {
                Key key(KeyType::Stm, index);
                StmMob::GetInstByKey(key)->ResetAllAlarm();
                ActivePM::GetInst().set_enable(StmMob::GetInstByKey(key), false);
            }

            if (mode != PortMode::NONE)
            {
                HAL::GetInst().SetPortMode(index, mode);
                if (mode == PortMode::ETH10G || mode == PortMode::ETH100G || mode == PortMode::ETH10G_GFP)
                {
                    Key key(KeyType::Eth, index);
                    printf("b %p\n", EthMob::GetInstByKey(key));
                    EthMob::GetInstByKey(key)->ResetAllAlarm();
                    ActivePM::GetInst().set_enable(EthMob::GetInstByKey(key), true);
                }
                else if (mode == PortMode::OTU2 || mode == PortMode::OTU2E || mode == PortMode::OTU4)
                {
                    Key key(KeyType::Otu, index);
                    OtuMob::GetInstByKey(key)->ResetAllAlarm();
                    ActivePM::GetInst().set_enable(OtuMob::GetInstByKey(key), true);
                }
                else if (mode == PortMode::STM64)
                {
                    Key key(KeyType::Stm, index);
                    StmMob::GetInstByKey(key)->ResetAllAlarm();
                    ActivePM::GetInst().set_enable(StmMob::GetInstByKey(key), true);
                }
            }
        }
    }
    else if (atrb == "AdminStatus")
    {
        bool enable = false;
        if (mob->AdminStatus.GetValue() != PortMob_AdminStatus_Off)
        {
            enable = true;
        }
        HAL::GetInst().SetLaserEnable(index, enable);
    }
    else if (atrb == "LoopbackType")
    {
        if (mob->PortMode.GetValue() == PortMob_PortMode_NONE)
        {
            return true;
        }
        
        LoopbackType::Enum type = LoopbackType::Disabled;
        switch(mob->LoopbackType.GetValue())
        {
            case PortMob_LoopbackType_Facility:
                type = LoopbackType::Facility;
                break;
            case PortMob_LoopbackType_Terminal:
                type = LoopbackType::Terminal;
                break;
            default:
                break;
        }
        HAL::GetInst().SetLoopbackType(index, type);
    }

    return true;
}

bool PortGet(BaseMob* part, std::string& atrb)
{
    PortMob* mob = (PortMob*)part;
    int index = mob->key.id();

    if (atrb == "OperStatus")
    {
        if (mob->AdminStatus.GetValue() == PortMob_AdminStatus_Off)
        {
            mob->OperStatus.SetValue(PortMob_OperStatus_Off);
        }
        else
        {
            OpticsInfo_s info;
            info.reset();
            HAL::GetInst().GetPortOpticsInfo(index, info);
            if (info.los == true)
            {
                mob->OperStatus.SetValue(PortMob_OperStatus_Down);
            }
            else
            {
                mob->OperStatus.SetValue(PortMob_OperStatus_Up);
            }
        }
    }
    //std::cout << "PortMob " << index << " " << atrb << " get " << std::endl;
    return true;
}

void PortInit()
{
    PortMob *mob = NULL;

    PortMob::RegSetCallback(PortSet);
    PortMob::RegGetCallback(PortGet);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        mob = PortMob::AddInst(KeyType::Port, port);
        mob->AliasName.SetValue("");
        //mob->PortType.SetValue(PortMob_PortType_Client);
        mob->PortMode.SetValue(PortMob_PortMode_NONE);
        mob->AdminStatus.SetValue(PortMob_AdminStatus_Off);
        mob->OperStatus.SetValue(PortMob_OperStatus_Down);
        mob->LoopbackType.SetValue(PortMob_LoopbackType_Disabled);
        mob->Load();
    }
}

void PortExe()
{
    PortMob *mob = NULL;
    int num = PortMob::GetInstNum();
    for (int i = 0; i < num; i++)
    {
        mob = PortMob::GetInst(i);
        mob->PortMode.Exe();
#if 0
        // this is a workaround for cold reboot issue
        if (PortMob_PortMode_ETH10G == mob->PortMode.GetValue())
        {
            mob->PortMode.Set(PortMob_PortMode_NONE);
            mob->PortMode.Set(PortMob_PortMode_ETH10G);
        }
#endif
        mob->AdminStatus.Exe();
        mob->LoopbackType.Exe();
    }
}

bool EthSet(BaseMob* part, std::string& atrb)
{
    return true;
}

bool EthGet(BaseMob* part, std::string& atrb)
{
    EthMob* mob = (EthMob*)part;
    int index = mob->key.id();

    if (atrb == "Counter")
    {
        EthCounter cnt;
        if (HAL::GetInst().GetEthCounter(index, &cnt))
        {
            mob->SetCounter(EthCnt_RxOctets, cnt.etherStatsOctets);
            mob->SetCounter(EthCnt_RxPkts, cnt.etherStatsPkts);
            mob->SetCounter(EthCnt_RxUcastPkts, cnt.ifInUcastPkts);
            mob->SetCounter(EthCnt_RxMulticastPkts, cnt.etherStatsMulticastPkts);
            mob->SetCounter(EthCnt_RxBroadcastPkts, cnt.etherStatsBroadcastPkts);
            mob->SetCounter(EthCnt_RxPkts64Octets, cnt.etherStatsPkts64Octets);
            mob->SetCounter(EthCnt_RxPkts65to127Octets, cnt.etherStatsPkts65to127Octets);
            mob->SetCounter(EthCnt_RxPkts128to255Octets, cnt.etherStatsPkts128to255Octets);
            mob->SetCounter(EthCnt_RxPkts256to511Octets, cnt.etherStatsPkts256to511Octets);
            mob->SetCounter(EthCnt_RxPkts512to1023Octets, cnt.etherStatsPkts512to1023Octets);
            mob->SetCounter(EthCnt_RxPkts1024to1518Octets, cnt.etherStatsPkts1024to1518Octets);
            mob->SetCounter(EthCnt_RxPkts1519toMaxOctets, cnt.etherStatsPkts1519toMaxOctets);
            mob->SetCounter(EthCnt_RxCRCAlignErrors, cnt.etherStatsCRCAlignErrors);
            mob->SetCounter(EthCnt_RxUndersizePkts, cnt.etherStatsUndersizePkts);
            mob->SetCounter(EthCnt_RxOversizePkts, cnt.etherStatsOversizePkt);
            mob->SetCounter(EthCnt_RxFragments, cnt.etherStatsFragments);
            mob->SetCounter(EthCnt_RxJabbers, cnt.etherStatsJabbers);
            mob->SetCounter(EthCnt_TxOctets, cnt.ifOutOctets);
            mob->SetCounter(EthCnt_TxPkts, 0);
            mob->SetCounter(EthCnt_TxUcastPkts, cnt.ifOutUcastPkts);
            mob->SetCounter(EthCnt_TxMulticastPkts, cnt.ifOutMulticastPkts);
            mob->SetCounter(EthCnt_TxBroadcastPkts, cnt.ifOutBroadcastPkts);
        }
    }

    return true;
}

void EthEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Eth, id);
    EthMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void EthInit()
{
    //EthMob *pMob = NULL;

    EthMob::RegSetCallback(EthSet);
    EthMob::RegGetCallback(EthGet);
    HAL::GetInst().RegEthCallback(EthEvent);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        //pMob = EthMob::AddInst(KeyType::Eth, port);
        EthMob::AddInst(KeyType::Eth, port);
        //pMob->FecMode.SetValue(EthMob_FecMode_Disabled);
    }
}

bool OtuSet(BaseMob* part, std::string& atrb)
{
    OtuMob* mob = (OtuMob*)part;
    int index = mob->key.id();

    Key key(KeyType::Port, index);
    int pMode = PortMob::GetInstByKey(key)->PortMode.GetValue();
    if (pMode != PortMob_PortMode_OTU2 &&
        pMode != PortMob_PortMode_OTU2E &&
        pMode != PortMob_PortMode_OTU4)
    {
        return false;
    }

    if (atrb == "FecMode")
    {        
        FecType::Enum type = FecType::NONE;
        switch(mob->FecMode.GetValue())
        {
            case OtuMob_FecMode_GFEC:
                type = FecType::GFEC;
                break;
            case OtuMob_FecMode_I4:
                type = FecType::I4;
                break;
            case OtuMob_FecMode_I7:
                type = FecType::I7;
                break;
            default:
                break;
        }
        HAL::GetInst().SetOtuFecMode(index, type);
    }
    else if (atrb == "TestSignalType")
    {
        bool enable = false;
        if (mob->TestSignalType.GetValue() == OtuMob_TestSignalType_PRBS)
        {
            enable = true;
        }
        HAL::GetInst().SetOtuPrbsEnable(index, enable);
    }

    return true;
}

bool OtuGet(BaseMob* part, std::string& atrb)
{
    OtuMob* mob = (OtuMob*)part;
    int index = mob->key.id();
    
    if (atrb == "TestSignalStatus")
    {
        if (mob->TestSignalType.GetValue() == OtuMob_TestSignalType_Disabled)
        {
            mob->TestSignalStatus.SetValue(OtuMob_TestSignalStatus_Disabled);
            return true;
        }

        int prbs_status = OtuMob_TestSignalStatus_Disabled;
        PrbsStatus::Enum status;
        HAL::GetInst().GetOtuPrbsStatus(index, status);
        switch(status)
        {
            case PrbsStatus::InSync:
                prbs_status = OtuMob_TestSignalStatus_InSync;
                break;
            case PrbsStatus::OutSync:
                prbs_status = OtuMob_TestSignalStatus_OutSync;
                break;
            case PrbsStatus::ErrorSync:
                prbs_status = OtuMob_TestSignalStatus_ErrorSync;
                break;
            default:
                break;
        }
        mob->TestSignalStatus.SetValue(prbs_status);
    }
    else if (atrb == "Counter")
    {
        OtuCounter cnt;
        if (HAL::GetInst().GetOtuCounter(index, &cnt))
        {
            mob->SetCounter(OtuCnt_CorrectBits, cnt.correctBits);
            mob->SetCounter(OtuCnt_UncorrectBits, cnt.uncorrectables);
            mob->SetCounter(OtuCnt_FrameCount, cnt.frameCount);
            mob->SetCounter(OtuCnt_Bip8, cnt.bip8);
            mob->SetCounter(OtuCnt_Bei, cnt.bei);
        }
    }

    return true;
}

void OtuEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Otu, id);
    OtuMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void OtuExe()
{
    OtuMob *mob = NULL;
    int num = OtuMob::GetInstNum();
    for (int i = 0; i < num; i++)
    {
        mob = OtuMob::GetInst(i);
        mob->FecMode.Exe();
        mob->TestSignalType.Exe();
    }
}

void OtuInit()
{
    OtuMob *pOtu = NULL;

    OtuMob::RegSetCallback(OtuSet);
    OtuMob::RegGetCallback(OtuGet);
    HAL::GetInst().RegOtuCallback(OtuEvent);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        pOtu = OtuMob::AddInst(KeyType::Otu, port);
        pOtu->FecMode.SetValue(OtuMob_FecMode_Disabled);
        pOtu->TestSignalType.SetValue(OtuMob_TestSignalType_Disabled);
    }
}

bool OchSet(BaseMob* part, std::string& atrb)
{
    OchMob* mob = (OchMob*)part;
    int index = mob->key.id();

    if (atrb == "FecMode")
    {        
        FecType::Enum type = FecType::NONE;
        switch(mob->FecMode.GetValue())
        {
            case OchMob_FecMode_SDFEC_ENHANCE:
                type = FecType::SDFEC_ENHANCE;
                break;
            case OchMob_FecMode_SDFEC_COMP:
                type = FecType::SDFEC_COMP;
                break;
            case OchMob_FecMode_GFEC:
                type = FecType::GFEC;
                break;
            case OchMob_FecMode_Transparent:
                type = FecType::TRANSPARENT;
                break;
            default:
                break;
        }
        HAL::GetInst().SetOchFecMode(index, type);
    }
    else if (atrb == "ChanNum")
    {
        int chan = mob->ChanNum.GetValue();
        HAL::GetInst().SetOchChan(index, chan);
    }

    return true;
}

bool OchGet(BaseMob* part, std::string& atrb)
{
    OchMob* mob = (OchMob*)part;
    int index = mob->key.id();
    
    if (atrb == "MinLaserFreq")
    {
        std::string tmp;
        HAL::GetInst().GetOchMinFreq(index, tmp);
        mob->MinLaserFreq.SetValue(tmp);
        HAL::GetInst().GetOchMaxFreq(index, tmp);
        mob->MaxLaserFreq.SetValue(tmp);
        HAL::GetInst().GetOchGridSpacing(index, tmp);
        mob->GridSpacing.SetValue(tmp);
        HAL::GetInst().GetOchMaxChanNum(index, tmp);
        mob->MaxChans.SetValue(tmp);
        HAL::GetInst().GetOchCurTxFreq(index, tmp);
        mob->CurTxLaserFreq.SetValue(tmp);
        HAL::GetInst().GetOchCurRxFreq(index, tmp);
        mob->CurRxLaserFreq.SetValue(tmp);
        HAL::GetInst().GetOchDGD(index, tmp);
        mob->DGD.SetValue(tmp);
        HAL::GetInst().GetOchCD(index, tmp);
        mob->CD.SetValue(tmp);
        HAL::GetInst().GetOchOSNR(index, tmp);
        mob->OSNR.SetValue(tmp);
        HAL::GetInst().GetOchQfactor(index, tmp);
        mob->Qfactor.SetValue(tmp);
    }
    else if (atrb == "Counter")
    {
        OchCounter cnt;
        if (HAL::GetInst().GetOchCounter(index, &cnt))
        {
            mob->SetCounter(OchCnt_CorrectBits, cnt.correctBits);
            mob->SetCounter(OchCnt_UncorrectBits, cnt.uncorrectables);
            mob->SetCounter(OtuCnt_Bip8, cnt.bip8);
            mob->SetCounter(OtuCnt_Bei, cnt.bei);
        }
    }

    return true;
}

void OchInit()
{
    OchMob *pOch = NULL;

    OchMob::RegSetCallback(OchSet);
    OchMob::RegGetCallback(OchGet);
    //HAL::GetInst().RegOtuCallback(OtuEvent);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        pOch = OchMob::AddInst(KeyType::Och, port);
        pOch->FecMode.SetValue(OchMob_FecMode_SDFEC_ENHANCE);
        pOch->ChanNum.SetValue(1);
        pOch->Load();
    }
}

void OchExe()
{
    OchMob *mob = NULL;
    int num = OchMob::GetInstNum();
    for (int i = 0; i < num; i++)
    {
        mob = OchMob::GetInst(i);
        mob->FecMode.Exe();
        mob->ChanNum.Exe();
    }
}

void StmEvent(int id, AlarmId::Enum alm, bool active)
{
    Key key(KeyType::Stm, id);
    StmMob::GetInstByKey(key)->UpdateAlarm(alm, active);
}

void StmInit()
{
    //StmMob *pMob = NULL;

    //StmMob::RegSetCallback(EthSet);
    //StmMob::RegGetCallback(EthGet);
    HAL::GetInst().RegStmCallback(StmEvent);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        StmMob::AddInst(KeyType::Stm, port);
        //pMob->FecMode.SetValue(EthMob_FecMode_Disabled);
    }
}

bool SnmpSet(BaseMob* part, std::string& atrb)
{
    SnmpMob* mob = (SnmpMob*)part;

    if (atrb == "TrapReceiver1" ||
        atrb == "TrapReceiver2" ||
        atrb == "TrapReceiver3")
    {
        SnmpCreateTrapSession(mob->TrapReceiver1.GetValue().c_str(),
                              mob->TrapReceiver2.GetValue().c_str(),
                              mob->TrapReceiver3.GetValue().c_str());
    }

    return true;
}

bool SnmpGet(BaseMob* part, std::string& atrb)
{  
    return true;
}

void SnmpInit()
{
    SnmpMob::RegGetCallback(SnmpGet);
    SnmpMob::RegSetCallback(SnmpSet);
    SnmpMob *mob = SnmpMob::AddInst(KeyType::Snmp, 1);
    mob->Load();
}

bool SnmpExe()
{
    SnmpMob* mob = SnmpMob::GetInstByKey(KeyType::Snmp, 1);
    mob->TrapReceiver1.Exe();
    mob->TrapReceiver2.Exe();
    mob->TrapReceiver3.Exe();
    return true;
}

void UserAdminInit()
{
    //UserAdminMob::RegGetCallback(SnmpGet);
    //UserAdminMob::RegSetCallback(SnmpSet);
    //UserAdminMob *mob = UserAdminMob::AddInst(KeyType::User, 1);
    UserAdminMob::AddInst(KeyType::User, 1);
    //mob->Load();
}

void* FileMgmtSwUpgrade(void *param)
{
    if (param == NULL) return NULL;
    FileMgmtMob* pMob = (FileMgmtMob*)param;
    
    char cmdline[256] = {0};
    std::string ipaddress = pMob->ServerIpAddress.GetValue();
    std::string filepath = pMob->FilePathName.GetValue();
    
    pMob->State.SetValue(FileMgmtMob_State_FileTransfer);
    sprintf(cmdline, "tftp -g -l /tmp/mxp_load.tgz -r %s %s; sync;", filepath.c_str(), ipaddress.c_str());
    system(cmdline);
    if (!IsFileExist("/tmp/mxp_load.tgz"))
    {
        pMob->State.SetValue(FileMgmtMob_State_FileTransferFailed);
        pMob->Action.SetValue(FileMgmtMob_Action_None);
        return NULL;
    }

    pMob->State.SetValue(FileMgmtMob_State_Execute);
    sprintf(cmdline, "tar zxvf /tmp/mxp_load.tgz -C /mnt/appfs; sync;");
    system(cmdline);
    system("rm /tmp/mxp_load.tgz");

    pMob->State.SetValue(FileMgmtMob_State_Finished);
    pMob->Action.SetValue(FileMgmtMob_Action_None);

    return NULL;
}

void* ExportDatabaseThread(void *param)
{
    if (param == NULL) return NULL;
    FileMgmtMob* pMob = (FileMgmtMob*)param;
    
    char cmdline[256] = {0};
    std::string ipaddress = pMob->ServerIpAddress.GetValue();
    std::string filepath = pMob->FilePathName.GetValue();

    pMob->State.SetValue(FileMgmtMob_State_Execute);
    sprintf(cmdline, "tar czf /tmp/mxp_database.tgz /mnt/sysfs/*");
    system(cmdline);
    APP_LOG(cmdline);
    
    pMob->State.SetValue(FileMgmtMob_State_FileTransfer);
    sprintf(cmdline, "tftp -p -l /tmp/mxp_database.tgz -r %s %s", filepath.c_str(), ipaddress.c_str());
    system(cmdline);
    APP_LOG(cmdline);

    system("rm /tmp/mxp_database.tgz");

    pMob->State.SetValue(FileMgmtMob_State_Finished);
    pMob->Action.SetValue(FileMgmtMob_Action_None);

    return NULL;
}

void* ImportDatabaseThread(void *param)
{
    if (param == NULL) return NULL;
    FileMgmtMob* pMob = (FileMgmtMob*)param;
    
    char cmdline[256] = {0};
    std::string ipaddress = pMob->ServerIpAddress.GetValue();
    std::string filepath = pMob->FilePathName.GetValue();
    
    pMob->State.SetValue(FileMgmtMob_State_FileTransfer);
    sprintf(cmdline, "tftp -g -l /tmp/mxp_database.tgz -r %s %s; sync;", filepath.c_str(), ipaddress.c_str());
    system(cmdline);
    APP_LOG(cmdline);
    if (!IsFileExist("/tmp/mxp_database.tgz"))
    {
        pMob->State.SetValue(FileMgmtMob_State_FileTransferFailed);
        pMob->Action.SetValue(FileMgmtMob_Action_None);
        return NULL;
    }

    pMob->State.SetValue(FileMgmtMob_State_Execute);
    sprintf(cmdline, "rm /mnt/sysfs/*; tar zxvf /tmp/mxp_database.tgz -C /mnt/sysfs; sync;");
    system(cmdline);
    APP_LOG(cmdline);
    system("rm /tmp/mxp_database.tgz");

    pMob->State.SetValue(FileMgmtMob_State_Finished);
    pMob->Action.SetValue(FileMgmtMob_Action_None);

    return NULL;
}

bool FileMgmtSet(BaseMob* part, std::string& atrb)
{
    FileMgmtMob* pMob = (FileMgmtMob*)part;

    if (atrb == "Action")
    {
        if (pMob->Action.GetValue() == FileMgmtMob_Action_UpgradeSoftware)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, FileMgmtSwUpgrade, pMob);
        }
        else if (pMob->Action.GetValue() == FileMgmtMob_Action_ExportDatabase)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, ExportDatabaseThread, pMob);
        }
        else if (pMob->Action.GetValue() == FileMgmtMob_Action_ImportDatabase)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, ImportDatabaseThread, pMob);
        }
    }

    return true;
}

void FileMgmtInit()
{
    FileMgmtMob *pMob = NULL;

    FileMgmtMob::RegSetCallback(FileMgmtSet);

    pMob = FileMgmtMob::AddInst(KeyType::Card, 1);
    pMob->ServerIpAddress.Set("192.168.1.2");
    pMob->ServerProtocol.Set(FileMgmtMob_ServerProtocol_Tftp);
    //pMob->ServerUsername.Set("");
    //pMob->ServerPassword.Set("");
    pMob->FilePathName.Set("mxp_load.tgz");
    pMob->Action.Set(FileMgmtMob_Action_None);
    pMob->State.Set(FileMgmtMob_State_None);
}

bool InventoryGet(BaseMob* part, std::string& atrb)
{
    InventoryMob* mob = (InventoryMob*)part;

    if (atrb == "Present")
    {
        int index = mob->key.id();
        Inventory_s inv;
        inv.reset();

        if (mob->key.type() == KeyType::Shelf)
        {
            HAL::GetInst().GetShelfInventory(index, inv);
        }
        else if (mob->key.type() == KeyType::Card)
        {
            HAL::GetInst().GetCardInventory(index, inv);
        }
        else if (mob->key.type() == KeyType::Fan)
        {
            HAL::GetInst().GetFanInventory(index, inv);
        }
        else if (mob->key.type() == KeyType::Psu)
        {
            HAL::GetInst().GetPsuInventory(index, inv);
        }
        else if (mob->key.type() == KeyType::Port)
        {
            HAL::GetInst().GetPortInventory(index, inv);
        }
        else if (mob->key.type() == KeyType::Osc)
        {
            HAL::GetInst().GetOscInventory(index, inv);
        }

        mob->Present.SetValue(inv.present ? InventoryMob_Present_Yes : InventoryMob_Present_No);
        mob->ModuleType.SetValue(inv.moduleType);
        mob->PartNumber.SetValue(inv.partNumber);
        mob->SerialNumber.SetValue(inv.serialNumber);
        mob->HwVersion.SetValue(inv.hwVersion);
        mob->VendorName.SetValue(inv.vendorName);
        mob->ProduceDate.SetValue(inv.produceDate);
        mob->FwVersion.SetValue(inv.fwVersion);
    }
    
    return true;
}

void InventoryInit()
{
    InventoryMob::RegGetCallback(InventoryGet);

    InventoryMob::AddInst(KeyType::Shelf, 1);
    InventoryMob::AddInst(KeyType::Card, 1);
    InventoryMob::AddInst(KeyType::Fan, 1);
    InventoryMob::AddInst(KeyType::Psu, 1);    
    InventoryMob::AddInst(KeyType::Psu, 2);

    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        InventoryMob::AddInst(KeyType::Port, port);
    }

    InventoryMob::AddInst(KeyType::Osc, 1);
    InventoryMob::AddInst(KeyType::Osc, 2);
}

void* SysThread(void *param)
{
    char stat_pre[32] = {0};
    char stat_line[128] = {0};
    
    // update system up time
    up_time++;

    // update and calc cpu usage
    static long int pre_all = 0, pre_idle = 0;
    FILE *fp = fopen("/proc/stat","r");  
    if(fp == NULL) return NULL;
    fgets(stat_line, sizeof(stat_line), fp);
    fclose(fp);
    fp = NULL;
    long int user, nice2, sys, idle, iowait, irq, softirq;
    sscanf(stat_line, "%s%ld%ld%ld%ld%ld%ld%ld", stat_pre, &user, &nice2, &sys, &idle,&iowait, &irq, &softirq);
    //printf("%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld, cpu = %d\n",cpu,user,nice2,sys,idle,iowait,irq,softirq, cpu_usage);
    long int all = user + nice2 + sys + idle + iowait + irq + softirq;  
    if ((all - pre_all) != 0)
    {
        cpu_usage = (int)((all - pre_all - (idle - pre_idle)) * 100 / (all - pre_all));
    }
    pre_all = all;
    pre_idle = idle;

    // update memory usage
    long int mem_total = 0, mem_free = 0;
    FILE *fp2 = fopen("/proc/meminfo","r");  
    if(fp2 == NULL) return NULL;
    fgets(stat_line, sizeof(stat_line), fp2);
    sscanf(stat_line, "%s%ld", stat_pre, &mem_total);
    fgets(stat_line, sizeof(stat_line), fp2);
    sscanf(stat_line, "%s%ld", stat_pre, &mem_free);
    fclose(fp2);
    fp2 = NULL;
    mem_usage = (int)((mem_total - mem_free) * 100 / mem_total);
    //APP_LOG("mem_total %ld, mem_free %ld, usage %d", mem_total, mem_free, mem_usage);

    // Reset Button action
    static int reset_cnt = 0;
    static bool reset_pre = false;
    bool reset_state = HAL::GetInst().GetCardResetButton();
    if (reset_pre == true && reset_state == false)
    {
        if (reset_cnt >= 3)
        {
            APP_LOG("Reset IP Address and User");
            NetworkMob *pMob = NetworkMob::GetInst(0);
            pMob->IpAddress.Set("192.168.1.10");
            pMob->Netmask.Set("255.255.255.0");
            pMob->Gateway.Set("192.168.1.1");
            pMob->Save();
            Key key(KeyType::Card, 1);
            HAL::GetInst().SetLedStatus(key,  LedType::PWR, LedState::ON, LedColor::GREEN);
        }
    }
    reset_state ? reset_cnt++ : reset_cnt = 0;
    reset_pre = reset_state;
    if (reset_cnt >= 3)
    {
        APP_LOG("Reset button %d reached", reset_cnt);
        Key key(KeyType::Card, 1);
        if (reset_cnt % 2 == 1)
        {
            HAL::GetInst().SetLedStatus(key,  LedType::PWR, LedState::OFF, LedColor::OFF);
        }
        else
        {
            HAL::GetInst().SetLedStatus(key,  LedType::PWR, LedState::ON, LedColor::GREEN);
        }

        if (reset_cnt >= 10)
        {
            APP_LOG("Clear Database and Reboot");
            HAL::GetInst().SetFanSpeed(100);
            Key key(KeyType::Card, 1);
            HAL::GetInst().SetLedStatus(key,  LedType::PWR, LedState::OFF, LedColor::OFF);
            HAL::GetInst().SetLedStatus(key,  LedType::MAJ, LedState::OFF, LedColor::OFF);
            HAL::GetInst().SetLedStatus(key,  LedType::MIN, LedState::OFF, LedColor::OFF);
#ifdef __TARGET__
            system("rm /mnt/sysfs/*; sync;");
            system("reboot");
#endif
        }
    }
    
    sleep(1);

    return NULL;
}

void* LedThread(void *param)
{
    int max_port = 0;
    HAL::GetInst().GetPortNum(max_port);

    for (int port = 1; port <= max_port; port++)
    {
        OpticsInfo_s info;
        info.reset();
        HAL::GetInst().GetPortOpticsInfo(port, info);

        Key key(KeyType::Port, port);
        HAL::GetInst().SetLedStatus(key, LedType::TX, (info.laser == true) ? LedState::ON : LedState::OFF, LedColor::GREEN);
        HAL::GetInst().SetLedStatus(key, LedType::RX, (info.los == false) ? LedState::ON : LedState::OFF, LedColor::GREEN);
    }

    for (int osc = 1; osc <= 2; osc++)
    {
        OpticsInfo_s info;
        info.reset();
        HAL::GetInst().GetOscOpticsInfo(osc, info);

        Key key(KeyType::Osc, osc);
        HAL::GetInst().SetLedStatus(key, LedType::TX, (info.laser == true) ? LedState::ON : LedState::OFF, LedColor::GREEN);
        HAL::GetInst().SetLedStatus(key, LedType::RX, (info.los == false) ? LedState::ON : LedState::OFF, LedColor::GREEN);
    }

    // update shelf inlet temperature
    int degree = 0;
    HAL::GetInst().GetShelfInletTemperature(degree);
    int low = ShelfMob::GetInst(0)->TempLowThd.GetValue();
    int high = ShelfMob::GetInst(0)->TempHighThd.GetValue();
    static bool low_alarm = false, high_alarm = false;
    bool low_now = (degree < low) ? true : false;
    bool high_now = (degree > high) ? true : false;
    //APP_LOG("low = %d, high = %d, degree = %d", low, high, degree);
    if (low_alarm != low_now)
    {
        Key key(KeyType::Shelf, 1);
        ShelfMob::GetInstByKey(key)->UpdateAlarm(AlarmId::TEMP_LT, low_now);
        low_alarm = low_now;
    }
    if (high_alarm != high_now)
    {
        Key key(KeyType::Shelf, 1);
        ShelfMob::GetInstByKey(key)->UpdateAlarm(AlarmId::TEMP_HT, high_now);
        high_alarm = high_now;
    }

    sleep(1);

    return NULL;
}

void* LosyncFixupThread(void *param)
{
    static int cnt = 0;
    static int fixed = 0;

    if (cnt++ != 10 || fixed == 1)
    {
        sleep(1);
        return NULL;
    }

    uint32_t mask = 0;
    PortMob *mob = NULL;
    int num = PortMob::GetInstNum();
    for (int i = 0; i < num; i++)
    {
        mob = PortMob::GetInst(i);
        if (PortMob_PortMode_ETH10G == mob->PortMode.GetValue())
        {
            APP_LOG("LosyncFixup: turn off %d", i);
            mob->PortMode.Set(PortMob_PortMode_NONE);
            mask |= (1 << i);
        }
    }
    for (int i = 0; i < num; i++)
    {
        if (((1 << i) & mask) == 0) continue;

        APP_LOG("LosyncFixup: turn on %d", i);
        sleep(1);
        mob = PortMob::GetInst(i);
        mob->PortMode.Set(PortMob_PortMode_ETH10G);
    }

    fixed = 1;
    return NULL;
}

void SFMInit()
{
    APP_LOG("SFM init %s %s", __DATE__, __TIME__);

    if (HAL::GetInst().GetCardResetButton() == true)
    {
        APP_LOG("Reset button pressed, Clear database");
        system("rm /mnt/sysfs/*; sync;"); 
    }

    new CThread("SYS", SysThread, NULL);
    PMInit();

    // Init, set default data and load from file
    SystemInit();
    ShelfInit();
    CardInit();
    FanInit();
    PsuInit();
    OpticsInit();
    NetworkInit();
    FileMgmtInit();
    InventoryInit();
    PortInit();
    EthInit();
    OtuInit();
    OchInit();
    StmInit();
    SnmpInit();
    UserAdminInit();

    // Populate, execute action by current data
    //SystemExe();
    //ShelfExe();
    //CardExe();
    NetworkExe();
    //FileMgmtExe();
    //InventoryExe();
    PortExe();
    //EthExe();
    OtuExe();
    OchExe();
    SnmpExe();

    new CThread("LED", LedThread, NULL);
    new CThread("LosyncFixup", LosyncFixupThread, NULL);
}

void SFMMenu(int argc, char *argv[])
{
}

