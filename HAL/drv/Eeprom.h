#ifndef _EEPROM_
#define _EEPROM_

#include <string>

//void EepromInit(uint32_t my_bus, uint32_t my_cs);
uint8_t EepromRead(uint32_t addr);
void EepromWrite(uint32_t addr, uint8_t data);
//void SpiFlashProgram(uint32_t addr, const char *fname);
//void SpiFlashEraseChip(void);
void EepromMenu(int argc, char *argv[]);
void EepromSetTag(char tag, std::string str);
std::string EepromGetTag(char tag);

#endif

