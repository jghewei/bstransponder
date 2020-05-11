#ifndef _HAL_API_H_
#define _HAL_API_H_

#include "HAL.h"

class HalApi : public HAL
{
public:

    virtual bool InitCold();
    //virtual bool InitWarm();
    //virtual bool ShutdownCold();
    //virtual bool ShutdownWarm();
    virtual bool HwMenu(int argc, char *argv[]);

    // System
    virtual bool SetLampTest(bool enable);
    virtual bool SetLedStatus(Key &key, LedType::Enum type, LedState::Enum state, LedColor::Enum color);

    // Network
    virtual bool SetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw);
    virtual bool GetMgmtIfcfg(std::string &ip, std::string &mask, std::string &gw);
    virtual bool SetOscIfcfg(std::string &ip, std::string &mask, std::string &gw);
    virtual bool GetOscIfcfg(std::string &ip, std::string &mask, std::string &gw);

    // Shelf
    virtual bool GetShelfInletTemperature(int &degree);
    virtual bool GetShelfOutletTemperature(int &degree);

    // Card
    virtual bool GetCardTemperature(int &degree);
    virtual bool GetCardResetButton();

    // Fan, <=0 auto, 0-100 speed value
    virtual bool SetFanSpeed(int speed);
    virtual bool GetFanSpeed(int &speed);

    // Optics
    virtual bool GetPortOpticsInfo(int id, OpticsInfo_s &info);
    virtual bool GetOscOpticsInfo(int id, OpticsInfo_s &info);

    // Port
    virtual bool GetPortNum(int &num);
    virtual bool SetPortMode(int id, PortMode::Enum mode);
    virtual bool GetPortMode(int id, PortMode::Enum &mode);
    virtual bool SetLaserEnable(int id, bool enable);
    virtual bool SetLoopbackType(int id, LoopbackType::Enum type);

    // Eth
    virtual bool GetEthCounter(int id, EthCounter *cnt);

    // Otn
    virtual bool SetOtuFecMode(int id, FecType::Enum type);
    //virtual bool SetOtuTimDefectMode(int id, TimDefectMode::Enum mode);
    //virtual bool SetOtuTxSAPI(int id, std::string &sapi);
    //virtual bool SetOtuTxDAPI(int id, std::string &dapi);
    //virtual bool SetOtuTxOPER(int id, std::string &oper);
    //virtual bool GetOtuRxSAPI(int id, std::string &sapi);
    //virtual bool GetOtuRxDAPI(int id, std::string &dapi);
    //virtual bool GetOtuRxOPER(int id, std::string &oper);
    //virtual bool SetOtuExpSAPI(int id, std::string &sapi);
    //virtual bool SetOtuExpDAPI(int id, std::string &dapi);
    //virtual bool SetOtuExpOPER(int id, std::string &oper);
    virtual bool SetOtuPrbsEnable(int id, bool enable);
    virtual bool GetOtuPrbsStatus(int id, PrbsStatus::Enum &status);
    virtual bool GetOtuCounter(int id, OtuCounter *cnt);

    // Och
    virtual bool SetOchFecMode(int id, FecType::Enum type);
    virtual bool SetOchChan(int id, int chan);
    virtual bool GetOchMinFreq(int id, std::string &freq);
    virtual bool GetOchMaxFreq(int id, std::string &freq);
    virtual bool GetOchGridSpacing(int id, std::string &freq);
    virtual bool GetOchMaxChanNum(int id, std::string &num);
    virtual bool GetOchCurTxFreq(int id, std::string &freq);
    virtual bool GetOchCurRxFreq(int id, std::string &freq);
    virtual bool GetOchDGD(int id, std::string &dgd);
    virtual bool GetOchCD(int id, std::string &cd);
    virtual bool GetOchOSNR(int id, std::string &osnr);
    virtual bool GetOchQfactor(int id, std::string &qfactor);
    virtual bool GetOchCounter(int id, OchCounter *cnt);

    // Inventory
    //virtual bool GetShelfInventory(int id, Inventory_s &inv);
    virtual bool GetCardInventory(int id, Inventory_s &inv);
    //virtual bool GetFanInventory(int id, Inventory_s &inv);
    //virtual bool GetPsuInventory(int id, Inventory_s &inv);
    virtual bool GetPortInventory(int id, Inventory_s &inv);
    virtual bool GetOscInventory(int id, Inventory_s &inv);


    virtual bool GetOptLos(int id, bool &los);

    //virtual bool GetUpTime(int &time);
    //virtual bool GetMemUsage(int &percent);
    //virtual bool GetDiskUsage(int &percent);

};

#endif

