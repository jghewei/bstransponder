#ifndef _MEM_MAP_
#define _MEM_MAP_

#include "DanOS.h"

#define IMMR_BASE       0xf0000000
#define IMMR_SIZE       0x00100000

typedef volatile unsigned char  *     MemAddr;
typedef volatile unsigned short *     MemAddr16;
typedef volatile unsigned int   *     MemAddr32;

MemAddr MemMap(uint32_t addr, uint32_t size);

void MemWrite(MemAddr addr, uint32_t data);
void MemWriteMask(MemAddr addr, uint32_t data, uint32_t mask);
uint32_t MemRead(MemAddr addr);

#endif

