#include "PsuMob.h"

std::vector<PsuMob*> PsuMob::mInst;
int PsuMob::mInstNum = 0;
onSetCallback PsuMob::mSetCb = NULL;
onGetCallback PsuMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/PsuMob.";
#else
static const char *DBFile = "/tmp/PsuMob.";
#endif

PsuMob* PsuMob::AddInst(KeyType::Enum type, int id)
{
    PsuMob *inst = new PsuMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

PsuMob* PsuMob::GetInst(int inst)
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

PsuMob* PsuMob::GetInstByKey(Key &key)
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

PsuMob* PsuMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int PsuMob::GetInstNum()
{
    return mInstNum;
}

PsuMob::PsuMob(KeyType::Enum type, int id) :
    State("State", this),
    key(type, id)
{
    AddAlarm(AlarmId::PWR_FAULT, new Alarm(key, AlarmId::PWR_FAULT, PsuAlm_desp_string(AlarmId::PWR_FAULT), false, AlarmSeverity::ALM_MJ));
}

void PsuMob::Save()
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

void PsuMob::Load()
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

const char* PsuMob_State_to_string(int value)
{
    switch (value) {
        case 0: return "Normal";
        case 1: return "Failure";
        case 2: return "NoPower";
        case 3: return "NotPresent";
        default: return "???";
    }
}

const char* PsuAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::PWR_FAULT: return "Power Fault";
        default: return "???";
    }
}

