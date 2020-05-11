#include "DanOS.h"
#include "MemMap.h"
#include "SpiFlash.h"
#include "Spi.h"

// Micron N25Q064A, 64Mb
#define SPIFLASH_SECTOR_SIZE         (64*1024)      // 64kB
#define SPIFLASH_PAGE_SIZE           (256)
#define SPIFLASH_MAX_SPI_BYTES       (SPIFLASH_PAGE_SIZE+4)

#define SPIFLASH_OP_WRITE_ENABLE     0x06
#define SPIFLASH_OP_WRITE_DISABLE    0x04
#define SPIFLASH_OP_READ_ID          0x9F
#define SPIFLASH_OP_READ_STATUS_REG  0x05
#define SPIFLASH_OP_WRITE_STATUS_REG 0x01
#define SPIFLASH_OP_READ_DATA        0x03
#define SPIFLASH_OP_FAST_READ        0x0B
#define SPIFLASH_OP_PAGE_PROGRAM     0x02
#define SPIFLASH_OP_SECTOR_ERASE     0xD8
#define SPIFLASH_OP_BULK_ERASE       0xC7

#define SPIFLASH_LOCK_MASK           0x54    // 6, 4:2

// spi_mode = SPI_CLK_INVERT | SPI_CLK_PHASE | SPI_NORM_DATA | SPI_MASTER | SPI_BIT_LENGTH(8);
//static uint32_t spi_mode = 0x37700000;
// spi_freq = 1MHz - could be faster, but slow it down to be safe for wider range of the platform
//static uint32_t spi_freq = 1000000;

static uint8_t statusReg = 0;
static uint8_t dout[SPIFLASH_MAX_SPI_BYTES];
static uint8_t din[SPIFLASH_MAX_SPI_BYTES];

static int chip_sel = 0;

void SpiFlashInit(int chip)
{
    chip_sel = chip;
    
    // Read the flash ID and find out what type of flash we see.
    dout[0] = SPIFLASH_OP_READ_ID;
    dout[1] = 0; dout[2] = 0; dout[3] = 0;

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 4);
    SpiChipSel(chip_sel, 0);

    APP_LOG("SpiFlash ID 0x%02x%02x%02x = 0x20ba17", din[1], din[2], din[3]);
}

void SpiFlashSendCommand(uint8_t opcode)
{
    dout[0] = opcode;

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 1);
    SpiChipSel(chip_sel, 0);
}

uint8_t SpiFlashReadByte(uint32_t addr)
{
    dout[0] = SPIFLASH_OP_READ_DATA;
    dout[1] = (uint8_t)(addr >> 16);
    dout[2] = (uint8_t)(addr >> 8);
    dout[3] = (uint8_t)(addr);

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 5);
    SpiChipSel(chip_sel, 0);

    return din[4];
}

void SpiFlashReadStatus(void)
{
    dout[0] = SPIFLASH_OP_READ_STATUS_REG;
    dout[1] = 0;
    din[1] = 0;

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(chip_sel, 0);

    // Store in static var so other fn()s can see it.
    statusReg = din[1];
}

void SpiFlashWriteStatus(uint8_t wrData)
{
    dout[0] = SPIFLASH_OP_WRITE_STATUS_REG;
    dout[1] = wrData;

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 2);
    SpiChipSel(chip_sel, 0);

    // cycle time on status register write is between 5 and 15 ms
    // we'll let caller worry about it.
}

uint8_t SpiFlashWaitBusyClear(uint32_t delay, uint32_t subdelay)
{
    uint32_t usecs = 0;

    while (usecs < delay)
    {
        SpiFlashReadStatus();
        if ((statusReg & 0x01) == 0)
        {
            return 1;
        }
        usleep(subdelay);
        usecs += subdelay;
    }
    
    return 0;
}

void SpiFlashWriteByte(uint32_t address, uint8_t *wrdata, int len)
{
    SpiFlashWaitBusyClear(3*1000*1000, 1000);

    SpiFlashSendCommand(SPIFLASH_OP_WRITE_ENABLE);
    
    dout[0] = SPIFLASH_OP_PAGE_PROGRAM;
    dout[1] = (uint8_t)(address >> 16);
    dout[2] = (uint8_t)(address >> 8);
    dout[3] = (uint8_t)(address);
    memcpy(&dout[4], wrdata, len);
    //dout[4] = wrdata;

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 4 + len);
    SpiChipSel(chip_sel, 0);

    if (!SpiFlashWaitBusyClear(5*1000, 500))
    {
        APP_LOG("%s timeout", __FUNCTION__);
    }
}

void SpiFlashWrite(uint32_t addr, uint8_t *data, uint16_t len)
{   
    int txlen = 0;
    while (len > 0)
    {
        txlen = (len >= SPIFLASH_PAGE_SIZE)?SPIFLASH_PAGE_SIZE:len;
        SpiFlashWriteByte(addr, data, txlen);
        addr += txlen;
        data += txlen;
        len -= txlen;
    }
}

void SpiFlashLock(void)
{
    SpiFlashWriteStatus(SPIFLASH_LOCK_MASK);
}

void SpiFlashUnLock(void)
{
    SpiFlashReadStatus();
    SpiFlashWriteStatus(statusReg & ~SPIFLASH_LOCK_MASK);
    // cycle time for status reg writes is between 1.3ms and 8ms
    if ((!SpiFlashWaitBusyClear(8000, 1000)) || ((statusReg & SPIFLASH_LOCK_MASK)!= 0))
    {
        APP_LOG("%s timeout", __FUNCTION__);
    }
}

void SpiFlashEraseSector(uint8_t sector)
{
    SpiFlashSendCommand(SPIFLASH_OP_WRITE_ENABLE);

    uint32_t addr = sector * SPIFLASH_SECTOR_SIZE;
    dout[0] = SPIFLASH_OP_SECTOR_ERASE;
    dout[1] = (uint8_t)(addr >> 16);
    dout[2] = (uint8_t)(addr >> 8);
    dout[3] = (uint8_t)(addr);

    SpiChipSel(chip_sel, 1);
    SpiIO(dout, din, 4);
    SpiChipSel(chip_sel, 0);

    // 0.7s - 3s 
    if (!SpiFlashWaitBusyClear(3*1000*1000, 1000))
    {
        APP_LOG("%s timeout", __FUNCTION__);
    }
}

void SpiFlashBulkErase(void)
{
    APP_LOG("Erase whole chip, need some time...");
    
    SpiFlashSendCommand(SPIFLASH_OP_WRITE_ENABLE);

    SpiFlashSendCommand(SPIFLASH_OP_BULK_ERASE);

    // max 240s
    if (!SpiFlashWaitBusyClear(240*1000*1000, 1000*1000))
    {
        APP_LOG("%s timeout", __FUNCTION__);
    }
}

void SpiFlashMenu(int argc, char *argv[])
{
    if (argc == 2 && argv[0][0] == 'i')
    {
        if (argv[1][0] == 'f')
            SpiFlashInit(SPI_CS_FPGA);
        else
            SpiFlashInit(SPI_CS_FLASH);
    }
    else if (argc == 2 && argv[0][0] == 'e')
    {
        if (argv[1][0] == 'c')
            SpiFlashBulkErase();
        else
            SpiFlashEraseSector(atoi(argv[1]));
    }
    else if (argc == 2 && argv[0][0] == 'p')
    {
        SpiFlashBulkErase();
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL)
        {
            APP_LOG("open file %s failed", argv[1]);
            return;
        }

        // obtain file size:
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        rewind(fp);

        int txlen = 0, rlen = 0;
        uint8_t txbuf[4096] = {0};
        while (txlen < size)
        {
            rlen = fread(txbuf, 1, 4096, fp);
            SpiFlashWrite(txlen, txbuf, rlen);
            printf(".");
            //printf("%d\n", txlen);
            fflush(stdout);
            txlen += rlen;
        }
        printf("\n");

        fclose(fp);
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = SpiFlashReadByte(reg + i);
            APP_LOG("[%08x]: 0x%02x", reg + i, (uint8_t)val); 
        }
    }
    else if (argc >= 2 && argv[0][0] == 'd')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            if (i % 16 == 0) printf("\n[%08x]:", reg + i);
            val = SpiFlashReadByte(reg + i);
            printf(" %02x", val);
            if ((i+1) % 8 == 0) printf(" ");
        }
        printf("\n");
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint8_t data = 0;
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        data = (uint8_t)val;
        SpiFlashWriteByte(reg, &data, 1);
    }
    else
    {
        APP_LOG("spiflash");
        APP_LOG("\t i <fpga|digi>");
        APP_LOG("\t erase <sector>");
        APP_LOG("\t dump <reg> [len]");
        APP_LOG("\t program <file name>");
        APP_LOG("\t read <reg> [len]");
        APP_LOG("\t write <reg> <val>");
    }
}

