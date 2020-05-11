#include "DanOS.h"
#include "Fpga.h"
#include "Mdio.h"
#include "CMutex.h"

#define MDIO_BASE_ADDRESS       0x20000

static CMutex mdioLock;    // to protect indirect access to fpga

void MdioInit(void)
{
    FpgaWrite(0x110, 0x8);
}

bool MdioBusy(void)
{
    uint32_t val = 0;
    int timeout = 10;
    while (timeout--)
    {
        val = FpgaRead(MDIO_BASE_ADDRESS + 0x0);
        if (val == 0)
        {
            return false;
        }
        usleep(10 * 1000);
    }
    
    return true;
}

bool MdioWrite(uint8_t phy, uint8_t dev, uint16_t addr, uint16_t data)
{
    CMutexLock lock(mdioLock);
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);
    FpgaWrite(MDIO_BASE_ADDRESS + 0x5, 0x0);    // Add preamble, Ext mode
    FpgaWrite(MDIO_BASE_ADDRESS + 0x1, (phy << 8) | dev);   // port and phy address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x6, addr);   // register address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x3, data);   // send data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x4, 0x0);    // clear previous data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x2, 0x1);    // start write
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);

    return true;
}

uint16_t MdioRead(uint8_t phy, uint8_t dev, uint16_t addr)
{
    CMutexLock lock(mdioLock);
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);
    FpgaWrite(MDIO_BASE_ADDRESS + 0x5, 0x0);    // Add preamble, Ext mode
    FpgaWrite(MDIO_BASE_ADDRESS + 0x1, (phy << 8) | dev);   // port and phy address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x6, addr);   // register address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x4, 0x0); // clear previous data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x2, 0x3);    // start read
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);

    return FpgaRead(MDIO_BASE_ADDRESS + 0x4);
}

bool MiiWrite(uint8_t phy, uint8_t addr, uint16_t data)
{
    CMutexLock lock(mdioLock);
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);
    FpgaWrite(MDIO_BASE_ADDRESS + 0x5, 0x1);    // Add preamble, Normal mode
    FpgaWrite(MDIO_BASE_ADDRESS + 0x1, (addr << 8) | phy);   // port and phy address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x3, data);   // send data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x4, 0x0);    // clear previous data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x2, 0x1);    // start write
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);

    return true;
}

uint16_t MiiRead(uint8_t phy, uint8_t addr)
{
    CMutexLock lock(mdioLock);
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);
    FpgaWrite(MDIO_BASE_ADDRESS + 0x5, 0x1);    // Add preamble, Normal mode
    FpgaWrite(MDIO_BASE_ADDRESS + 0x1, (addr << 8) | phy);   // port and phy address
    FpgaWrite(MDIO_BASE_ADDRESS + 0x4, 0x0);    // clear previous data
    FpgaWrite(MDIO_BASE_ADDRESS + 0x2, 0x3);    // start read
    if (MdioBusy()) APP_LOG("%s:%d timeout", __FUNCTION__, __LINE__);

    return FpgaRead(MDIO_BASE_ADDRESS + 0x4);
}

void MdioMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        MdioInit();
    }
    else if (argc == 5 && argv[0][0] == 'w')
    {
        long phy = strtol(argv[1], NULL, 0);
        long dev = strtol(argv[2], NULL, 0);
        long addr = strtol(argv[3], NULL, 0);
        long data = strtol(argv[4], NULL, 0);
        MdioWrite((uint8_t)phy, (uint8_t)dev, (uint16_t)addr, (uint16_t)data);
    }
    else if (argc >= 4 && argv[0][0] == 'r')
    {
        long phy = strtol(argv[1], NULL, 0);
        long dev = strtol(argv[2], NULL, 0);
        long addr = strtol(argv[3], NULL, 0);
        int len = 1;
        uint16_t data;
        if (argc == 5) len = atoi(argv[4]);
        for (int i = 0; i < len; i++)
        {
            data = MdioRead((uint8_t)phy, (uint8_t)dev, (uint16_t)(addr + i));
            APP_LOG("[%02x:%02x:%04x]: 0x%04x", (uint8_t)phy, (uint8_t)dev, (uint16_t)(addr + i), data); 
        }
    }
    else if (argc == 4 && argv[0][1] == 'w')
    {
        long phy = strtol(argv[1], NULL, 0);
        long addr = strtol(argv[2], NULL, 0);
        long data = strtol(argv[3], NULL, 0);
        MiiWrite((uint8_t)phy, (uint8_t)addr, (uint16_t)data);
    }
    else if (argc >= 3 && argv[0][1] == 'r')
    {
        long phy = strtol(argv[1], NULL, 0);
        long addr = strtol(argv[2], NULL, 0);
        int len = 1;
        uint16_t data;
        if (argc == 4) len = atoi(argv[3]);
        for (int i = 0; i < len; i++)
        {
            data = MiiRead((uint8_t)phy, (uint8_t)(addr + i));
            APP_LOG("[%02x:%02x]: 0x%04x", (uint8_t)phy, (uint8_t)(addr + i), data); 
        }
    }
    else if (argc == 1 && argv[0][1] == 'd')
    {
        uint8_t phy;
        uint16_t data;
        for (phy = 0; phy < 32; phy++)
        {
            data = MiiRead(phy, 0x2);
            if (data != 0 && data != 0xffff)
            {
                APP_LOG("[%02x]: 0x%04x, 0x%04x", phy, data, MiiRead(phy, 0x3)); 
            }
        }
    }
    else
    {
        APP_LOG("Mdio");
        APP_LOG("\t i");
        APP_LOG("\t w <phy> <dev> <addr> <data>");
        APP_LOG("\t r <phy> <dev> <addr> [len]");
        APP_LOG("\t nw <phy> <addr> <data>");
        APP_LOG("\t nr <phy> <addr> [len]");
        APP_LOG("\t nd");
    }
}

