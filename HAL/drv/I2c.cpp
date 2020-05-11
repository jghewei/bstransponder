#include "DanOS.h"
#include "Immr.h"
#include "I2c.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_DEV_NUM     2

const char *i2c_dev[I2C_DEV_NUM] =
{
     "/dev/i2c-0",
     "/dev/i2c-1",
};

#ifdef __TARGET__
static int i2c_fd[I2C_DEV_NUM] = {0};
#endif

void I2cInit(void)
{
#ifdef __TARGET__
    for (int i = 0; i < I2C_DEV_NUM; i++)
    {
        i2c_fd[i] = open(i2c_dev[i], O_RDWR | O_SYNC);
        if (i2c_fd[i] < 0)
        {
            APP_LOG("ERROR: i2c device open failed, dev = %d", i);
        }
        // TODO set i2c device frequency
    }
#endif
}

bool I2cWrite(int dev, uint8_t *tbuf, uint16_t tlen)
{
    bool rv = true;
#ifdef __TARGET__
    int ret;
    uint8_t slaveAddr = tbuf[0];
    struct i2c_rdwr_ioctl_data i2c_work_queue = {0, 0};

    i2c_work_queue.nmsgs = 1;
    i2c_work_queue.msgs = (struct i2c_msg *)malloc(i2c_work_queue.nmsgs * sizeof(struct i2c_msg));
    if (!i2c_work_queue.msgs)
    {
        APP_LOG("ERROR: i2c write message malloc failed, dev = %d", dev);
        rv = false;
    }
    else
    {
        /* configure write msg */
        i2c_work_queue.msgs[0].addr = (int)slaveAddr;
        i2c_work_queue.msgs[0].flags = 0; // write
        i2c_work_queue.msgs[0].buf = tbuf + 1;
        i2c_work_queue.msgs[0].len = tlen - 1;

        ret = ioctl(i2c_fd[dev], I2C_RDWR, &i2c_work_queue);
        if (ret < 0)
        {
            APP_LOG("ERROR: i2c ioctl write failed, dev = %d:0x%02x, ret = %d", dev, slaveAddr, ret);
            rv = false;
        }

        free (i2c_work_queue.msgs);
    }
#endif
    return rv;
}

bool I2cRead(int dev, uint8_t *tbuf, uint16_t tlen, uint8_t *rbuf, uint16_t rlen)
{
    bool rv = true;
#ifdef __TARGET__
    int ret;
    uint8_t slaveAddr = tbuf[0];
    struct i2c_rdwr_ioctl_data i2c_work_queue = {0, 0};

    // Clear the rx buffer.
    memset(rbuf, 0, rlen);

    if (tlen > 1)
    {
        i2c_work_queue.nmsgs = 2;
    }
    else
    {
        i2c_work_queue.nmsgs = 1;
    }

    i2c_work_queue.msgs = (struct i2c_msg *)malloc(i2c_work_queue.nmsgs * sizeof(struct i2c_msg));
    if (!i2c_work_queue.msgs)
    {
        APP_LOG("ERROR: i2c read message malloc failed, dev = %d", dev);
        rv = false;
    }
    else
    {
        if (tlen > 1)
        {
            /* configure write msg */
            i2c_work_queue.msgs[0].addr = (int)slaveAddr;
            i2c_work_queue.msgs[0].flags = 0; // write
            i2c_work_queue.msgs[0].buf = tbuf + 1;
            i2c_work_queue.msgs[0].len = tlen - 1;

            /* configure read msg (i2c driver will automagically do rpt-start) */
            i2c_work_queue.msgs[1].addr = (int)slaveAddr;
            i2c_work_queue.msgs[1].flags = I2C_M_RD; // read
            i2c_work_queue.msgs[1].buf = rbuf;
            i2c_work_queue.msgs[1].len = rlen;
        }
        else
        {
            /* configure read msg  */
            i2c_work_queue.msgs[0].addr = (int)slaveAddr;
            i2c_work_queue.msgs[0].flags = I2C_M_RD; // read
            i2c_work_queue.msgs[0].buf = rbuf;
            i2c_work_queue.msgs[0].len = rlen;
        }

        ret = ioctl(i2c_fd[dev], I2C_RDWR, &i2c_work_queue);
        if (ret < 0)
        {
            APP_LOG("ERROR: i2c ioctl read failed, dev = %d:0x%02x, ret = %d", dev, slaveAddr, ret);
            rv = false;
        }

        free (i2c_work_queue.msgs);
    }
#endif
    return rv;
}

static uint16_t i2c_dev_sel = I2C_SEL_NULL;
void I2cSel(uint16_t i2cdev)
{
    uint8_t tbuf[2] = {0, 0};
    if (i2c_dev_sel == i2cdev) return;

    // reset the previous state
    tbuf[0] = 0x72;
    tbuf[1] = 0x00;
    I2cWrite(1, tbuf, sizeof(tbuf));

    tbuf[0] = 0x75;
    tbuf[1] = 0x00;
    I2cWrite(1, tbuf, sizeof(tbuf));

    // select new
    tbuf[0] = (uint8_t)((i2cdev >> 8) & 0xff);
    tbuf[1] = (uint8_t)((i2cdev >> 0) & 0xff);
    I2cWrite(1, tbuf, sizeof(tbuf));
}

static uint8_t txbuf[16] = {0};
static uint8_t rxbuf[16] = {0};
static uint16_t txlen = 0;

uint16_t str2hex(const char *str, uint8_t *buffer)
{
    char *p = (char *)str;
    char buf[3] = {0};
    int len = strlen(str);
    int num = len / 2;
    int val = 0;
    if ((len % 2) != 0 || num > 16)
    {
        APP_LOG("%s param error, len = %d, num = %d", __FILE__, len, num);
        return 0;
    }

    for (int i = 0; i < num; i++)
    {
        buf[0] = *p++;
        buf[1] = *p++;
        sscanf(buf, "%x", &val);
        buffer[i] = (uint8_t)val;
    }

    return (uint16_t)num;
}

void I2cMenu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        I2cInit();
    }
    else if (argc == 1 && argv[0][0] == 't')
    {
        // TODO I2cInit();
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        int dev = atoi(argv[1]);
        txlen = str2hex(argv[2], txbuf);

        I2cWrite(dev, txbuf, txlen);

        printf("TX: ");
        for (int i = 0; i < txlen; i++)
        {
            printf("%02x ", txbuf[i]);
        }
        printf("\n");
    }
    else if (argc == 4 && argv[0][0] == 'r')
    {
        int dev = atoi(argv[1]);
        int rxlen = atoi(argv[3]);
        txlen = str2hex(argv[2], txbuf);

        I2cRead(dev, txbuf, txlen, rxbuf, (uint16_t)rxlen);

        printf("TX: ");
        for (int i = 0; i < txlen; i++)
        {
            printf("%02x ", txbuf[i]);
        }
        printf("\n");
        printf("RX: ");
        for (int i = 0; i < rxlen; i++)
        {
            printf("%02x ", rxbuf[i]);
        }
        printf("\n");
    }
    else
    {
        APP_LOG("I2c");
        APP_LOG("\t i");
        APP_LOG("\t t - to verify i2c driver");
        APP_LOG("\t w <i2c bus> <tx hex data eg. 001122aabbcc>");
        APP_LOG("\t r <i2c bus> <tx hex data eg. 001122aabbcc> <rx len>");
    }
}
