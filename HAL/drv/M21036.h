#ifndef _M21036_H
#define _M21036_H

#define M21036_LANE_DIGI        0
#define M21036_LANE_SFP         1
#define M21036_LANE_CFP         2

void M21036Init(void);
void M21036Menu(int argc, char *argv[]);

void M21036SetRxParam(int type, int lane, uint8_t eq);
void M21036SetTxParam(int type, int lane, uint8_t de, uint8_t drv);

#endif
