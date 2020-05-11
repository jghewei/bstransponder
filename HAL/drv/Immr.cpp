#include "DanOS.h"
#include "MemMap.h"
#include "Immr.h"

#define IMMR_SPRIDR_REG      0x108

static MemAddr immrBase = NULL;

void ImmrInit(void)
{
    uint32_t val = 0, partid = 0, revid = 0;
    immrBase = MemMap(IMMR_BASE, IMMR_SIZE);

    val = ImmrRead(IMMR_SPRIDR_REG);
    partid = (val >> 8) & 0xFFFFFF;
    revid = val & 0xFF;
    if (partid == 0x810101)
    {
        APP_LOG("CPU Device: MPC8308, Rev: %d.%d", (revid >> 4)&0xF, revid&0xF);
    }
    else
    {
        APP_LOG("CPU unknown part number 0x%08x", val);
    }
}

void ImmrWrite(uint32_t reg, uint32_t val)
{
    MemWrite(immrBase + reg, val);
}

void ImmrWriteMask(uint32_t reg, uint32_t val, uint32_t mask)
{
    MemWriteMask(immrBase + reg, val, mask);
}

uint32_t ImmrRead(uint32_t reg)
{
    return MemRead(immrBase + reg);
}

void ImmrMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        ImmrInit();
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = ImmrRead(reg + i*4);
            APP_LOG("[%08x]: 0x%08x", reg + i*4, val); 
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        ImmrWrite(reg, val);
    }
    else
    {
        APP_LOG("immr");
        APP_LOG("\t i");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
    }
}

