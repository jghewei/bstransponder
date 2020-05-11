#ifndef _MSA_SFP_H_
#define _MSA_SFP_H_

#include <vector>

namespace MsaSfpApi
{
    const int SFP_I2C_BUS           = 1;
    const uint8_t SFP_I2C_ADDR      = 0x50;
    const uint8_t SFP_DDM_I2C_ADDR  = 0x51;

    const uint8_t VENDOR_NAME_LEN   = 16;
    const uint8_t VENDOR_PN_LEN     = 16;
    const uint8_t VENDOR_SN_LEN     = 16;
    const uint8_t VENDOR_REV_LEN    = 4;
    const uint8_t DATE_CODE_LEN     = 8;

    const int NUM_INST              = 12;

    const float INVALID_POWER       = -99.0;
    const unsigned int RX_PWR_DATA  = 5;
}

class MsaSfp
{
public:
    static MsaSfp& GetInst(uint32_t inst);
    static void DelInst(uint32_t inst);

    MsaSfp(int dev);
    ~MsaSfp() {};

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

    // Eqpt
    bool GetPresent();
    bool GetLos();
    bool GetLaserOn();
    void SetLaserOn(bool on);
    bool GetEthTransceiver() { return isEthTransceiver; }
    void StateMachine(void);

    // Diag
    float GetTxPower();
    float GetRxPower();
    float GetTemperature();

    // Debug
    void Dump();
    void Menu(int argc, char *argv[]);

private:
    uint8_t read(uint8_t reg);
    void write(uint8_t reg, uint8_t val);
    uint8_t ddm_read(uint8_t reg);
    void ddm_write(uint8_t reg, uint8_t val);
    uint16_t ddm_read16(uint8_t reg);
    uint32_t ddm_read32(uint8_t reg);

    float getTxPwr();
    float getRxPwr();
    bool getDdmImplementation();
    bool getInternalCalibration();

private:
    static std::vector<MsaSfp *> mInst;
    static int mInstNum;

    int mDevId;
    uint16_t i2cSel;

    std::string mVendorName;
    std::string mPartNumber;
    std::string mSerialNumber;
    std::string mDateCode;
    std::string mHwVersion;
    bool mInternalCalibration;
    bool isEthTransceiver;
    bool mPresent;
};

#endif  // _MSA_SFP_H_

