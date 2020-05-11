#include "DanOS.h"
#include "Eeprom.h"
#include "I2c.h"
#include <ctype.h>

#define SFP_I2C_BUS             0
#define SFP_I2C_ADDR            0x52

#define EEPROM_ENTRY_LEN        32

uint32_t EepromGetAddress(char tag)
{
    switch(tag)
    {
        case 'M': return 0x00;
        case 'P': return 0x20;
        case 'S': return 0x40;
        case 'H': return 0x60;
        case 'N': return 0x80;
        case 'D': return 0xA0;
        case 'F': return 0xC0;
        case 'A': return 0xE0;
        default : return 0x00;
    }
}

uint8_t EepromRead(uint32_t addr)
{   
    uint8_t tbuf[3] = {0};
    uint8_t rbuf[1] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = (addr >> 8) & 0xff;
    tbuf[2] = (addr >> 0) & 0xff;
    I2cRead(SFP_I2C_BUS, tbuf, sizeof(tbuf), rbuf, sizeof(rbuf));
    
    return rbuf[0];
}

void EepromWrite(uint32_t addr, uint8_t data)
{   
    uint8_t tbuf[4] = {0};

    tbuf[0] = SFP_I2C_ADDR;
    tbuf[1] = (addr >> 8) & 0xff;
    tbuf[2] = (addr >> 0) & 0xff;
    tbuf[3] = data;
    I2cWrite(SFP_I2C_BUS, tbuf, sizeof(tbuf));
    usleep(20*1000);
}

void EepromMenu(int argc, char *argv[])
{
    if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg;
        uint8_t val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            if (i % 16 == 0) printf("\n[%08x]:", reg + i);
            val = EepromRead(reg + i);
            printf(" %02x", val);
            if ((i+1) % 8 == 0) printf(" ");
        }
        printf("\n");
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        EepromWrite(reg, (uint8_t)val);
    }
    else if (argc == 2 && argv[0][0] == 'i')
    {
        if (argv[1][0] == 'T' || argv[1][0] == 't')
        {
            EepromSetTag('M', "Tranponder");
            EepromSetTag('P', "MXP-100GT");
        }
        else if (argv[1][0] == 'M' || argv[1][0] == 'm')
        {
            EepromSetTag('M', "Muxponder");
            EepromSetTag('P', "MXP-100GM");
        }
        EepromSetTag('S', "SN12345678");
        EepromSetTag('H', "1.0");
        EepromSetTag('N', "DanRiver");
        EepromSetTag('D', "2017-09-12");
        EepromSetTag('F', "N/A");
        EepromSetTag('A', "00:00:00:00:00:00");
        system("rm /mnt/sysfs/*; sync;");
    }
    else if (argc == 1 && argv[0][0] == 'd')
    {
        APP_LOG("\t%c - %s", 'M', EepromGetTag('M').c_str());
        APP_LOG("\t%c - %s", 'P', EepromGetTag('P').c_str());
        APP_LOG("\t%c - %s", 'S', EepromGetTag('S').c_str());
        APP_LOG("\t%c - %s", 'H', EepromGetTag('H').c_str());
        APP_LOG("\t%c - %s", 'N', EepromGetTag('N').c_str());
        APP_LOG("\t%c - %s", 'D', EepromGetTag('D').c_str());
        APP_LOG("\t%c - %s", 'F', EepromGetTag('F').c_str());
        APP_LOG("\t%c - %s", 'A', EepromGetTag('A').c_str());
    }
    else if (argc == 3 && argv[0][0] == 's')
    {
        EepromSetTag(argv[1][0], argv[2]);
    }
    else
    {
        APP_LOG("eeprom");
        APP_LOG("\t dump");
        APP_LOG("\t init <Muxponder|Tranponder>");
        APP_LOG("\t read <reg> [len]");
        APP_LOG("\t write <reg> <val>");
        APP_LOG("\t set <tag> <string max 32Byte>");
        APP_LOG("\t\t tag:");
        APP_LOG("\t\t\t M - Module Type");
        APP_LOG("\t\t\t P - Part Number");
        APP_LOG("\t\t\t S - Serial Number");
        APP_LOG("\t\t\t H - Hardware Version");
        APP_LOG("\t\t\t N - Vendor Name");
        APP_LOG("\t\t\t D - Produce Date");
        APP_LOG("\t\t\t F - Firmware Version");
        APP_LOG("\t\t\t A - MAC address");
    }
}

void EepromSetTag(char tag, std::string str)
{
    uint8_t data;
    uint32_t addr = EepromGetAddress(tag);
    for (uint32_t i = 0; i < EEPROM_ENTRY_LEN; i++)
    {
        data = (i < str.size()) ? ((uint8_t)str[i]) : 0;
        EepromWrite(addr + i, data);
    }
}

std::string EepromGetTag(char tag)
{
    int data;
    std::string str;
    uint32_t addr = EepromGetAddress(tag);
    for (uint32_t i = 0; i < EEPROM_ENTRY_LEN; i++)
    {
        data = (int)EepromRead(addr + i);
        if (isgraph(data))
            str += (char)data;
        else
            break;
    }
    return str;
}

