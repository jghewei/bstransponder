#include "DanOS.h"
#include "Immr.h"
#include "Gpio.h"
#include "MemMap.h"

#define IMMR_SICRH_REG      0x118
#define IMMR_GPDIR_REG      0xC00
#define IMMR_GPODR_REG      0xC04
#define IMMR_GPDAT_REG      0xC08

void GpioDump(void);

void GpioInit(void)
{
    //ImmrWriteMask(IMMR_SICRH_REG, 0x0030C000, 0x0030C000);    // enable Pin function for GPIO8,11,12,13,14,TSEC2,TSOBI2
    ImmrWrite(IMMR_SICRH_REG, 0xdd77d101);

    uint32_t out_pin_mask = FPGA_SPI_CS | FF_SPI_CS | SI5368_RESET | SI5368_SPI_CS | ENET_RESET | FPGA_PROGRAM;
    ImmrWriteMask(IMMR_GPDIR_REG, out_pin_mask, out_pin_mask);

    //GpioDump();

    if (GpioGetPin(RESET_BUTTON) == 0)
    {
        APP_LOG("Reset Button is low at startup, do something!");
    }
}

void GpioSetPin(uint32_t pin, int val)
{
    uint32_t data = (val == 0) ? 0 : pin;
    ImmrWriteMask(IMMR_GPDAT_REG, data, pin);
}

int GpioGetPin(uint32_t pin)
{
    uint32_t data = ImmrRead(IMMR_GPDAT_REG);
    return ((data & pin) == 0) ? 0 : 1;
}

void GpioDump(void)
{
    APP_LOG("GPIO status:");
    APP_LOG("\tFPGA_SPI_CS    GPIO1  : %d", GpioGetPin(FPGA_SPI_CS));
    APP_LOG("\tFF_SPI_CS      GPIO4  : %d", GpioGetPin(FF_SPI_CS));
    APP_LOG("\tSI5368_RESET   GPIO16 : %d", GpioGetPin(SI5368_RESET));
    APP_LOG("\tSI5368_SPI_CS  GPIO8  : %d", GpioGetPin(SI5368_SPI_CS));
    APP_LOG("\tSI5368_LOL     GPIO10 : %d", GpioGetPin(SI5368_LOL));
    APP_LOG("\tReset Button   GPIO11 : %d", GpioGetPin(RESET_BUTTON));
    APP_LOG("\tENET_RESET     GPIO13 : %d", GpioGetPin(ENET_RESET));
    APP_LOG("\tFPGA_PROGRAM   GPIO15 : %d", GpioGetPin(FPGA_PROGRAM));
    APP_LOG("\tBOARD_ADDRESS         : 0x%08x", GpioGetAddress());
}

void GpioMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        GpioInit();
    }
    else if (argc == 1 && argv[0][0] == 'd')
    {
        GpioDump();
    }
    else if (argc == 3 && argv[0][0] == 's')
    {
        int pin = atoi(argv[1]);
        int val = atoi(argv[2]);
        uint32_t mask = 0x80000000 >> pin;
        GpioSetPin(mask, val);
    }
    else
    {
        APP_LOG("gpio");
        APP_LOG("\t i");
        APP_LOG("\t d");
        APP_LOG("\t s <GPIO Num> <0|1 value>");
    }
}

uint32_t GpioGetAddress()
{
    static MemAddr addrBase = NULL;

    if (addrBase == NULL)
    {
        addrBase = MemMap(0x20000000, 0xff);
    }

    return MemRead(addrBase);
}

