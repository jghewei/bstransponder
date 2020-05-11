#ifndef _SPI_H_
#define _SPI_H_

#define SPI_LOOP           0x40000000
#define SPI_CI             0x20000000
#define SPI_CP             0x10000000
#define SPI_DIV16          0x08000000
#define SPI_REV            0x04000000
#define SPI_MS             0x02000000
#define SPI_EN             0x01000000
#define SPI_LEN            20
#define SPI_PM             16
#define SPI_OD             0x00001000

#define SPI_CS_NULL         0
#define SPI_CS_FLASH        1
#define SPI_CS_TIMING       2
#define SPI_CS_FPGA         3

void SpiInit(void);
void SpiSetMode(uint32_t mode);
void SpiChipSel(int chip, int sel);
void SpiIO(uint8_t *txbuf, uint8_t *rxbuf, uint16_t len);
void SpiMenu(int argc, char *argv[]);

#endif

