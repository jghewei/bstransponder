#include "DanOS.h"
#include "MemMap.h"
#include "Immr.h"
#include "Pcie.h"
#include "Fpga.h"

#define IMMR_PECR1_REG      0x140
#define IMMR_PCI_LTSSM      0x9404
#define PCI_LTSSM_L0        0x16

#define PCI_CFG_CMD         0x04
#define PCI_CFG_BAR0        0x10

#define PCIE_BASE           0xB0000000
#define PCIE_SIZE           0x00100000
static MemAddr pcieBase = NULL;

#define RESET_DELAY         100*1000        // 100ms

void PcieDump(void)
{
    uint32_t linkStatus = ImmrRead(IMMR_PCI_LTSSM);
    APP_LOG("PCIE link status 0x%08x", linkStatus);
    linkStatus = (linkStatus >> 24) & 0x7F;
    if (linkStatus >= PCI_LTSSM_L0)
    {
        APP_LOG("PCIE link up");
    }
    else
    {
        APP_LOG("PCIE no link");
    }
}

void PcieInit(void)
{
    APP_LOG("PCIE init");
        
    FpgaWrite(0x16, 0);
    FpgaWrite(0x17, 0);
    usleep(RESET_DELAY);
    FpgaWrite(0x17, 1);
    usleep(RESET_DELAY);
    FpgaWrite(0x16, 1);
    usleep(RESET_DELAY);

    // after pcie hot reset, detect from [00009bfc]: 0x02000000
    // SW need to reset PCIE-CSB and reconfig all the related register
    // include PIO/inbound/outbound etc.
    ImmrWrite(IMMR_PECR1_REG, 0xa0008000);
    ImmrWrite(IMMR_PECR1_REG, 0xe0008000);
    ImmrWrite(0x9808, 0x33010000);
    ImmrWrite(0x9840, 0x0f000000);
    ImmrWrite(0x98e0, 0x01000000);
    ImmrWrite(0x9ca0, 0x01000001);
    ImmrWrite(0x9ca4, 0x000000b0);
    ImmrWrite(0x9cb0, 0x05000020);
    ImmrWrite(0x9cb4, 0x00000080);
    ImmrWrite(0x9cb8, 0x00000080);
    ImmrWrite(0x9cc0, 0x03008000);
    ImmrWrite(0x9cc4, 0x000000b1);
    ImmrWrite(0x9e60, 0x0df0ff0f);
    ImmrWrite(0x9e70, 0x0df0ff0f);
    ImmrWrite(0x9e74, 0x00000010);
    ImmrWrite(0x9e78, 0x00000010);
    ImmrWrite(0x9e80, 0x07f00f00);
    ImmrWrite(0x9e84, 0x000000f0);
    ImmrWrite(0x9e88, 0x000000f0);
    
    pcieBase = MemMap(PCIE_BASE, PCIE_SIZE);
    PcieWrite(PCI_CFG_BAR0, 0x00000080);
    PcieWrite(PCI_CFG_CMD, 0x06000000);
}

uint32_t PcieRead(uint32_t addr)
{
    return MemRead(pcieBase + addr);
}

void PcieWrite(uint32_t addr, uint32_t data)
{
    MemWrite(pcieBase + addr, data);
}

void PcieMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        PcieInit();
    }
    else if (argc == 1 && argv[0][0] == 'd')
    {
        PcieDump();
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = PcieRead(reg + i*4);
            APP_LOG("[%08x]: 0x%08x", reg + i*4, val); 
        }
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        PcieWrite(reg, val);
    }
    else
    {
        APP_LOG("pcie");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t r <addr>");
        APP_LOG("\t w <addr> <data>");
    }
}

