#ifndef _PCIE_H_
#define _PCIE_H_

void PcieInit(void);
uint32_t PcieRead(uint32_t addr);
void PcieWrite(uint32_t addr, uint32_t data);
void PcieMenu(int argc, char *argv[]);

#endif

