#include "DanOS.h"
#include "MemMap.h"
#include "Fpga.h"
#include "CMutex.h"

#define FPGA_BASE       0x40000000
#define FPGA_SIZE       0x04000000

MemAddr fpgaBase = NULL;

extern int isTranspondMode;

static CMutex fpgaLock;    // to protect indirect access to fpga
static CMutex fpgaBitLock;

uint32_t bit_reverse(uint32_t n)  
{  
    n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);  
    n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);  
    n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);  
    n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);  
    n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);  
    return n;  
}  

uint32_t reg_reverse(uint32_t n)
{
    n <<= 1;
    n = bit_reverse(n);
    n >>= 6;

    return n;
}

void FpgaInit(void)
{
    fpgaBase = MemMap(FPGA_BASE, FPGA_SIZE);
    APP_LOG("FPGA DeviceID: 0x%08x, Timestamp: 0x%08x", FpgaRead(0), FpgaRead(1));

    // de-assert reset of PCA9548A, IIC Expander
    FpgaWrite(0x15, 0x03);

    FpgaWrite(0x110, 0x8);  // MDIO to low speed
    FpgaWrite(0x8, 0x0);    // reset CFP by default
    FpgaWrite(0xC, 0x0);    // reset CFP by default
}

void FpgaWrite(uint32_t reg, uint32_t val)
{
    uint32_t l, h;
    MemAddr addr = fpgaBase + reg_reverse(reg);
    val = bit_reverse(val);
    h = val & 0xffff;
    l = (val >> 16) & 0xffff;
    h |= ((h << 16) & 0xffff0000);
    l |= ((l << 16) & 0xffff0000);

    CMutexLock lock(fpgaLock);
    MemWrite(addr, l);
    MemWrite(addr + 0x02000000, h);
}

uint32_t FpgaRead(uint32_t reg)
{
    uint32_t l, h, val;
    MemAddr addr = fpgaBase + reg_reverse(reg);

    CMutexLock lock(fpgaLock);
    l = MemRead(addr);
    h = MemRead(addr + 0x02000000);
    val = ((l << 16) & 0xffff0000) | (h & 0x0000ffff);
    val = bit_reverse(val);

    return val;
}

void FpgaOrBitMask(uint32_t reg, uint32_t mask)
{
    uint32_t val;
    CMutexLock lock(fpgaBitLock);
    val = FpgaRead(reg);
    val |= mask;
    FpgaWrite(reg, val);
}

void FpgaAndBitMask(uint32_t reg, uint32_t mask)
{
    uint32_t val;
    CMutexLock lock(fpgaBitLock);
    val = FpgaRead(reg);
    val &= ~mask;
    FpgaWrite(reg, val);
}

bool FpgaGetSfpPresent(int dev)
{
    uint32_t reg;
    if (dev >= 0 && dev <= 9)
    {
        reg = 0x21 + dev * 2;
    }
    else if (dev >= 10 && dev <= 11)
    {
        reg = 0x13 - (dev - 10) * 2;
    }
    else
    {
        return false;
    }

    return ((FpgaRead(reg) & 0x4) == 0) ? true : false;
}

bool FpgaGetSfpLos(int dev)
{
    uint32_t reg;
    if (dev >= 0 && dev <= 9)
    {
        reg = 0x21 + dev * 2;
    }
    else if (dev >= 10 && dev <= 11)
    {
        reg = 0x13 - (dev - 10) * 2;
    }
    else
    {
        return false;
    }

    return ((FpgaRead(reg) & 0x1) == 0x1) ? true : false;
}

bool FpgaGetSfpLaserOn(int dev)
{
    uint32_t reg;
    if (dev >= 0 && dev <= 9)
    {
        reg = 0x20 + dev * 2;
    }
    else if (dev >= 10 && dev <= 11)
    {
        reg = 0x12 - (dev - 10) * 2;
    }
    else
    {
        return false;
    }

    return ((FpgaRead(reg) & 0x1) == 0) ? true : false;
}

void FpgaSetSfpLaserOn(int dev, bool on)
{
    uint32_t reg, val;
    if (dev >= 0 && dev <= 9)
    {
        reg = 0x20 + dev * 2;
    }
    else if (dev >= 10 && dev <= 11)
    {
        reg = 0x12 - (dev - 10) * 2;
    }
    else
    {
        return;
    }

    val = FpgaRead(reg);
    (on) ? (val &= ~0x1) : (val |= 0x1);
    //APP_LOG("dev = %d, reg = %08x, on = %d, val = %08x", dev, reg, on, val);
    FpgaWrite(reg, val);
}

void FpgaSetSfpRateSelect(int dev, bool sel)
{
    uint32_t reg, val;
    if (dev >= 0 && dev <= 9)
    {
        reg = 0x20 + dev * 2;
    }
    else if (dev >= 10 && dev <= 11)
    {
        reg = 0x12 - (dev - 10) * 2;
    }
    else
    {
        return;
    }

    val = FpgaRead(reg);
    (sel) ? (val |= 0x6) : (val &= ~0x6);
    //APP_LOG("dev = %d, reg = %08x, on = %d, val = %08x", dev, reg, on, val);
    FpgaWrite(reg, val);
}

bool FpgaGetCfpPresent(int dev)
{
    const uint32_t addr[] = {0x9, 0xD};
    if (dev > 1) return false;
    return ((FpgaRead(addr[dev]) & 0x4) == 0) ? true : false;
}

bool FpgaGetCfpLos(int dev)
{
    const uint32_t addr[] = {0x9, 0xD};
    if (dev > 1) return false;
    return ((FpgaRead(addr[dev]) & 0x1) == 0x1) ? true : false;
}

bool FpgaGetCfpLaserOn(int dev)
{
    const uint32_t addr[] = {0x8, 0xC};
    if (dev > 1) return false;
    return ((FpgaRead(addr[dev]) & 0x1) == 0) ? true : false;
}

void FpgaGetPwrFault(int *a, int *b)
{
    uint32_t val = FpgaRead(0x1C);
    *a = (val & 0x1) ? 0 : 1;
    *b = (val & 0x2) ? 0 : 1;
}

bool FpgaIsAlive(void)
{
    return (FpgaRead(0) == 0x0a307007);
}

void FpgaMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        FpgaInit();
    }
    else if (argc == 1 && argv[0][0] == 'v')
    {
        APP_LOG("FPGA DeviceID: 0x%08x, Timestamp: 0x%08x", FpgaRead(0), FpgaRead(1));
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = FpgaRead(reg + i);
            APP_LOG("[%08x]: 0x%08x", reg + i, val); 
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        FpgaWrite(reg, val);
    }
    else
    {
        APP_LOG("fpga");
        APP_LOG("\t i");
        APP_LOG("\t v");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
    }
}

