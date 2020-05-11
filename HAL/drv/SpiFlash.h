#ifndef _SPI_FLASH_
#define _SPI_FLASH_

void SpiFlashInit(uint32_t my_bus, uint32_t my_cs);
void SpiFlashRead(uint32_t addr, uint8_t *data, uint16_t len);
void SpiFlashWrite(uint32_t addr, uint8_t *data, uint16_t len);
void SpiFlashProgram(uint32_t addr, const char *fname);
void SpiFlashEraseChip(void);
void SpiFlashMenu(int argc, char *argv[]);

#endif

