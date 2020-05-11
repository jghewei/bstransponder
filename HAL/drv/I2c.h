#ifndef _I2C_H_
#define _I2C_H_

#define I2C_SEL_NULL        (0)
#define I2C_SEL_SFP0        ((0x75<<8)|(0x01))
#define I2C_SEL_SFP1        ((0x75<<8)|(0x02))
#define I2C_SEL_SFP2        ((0x75<<8)|(0x04))
#define I2C_SEL_SFP3        ((0x75<<8)|(0x08))
#define I2C_SEL_SFP4        ((0x75<<8)|(0x10))
#define I2C_SEL_SFP5        ((0x75<<8)|(0x20))
#define I2C_SEL_SFP6        ((0x75<<8)|(0x40))
#define I2C_SEL_SFP7        ((0x75<<8)|(0x80))
#define I2C_SEL_SFP8        ((0x72<<8)|(0x01))
#define I2C_SEL_SFP9        ((0x72<<8)|(0x02))
#define I2C_SEL_OSC_SFP0    ((0x72<<8)|(0x08))
#define I2C_SEL_OSC_SFP1    ((0x72<<8)|(0x04))
#define I2C_SEL_1V2_M       ((0x72<<8)|(0x10))
#define I2C_SEL_0V9_A       ((0x72<<8)|(0x20))
#define I2C_SEL_0V9_D1      ((0x72<<8)|(0x40))
#define I2C_SEL_0V9_D2      ((0x72<<8)|(0x80))

void I2cInit(void);
bool I2cWrite(int dev, uint8_t *tbuf, uint16_t tlen);
bool I2cRead(int dev, uint8_t *tbuf, uint16_t tlen, uint8_t *rbuf, uint16_t rlen);
void I2cSel(uint16_t i2cdev);
void I2cMenu(int argc, char *argv[]);

#endif

