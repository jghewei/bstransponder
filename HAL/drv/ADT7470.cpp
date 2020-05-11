#include "DanOS.h"
#include "ADT7470.h"
#include "I2c.h"

#define SFP_I2C_BUS             0
#define SFP_I2C_ADDR            0x2c

uint8_t Adt7470Read(uint8_t addr)
{   
    uint8_t tbuf[2] = {0};
    uint8_t rbuf[1] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = addr;
    if (I2cRead(SFP_I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf)))
    {
        return rbuf[0];
    }
    else
    {
        return 0xff;
    }
}

void Adt7470Write(uint8_t addr, uint8_t data)
{   
    uint8_t tbuf[3] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = addr;
    tbuf[2] = data;
    I2cWrite(SFP_I2C_BUS, tbuf, sizeof(tbuf));
}

uint16_t Adt7470GetTach(uint8_t id)
{
    uint8_t reg = 0x2a + id * 2;
    uint16_t lsb = (uint16_t)Adt7470Read(reg + 0);
    uint16_t msb = (uint16_t)Adt7470Read(reg + 1);
    return ((msb << 8) | lsb);
}

void Adt7470Dump(void)
{   
    APP_LOG("Mfg ID  : 0x%02x", Adt7470Read(0x3e));
    APP_LOG("Dev ID  : 0x%02x", Adt7470Read(0x3d));
    APP_LOG("Tach 0  : 0x%04x", Adt7470GetTach(0));
    APP_LOG("Tach 1  : 0x%04x", Adt7470GetTach(1));
    APP_LOG("Tach 2  : 0x%04x", Adt7470GetTach(2));
}

void Adt7470SetSpeed(uint8_t speed)
{   
    Adt7470Write(0x32, speed);
    Adt7470Write(0x33, speed);
    Adt7470Write(0x34, speed);
}

bool Adt7470GetFault()
{
    if (Adt7470GetTach(0) == 0xffff)
        return true;

    if (Adt7470GetTach(1) == 0xffff)
        return true;

    if (Adt7470GetTach(2) == 0xffff)
        return true;

    return false;
}

void Adt7470Menu(int argc, char *argv[])
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
            val = Adt7470Read((uint8_t)(reg + i));
            APP_LOG("[%08x]: 0x%02x", reg + i, (uint8_t)val); 
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        Adt7470Write((uint8_t)reg, (uint8_t)val);
    }
    else if (argc >= 1 && argv[0][0] == 'd')
    {
        Adt7470Dump();
    }
    else if (argc == 2 && argv[0][0] == 's')
    {
        int speed = atoi(argv[1]);
        Adt7470SetSpeed((uint8_t)speed);
    }
    else
    {
        APP_LOG("adt7470");
        APP_LOG("\t dump");
        APP_LOG("\t set <speed 0-255>");
        APP_LOG("\t read <reg> [len]");
        APP_LOG("\t write <reg> <val>");
    }
}

