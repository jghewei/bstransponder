#include "DanOS.h"
#include "Gpio.h"
#include "Si5368.h"
#include "Spi.h"

#define SI5368_ADDR             0x00
#define SI5368_READ             0x80
#define SI5368_WRITE            0x40

#define SI5368_LOCK_TIMEOUT     5           // second

static uint8_t dout[2];
static uint8_t din[2];

void Si5368Write(uint8_t reg, uint8_t val)
{
    dout[0] = SI5368_ADDR;
    dout[1] = reg;
    SpiChipSel(SPI_CS_TIMING, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(SPI_CS_TIMING, 0);

    dout[0] = SI5368_WRITE;
    dout[1] = val;
    SpiChipSel(SPI_CS_TIMING, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(SPI_CS_TIMING, 0);
}

uint8_t Si5368Read(uint8_t reg)
{
    dout[0] = SI5368_ADDR;
    dout[1] = reg;
    SpiChipSel(SPI_CS_TIMING, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(SPI_CS_TIMING, 0);

    dout[0] = SI5368_READ;
    din[1] = 0;
    SpiChipSel(SPI_CS_TIMING, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(SPI_CS_TIMING, 0);

    return din[1];
}

void Si5368Init(void)
{
    //int retry = 0;

    GpioSetPin(SI5368_RESET, 0);
    usleep(1000);
    GpioSetPin(SI5368_RESET, 1);
    usleep(10 * 1000);
    APP_LOG("Si5368 DeviceID: 0x%02x%02x = 0x0442", Si5368Read(134), Si5368Read(135));

    Si5368Write(   0, 0x14);
    Si5368Write(   1, 0xE4);
    Si5368Write(   2, 0x52);
    Si5368Write(   3, 0x15);
    Si5368Write(   4, 0x92);
    Si5368Write(   5, 0xFF);
    Si5368Write(   6, 0x17);
    Si5368Write(   7, 0x12);
    Si5368Write(   8, 0x00);
    Si5368Write(   9, 0xC0);
    Si5368Write(  10, 0x00);
    Si5368Write(  11, 0x40);
    Si5368Write(  12, 0x88);
    Si5368Write(  13, 0x01);
    Si5368Write(  14, 0x00);
    Si5368Write(  15, 0x00);
    Si5368Write(  16, 0x00);
    Si5368Write(  17, 0x80);
    Si5368Write(  18, 0x00);
    Si5368Write(  19, 0x2C);
    Si5368Write(  20, 0x3E);
    Si5368Write(  21, 0xFF);
    Si5368Write(  22, 0xDF);
    Si5368Write(  23, 0x1F);
    Si5368Write(  24, 0x3F);
    Si5368Write(  25, 0xA0);
    Si5368Write(  26, 0x00);
    Si5368Write(  27, 0x03);
    Si5368Write(  28, 0x00);
    Si5368Write(  29, 0x00);
    Si5368Write(  30, 0x03);
    Si5368Write(  31, 0x00);
    Si5368Write(  32, 0x00);
    Si5368Write(  33, 0x03);
    Si5368Write(  34, 0x00);
    Si5368Write(  35, 0x00);
    Si5368Write(  36, 0x1F);
    Si5368Write(  37, 0x01);
    Si5368Write(  38, 0x2F);
    Si5368Write(  39, 0xBF);
    Si5368Write(  40, 0x40);
    Si5368Write(  41, 0x5B);
    Si5368Write(  42, 0x1F);
    Si5368Write(  43, 0x00);
    Si5368Write(  44, 0x02);
    Si5368Write(  45, 0x70);
    Si5368Write(  46, 0x00);
    Si5368Write(  47, 0x02);
    Si5368Write(  48, 0x70);
    Si5368Write(  49, 0x00);
    Si5368Write(  50, 0x02);
    Si5368Write(  51, 0x70);
    Si5368Write(  52, 0x00);
    Si5368Write(  53, 0x02);
    Si5368Write(  54, 0x70);
    Si5368Write(  55, 0x00);
    Si5368Write(  56, 0x00);
    Si5368Write( 131, 0x1F);
    Si5368Write( 132, 0x02);
    Si5368Write( 138, 0x0F);
    Si5368Write( 139, 0xFF);
    Si5368Write( 140, 0x00);
    Si5368Write( 141, 0x00);
    Si5368Write( 142, 0x00);
    Si5368Write( 143, 0x00);
    Si5368Write( 144, 0x00);
    Si5368Write( 136, 0x40);

    sleep(1);
    APP_LOG("Si5368 lock in 1 s");
#if 0
    while (retry < SI5368_LOCK_TIMEOUT * 1000 * 1000)
    {
        if (GpioGetPin(SI5368_LOL) == 0)
        {
            APP_LOG("Si5368 lock in %d us", retry++);
            break;
        }
        usleep(1);
    }
    if (retry == SI5368_LOCK_TIMEOUT * 1000 * 1000)
    {
        APP_LOG("Si5368 lock timeout in %d s", SI5368_LOCK_TIMEOUT);
    }
#endif
}

void Si5368Menu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        Si5368Init();
    }
    else if (argc == 1 && argv[0][0] == 'd')
    {
        APP_LOG("Device ID, expect to be 0x04, 0x42");
        APP_LOG("[%02x]: 0x%02x", 134, Si5368Read(134));
        APP_LOG("[%02x]: 0x%02x", 135, Si5368Read(135));
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg;
        uint8_t val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = Si5368Read((uint8_t)(reg + i));
            APP_LOG("[%02x]: 0x%02x", (uint8_t)(reg + i), val);
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        Si5368Write((uint8_t)reg, (uint8_t)val);
    }
    else
    {
        APP_LOG("si5368");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
    }
}

