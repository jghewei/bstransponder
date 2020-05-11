#ifndef _GPIO_H_
#define _GPIO_H_

#define FPGA_SPI_CS         (0x80000000 >> 1)       // GPIO1
#define FF_SPI_CS           (0x80000000 >> 4)       // GPIO4
#define SI5368_RESET        (0x80000000 >> 16)      // GPIO16
#define SI5368_SPI_CS       (0x80000000 >> 8)       // GPIO8
#define SI5368_LOL          (0x80000000 >> 10)      // GPIO10
#define RESET_BUTTON        (0x80000000 >> 11)      // GPIO11
#define ENET_RESET          (0x80000000 >> 13)      // GPIO13
#define FPGA_PROGRAM        (0x80000000 >> 15)      // GPIO15

void GpioInit(void);
void GpioSetPin(uint32_t pin, int val);
int GpioGetPin(uint32_t pin);
void GpioMenu(int argc, char *argv[]);
uint32_t GpioGetAddress();

#endif

