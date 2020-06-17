#ifndef _MSA_CFP_H_
#define _MSA_CFP_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "CommonType.h"

namespace MsaCfpApi
{
    const uint8_t VENDOR_NAME_LEN = 16;
    const uint8_t VENDOR_PN_LEN   = 16;
    const uint8_t VENDOR_SN_LEN   = 16;
    const uint8_t DATE_CODE_LEN   = 8;
    const uint8_t CLEI_CODE_LEN   = 10;

    const uint16_t HW_VER_ADDR    = 0x806A;
    const uint16_t FW_VER_ADDR    = 0x806C;
    const uint16_t FWB_VER_ADDR   = 0x807B;

    const int NUM_INST            = 2;
    const uint8_t devAddr[NUM_INST] = {0x3, 0x7};
}

namespace CfpState
{
    enum Enum
    {
        Reset,
        Initialize,
        LowPower,
        HighPowerUp,
        TxOff,
        TxTurnOn,
        Ready,
        Fault,
        TxTurnOff,
        HighPowerDown,
    };
}
#define CfpState_to_String(x) \
        ((x) == CfpState::Reset         ? "Reset"           : \
         (x) == CfpState::Initialize    ? "Initialize"      : \
         (x) == CfpState::LowPower      ? "LowPower"        : \
         (x) == CfpState::HighPowerUp   ? "HighPowerUp"     : \
         (x) == CfpState::TxOff         ? "TxOff"           : \
         (x) == CfpState::TxTurnOn      ? "TxTurnOn"        : \
         (x) == CfpState::Ready         ? "Ready"           : \
         (x) == CfpState::Fault         ? "Fault"           : \
         (x) == CfpState::TxTurnOff     ? "TxTurnOff"       : \
         (x) == CfpState::HighPowerDown ? "HighPowerDown"   : "?" )

class MsaCfp
{
public:
    static MsaCfp& GetInst(uint32_t inst);
    static void DelInst(uint32_t inst);

    MsaCfp(int dev);
    ~MsaCfp() {};

    void InitCold();
    void InitWarm() {}
    void ShutdownCold() {}
    void ShutdownWarm() {}

    // inventory
    char CheckPrint(uint8_t val);
    void UpdateInventory();
    std::string GetVendorName() { return mVendorName; }
    std::string GetPartNumber() { return mPartNumber; }
    std::string GetSerialNumber() { return mSerialNumber; }
    std::string GetDateCode() { return mDateCode; }
    std::string GetHwVersion() { return mHwVersion; }
    std::string GetFwVersion() { return mFwVersion; }
    std::string GetFwBVersion() { return mFwBVersion; }

    // Diag
    float GetTxPower();
    float GetRxPower();
    float GetTemperature();
    CfpState::Enum GetModuleState();
    void StateMachine(void);

    // Eqpt
    bool GetPresent();
    bool GetLos();
    bool GetLaserOn();
    void SetLaserOn(bool on);
    void SetSignalType(PortMode::Enum sigType);

    // Debug
    void Dump();
    void Menu(int argc, char *argv[]);

    // Coherent
    bool SetFecMode(FecType::Enum type);
    bool SetChan(int chan);
    std::string GetMinFreq();
    std::string GetMaxFreq();
    std::string GetGridSpacing();
    std::string GetMaxChanNum();
    std::string GetCurTxFreq();
    std::string GetCurRxFreq();
    std::string GetDGD();
    std::string GetCD();
    std::string GetOSNR();
    std::string GetQfactor();
    bool GetFecCounter(OchCounter *cnt);
	bool isMenaraCfp(){ return mIsMenara;}
	bool isHisiliconCfp(){ return mIsHisilicon;}

private:
    uint16_t read(uint16_t reg);
    void write(uint16_t reg, uint16_t val);

    double convertMWToDBM(double theNumber);

    void SetModRst(bool active);
    void SetModLowPwr(bool active);
    void SetTxDis(bool active);
    void SetConfig();

private:
    static std::vector<MsaCfp *> mInst;
    static int mInstNum;

    int mDevId;
    bool mIsAcacia;
	bool mIsMenara;
	bool mIsHisilicon;

    std::string mVendorName;
    std::string mPartNumber;
    std::string mSerialNumber;
    std::string mDateCode;
    std::string mHwVersion;
    std::string mFwVersion;
    std::string mFwBVersion;

    bool mPresent;
    bool mLaserOn;
    PortMode::Enum mPortMode;
    CfpState::Enum mState;
    uint16_t *mReg;

    FecType::Enum mFecType;
    int mChan;
};

#endif  // _MSA_CFP_H_

