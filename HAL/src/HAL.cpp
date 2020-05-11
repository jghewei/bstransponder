#include "HAL.h"
#ifdef __TARGET__
#include "HalApi.h"
#endif

HAL* HAL::mInst = NULL;

bool HALInit()
{
#ifdef __TARGET__
    HAL::SetInst(new HalApi());
#else
    HAL::SetInst(new HAL());
#endif

    return HAL::GetInst().InitCold();
}

void HALMenu(int argc, char *argv[])
{
    if (argc == 1)
    {
        APP_LOG("hal menu usage:");
        APP_LOG("\t hal info");
        APP_LOG("\t hal crash");
        APP_LOG("\t hal debug");
        APP_LOG("\t hal alarm <eth|otu|odu|stm|fan|shelf> <id 1> <AlarmId::Enum> <active 0|1>");
    }
    else
    {
        if (strcmp(argv[1], "info") == 0 && argc == 2)
        {
        }
        else if (strcmp(argv[1], "crash") == 0 && argc == 2)
        {
            int *p = NULL;
            *p = 0x55aa;
        }
        else if (strcmp(argv[1], "alarm") == 0 && argc == 6)
        {
            int ins = atoi(argv[3]);
            int aid = atoi(argv[4]);
            int act = atoi(argv[5]);
            if (strcmp(argv[2], "eth") == 0)
                HAL::GetInst().SendEthEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "otu") == 0)
                HAL::GetInst().SendOtuEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "odu") == 0)
                HAL::GetInst().SendOduEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "stm") == 0)
                HAL::GetInst().SendStmEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "fan") == 0)
                HAL::GetInst().SendFanEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "pwr") == 0)
                HAL::GetInst().SendPsuEvent(ins, (AlarmId::Enum)aid, act == 1);
            else if (strcmp(argv[2], "shelf") == 0)
                HAL::GetInst().SendShelfEvent(ins, (AlarmId::Enum)aid, act == 1);
        }
    }
}

void HwMenu(int argc, char *argv[])
{
    HAL::GetInst().HwMenu(argc, argv);
}

HAL::HAL()
{
    eth_cb_ = NULL;
    otu_cb_ = NULL;
    odu_cb_ = NULL;
}

bool HAL::InitCold() { APP_LOG_LINE(); return true; }
bool HAL::InitWarm() { APP_LOG_LINE(); return true; }
bool HAL::ShutdownCold() { APP_LOG_LINE(); return true; }
bool HAL::ShutdownWarm() { APP_LOG_LINE(); return true; }
bool HAL::HwMenu(int argc, char *argv[]) { APP_LOG_LINE(); return true; }

// System
bool HAL::SetLampTest(bool enable) { APP_LOG_LINE(); return true; }
bool HAL::SetLedStatus(Key &key, LedType::Enum type, LedState::Enum state, LedColor::Enum color) { /*APP_LOG_LINE();*/ return true; }

// Network
bool HAL::SetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw) { APP_LOG_LINE(); return true; }
bool HAL::GetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    ip = "192.168.1.10";
    mask = "255.255.255.0";
    gw = "192.168.1.1";
    return true;
}
bool HAL::SetOscIfcfg(std::string &ip, std::string &mask, std::string &gw) { APP_LOG_LINE(); return true; }
bool HAL::GetOscIfcfg(std::string &ip, std::string &mask, std::string &gw)
{
    ip = "192.168.1.10";
    mask = "255.255.255.0";
    gw = "192.168.1.1";
    return true;
}

// Shelf
bool HAL::GetShelfInletTemperature(int &degree) { return true; }
bool HAL::GetShelfOutletTemperature(int &degree) { return true; }

// Card
bool HAL::GetCardTemperature(int &degree)
{
    static int tmp = 0;
    degree = tmp++;
    return true;
}

bool HAL::GetCardResetButton() { return false; }

// Fan
bool HAL::SetFanSpeed(int speed) { return true; }
bool HAL::GetFanSpeed(int &speed) { speed = 60; return true; }

// Optics
bool HAL::GetPortOpticsInfo(int id, OpticsInfo_s &info) { return true; }
bool HAL::GetOscOpticsInfo(int id, OpticsInfo_s &info) { return true; }

// Port
bool HAL::GetPortNum(int &num) { /*APP_LOG_LINE();*/ num = 11; return true; }
bool HAL::SetPortMode(int id, PortMode::Enum mode) { APP_LOG_LINE(); return true; }
bool HAL::GetPortMode(int id, PortMode::Enum &mode) { APP_LOG_LINE(); mode = PortMode::NONE; return true; }
bool HAL::SetLaserEnable(int id, bool enable) { APP_LOG_LINE(); return true; }
bool HAL::SetLoopbackType(int id, LoopbackType::Enum type) { APP_LOG_LINE(); return true; }

// Eth
bool HAL::GetEthCounter(int id, EthCounter *cnt) { return true; }

// Otu
bool HAL::SetOtuFecMode(int id, FecType::Enum type) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuTimDefectMode(int id, TimDefectMode::Enum mode) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuTxSAPI(int id, std::string &sapi) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuTxDAPI(int id, std::string &dapi) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuTxOPER(int id, std::string &oper) { APP_LOG_LINE(); return true; }
bool HAL::GetOtuRxSAPI(int id, std::string &sapi) { APP_LOG_LINE(); return true; }
bool HAL::GetOtuRxDAPI(int id, std::string &dapi) { APP_LOG_LINE(); return true; }
bool HAL::GetOtuRxOPER(int id, std::string &oper) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuExpSAPI(int id, std::string &sapi) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuExpDAPI(int id, std::string &dapi) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuExpOPER(int id, std::string &oper) { APP_LOG_LINE(); return true; }
bool HAL::SetOtuPrbsEnable(int id, bool enable) { APP_LOG_LINE(); return true; }
bool HAL::GetOtuPrbsStatus(int id, PrbsStatus::Enum &status) { APP_LOG_LINE(); return true; }
bool HAL::GetOtuCounter(int id, OtuCounter *cnt)
{
    static OtuCounter otu_counter;
    otu_counter.frameCount += 100;
    otu_counter.bip8 += 10;
    cnt->frameCount = otu_counter.frameCount;
    cnt->bip8 = otu_counter.bip8;
    return true;
}

// Och
bool HAL::SetOchFecMode(int id, FecType::Enum type) { APP_LOG_LINE(); return true; }
bool HAL::SetOchChan(int id, int chan) { APP_LOG_LINE(); return true; }
bool HAL::GetOchMinFreq(int id, std::string &freq) { APP_LOG_LINE(); return true; }
bool HAL::GetOchMaxFreq(int id, std::string &freq) { APP_LOG_LINE(); return true; }
bool HAL::GetOchGridSpacing(int id, std::string &freq) { APP_LOG_LINE(); return true; }
bool HAL::GetOchMaxChanNum(int id, std::string &num) { APP_LOG_LINE(); return true; }
bool HAL::GetOchCurTxFreq(int id, std::string &freq) { APP_LOG_LINE(); return true; }
bool HAL::GetOchCurRxFreq(int id, std::string &freq) { APP_LOG_LINE(); return true; }
bool HAL::GetOchDGD(int id, std::string &dgd) { APP_LOG_LINE(); return true; }
bool HAL::GetOchCD(int id, std::string &cd) { APP_LOG_LINE(); return true; }
bool HAL::GetOchOSNR(int id, std::string &osnr) { APP_LOG_LINE(); return true; }
bool HAL::GetOchQfactor(int id, std::string &qfactor) { APP_LOG_LINE(); return true; }
bool HAL::GetOchCounter(int id, OchCounter *cnt) { APP_LOG_LINE(); return true; }

// Inventory
bool HAL::GetShelfInventory(int id, Inventory_s &inv)
{
    inv.present = true;
    //inv.partNumber = "CTX2000";
    //inv.vendorName = "DanRiver";
    //inv.partNumber = "eWAVE5110";
    //inv.vendorName = "CNMP";
    APP_LOG_LINE(); return true; 
}

bool HAL::GetCardInventory(int id, Inventory_s &inv)
{
    inv.present = true;
    inv.partNumber = "CTX2000";
    inv.vendorName = "DanRiver";
    //inv.partNumber = "eWAVE5110";
    //inv.vendorName = "CNMP";
    APP_LOG_LINE(); return true;
}

bool HAL::GetFanInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
bool HAL::GetPsuInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
bool HAL::GetPortInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }
bool HAL::GetOscInventory(int id, Inventory_s &inv) { APP_LOG_LINE(); return true; }

// Event Callback
void HAL::RegEthCallback(EventCallback cb) { eth_cb_ = cb; }
void HAL::SendEthEvent(int id, AlarmId::Enum alm, bool active)
{
    if (eth_cb_) eth_cb_(id, alm, active);
}
void HAL::RegOtuCallback(EventCallback cb) { otu_cb_ = cb; }
void HAL::SendOtuEvent(int id, AlarmId::Enum alm, bool active)
{
    if (otu_cb_) otu_cb_(id, alm, active);
}
void HAL::RegOduCallback(EventCallback cb) { odu_cb_ = cb; }
void HAL::SendOduEvent(int id, AlarmId::Enum alm, bool active)
{
    if (odu_cb_) odu_cb_(id, alm, active);
}
void HAL::RegStmCallback(EventCallback cb) { stm_cb_ = cb; }
void HAL::SendStmEvent(int id, AlarmId::Enum alm, bool active)
{
    if (stm_cb_) stm_cb_(id, alm, active);
}
void HAL::RegFanCallback(EventCallback cb) { fan_cb_ = cb; }
void HAL::SendFanEvent(int id, AlarmId::Enum alm, bool active)
{
    if (fan_cb_) fan_cb_(id, alm, active);
}
void HAL::RegPsuCallback(EventCallback cb) { psu_cb_ = cb; }
void HAL::SendPsuEvent(int id, AlarmId::Enum alm, bool active)
{
    if (psu_cb_) psu_cb_(id, alm, active);
}
void HAL::RegShelfCallback(EventCallback cb) { shelf_cb_ = cb; }
void HAL::SendShelfEvent(int id, AlarmId::Enum alm, bool active)
{
    if (shelf_cb_) shelf_cb_(id, alm, active);
}

bool HAL::GetOptLos(int id, bool &los)
{
    if (id % 2 == 0)
        los = false;
    else
        los = true;
    return true;
}

