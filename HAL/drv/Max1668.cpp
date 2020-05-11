#include "DanOS.h"
#include "Max1668.h"
#include "I2c.h"

#define SFP_I2C_BUS             0
#define SFP_I2C_ADDR            0x18

uint8_t Max1668Read(uint8_t cmd)
{   
    uint8_t tbuf[2] = {0};
    uint8_t rbuf[1] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = cmd;
    I2cRead(SFP_I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf));
    
    return rbuf[0];
}

void Max1668Write(uint8_t cmd, uint8_t data)
{   
    uint8_t tbuf[3] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = cmd;
    tbuf[2] = data;
    I2cWrite(SFP_I2C_BUS, tbuf, sizeof(tbuf));
}

int Max1668GetTemp(uint8_t cmd)
{
    uint8_t utemp = Max1668Read(cmd);
    int8_t itemp = (int8_t)utemp;
    return (int)itemp;
}

void Max1668Dump(void)
{   
    APP_LOG("Mfg ID  : 0x%02x", Max1668Read(0xfe));
    APP_LOG("Dev ID  : 0x%02x", Max1668Read(0xff));
    APP_LOG("Local   : %d", Max1668GetTemp(0x00));
    APP_LOG("AIR OUT : %d", Max1668GetTemp(0x01));
    APP_LOG("AIR IN  : %d", Max1668GetTemp(0x02));
    APP_LOG("FPGA    : %d", Max1668GetTemp(0x03));
    APP_LOG("Framer  : %d", Max1668GetTemp(0x04));
}

void Max1668Menu(int argc, char *argv[])
{
    if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg;
        uint8_t val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = Max1668Read((uint8_t)(reg + i));
            APP_LOG("[%08x]: 0x%02x", reg + i, (uint8_t)val); 
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        Max1668Write((uint8_t)reg, (uint8_t)val);
    }
    else if (argc >= 1 && argv[0][0] == 'd')
    {
        Max1668Dump();
    }
    else
    {
        APP_LOG("max1668");
        APP_LOG("\t dump");
        APP_LOG("\t read <reg> [len]");
        APP_LOG("\t write <reg> <val>");
    }
}

