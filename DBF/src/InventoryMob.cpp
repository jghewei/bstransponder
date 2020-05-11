#include "InventoryMob.h"

std::vector<InventoryMob*> InventoryMob::mInst;
int InventoryMob::mInstNum = 0;
onSetCallback InventoryMob::mSetCb = NULL;
onGetCallback InventoryMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/InventoryMob.";
#else
static const char *DBFile = "/tmp/InventoryMob.";
#endif

InventoryMob* InventoryMob::AddInst(KeyType::Enum type, int id)
{
    InventoryMob *inst = new InventoryMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

InventoryMob* InventoryMob::GetInst(int inst)
{
    if (inst < GetInstNum())
    {
        return mInst[inst];
    }
    else
    {
        return NULL;
    }
}

InventoryMob* InventoryMob::GetInstByKey(Key &key)
{
    for (int i = 0; i < GetInstNum(); i++)
    {
        if (mInst[i]->key == key)
        {
            return mInst[i];
        }
    }
    return NULL;
}

InventoryMob* InventoryMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int InventoryMob::GetInstNum()
{
    return mInstNum;
}

InventoryMob::InventoryMob(KeyType::Enum type, int id) :
    Present("Present", this),
    ModuleType("ModuleType", this),
    PartNumber("PartNumber", this),
    SerialNumber("SerialNumber", this),
    HwVersion("HwVersion", this),
    VendorName("VendorName", this),
    ProduceDate("ProduceDate", this),
    FwVersion("FwVersion", this),
    key(type, id)
{
}

void InventoryMob::Save()
{
    std::string buffer;

    if (buffer.size() == 0) return;
    APP_LOG("%s", buffer.c_str());
    std::string fname = DBFile;
    fname += to_string(key.id());
    FILE *fp = fopen(fname.c_str(), "w");
    if (fp == NULL) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);
}

void InventoryMob::Load()
{
    std::string fname = DBFile;
    fname += to_string(key.id());
    FILE *fp = fopen(fname.c_str(), "r");
    if (fp == NULL) return;
    char linebuf[256] = {0};

    while (fgets(linebuf, 256, fp) != NULL)
    {
        int len = strlen(linebuf);
        if (len < 3) continue;
        if (linebuf[len-1] == '\n') linebuf[len-1] = 0;

        if (0) {}
    }

    fclose(fp);
}

const char* InventoryMob_Present_to_string(int value)
{
    switch (value) {
        case 0: return "No";
        case 1: return "Yes";
        default: return "???";
    }
}

