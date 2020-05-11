#include "FanMob.h"

std::vector<FanMob*> FanMob::mInst;
int FanMob::mInstNum = 0;
onSetCallback FanMob::mSetCb = NULL;
onGetCallback FanMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/FanMob.";
#else
static const char *DBFile = "/tmp/FanMob.";
#endif

FanMob* FanMob::AddInst(KeyType::Enum type, int id)
{
    FanMob *inst = new FanMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

FanMob* FanMob::GetInst(int inst)
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

FanMob* FanMob::GetInstByKey(Key &key)
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

FanMob* FanMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int FanMob::GetInstNum()
{
    return mInstNum;
}

FanMob::FanMob(KeyType::Enum type, int id) :
    State("State", this),
    Speed("Speed", this),
    ForceSpeed("ForceSpeed", this),
    key(type, id)
{
    AddAlarm(AlarmId::FAN_FAULT, new Alarm(key, AlarmId::FAN_FAULT, FanAlm_desp_string(AlarmId::FAN_FAULT), true, AlarmSeverity::ALM_CR));
}

void FanMob::Save()
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

void FanMob::Load()
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

const char* FanMob_State_to_string(int value)
{
    switch (value) {
        case 0: return "Normal";
        case 1: return "Failure";
        case 2: return "NotPresent";
        default: return "???";
    }
}

const char* FanMob_ForceSpeed_to_string(int value)
{
    switch (value) {
        case 0: return "Auto";
        case 1: return "FullSpeed";
        default: return "???";
    }
}

const char* FanAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::FAN_FAULT: return "Fan Fault";
        default: return "???";
    }
}

