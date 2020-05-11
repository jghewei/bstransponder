#include "DanOS.h"
#include "Fpga.h"
#include "Mdio.h"
#include "MsaCfp.h"
#include <math.h>
#include "Digi.h"


using namespace std;
using namespace MsaCfpApi;

const uint32_t cfpCtrlReg[2] = {0x08, 0x0C};
const uint32_t cfpAlarmReg[2] = {0x06, 0x0A};

#define NTWK_TX_CTRL        0
#define NTWK_RX_CTRL        1
#define MODULE_TEMP         2
#define MODULE_STATE        3
#define NTWK_TX_POWER       4
#define NTWK_RX_POWER       5
uint16_t cfpReg[3][6] =
{
    {0xA011, 0xA012, 0xA02F, 0xA016, 0xA2B0, 0xA2D0},
    {0xB011, 0xB012, 0x9094, 0xB016, 0xB4A0, 0xB4E0},
    {0xB011, 0xB012, 0xB02F, 0xB016, 0xB4A0, 0xB4E0},
};

std::vector<MsaCfp *> MsaCfp::mInst(NUM_INST, (MsaCfp *)0);
int MsaCfp::mInstNum = 0;

MsaCfp& MsaCfp::GetInst(uint32_t inst)
{
    if (inst >= mInst.size())
    {
        std::cout << __FUNCTION__ << " invalid instance number " << inst << std::endl;
        inst = 0;
    }

    if (mInst[inst] == NULL)
    {
        mInst.at(inst) = new MsaCfp(inst);
        mInstNum++;
    }

    return(*(mInst.at(inst)));
}

void MsaCfp::DelInst(uint32_t inst)
{
    if (inst < mInst.size())
    {
        if(mInst.at(inst) != NULL)
        {
            delete mInst.at(inst);
            mInst.at(inst) = NULL;
            mInstNum--;
        }
    }
}

MsaCfp::MsaCfp(int dev) : mDevId(dev), mIsAcacia(false), mIsMenara(false),
    mPresent(false), mLaserOn(false), mPortMode(PortMode::NONE),
    mState(CfpState::Reset), mReg(&cfpReg[0][0]), mFecType(FecType::NONE),
    mChan(0)
{
}

void MsaCfp::write(uint16_t reg, uint16_t val)
{
    MdioWrite(devAddr[mDevId], 0x1, reg, val);
}

uint16_t MsaCfp::read(uint16_t reg)
{
    return MdioRead(devAddr[mDevId], 0x1, reg);
}

void MsaCfp::InitCold()
{
    SetModRst(true);
    SetModLowPwr(true);
    SetTxDis(true);
    FpgaWrite(cfpAlarmReg[mDevId], 0x7);
    sleep(1);
    SetModRst(false);
    sleep(3);
    UpdateInventory();
    Dump();
    mState = GetModuleState();
    if (strncmp(mPartNumber.c_str(), "AC100", 5) == 0)
    {
        APP_LOG("MsaCfp AC100 InitCold");
        Digi::GetInst().SetFecType(11, FecType::NONE);
        mIsAcacia = true;
        mIsMenara = false;
        mReg = &cfpReg[1][0];
    }
    else if (strncmp(mVendorName.c_str(), "eWAVE", 5) == 0)
    {
        APP_LOG("MsaCfp eWAVE InitCold");
        Digi::GetInst().SetFecType(11, FecType::GFEC);
        mIsMenara = true;
        mIsAcacia = false;
        mReg = &cfpReg[2][0];
    }
    else
    {
        APP_LOG("MsaCfp Grey InitCold");
        Digi::GetInst().SetFecType(11, FecType::NONE);
        mIsAcacia = false;
        mIsMenara = false;
        mReg = &cfpReg[0][0];
    }
}

char MsaCfp::CheckPrint(uint8_t val)
{
    if (isprint(val))
        return (char)val;
    else
        return ' ';
}

void MsaCfp::UpdateInventory()
{
    mVendorName.resize(VENDOR_NAME_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_NAME_LEN; i++)
    {
        mVendorName[i] = CheckPrint(read(0x8021 + i));
    }

    mPartNumber.resize(VENDOR_PN_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_PN_LEN; i++)
    {
        mPartNumber[i] = CheckPrint(read(0x8034 + i));
    }

    mSerialNumber.resize(VENDOR_SN_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_SN_LEN; i++)
    {
        mSerialNumber[i] = CheckPrint(read(0x8044 + i));
    }
    
    mDateCode.resize(DATE_CODE_LEN, ' ');
    for(uint8_t i = 0; i < DATE_CODE_LEN; i++)
    {
        mDateCode[i] = CheckPrint(read(0x8054 + i));
    }
    
    mHwVersion = "";
    mHwVersion += CheckPrint(read(HW_VER_ADDR) + '0');
    mHwVersion += ".";
    mHwVersion += CheckPrint(read(HW_VER_ADDR + 1) + '0');
    
    mFwVersion = "";
    mFwVersion += CheckPrint(read(FW_VER_ADDR) + '0');
    mFwVersion += ".";
    mFwVersion += CheckPrint(read(FW_VER_ADDR + 1) + '0');
    
    mFwBVersion = "";
    mFwBVersion += CheckPrint(read(FWB_VER_ADDR) + '0');
    mFwBVersion += ".";
    mFwBVersion += CheckPrint(read(FWB_VER_ADDR + 1) + '0');
}

double MsaCfp::convertMWToDBM(double theNumber)
{
    double mW_Value;
    double dBm_Value = -99.00;
    uint32_t aTemp;

    mW_Value = theNumber;

    // check for zero input using a 0.0001mW granularity
    aTemp = static_cast<uint32_t>(theNumber * 10000.0);
    if(aTemp != 0)
    {
        // multiply by 10 to get dBm
        dBm_Value = (log10(mW_Value) * 10.0);
    }

    return (dBm_Value);
}

void MsaCfp::SetModRst(bool active)
{
    if (active)
        FpgaAndBitMask(cfpCtrlReg[mDevId], 0x4);
    else
        FpgaOrBitMask(cfpCtrlReg[mDevId], 0x4);
}

void MsaCfp::SetModLowPwr(bool active)
{
    if (active)
        FpgaOrBitMask(cfpCtrlReg[mDevId], 0x2);
    else
        FpgaAndBitMask(cfpCtrlReg[mDevId], 0x2);
}

void MsaCfp::SetTxDis(bool active)
{
    if (active)
        FpgaOrBitMask(cfpCtrlReg[mDevId], 0x1);
    else
        FpgaAndBitMask(cfpCtrlReg[mDevId], 0x1);
}

void MsaCfp::SetConfig(void)
{
    if (mIsAcacia)
    {
        if (mPortMode == PortMode::OTU4)
        {
            write(mReg[NTWK_TX_CTRL], 0x1007);
            write(mReg[NTWK_RX_CTRL], 0x1007);
        }
        else if(mPortMode == PortMode::ETH100G)
        {
            write(mReg[NTWK_TX_CTRL], 0x1001);
            write(mReg[NTWK_RX_CTRL], 0x1001);
        }

        write(0x90a4, 0x0002);
        write(0x9020, 0x1a00);
        write(0x9080, 0x000b);
        write(0xb00b, 0x3000);
        write(0x901E, 0x0006);
        write(0xB410, 0x00C8);

        {   // FecMode
            uint16_t val = 0;
            if (mFecType == FecType::SDFEC_ENHANCE)
                val = 0;
            else if (mFecType == FecType::SDFEC_COMP)
                val = 1;
            else if (mFecType == FecType::GFEC)
                val = 2;
            else if (mFecType == FecType::TRANSPARENT)
                val = 3;
            write(0x900B, val);
        }

        {   // Chan
            uint16_t val = 0;
            val = read(0xB400);
            val &= 0xE000;
            val += mChan;
            write(0xB400, val);
        }
    }
    else if (mIsMenara)
    {
        write(0x9089, 0x5353);
        write(0x908a, 0x4657);
        
        if (mPortMode == PortMode::OTU4)
        {
            write(mReg[NTWK_TX_CTRL], 0x0207);
            write(mReg[NTWK_RX_CTRL], 0x0207);

            write(0x9231, 0x0002);
            write(0x9232, 0x0001);
            write(0x9233, 0x006e);
            write(0x9238, 0x0020);
            
        }
        else if(mPortMode == PortMode::ETH100G)
        {
            write(mReg[NTWK_TX_CTRL], 0x0201);
            write(mReg[NTWK_RX_CTRL], 0x0201);
            
            write(0x9231, 0x0002);
            write(0x9232, 0x0000);
            write(0x9233, 0x0072);
            write(0x9238, 0x0020);
        }

        {   // Chan
            uint16_t val = 0;
            //val = read(0xB400);
            //val &= 0xE000;
            val = 0x2000;
            val += mChan;
            write(0xB400, val);
        }
    }
    else
    {
        if (mPortMode == PortMode::OTU4)
        {
            write(mReg[NTWK_TX_CTRL], 0x0207);
            write(mReg[NTWK_RX_CTRL], 0x0207);
        }
        else if(mPortMode == PortMode::ETH100G)
        {
            write(mReg[NTWK_TX_CTRL], 0x0201);
            write(mReg[NTWK_RX_CTRL], 0x0201);
        }
    }
}

void MsaCfp::StateMachine(void)
{
    bool present = GetPresent();
    if (present != mPresent)
    {
        APP_LOG("MsaCfp %d, Present Change To: %s", mDevId, present ? "true" : "false");
        if (present)
        {
            InitCold();
        }
        else
        {
            SetModRst(true);
        }
        mPresent = present;
    }
    if (mPresent == false)
    {
        return;
    }
    
    CfpState::Enum state = GetModuleState();
    if (state != mState)
    {
        APP_LOG("MsaCfp %d, State Change: %s -> %s", mDevId, CfpState_to_String(mState), CfpState_to_String(state));
        mState = state;
    }

    switch(state)
    {
        case CfpState::LowPower:
            if (mPortMode == PortMode::OTU4 || mPortMode == PortMode::ETH100G)
            {
                SetConfig();
                SetModLowPwr(false);
            }
            break;
        
        case CfpState::TxOff:
            if (mLaserOn == true) SetTxDis(false);
            break;
        
        case CfpState::Ready:
            if (mLaserOn == false) SetTxDis(true);
            break;
        
        case CfpState::Fault:
            InitCold();
            break;
            
        case CfpState::Reset:
        case CfpState::Initialize:
        case CfpState::HighPowerUp:
        case CfpState::TxTurnOn:
        case CfpState::TxTurnOff:
        case CfpState::HighPowerDown:
        default:
            break;
    }
}

float MsaCfp::GetTxPower()
{
    double aRetVal = 0;
    double aTemp;

    if ((!mIsAcacia)&&(!mIsMenara))
    {
        // read value is in 0.1uW increments
        // divide by 10 then divide by 1000 to get mW
        aTemp = (read(mReg[NTWK_TX_POWER]) / 10.0) / 1000.0;
        aRetVal = convertMWToDBM(aTemp);
    }
    else
    {
        double dDivisor = 100.0;
        double dRoundFactor = 0.005;  // 100th accuracy
        int16_t iTemp;

        // read value is in 0.01dBm or 0.1dBm increments based on OIF version
        iTemp = static_cast<int16_t>(read(mReg[NTWK_TX_POWER]));

        if(iTemp < 0)
        {
            dRoundFactor *= -1;
        }

        aRetVal = (iTemp / dDivisor) + dRoundFactor;

#if 0
        if(iTemp != 0)
        {
            aRetVal = (iTemp / dDivisor) + dRoundFactor;
        }
        else
        {
            aRetVal = -99.0;
        }
#endif
    }

    return (float)aRetVal;
}

float MsaCfp::GetRxPower()
{
    double aRetVal = 0;
    double aTemp;

    if ((!mIsAcacia)&&(!mIsMenara))
    {
        // read value is in 0.1uW increments
        // divide by 10 then divide by 1000 to get mW
        aTemp = (read(mReg[NTWK_RX_POWER]) / 10.0) / 1000.0; 
        aRetVal = convertMWToDBM(aTemp);
    }
    else
    {
        double dDivisor = 100.0;
        double dRoundFactor = 0.005;  // 100th accuracy
        int16_t iTemp;

        // read value is in 0.01dBm or 0.1dBm increments based on OIF version
        iTemp = static_cast<int16_t>(read(mReg[NTWK_RX_POWER]));

        if(iTemp < 0)
        {
            dRoundFactor *= -1;
        }

        if(iTemp != 0)
        {
            aRetVal = (iTemp / dDivisor) + dRoundFactor;
        }
        else
        {
            aRetVal = -99.0;
        }
    }

    return (float)aRetVal;
}

float MsaCfp::GetTemperature()
{
    float temp;
    int16_t val = (int16_t)read(mReg[MODULE_TEMP]);
    temp = val / 256.0F;

    return temp;
}

CfpState::Enum MsaCfp::GetModuleState()
{
    CfpState::Enum state = CfpState::Reset;
    uint16_t st = read(mReg[MODULE_STATE]);
    for (int i = 0; i < 9; i++)
    {
        if ((st & (0x1 << i)) != 0)
        {
            state = (CfpState::Enum)((int)state + i + 1);
            break;
        }
    }

    return state;
}

bool MsaCfp::GetPresent()
{
    return FpgaGetCfpPresent(mDevId);
}

bool MsaCfp::GetLos()
{
    return FpgaGetCfpLos(mDevId);
}

bool MsaCfp::GetLaserOn()
{
    return FpgaGetCfpLaserOn(mDevId);
}

void MsaCfp::SetLaserOn(bool on)
{
    mLaserOn = on;
    if (on == false)
    {
        SetTxDis(true);
    }
}

void MsaCfp::SetSignalType(PortMode::Enum sigType)
{
    if (sigType != mPortMode)
    {
        mPortMode = sigType;
        SetModLowPwr(true);
    }
}

bool MsaCfp::SetFecMode(FecType::Enum type)
{
    if (type != mFecType)
    {
        mFecType = type;
        SetModLowPwr(true);
    }
    return true;
}

bool MsaCfp::SetChan(int chan)
{
    if (chan != mChan)
    {
        mChan = chan;
        SetModLowPwr(true);
    }
    return true;
}

std::string MsaCfp::GetMinFreq()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";
    uint16_t v1 = read(0x818A) & 0xFF;
    uint16_t v2 = read(0x818B) & 0xFF;
    uint16_t v3 = read(0x818C) & 0xFF;
    uint16_t v4 = read(0x818D) & 0xFF;
    uint32_t d1 = (v1 << 8) | v2;
    uint32_t d2 = (v3 << 8) | v4;
    char str[32] = {0};
    sprintf(str, "%d.%03d THz", (int)d1, (int)(d2/20));
    return str;    
}

std::string MsaCfp::GetMaxFreq()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";
    uint16_t v1 = read(0x818E) & 0xFF;
    uint16_t v2 = read(0x818F) & 0xFF;
    uint16_t v3 = read(0x8190) & 0xFF;
    uint16_t v4 = read(0x8191) & 0xFF;
    uint32_t d1 = (v1 << 8) | v2;
    uint32_t d2 = (v3 << 8) | v4;
    char str[32] = {0};
    sprintf(str, "%d.%03d THz", (int)d1, (int)(d2/20));
    return str;    
}

std::string MsaCfp::GetGridSpacing()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";

    uint16_t val = 0;
    val = read(0xB400);
    val >>= 13;
    val &= 0x7;

    if (val == 0)
        return "100 GHz";
    else if (val == 1)
        return "50 GHz";
    else if (val == 2)
        return "33 GHz";
    else if (val == 3)
        return "25 GHz";
    else if (val == 4)
        return "12.5 GHz";
    else if (val == 5)
        return "6.25 GHz";
    else
        return "N/A";
}

std::string MsaCfp::GetMaxChanNum()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";
    if(mIsMenara)
    {
        return "103";
    }
    uint16_t v1 = read(0x8196);
    uint16_t v2 = read(0x8197);
    uint16_t v3 = ((v1 & 0x3) << 8) | v2;
    char str[32] = {0};
    sprintf(str, "%d", (int)v3);
    return str;    
}

std::string MsaCfp::GetCurTxFreq()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";
    uint16_t v1 = read(0xB450);
    uint16_t v2 = read(0xB460);
    char str[32] = {0};
    sprintf(str, "%d.%03d THz", (int)v1, (int)(v2/20));
    return str;    
}

std::string MsaCfp::GetCurRxFreq()
{
    if (!mPresent || ((!mIsAcacia)&&(!mIsMenara))) return "N/A";
    uint16_t v1 = read(0xB470);
    uint16_t v2 = read(0xB480);
    char str[32] = {0};
    sprintf(str, "%d.%03d THz", (int)v1, (int)(v2/20));
    return str;    
}

std::string MsaCfp::GetDGD()
{
    if (!mPresent || !mIsAcacia) return "N/A";
    uint16_t val = read(0xB880);
    char str[32] = {0};
    sprintf(str, "%d ps", (int)val);
    return str;
}

std::string MsaCfp::GetCD()
{
    if (!mPresent || !mIsAcacia) return "N/A";
    uint16_t v1 = read(0xB800);
    uint16_t v2 = read(0xB810);
    uint32_t v3 = (v1 << 16) | v2;
    char str[32] = {0};
    sprintf(str, "%d ps/nm", (int)v3);
    return str;
}

std::string MsaCfp::GetOSNR()
{
    if (!mPresent || !mIsAcacia) return "N/A";
    uint16_t val = read(0x9130);
    char str[32] = {0};
    float data = (int)val / 10.0;
    sprintf(str, "%.1f dB", data);
    return str;
}

std::string MsaCfp::GetQfactor()
{
    if (!mPresent || !mIsAcacia) return "N/A";
    uint16_t val = read(0xB980);
    char str[32] = {0};
    float data = (int)val / 10.0;
    sprintf(str, "%.1f dB", data);
    return str;
}

bool MsaCfp::GetFecCounter(OchCounter *cnt)
{
    if (!mPresent || !mIsAcacia) return false;

    return true;
}

void MsaCfp::Dump()
{
    if (GetPresent())
    {
        APP_LOG("\tVendor Name         : %s",   GetVendorName().c_str());
        APP_LOG("\tPart Number         : %s",   GetPartNumber().c_str());
        APP_LOG("\tSerial Number       : %s",   GetSerialNumber().c_str());
        APP_LOG("\tDate code (yymmdd)  : %s",   GetDateCode().c_str());
        APP_LOG("\tHw Version          : %s",   GetHwVersion().c_str());
        APP_LOG("\tFw A Version        : %s",   GetFwVersion().c_str());
        APP_LOG("\tFw B Version        : %s",   GetFwBVersion().c_str());
        APP_LOG("\tTx Power (dBm)      : %.2f", GetTxPower());
        APP_LOG("\tRx Power (dBm)      : %.2f", GetRxPower());
        APP_LOG("\tTemperature (C)     : %.1f", GetTemperature());
        APP_LOG("\tModule State        : %s",   CfpState_to_String(GetModuleState()));
        APP_LOG("\tLaser               : %d",   GetLaserOn());
        APP_LOG("\tLOS                 : %d",   GetLos());
        APP_LOG("\tMinFreq             : %s",   GetMinFreq().c_str());
        APP_LOG("\tMaxFreq             : %s",   GetMaxFreq().c_str());
        APP_LOG("\tMaxChannel          : %s",   GetMaxChanNum().c_str());     
        APP_LOG("\tCurTxFreq           : %s",   GetCurTxFreq().c_str());
        APP_LOG("\tCurRxFreq           : %s",   GetCurRxFreq().c_str());
        APP_LOG("\tmIsAcacia           : %d",   mIsAcacia);
        APP_LOG("\tmIsMenara           : %d",   mIsMenara);
    }
    else
    {
        APP_LOG("\tnot present");
    }
}

void MsaCfp::Menu(int argc, char *argv[])
{
    bool help = false;
    
    if (argc >= 1)
    {
        if (argc == 1 && argv[0][0] == 'i')
        {
            InitCold();
        }
        else if (strcmp(argv[0], "on") == 0)
        {
            APP_LOG("Laser on");
            SetTxDis(false);
        }
        else if (strcmp(argv[0], "off") == 0)
        {
            APP_LOG("Laser off");
            SetTxDis(true);
        }
        else if (strcmp(argv[0], "reseton") == 0)
        {
            APP_LOG("Reset on");
            SetModRst(true);
        }
        else if (strcmp(argv[0], "resetoff") == 0)
        {
            APP_LOG("Reset off");
            SetModRst(false);
        }
        else if (strcmp(argv[0], "lowon") == 0)
        {
            APP_LOG("Low power on");
            SetModLowPwr(true);
        }
        else if (strcmp(argv[0], "lowoff") == 0)
        {
            APP_LOG("Low power off");
            SetModLowPwr(false);
        }
        else if (argc == 1 && argv[0][0] == 'd')
        {
            Dump();
        }
        else if (argc >= 2 && argv[0][0] == 'r')
        {
            int len = 1;
            long reg;
            uint16_t val;
            if (argc == 3) len = atoi(argv[2]);
            reg = strtol(argv[1], NULL, 0);
            for (int i = 0; i < len; i++)
            {
                val = read((uint16_t)(reg + i));
                APP_LOG("[%04x]: 0x%04x", (uint16_t)(reg + i), val); 
            }
        }
        else if (argc == 3 && argv[0][0] == 'w')
        {
            long reg, val;
            reg = strtol(argv[1], NULL, 0);
            val = strtol(argv[2], NULL, 0);
            write((uint16_t)(reg), (uint16_t)(val));
        }
        else if (argc == 2 && argv[0][0] == 's')
        {
            int type = atoi(argv[1]);
            SetSignalType((PortMode::Enum)type);
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
        APP_LOG("cfp <id>");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
        APP_LOG("\t s <PortMode>");
        APP_LOG("\t id: 0 - Line CFP, 1 - Client CFP");
        APP_LOG("\t PortMode: 0 - OTU4, 1 - 100GE");
    }
}

