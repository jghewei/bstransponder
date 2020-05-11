#include "DanOS.h"
#include "PCA9535.h"
#include "I2c.h"
#include "CMutex.h"

#define I2C_BUS             1
const uint8_t i2c_addr[3] = {0x20, 0x22, 0x23};

#define PCA_REG_INPUT       0x0
#define PCA_REG_OUTPUT      0x2
#define PCA_REG_CONFIG      0x6

static CMutex pcaLock;

uint16_t Pca9535Read(int dev, uint8_t addr)
{   
    uint8_t tbuf[2] = {0};
    uint8_t rbuf[2] = {0};

    tbuf[0] = i2c_addr[dev];
    tbuf[1] = addr;
    I2cRead(I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf));
    
    return (uint16_t)(rbuf[0] | ((uint16_t)rbuf[1] << 8));
}

void Pca9535Write(int dev, uint8_t addr, uint16_t data)
{   
    uint8_t tbuf[4] = {0};

    tbuf[0] = i2c_addr[dev];
    tbuf[1] = addr;
    tbuf[2] = (uint8_t)((data >> 0) & 0xff);
    tbuf[3] = (uint8_t)((data >> 8) & 0xff);
    I2cWrite(I2C_BUS, tbuf, sizeof(tbuf));
}

void Pca9535SetPin(int dev, int pin, int val)
{
    if (pin > 7) pin -= 2;
    CMutexLock lock(pcaLock);

    // set output mode
    uint16_t tmp = Pca9535Read(dev, PCA_REG_CONFIG);
    tmp &= ~(1<<pin);
    Pca9535Write(dev, PCA_REG_CONFIG, tmp);

    tmp = Pca9535Read(dev, PCA_REG_OUTPUT);
    if (val == 0)
        tmp &= ~(1<<pin);
    else
        tmp |= (1<<pin);
    Pca9535Write(dev, PCA_REG_OUTPUT, tmp);
}

int Pca9535GetPin(int dev, int pin)
{
    if (pin > 7) pin -= 2;
    CMutexLock lock(pcaLock);

    // set input mode
    uint16_t val = Pca9535Read(dev, PCA_REG_CONFIG);
    val |= (1<<pin);
    Pca9535Write(dev, PCA_REG_CONFIG, val);

    val = Pca9535Read(dev, PCA_REG_INPUT);
    if ((val & (1<<pin)) == 0)
        return 0;
    else
        return 1;
}

void Pca9535Dump(int dev)
{   
    for (int i = 0; i < 18; i++)
    {
        APP_LOG("P%02d = %d", i, Pca9535GetPin(dev, i));
        if (i == 7) i += 2;
    }
}

void Pca9535Menu(int argc, char *argv[])
{   
    if (argc == 3 && argv[1][0] == 'r')
    {
        int dev = atoi(argv[0]);
        uint8_t cmd = (uint8_t)(atoi(argv[2]));
        APP_LOG("[%08x]: 0x%04x", cmd, Pca9535Read(dev, cmd));
    }
    else if (argc == 4 && argv[1][0] == 'w')
    {
        int dev = atoi(argv[0]);
        uint8_t cmd = (uint8_t)(atoi(argv[2]));
        uint32_t val;
        sscanf(argv[3], "%x", &val);
        Pca9535Write(dev, cmd, (uint16_t)val);
    }
    else if (argc == 3 && argv[1][0] == 'g')
    {
        int dev = atoi(argv[0]);
        int pin = atoi(argv[2]);
        APP_LOG("P%02d = %d", pin, Pca9535GetPin(dev, pin));
    }
    else if (argc == 4 && argv[1][0] == 's')
    {
        int dev = atoi(argv[0]);
        int pin = atoi(argv[2]);
        int val = atoi(argv[3]);
        Pca9535SetPin(dev, pin, val);
    }
    else if (argc == 2 && argv[1][0] == 'd')
    {
        Pca9535Dump(atoi(argv[0]));
    }
    else
    {
        APP_LOG("pca9535");
        APP_LOG("\t <dev> dump");
        APP_LOG("\t <dev> setpin <pin> <val>");
        APP_LOG("\t <dev> getpin <pin>");
        APP_LOG("\t <dev> read <reg>");
        APP_LOG("\t <dev> write <reg> <val>");
        APP_LOG("\t dev - 0-0x20, 1-0x22, 2-0x23");
        APP_LOG("\t reg - 0-in, 2-out, 6-cfg");
        APP_LOG("\t pin - 0~17");
        APP_LOG("\t val - 0|1");
    }
}


