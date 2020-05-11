#include "ShelfMob.h"

std::vector<ShelfMob*> ShelfMob::mInst;
int ShelfMob::mInstNum = 0;
onSetCallback ShelfMob::mSetCb = NULL;
onGetCallback ShelfMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/ShelfMob.";
#else
static const char *DBFile = "/tmp/ShelfMob.";
#endif

ShelfMob* ShelfMob::AddInst(KeyType::Enum type, int id)
{
    ShelfMob *inst = new ShelfMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

ShelfMob* ShelfMob::GetInst(int inst)
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

ShelfMob* ShelfMob::GetInstByKey(Key &key)
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

ShelfMob* ShelfMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int ShelfMob::GetInstNum()
{
    return mInstNum;
}

ShelfMob::ShelfMob(KeyType::Enum type, int id) :
    InletTemp("InletTemp", this),
    OutletTemp("OutletTemp", this),
    TempLowThd("TempLowThd", this),
    TempHighThd("TempHighThd", this),
    key(type, id)
{
    AddAlarm(AlarmId::TEMP_LT, new Alarm(key, AlarmId::TEMP_LT, ShelfAlm_desp_string(AlarmId::TEMP_LT), false, AlarmSeverity::ALM_MN));
    AddAlarm(AlarmId::TEMP_HT, new Alarm(key, AlarmId::TEMP_HT, ShelfAlm_desp_string(AlarmId::TEMP_HT), false, AlarmSeverity::ALM_CR));
}

void ShelfMob::Save()
{
    std::string buffer;

    buffer += "TempLowThd=";
    buffer += to_string(TempLowThd.GetValue());
    buffer += "\n";

    buffer += "TempHighThd=";
    buffer += to_string(TempHighThd.GetValue());
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

void ShelfMob::Load()
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
        else if (check_cfg_name(TempLowThd.name(), linebuf))
            TempLowThd.SetValue(atoi((const char*)&linebuf[TempLowThd.name().size()+1]));
        else if (check_cfg_name(TempHighThd.name(), linebuf))
            TempHighThd.SetValue(atoi((const char*)&linebuf[TempHighThd.name().size()+1]));
    }

    fclose(fp);
}

const char* ShelfAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::TEMP_LT: return "Low Temperature Alarm";
        case AlarmId::TEMP_HT: return "High Temperature Alarm";
        default: return "???";
    }
}

