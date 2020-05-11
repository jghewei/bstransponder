#include "DanOS.h"
#include "I2c.h"
#include "M21036.h"
#include "Fpga.h"

#define LANE_NUM            48
#define LANE_POWER_DOWN     0x3F

const int I2C_DEV = 0;
const uint8_t I2C_ADDR = 0x21;

static int asc_mode = 1;

extern int isTranspondMode;

const int LoopbackTable[LANE_NUM] =
{
    18,  5, 16, 43, -1, -1, -1, -1,     /* 00 - 07 */
    10,  7,  8, 31, -1, 39, -1, 20,     /* 08 - 15 */
     2,  1, -1,  0, -1, 15, -1, -1,     /* 16 - 23 */
    40, -1, -1, -1, 17, -1, -1, 23,     /* 24 - 31 */
    32, -1, -1,  6, -1, -1, -1, 21,     /* 32 - 39 */
    24, -1, -1, 13, -1, -1, -1, -1,     /* 40 - 47 */
};

const int MuxponderTable[LANE_NUM] =
{
     7, 17, 31, 13, -1, -1, -1, -1,     /* 00 - 07 */
    15, 18, 39, 16, -1,  8, -1, 32,     /* 08 - 15 */
    23, 21, -1, -1, -1, 10, -1, -1,     /* 16 - 23 */
     6, -1, -1, -1,  5, -1, -1,  2,     /* 24 - 31 */
    20, -1, -1, 40, -1, -1, -1,  1,     /* 32 - 39 */
    -1, -1, -1, 43, -1, -1, -1, -1,     /* 40 - 47 */
};

const int TransponderTable[LANE_NUM] =
{
    12, -1, 14, 44,  1, 32,  2, -1,     /* 00 - 07 */
    19, -1, 11, -1, 43, -1, 10, -1,     /* 08 - 15 */
    27, 35, -1, -1, 17, -1, 18, 40,     /* 16 - 23 */
    36, -1, -1, -1,  4, -1, -1, -1,     /* 24 - 31 */
     9, -1, -1, -1,  8, -1, -1, -1,     /* 32 - 39 */
    -1, -1, -1, -1, 16, -1, -1, -1,     /* 40 - 47 */
};

void M21036ModeSel(const int *AscTable);

void M21036Write(uint8_t reg, uint8_t val)
{
    uint8_t tbuf[3] = {I2C_ADDR, reg, val};
    I2cWrite(I2C_DEV, tbuf, sizeof(tbuf));
}

uint8_t M21036Read(uint8_t reg)
{
    uint8_t tbuf[2] = {I2C_ADDR, reg};
    uint8_t rbuf = 0;
    I2cRead(I2C_DEV, tbuf, sizeof(tbuf), &rbuf, 1);
    return rbuf;
}

void M21036Init(void)
{
    FpgaWrite(0x18, 0x0);
    FpgaWrite(0x19, 0xf);
    sleep(1);

    FpgaWrite(0x18, 0x1);   // de-assert reset pin of M21036
    M21036Write(0xff, 0x00);
    APP_LOG("M21036 Init, DeviceID: 0x%02x", M21036Read(0x01));

    FpgaWrite(0x19, 0x0);

    M21036Write(0xff, 0x00);
    M21036Write(0x00, 0xaa);
    sleep(1);
    M21036Write(0x00, 0x00);
    sleep(1);
    M21036Write(0x09, 0x30);
    M21036Write(0x0b, 0x04);
    M21036Write(0x03, 0x88);

    if (isTranspondMode)
    {
        M21036ModeSel(TransponderTable);
#if 1
        APP_LOG("Set M21036 -> Client CFP Output Drv to 1.2V");
        M21036Write(0xff, 0x0E);
        M21036Write(0x04, 0x06);
        M21036Write(0x05, 0x06);
        M21036Write(0x06, 0x06);
        M21036Write(0x0C, 0x06);
        M21036Write(0x0E, 0x06);
        M21036Write(0x14, 0x06);
        M21036Write(0x16, 0x06);
        M21036Write(0x17, 0x06);
        M21036Write(0x24, 0x06);
        M21036Write(0x2C, 0x06);
#endif
    }
    else
    {
        M21036ModeSel(MuxponderTable);
    }
}

void M21036ModeSel(const int *AscTable)
{
    int data = -1;
    uint8_t val = 0, lane = 0;
    uint32_t val4 = 0;
    
    val4 = FpgaRead(0x19);
    val4 &= ~0x2;
    FpgaWrite(0x19, val4);      // xSet -> 0
    
    M21036Write(0xff, 0x00);
    val = M21036Read(0x03);
    val &= ~0x30;
    M21036Write(0x03, val);     // Direct ASC Mode

    M21036Write(0xff, 0x01);
    for (lane = 0; lane < LANE_NUM; lane++)
    {
        if (AscTable != NULL)
        {
            data = AscTable[lane];
        }
        val = (data == -1) ? LANE_POWER_DOWN : ((uint8_t)data);
        M21036Write(lane, val);
    }
}

void M21036Menu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        M21036Init();
    }
    else if (argc == 3 && !strcmp(argv[0], "rx"))
    {
        int type = atoi(argv[1]);
        int lane = 0;
        int eq = atoi(argv[2]);
        for (lane = 0; lane < 10; lane++)
        {
            M21036SetRxParam(type, lane, (uint8_t)eq);
        }
    }
    else if (argc == 4 && !strcmp(argv[0], "tx"))
    {
        int type = atoi(argv[1]);
        int lane = 0;
        int de = atoi(argv[2]);
        int drv = atoi(argv[3]);
        for (lane = 0; lane < 10; lane++)
        {
            M21036SetTxParam(type, lane, (uint8_t)de, (uint8_t)drv);
        }
    }
    else if (argc >= 1 && argv[0][0] == 's')
    {
        if (argc == 2)
        {
            asc_mode = atoi(argv[1]);
            if (asc_mode == 0)
                M21036ModeSel(NULL);
            else if (asc_mode == 1)
                M21036ModeSel(LoopbackTable);
            else if (asc_mode == 2)
                M21036ModeSel(MuxponderTable);
            else if (asc_mode == 3)
                M21036ModeSel(TransponderTable);
            else
                APP_LOG("Invalid mode %d", asc_mode);
        }
        else
        {
            APP_LOG("Current mode is %d", asc_mode);
        }
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
            val = M21036Read((uint8_t)(reg + i));
            APP_LOG("[%02x]: 0x%02x", (uint8_t)(reg + i), val);
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        M21036Write((uint8_t)reg, (uint8_t)val);
    }
    else
    {
        APP_LOG("M21036");
        APP_LOG("\t i");
        APP_LOG("\t s [0-off,1-lpbk,2-mux,3-tran]");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
        APP_LOG("\t rx <type> <eq>");
        APP_LOG("\t tx <type> <de> <drv>");
        APP_LOG("\t\t type: 0-Digi, 1-Sfp, 2-Cfp");
        //APP_LOG("\t\t lane: 0-9");
        APP_LOG("\t\t eq: 0x00-0x1f");
        APP_LOG("\t\t de: 0-7");
        APP_LOG("\t\t drv: 1-3");
    }
}

const int RxLane[3][10] = 
{
    { 17, 18, 16, 40, 43, 10,  8, 32,  1,  2 },
    {  5,  7, 31,  6, 13, 15, 39, 20, 21, 23 },
    {  4, 12, 14, 36, 44, 19, 11,  9, 35, 27 },
};

const int TxLane[3][10] = 
{
    { 28,  0,  2, 24,  3,  8, 10, 32, 17, 16 },
    {  1,  9, 11, 35, 43, 21, 13, 15, 39, 31 },
    { 20, 22, 44, 23, 12, 14, 36,  5,  4,  6 },
};

void M21036SetRxParam(int type, int lane, uint8_t eq)
{
    M21036Write(0xff, 0x0C);
    M21036Write(RxLane[type][lane], eq);
}

void M21036SetTxParam(int type, int lane, uint8_t de, uint8_t drv)
{
    M21036Write(0xff, 0x0E);
    M21036Write(TxLane[type][lane], (de << 4) | (drv << 1));
}

