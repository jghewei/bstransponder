#include "PortMob.h"

std::vector<PortMob*> PortMob::mInst;
int PortMob::mInstNum = 0;
onSetCallback PortMob::mSetCb = NULL;
onGetCallback PortMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/PortMob.";
#else
static const char *DBFile = "/tmp/PortMob.";
#endif

PortMob* PortMob::AddInst(KeyType::Enum type, int id)
{
    PortMob *inst = new PortMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

PortMob* PortMob::GetInst(int inst)
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

PortMob* PortMob::GetInstByKey(Key &key)
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

PortMob* PortMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int PortMob::GetInstNum()
{
    return mInstNum;
}

PortMob::PortMob(KeyType::Enum type, int id) :
    AliasName("AliasName", this),
    PortMode("PortMode", this),
    AdminStatus("AdminStatus", this),
    OperStatus("OperStatus", this),
    LoopbackType("LoopbackType", this),
    key(type, id)
{
}

void PortMob::Save()
{
    std::string buffer;

    buffer += "AliasName=";
    buffer += AliasName.GetValue();
    buffer += "\n";

    buffer += "PortMode=";
    buffer += to_string(PortMode.GetValue());
    buffer += "\n";

    buffer += "AdminStatus=";
    buffer += to_string(AdminStatus.GetValue());
    buffer += "\n";

    buffer += "LoopbackType=";
    buffer += to_string(LoopbackType.GetValue());
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

void PortMob::Load()
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
        else if (check_cfg_name(AliasName.name(), linebuf))
            AliasName.SetValue((const char*)&linebuf[AliasName.name().size()+1]);
        else if (check_cfg_name(PortMode.name(), linebuf))
            PortMode.SetValue(atoi((const char*)&linebuf[PortMode.name().size()+1]));
        else if (check_cfg_name(AdminStatus.name(), linebuf))
            AdminStatus.SetValue(atoi((const char*)&linebuf[AdminStatus.name().size()+1]));
        else if (check_cfg_name(LoopbackType.name(), linebuf))
            LoopbackType.SetValue(atoi((const char*)&linebuf[LoopbackType.name().size()+1]));
    }

    fclose(fp);
}

const char* PortMob_PortMode_to_string(int value)
{
    switch (value) {
        case 0: return "NONE";
        case 1: return "ETH10G";
        case 2: return "ETH100G";
        case 3: return "STM64";
        case 5: return "OTU2";
        case 6: return "OTU2E";
        case 7: return "OTU4";
        case 10: return "ETH10G_GFP";
        default: return "???";
    }
}

const char* PortMob_AdminStatus_to_string(int value)
{
    switch (value) {
        case 0: return "Off";
        case 1: return "On";
        default: return "???";
    }
}

const char* PortMob_OperStatus_to_string(int value)
{
    switch (value) {
        case 0: return "Off";
        case 1: return "Up";
        case 2: return "Down";
        default: return "???";
    }
}

const char* PortMob_LoopbackType_to_string(int value)
{
    switch (value) {
        case 0: return "Disabled";
        case 1: return "Facility";
        case 2: return "Terminal";
        default: return "???";
    }
}

