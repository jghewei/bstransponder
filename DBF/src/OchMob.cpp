#include "OchMob.h"

std::vector<OchMob*> OchMob::mInst;
int OchMob::mInstNum = 0;
onSetCallback OchMob::mSetCb = NULL;
onGetCallback OchMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/OchMob.";
#else
static const char *DBFile = "/tmp/OchMob.";
#endif

OchMob* OchMob::AddInst(KeyType::Enum type, int id)
{
    OchMob *inst = new OchMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

OchMob* OchMob::GetInst(int inst)
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

OchMob* OchMob::GetInstByKey(Key &key)
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

OchMob* OchMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int OchMob::GetInstNum()
{
    return mInstNum;
}

OchMob::OchMob(KeyType::Enum type, int id) :
    FecMode("FecMode", this),
    ChanNum("ChanNum", this),
    MinLaserFreq("MinLaserFreq", this),
    MaxLaserFreq("MaxLaserFreq", this),
    GridSpacing("GridSpacing", this),
    MaxChans("MaxChans", this),
    CurTxLaserFreq("CurTxLaserFreq", this),
    CurRxLaserFreq("CurRxLaserFreq", this),
    DGD("DGD", this),
    CD("CD", this),
    OSNR("OSNR", this),
    Qfactor("Qfactor", this),
    key(type, id)
{
    AddCounter(OchCnt_CorrectBits, new Counter(key, OchCnt_CorrectBits, OchCnt_name_string(OchCnt_CorrectBits), OchCnt_unit_string(OchCnt_CorrectBits)));
    AddCounter(OchCnt_UncorrectBits, new Counter(key, OchCnt_UncorrectBits, OchCnt_name_string(OchCnt_UncorrectBits), OchCnt_unit_string(OchCnt_UncorrectBits)));
    AddCounter(OchCnt_Bip8, new Counter(key, OchCnt_Bip8, OchCnt_name_string(OchCnt_Bip8), OchCnt_unit_string(OchCnt_Bip8)));
    AddCounter(OchCnt_Bei, new Counter(key, OchCnt_Bei, OchCnt_name_string(OchCnt_Bei), OchCnt_unit_string(OchCnt_Bei)));
}

void OchMob::Save()
{
    std::string buffer;

    buffer += "FecMode=";
    buffer += to_string(FecMode.GetValue());
    buffer += "\n";

    buffer += "ChanNum=";
    buffer += to_string(ChanNum.GetValue());
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

void OchMob::Load()
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
        else if (check_cfg_name(FecMode.name(), linebuf))
            FecMode.SetValue(atoi((const char*)&linebuf[FecMode.name().size()+1]));
        else if (check_cfg_name(ChanNum.name(), linebuf))
            ChanNum.SetValue(atoi((const char*)&linebuf[ChanNum.name().size()+1]));
    }

    fclose(fp);
}

const char* OchMob_FecMode_to_string(int value)
{
    switch (value) {
        case 0: return "SDFEC_ENHANCE";
        case 1: return "SDFEC_COMP";
        case 2: return "GFEC";
        case 3: return "Transparent";
        default: return "???";
    }
}

const char* OchCnt_name_string(int cid)
{
    switch (cid) {
        case OchCnt_CorrectBits: return "CorrectBits";
        case OchCnt_UncorrectBits: return "UncorrectBits";
        case OchCnt_Bip8: return "Bip8";
        case OchCnt_Bei: return "Bei";
        default: return "???";
    }
}

const char* OchCnt_unit_string(int cid)
{
    switch (cid) {
        case OchCnt_CorrectBits: return "Bits";
        case OchCnt_UncorrectBits: return "Bits";
        case OchCnt_Bip8: return "Bits";
        case OchCnt_Bei: return "Bits";
        default: return "???";
    }
}

