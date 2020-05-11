#include "OtuMob.h"

std::vector<OtuMob*> OtuMob::mInst;
int OtuMob::mInstNum = 0;
onSetCallback OtuMob::mSetCb = NULL;
onGetCallback OtuMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/OtuMob.";
#else
static const char *DBFile = "/tmp/OtuMob.";
#endif

OtuMob* OtuMob::AddInst(KeyType::Enum type, int id)
{
    OtuMob *inst = new OtuMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

OtuMob* OtuMob::GetInst(int inst)
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

OtuMob* OtuMob::GetInstByKey(Key &key)
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

OtuMob* OtuMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int OtuMob::GetInstNum()
{
    return mInstNum;
}

OtuMob::OtuMob(KeyType::Enum type, int id) :
    FecMode("FecMode", this),
    TestSignalType("TestSignalType", this),
    TestSignalStatus("TestSignalStatus", this),
    key(type, id)
{
    AddAlarm(AlarmId::OTU_LOS, new Alarm(key, AlarmId::OTU_LOS, OtuAlm_desp_string(AlarmId::OTU_LOS), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::OTU_LOL, new Alarm(key, AlarmId::OTU_LOL, OtuAlm_desp_string(AlarmId::OTU_LOL), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::OTU_LOF, new Alarm(key, AlarmId::OTU_LOF, OtuAlm_desp_string(AlarmId::OTU_LOF), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::OTU_LOM, new Alarm(key, AlarmId::OTU_LOM, OtuAlm_desp_string(AlarmId::OTU_LOM), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::OTU_TIM, new Alarm(key, AlarmId::OTU_TIM, OtuAlm_desp_string(AlarmId::OTU_TIM), false, AlarmSeverity::ALM_MN));
    AddAlarm(AlarmId::OTU_BDI, new Alarm(key, AlarmId::OTU_BDI, OtuAlm_desp_string(AlarmId::OTU_BDI), false, AlarmSeverity::ALM_NR));
    AddAlarm(AlarmId::OTU_BERSD, new Alarm(key, AlarmId::OTU_BERSD, OtuAlm_desp_string(AlarmId::OTU_BERSD), false, AlarmSeverity::ALM_MN));
    AddCounter(OtuCnt_CorrectBits, new Counter(key, OtuCnt_CorrectBits, OtuCnt_name_string(OtuCnt_CorrectBits), OtuCnt_unit_string(OtuCnt_CorrectBits)));
    AddCounter(OtuCnt_UncorrectBits, new Counter(key, OtuCnt_UncorrectBits, OtuCnt_name_string(OtuCnt_UncorrectBits), OtuCnt_unit_string(OtuCnt_UncorrectBits)));
    AddCounter(OtuCnt_FrameCount, new Counter(key, OtuCnt_FrameCount, OtuCnt_name_string(OtuCnt_FrameCount), OtuCnt_unit_string(OtuCnt_FrameCount)));
    AddCounter(OtuCnt_Bip8, new Counter(key, OtuCnt_Bip8, OtuCnt_name_string(OtuCnt_Bip8), OtuCnt_unit_string(OtuCnt_Bip8)));
    AddCounter(OtuCnt_Bei, new Counter(key, OtuCnt_Bei, OtuCnt_name_string(OtuCnt_Bei), OtuCnt_unit_string(OtuCnt_Bei)));
}

void OtuMob::Save()
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

void OtuMob::Load()
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

const char* OtuMob_FecMode_to_string(int value)
{
    switch (value) {
        case 0: return "Disabled";
        case 1: return "GFEC";
        case 2: return "I4";
        case 3: return "I7";
        default: return "???";
    }
}

const char* OtuMob_TestSignalType_to_string(int value)
{
    switch (value) {
        case 0: return "Disabled";
        case 1: return "PRBS";
        default: return "???";
    }
}

const char* OtuMob_TestSignalStatus_to_string(int value)
{
    switch (value) {
        case 0: return "Disabled";
        case 1: return "InSync";
        case 2: return "OutSync";
        case 3: return "ErrorSync";
        default: return "???";
    }
}

const char* OtuAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::OTU_LOS: return "Loss Of Signal";
        case AlarmId::OTU_LOL: return "Loss Of Lock";
        case AlarmId::OTU_LOF: return "Loss Of Frame - OTU";
        case AlarmId::OTU_LOM: return "Loss of MultiFrame - OTU";
        case AlarmId::OTU_TIM: return "Trace Identifier Mismatch on OTU";
        case AlarmId::OTU_BDI: return "BackWard Defect Indication - OTU";
        case AlarmId::OTU_BERSD: return "Bit Error Rate Signal Degrade - OTU";
        default: return "???";
    }
}

const char* OtuCnt_name_string(int cid)
{
    switch (cid) {
        case OtuCnt_CorrectBits: return "CorrectBits";
        case OtuCnt_UncorrectBits: return "UncorrectBits";
        case OtuCnt_FrameCount: return "FrameCount";
        case OtuCnt_Bip8: return "Bip8";
        case OtuCnt_Bei: return "Bei";
        default: return "???";
    }
}

const char* OtuCnt_unit_string(int cid)
{
    switch (cid) {
        case OtuCnt_CorrectBits: return "Bits";
        case OtuCnt_UncorrectBits: return "Bits";
        case OtuCnt_FrameCount: return "Frames";
        case OtuCnt_Bip8: return "Bits";
        case OtuCnt_Bei: return "Bits";
        default: return "???";
    }
}

