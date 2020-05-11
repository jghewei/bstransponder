#ifndef _MDIO_H_
#define _MDIO_H_

void MdioInit(void);
bool MdioWrite(uint8_t phy, uint8_t dev, uint16_t addr, uint16_t data);
uint16_t MdioRead(uint8_t phy, uint8_t dev, uint16_t addr);
bool MiiWrite(uint8_t phy, uint8_t addr, uint16_t data);
uint16_t MiiRead(uint8_t phy, uint8_t addr);
void MdioMenu(int argc, char *argv[]);

#endif

