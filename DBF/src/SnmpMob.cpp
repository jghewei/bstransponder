#include "SnmpMob.h"

std::vector<SnmpMob*> SnmpMob::mInst;
int SnmpMob::mInstNum = 0;
onSetCallback SnmpMob::mSetCb = NULL;
onGetCallback SnmpMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/SnmpMob.";
#else
static const char *DBFile = "/tmp/SnmpMob.";
#endif

SnmpMob* SnmpMob::AddInst(KeyType::Enum type, int id)
{
    SnmpMob *inst = new SnmpMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

SnmpMob* SnmpMob::GetInst(int inst)
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

SnmpMob* SnmpMob::GetInstByKey(Key &key)
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

SnmpMob* SnmpMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int SnmpMob::GetInstNum()
{
    return mInstNum;
}

SnmpMob::SnmpMob(KeyType::Enum type, int id) :
    Enable("Enable", this),
    GetCommunity("GetCommunity", this),
    SetCommunity("SetCommunity", this),
    TrapReceiver1("TrapReceiver1", this),
    TrapReceiver2("TrapReceiver2", this),
    TrapReceiver3("TrapReceiver3", this),
    key(type, id)
{
}

void SnmpMob::Save()
{
    std::string buffer;

    buffer += "GetCommunity=";
    buffer += GetCommunity.GetValue();
    buffer += "\n";

    buffer += "SetCommunity=";
    buffer += SetCommunity.GetValue();
    buffer += "\n";

    buffer += "TrapReceiver1=";
    buffer += TrapReceiver1.GetValue();
    buffer += "\n";

    buffer += "TrapReceiver2=";
    buffer += TrapReceiver2.GetValue();
    buffer += "\n";

    buffer += "TrapReceiver3=";
    buffer += TrapReceiver3.GetValue();
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

void SnmpMob::Load()
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
        else if (check_cfg_name(GetCommunity.name(), linebuf))
            GetCommunity.SetValue((const char*)&linebuf[GetCommunity.name().size()+1]);
        else if (check_cfg_name(SetCommunity.name(), linebuf))
            SetCommunity.SetValue((const char*)&linebuf[SetCommunity.name().size()+1]);
        else if (check_cfg_name(TrapReceiver1.name(), linebuf))
            TrapReceiver1.SetValue((const char*)&linebuf[TrapReceiver1.name().size()+1]);
        else if (check_cfg_name(TrapReceiver2.name(), linebuf))
            TrapReceiver2.SetValue((const char*)&linebuf[TrapReceiver2.name().size()+1]);
        else if (check_cfg_name(TrapReceiver3.name(), linebuf))
            TrapReceiver3.SetValue((const char*)&linebuf[TrapReceiver3.name().size()+1]);
    }

    fclose(fp);
}

const char* SnmpMob_Enable_to_string(int value)
{
    switch (value) {
        case 0: return "No";
        case 1: return "Yes";
        default: return "???";
    }
}

