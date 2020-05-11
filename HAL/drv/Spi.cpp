#include "DanOS.h"
#include "Immr.h"
#include "Gpio.h"
#include "Spi.h"

#define IMMR_SPMODE_REG     0x7020
#define IMMR_SPIE_REG       0x7024
#define IMMR_SPIM_REG       0x7028
#define IMMR_SPCOM_REG      0x702C
#define IMMR_SPITD_REG      0x7030
#define IMMR_SPIRD_REG      0x7034

#define SPIE_LT             0x00004000
#define SPIE_NE             0x00000200
#define SPIE_NF             0x00000100
#define SPCOM_LST           0x00400000

#define SPI_MODE_FLASH      0x3F700000
#define SPI_MODE_TIMING     0x3F700000

static int chipSel = SPI_CS_NULL;
static uint32_t spiFlashMode = SPI_MODE_FLASH;
static uint32_t spiTimingMode = SPI_MODE_TIMING;

void SpiInit(void)
{
}

void SpiSetMode(uint32_t mode)
{
    ImmrWrite(IMMR_SPMODE_REG, mode);
}

void SpiChipSel(int chip, int sel)
{
#ifdef __TARGET__
    if (chip == SPI_CS_FLASH)
    {
        if (sel == 1)
        {
            GpioSetPin(FF_SPI_CS, 0);
            SpiSetMode(spiFlashMode);
            chipSel = SPI_CS_FLASH;
        }
        else
        {
            GpioSetPin(FF_SPI_CS, 1);
            chipSel = SPI_CS_NULL;
        }
    }
    else if (chip == SPI_CS_TIMING)
    {
        if (sel == 1)
        {
            GpioSetPin(SI5368_SPI_CS, 0);
            SpiSetMode(spiTimingMode);
            chipSel = SPI_CS_TIMING;
        }
        else
        {
            GpioSetPin(SI5368_SPI_CS, 1);
            chipSel = SPI_CS_NULL;
        }
    }
    else if (chip == SPI_CS_FPGA)
    {
        if (sel == 1)
        {
            GpioSetPin(FPGA_SPI_CS, 0);
            SpiSetMode(spiFlashMode);
            chipSel = SPI_CS_TIMING;
        }
        else
        {
            GpioSetPin(FPGA_SPI_CS, 1);
            chipSel = SPI_CS_NULL;
        }
    }
#else
    APP_LOG("SpiChipSel chip = %d, sel = %d", chip, sel);
    chipSel = SPI_CS_NULL;
#endif
}

void SpiClearEvent(void)
{
    ImmrRead(IMMR_SPIRD_REG); // clear event
    ImmrWrite(IMMR_SPIE_REG, 0xffffffff);
}

void SpiSetLst(void)
{
    ImmrWrite(IMMR_SPCOM_REG, SPCOM_LST);
}

void SpiWaitLt(void)
{
    uint32_t val = 0;
    while ((val & SPIE_LT) == 0)
    {
        val = ImmrRead(IMMR_SPIE_REG);
    }
}

void SpiWaitNe(void)
{
    uint32_t val = 0;
    while ((val & SPIE_NE) == 0)
    {
        val = ImmrRead(IMMR_SPIE_REG);
    }
}

void SpiIO(uint8_t *txbuf, uint8_t *rxbuf, uint16_t len)
{
    uint16_t i;

#ifdef __TARGET__
    for (i = 0; i < len; i++)
    {
        SpiClearEvent();
        ImmrWrite(IMMR_SPITD_REG, txbuf[i]);
        //SpiSetLst();
        //SpiWaitLt();
        SpiWaitNe();
        //usleep(200*1000);   // 200ms
        rxbuf[i] = (uint8_t)ImmrRead(IMMR_SPIRD_REG);
    }
#else
    printf("TX: ");
    for (i = 0; i < len; i++)
    {
        printf("%02x ", txbuf[i]);
    }
    printf("\n");
#endif
}

void SpiMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        SpiInit();
    }
    else if (argc == 3 && argv[0][0] == 's')
    {
        uint32_t mode = 0;
        sscanf(argv[2], "%x", &mode);
        if (argv[1][0] == 'f')
        {
            spiFlashMode = mode;
        }
        else
        {
            spiTimingMode = mode;
        }
    }
    else if (argc >= 1 && argv[0][0] == 's')
    {
        APP_LOG("spiFlashMode  = 0x%08x", spiFlashMode);
        APP_LOG("spiTimingMode = 0x%08x", spiTimingMode);
    }
    else if (argc == 3 && argv[0][0] == 'c')
    {
        int sel = atoi(argv[2]);
        int chip = (argv[1][0] == 'f') ? SPI_CS_FLASH : SPI_CS_TIMING;
        SpiChipSel(chip, sel);
    }
    else if (argc == 2 && argv[0][0] == 'x')
    {
        uint8_t txbuf[16] = {0}, rxbuf[16]= {0};
        char *p = argv[1];
        char buf[3] = {0};
        int len = strlen(argv[1]);
        int num = len / 2;
        int val = 0;
        if ((len % 2) != 0 || num > 16)
        {
            APP_LOG("param error, len = %d, num = %d", len, num);
            return;
        }

        for (int i = 0; i < num; i++)
        {
            buf[0] = *p++;
            buf[1] = *p++;
            sscanf(buf, "%x", &val);
            txbuf[i] = (uint8_t)val;
        }

#ifdef __TARGET__
        SpiIO(txbuf, rxbuf, num);
#endif

        printf("TX: ");
        for (int i = 0; i < num; i++)
        {
            printf("%02x ", txbuf[i]);
        }
        printf("\n");
        printf("RX: ");
        for (int i = 0; i < num; i++)
        {
            printf("%02x ", rxbuf[i]);
        }
        printf("\n");
    }
    else
    {
        APP_LOG("spi");
        APP_LOG("\t i");
        APP_LOG("\t c <f|t> <0|1>");
        APP_LOG("\t s <f|t> <mode>");
        APP_LOG("\t x <bytes 001122334455>");
    }
}

