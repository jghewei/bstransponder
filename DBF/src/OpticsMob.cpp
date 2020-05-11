#include "OpticsMob.h"

std::vector<OpticsMob*> OpticsMob::mInst;
int OpticsMob::mInstNum = 0;
onSetCallback OpticsMob::mSetCb = NULL;
onGetCallback OpticsMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/OpticsMob.";
#else
static const char *DBFile = "/tmp/OpticsMob.";
#endif

OpticsMob* OpticsMob::AddInst(KeyType::Enum type, int id)
{
    OpticsMob *inst = new OpticsMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

OpticsMob* OpticsMob::GetInst(int inst)
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

OpticsMob* OpticsMob::GetInstByKey(Key &key)
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

OpticsMob* OpticsMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int OpticsMob::GetInstNum()
{
    return mInstNum;
}

OpticsMob::OpticsMob(KeyType::Enum type, int id) :
    Type("Type", this),
    Laser("Laser", this),
    Los("Los", this),
    TxPower("TxPower", this),
    RxPower("RxPower", this),
    Temp("Temp", this),
    key(type, id)
{
}

void OpticsMob::Save()
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

void OpticsMob::Load()
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

const char* OpticsMob_Laser_to_string(int value)
{
    switch (value) {
        case 0: return "Off";
        case 1: return "On";
        default: return "???";
    }
}

const char* OpticsMob_Los_to_string(int value)
{
    switch (value) {
        case 0: return "On";
        case 1: return "Off";
        default: return "???";
    }
}

