#ifndef _IMMR_H_
#define _IMMR_H_

void ImmrInit(void);
void ImmrWrite(uint32_t reg, uint32_t val);
void ImmrWriteMask(uint32_t reg, uint32_t val, uint32_t mask);
uint32_t ImmrRead(uint32_t reg);
void ImmrMenu(int argc, char *argv[]);

#endif

