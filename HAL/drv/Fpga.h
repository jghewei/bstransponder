#ifndef _FPGA_H_
#define _FPGA_H_

void FpgaInit(void);
void FpgaWrite(uint32_t reg, uint32_t val);
void FpgaOrBitMask(uint32_t reg, uint32_t mask);
void FpgaAndBitMask(uint32_t reg, uint32_t mask);
uint32_t FpgaRead(uint32_t reg);
void FpgaMenu(int argc, char *argv[]);
bool FpgaGetSfpPresent(int dev);
bool FpgaGetSfpLos(int dev);
bool FpgaGetSfpLaserOn(int dev);
void FpgaSetSfpLaserOn(int dev, bool on);
void FpgaSetSfpRateSelect(int dev, bool sel);
bool FpgaGetCfpPresent(int dev);
bool FpgaGetCfpLos(int dev);
bool FpgaGetCfpLaserOn(int dev);
void FpgaGetPwrFault(int *a, int *b);
bool FpgaIsAlive(void);

#endif

