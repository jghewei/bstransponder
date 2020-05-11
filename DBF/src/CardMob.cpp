#include "CardMob.h"

std::vector<CardMob*> CardMob::mInst;
int CardMob::mInstNum = 0;
onSetCallback CardMob::mSetCb = NULL;
onGetCallback CardMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/CardMob.";
#else
static const char *DBFile = "/tmp/CardMob.";
#endif

CardMob* CardMob::AddInst(KeyType::Enum type, int id)
{
    CardMob *inst = new CardMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

CardMob* CardMob::GetInst(int inst)
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

CardMob* CardMob::GetInstByKey(Key &key)
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

CardMob* CardMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int CardMob::GetInstNum()
{
    return mInstNum;
}

CardMob::CardMob(KeyType::Enum type, int id) :
    BoardTemp("BoardTemp", this),
    LowTempThd("LowTempThd", this),
    HighTempThd("HighTempThd", this),
    OperStatus("OperStatus", this),
    key(type, id)
{
    AddAlarm(AlarmId::TEMP_LT, new Alarm(key, AlarmId::TEMP_LT, CardAlm_desp_string(AlarmId::TEMP_LT), false, AlarmSeverity::ALM_MN));
    AddAlarm(AlarmId::TEMP_HT, new Alarm(key, AlarmId::TEMP_HT, CardAlm_desp_string(AlarmId::TEMP_HT), false, AlarmSeverity::ALM_CR));
}

void CardMob::Save()
{
    std::string buffer;

    buffer += "LowTempThd=";
    buffer += to_string(LowTempThd.GetValue());
    buffer += "\n";

    buffer += "HighTempThd=";
    buffer += to_string(HighTempThd.GetValue());
    buffer += "\n";

    if (buffer.size() == 0) return;
    APP_LOG("%s", buffer.c_str());
    std::string fname = DBFile;
    fname += to_string(key.id());
    FILE *fp = fopen(fname.c_str(), "w");
    if (fp == NULL) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);
}

void CardMob::Load()
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
        else if (check_cfg_name(LowTempThd.name(), linebuf))
            LowTempThd.SetValue(atoi((const char*)&linebuf[LowTempThd.name().size()+1]));
        else if (check_cfg_name(HighTempThd.name(), linebuf))
            HighTempThd.SetValue(atoi((const char*)&linebuf[HighTempThd.name().size()+1]));
    }

    fclose(fp);
}

const char* CardMob_OperStatus_to_string(int value)
{
    switch (value) {
        case 0: return "Normal";
        case 1: return "Minor";
        case 2: return "Major";
        case 3: return "Critical";
        default: return "???";
    }
}

const char* CardAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::TEMP_LT: return "LowTemp";
        case AlarmId::TEMP_HT: return "HighTemp";
        default: return "???";
    }
}

