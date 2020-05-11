#include "StmMob.h"

std::vector<StmMob*> StmMob::mInst;
int StmMob::mInstNum = 0;
onSetCallback StmMob::mSetCb = NULL;
onGetCallback StmMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/StmMob.";
#else
static const char *DBFile = "/tmp/StmMob.";
#endif

StmMob* StmMob::AddInst(KeyType::Enum type, int id)
{
    StmMob *inst = new StmMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

StmMob* StmMob::GetInst(int inst)
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

StmMob* StmMob::GetInstByKey(Key &key)
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

StmMob* StmMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int StmMob::GetInstNum()
{
    return mInstNum;
}

StmMob::StmMob(KeyType::Enum type, int id) :
    key(type, id)
{
    AddAlarm(AlarmId::STM_LOS, new Alarm(key, AlarmId::STM_LOS, StmAlm_desp_string(AlarmId::STM_LOS), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::STM_LOF, new Alarm(key, AlarmId::STM_LOF, StmAlm_desp_string(AlarmId::STM_LOF), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::STM_TIM_R, new Alarm(key, AlarmId::STM_TIM_R, StmAlm_desp_string(AlarmId::STM_TIM_R), false, AlarmSeverity::ALM_MN));
    AddAlarm(AlarmId::STM_AIS_L, new Alarm(key, AlarmId::STM_AIS_L, StmAlm_desp_string(AlarmId::STM_AIS_L), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::STM_RFI_L, new Alarm(key, AlarmId::STM_RFI_L, StmAlm_desp_string(AlarmId::STM_RFI_L), true, AlarmSeverity::ALM_MN));
}

void StmMob::Save()
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

void StmMob::Load()
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

const char* StmAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::STM_LOS: return "Loss Of Signal";
        case AlarmId::STM_LOF: return "Loss Of Frame";
        case AlarmId::STM_TIM_R: return "TIM";
        case AlarmId::STM_AIS_L: return "AIS";
        case AlarmId::STM_RFI_L: return "RFI";
        default: return "???";
    }
}

