#include "DanOS.h"
#include "Immr.h"
#include "I2c.h"
#include "Fpga.h"
#include "MsaSfp.h"
#include <math.h>

using namespace std;
using namespace MsaSfpApi;

std::vector<MsaSfp *> MsaSfp::mInst(NUM_INST, (MsaSfp *)0);
int MsaSfp::mInstNum = 0;

MsaSfp::MsaSfp(int dev) : mDevId(dev), mPresent(false)
{
    const uint16_t i2c_sel[13] = 
    {
        I2C_SEL_NULL, I2C_SEL_SFP0, I2C_SEL_SFP1, I2C_SEL_SFP2,
        I2C_SEL_SFP3, I2C_SEL_SFP4, I2C_SEL_SFP5, I2C_SEL_SFP6,
        I2C_SEL_SFP7, I2C_SEL_SFP8, I2C_SEL_SFP9,
        I2C_SEL_OSC_SFP0, I2C_SEL_OSC_SFP1,
    };
    i2cSel = i2c_sel[dev + 1];
}

MsaSfp& MsaSfp::GetInst(uint32_t inst)
{
    if (inst >= mInst.size())
    {
        std::cout << __FUNCTION__ << " invalid instance number " << inst << std::endl;
        inst = 0;
    }

    if (mInst[inst] == NULL)
    {
        mInst.at(inst) = new MsaSfp(inst);
        mInstNum++;
    }

    return(*(mInst.at(inst)));
}

void MsaSfp::DelInst(uint32_t inst)
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

void MsaSfp::write(uint8_t reg, uint8_t val)
{
    uint8_t tbuf[3] = {0};
    I2cSel(i2cSel);

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = reg;
    tbuf[2] = val;
    I2cWrite(SFP_I2C_BUS, tbuf, sizeof(tbuf));
}

uint8_t MsaSfp::read(uint8_t reg)
{
    uint8_t tbuf[2] = {0};
    uint8_t rbuf[1] = {0};
    I2cSel(i2cSel);

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = reg;
    I2cRead(SFP_I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf));
    
    return rbuf[0];
}

void MsaSfp::ddm_write(uint8_t reg, uint8_t val)
{
    uint8_t tbuf[3] = {0};
    I2cSel(i2cSel);

    tbuf[0] = SFP_DDM_I2C_ADDR;
    tbuf[1] = reg;
    tbuf[2] = val;
    I2cWrite(SFP_I2C_BUS, tbuf, sizeof(tbuf));
}

uint8_t MsaSfp::ddm_read(uint8_t reg)
{
    uint8_t tbuf[2] = {0};
    uint8_t rbuf[1] = {0};
    I2cSel(i2cSel);

    tbuf[0] = SFP_DDM_I2C_ADDR;
    tbuf[1] = reg;
    I2cRead(SFP_I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf));
    
    return rbuf[0];
}

uint16_t MsaSfp::ddm_read16(uint8_t reg)
{
    uint16_t val1 = (uint16_t)ddm_read(reg);
    uint16_t val2 = (uint16_t)ddm_read(reg + 1);
    return ((val1<<8)|(val2<<0));
}

uint32_t MsaSfp::ddm_read32(uint8_t reg)
{
    uint32_t val1 = (uint32_t)ddm_read(reg);
    uint32_t val2 = (uint32_t)ddm_read(reg + 1);
    uint32_t val3 = (uint32_t)ddm_read(reg + 2);
    uint32_t val4 = (uint32_t)ddm_read(reg + 3);
    return ((val1<<24)|(val2<<16)|(val3<<8)|(val4<<0));
}

void MsaSfp::InitCold()
{
    if (!GetPresent()) return;
    // force tx disable
    // rate select to 9G
    // CDR select to 9G

    sleep(1);

    uint8_t val = 0;
    val = ddm_read(0x6e);
    ddm_write(0x6e, val | 0x08);
    val = ddm_read(0x76);
    ddm_write(0x76, val | 0x08);

    FpgaSetSfpRateSelect(mDevId, true);

    UpdateInventory();
}

float MsaSfp::GetTxPower()
{
    float val = INVALID_POWER;

    // We still use some SFPs that don't have DDM implemented.
    if (GetLaserOn() && GetPresent() && getDdmImplementation())
    {
        //          (  dBm     (   mW   (       uW       ))))
        float mBm = static_cast<float>(((10.0 * log10(0.001 * getTxPwr()))));
        // Round off and cast to proper type.
        //val = static_cast<float>(floor(mBm + 0.5));
        val = mBm;
        // Check for application defined threshold.
        val = (val > INVALID_POWER) ? val : INVALID_POWER;
    }

    return(val);
}

float MsaSfp::GetRxPower()
{
    float val = INVALID_POWER;

    // We still use some SFPs that don't have DDM implemented.
    if (GetPresent() && getDdmImplementation())
    {
        //          (  dBm     (   mW   (      uW       ))))
        float mBm = static_cast<float>(((10.0 * log10(0.001 * getRxPwr()))));
        // Round off and cast to proper type.
        //val = static_cast<float>(floor(mBm + 0.5));
        val = mBm;
        // Check for application defined threshold.
        val = (val > INVALID_POWER) ? val : INVALID_POWER;
    }

    return(val);
}

char MsaSfp::CheckPrint(uint8_t val)
{
    if (isprint(val))
        return (char)val;
    else
        return ' ';
}

void MsaSfp::UpdateInventory()
{
    mVendorName.resize(VENDOR_NAME_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_NAME_LEN; i++)
    {
        mVendorName[i] = CheckPrint(read(0x14 + i));
    }

    mPartNumber.resize(VENDOR_PN_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_PN_LEN; i++)
    {
        mPartNumber[i] = CheckPrint(read(0x28 + i));
    }

    mSerialNumber.resize(VENDOR_SN_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_SN_LEN; i++)
    {
        mSerialNumber[i] = CheckPrint(read(0x44 + i));
    }

    mDateCode.resize(DATE_CODE_LEN, ' ');
    for(uint8_t i = 0; i < DATE_CODE_LEN; i++)
    {
        mDateCode[i] = CheckPrint(read(0x54 + i));
    }

    mHwVersion.resize(VENDOR_REV_LEN, ' ');
    for(uint8_t i = 0; i < VENDOR_REV_LEN; i++)
    {
        mHwVersion[i] = CheckPrint(read(0x38 + i));
    }

    mInternalCalibration = getInternalCalibration();
    isEthTransceiver = ((read(0x6) & 0xC) != 0) ? true : false;
    if (isEthTransceiver) APP_LOG("It is EthTransceiver");
}

bool MsaSfp::GetPresent()
{
    return FpgaGetSfpPresent(mDevId);
}

bool MsaSfp::GetLos()
{
    return FpgaGetSfpLos(mDevId);
}

bool MsaSfp::GetLaserOn()
{
    return FpgaGetSfpLaserOn(mDevId);
}

void MsaSfp::SetLaserOn(bool on)
{
    FpgaSetSfpLaserOn(mDevId, on);
}

void MsaSfp::Dump()
{
    if (GetPresent())
    {
        APP_LOG("\tVendor Name         : %s",   GetVendorName().c_str());
        APP_LOG("\tPart Number         : %s",   GetPartNumber().c_str());
        APP_LOG("\tSerial Number       : %s",   GetSerialNumber().c_str());
        APP_LOG("\tHW Revision         : %s",   GetHwVersion().c_str());
        APP_LOG("\tDate code (yymmdd)  : %s",   GetDateCode().c_str());
        APP_LOG("\tTx Power (mBm)      : %.2f", GetTxPower());
        APP_LOG("\tRx Power (mBm)      : %.2f", GetRxPower());
        APP_LOG("\tLaser               : %d",   GetLaserOn());
        APP_LOG("\tLOS                 : %d",   GetLos());
    }
    else
    {
        APP_LOG("\tnot present");
    }
}

void MsaSfp::Menu(int argc, char *argv[])
{
    bool help = false;
    
    if (argc >= 1)
    {
        if (argc == 1 && argv[0][0] == 'i')
        {
            InitCold();
        }
        else if (argc == 1 && argv[0][0] == 'd')
        {
            Dump();
        }
        else if (argc >= 2 && argv[0][0] == 'r')
        {
            int len = 1;
            long reg;
            uint8_t val;
            if (argc == 3) len = atoi(argv[2]);
            reg = strtol(argv[1], NULL, 0);
            for (int i = 0; i < len; i++)
            {
                val = read((uint8_t)(reg + i));
                APP_LOG("[%02x]: 0x%02x", (uint8_t)(reg + i), (uint8_t)(val)); 
            }
        }
        else if (argc == 3 && argv[0][0] == 'w')
        {
            long reg, val;
            reg = strtol(argv[1], NULL, 0);
            val = strtol(argv[2], NULL, 0);
            write((uint8_t)(reg), (uint8_t)(val));
        }
        else if (argc >= 2 && argv[0][0] == 'm' && argv[0][1] == 'r')
        {
            int len = 1;
            long reg;
            uint8_t val;
            if (argc == 3) len = atoi(argv[2]);
            reg = strtol(argv[1], NULL, 0);
            for (int i = 0; i < len; i++)
            {
                val = ddm_read((uint8_t)(reg + i));
                APP_LOG("[%02x]: 0x%02x", (uint8_t)(reg + i), (uint8_t)(val)); 
            }
        }
        else if (argc == 3 && argv[0][0] == 'm' && argv[0][1] == 'w')
        {
            long reg, val;
            reg = strtol(argv[1], NULL, 0);
            val = strtol(argv[2], NULL, 0);
            ddm_write((uint8_t)(reg), (uint8_t)(val));
        }
        else if (argc == 2 && argv[0][0] == 'l')
        {
            bool enable = false;
            if (argv[1][0] == '1' || argv[1][0] == 't')
            {
                enable = true;
            }
            SetLaserOn(enable);
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
        APP_LOG("sfp <id>");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
        APP_LOG("\t mr <reg> [len]");
        APP_LOG("\t mw <reg> <val>");
        APP_LOG("\t laser <0|1>");
        APP_LOG("\t id - SFP+ 0 ~ 9, OSC1 10, OSC2 11");
    }
}

float MsaSfp::getTxPwr()
{
    float aOutputPwr;

    if(mInternalCalibration)
    {
        aOutputPwr = static_cast<float>(ddm_read16(0x66) * 0.1f);
    }
    else
    {
        uint16_t power = ddm_read16(0x66);
        float slope = static_cast<float>(ddm_read16(0x50) / 256.0f);
        int16_t offset = static_cast<int16_t>(ddm_read16(0x52));

        aOutputPwr = static_cast<float>(((slope * power) + static_cast<float>(offset)) * 0.1f);
    }

    // LSb in in 0.1 uW steps
    return(aOutputPwr);
}

float MsaSfp::getRxPwr()
{
    float aInputPwr;

    if(mInternalCalibration)
    {
        aInputPwr = static_cast<float>(ddm_read16(0x68) * 0.1f);
    }
    else
    {
        uint16_t x = ddm_read16(0x68);
        double power = 0.0;
        uint32_t temp;
        for(uint8_t i = 0; i < RX_PWR_DATA; i++)
        {
            temp = ddm_read32(0x38 + i*4);
            power += (pow(static_cast<double>(x), static_cast<int>(RX_PWR_DATA - 1 - i)) * (*reinterpret_cast<float*>(&temp)));
            //power += (pow(static_cast<double>(x), static_cast<int>(RX_PWR_DATA - 1 - i)) * (float)(temp));
        }

        aInputPwr = static_cast<float>(power * 0.1);
    }

    // LSb in in 0.1 uW steps
    return(aInputPwr);
}

float MsaSfp::GetTemperature()
{
    float temp;
    int16_t val;
    
    if(mInternalCalibration)
    {
        val = (int16_t)ddm_read16(0x60);
        temp = val / 256.0F;
    } 
    else
    {
        val = (int16_t)ddm_read16(0x60);
        temp = val / 256.0F;

        val = (int16_t)ddm_read16(0x56);
        float temp2 = val / 256.0F;

        val = (int16_t)ddm_read16(0x54);
        float temp3 = val / 256.0F;

        temp = (temp3 * temp) + temp2;
    }

    return temp;
}

bool MsaSfp::getDdmImplementation()
{
    uint8_t val = read(0x5C);
    return ((val & 0x40) == 0x40);
}

bool MsaSfp::getInternalCalibration()
{
    uint8_t val = read(0x5C);
    return ((val & 0x20) == 0x20);
}

void MsaSfp::StateMachine(void)
{
    //APP_LOG("MsaSfp::StateMachine");
    bool present = GetPresent();
    if (present != mPresent)
    {
        APP_LOG("MsaSfp %d, Present Change To: %s", mDevId, present ? "true" : "false");
        if (present)
        {
            InitCold();
        }
        else
        {
            //SetModRst(true);
        }
        mPresent = present;
    }
}

