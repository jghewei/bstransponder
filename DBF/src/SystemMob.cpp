#include "SystemMob.h"

std::vector<SystemMob*> SystemMob::mInst;
int SystemMob::mInstNum = 0;
onSetCallback SystemMob::mSetCb = NULL;
onGetCallback SystemMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/SystemMob.";
#else
static const char *DBFile = "/tmp/SystemMob.";
#endif

SystemMob* SystemMob::AddInst(KeyType::Enum type, int id)
{
    SystemMob *inst = new SystemMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

SystemMob* SystemMob::GetInst(int inst)
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

SystemMob* SystemMob::GetInstByKey(Key &key)
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

SystemMob* SystemMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int SystemMob::GetInstNum()
{
    return mInstNum;
}

SystemMob::SystemMob(KeyType::Enum type, int id) :
    Name("Name", this),
    Contact("Contact", this),
    Location("Location", this),
    Description("Description", this),
    UpTime("UpTime", this),
    CpuUsage("CpuUsage", this),
    MemUsage("MemUsage", this),
    CurrentDateTime("CurrentDateTime", this),
    SetDateTime("SetDateTime", this),
    SwVersion("SwVersion", this),
    Restart("Restart", this),
    Database("Database", this),
    LampTest("LampTest", this),
    key(type, id)
{
}

void SystemMob::Save()
{
    std::string buffer;

    buffer += "Name=";
    buffer += Name.GetValue();
    buffer += "\n";

    buffer += "Contact=";
    buffer += Contact.GetValue();
    buffer += "\n";

    buffer += "Location=";
    buffer += Location.GetValue();
    buffer += "\n";

    buffer += "Description=";
    buffer += Description.GetValue();
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

void SystemMob::Load()
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
        else if (check_cfg_name(Name.name(), linebuf))
            Name.SetValue((const char*)&linebuf[Name.name().size()+1]);
        else if (check_cfg_name(Contact.name(), linebuf))
            Contact.SetValue((const char*)&linebuf[Contact.name().size()+1]);
        else if (check_cfg_name(Location.name(), linebuf))
            Location.SetValue((const char*)&linebuf[Location.name().size()+1]);
        else if (check_cfg_name(Description.name(), linebuf))
            Description.SetValue((const char*)&linebuf[Description.name().size()+1]);
    }

    fclose(fp);
}

const char* SystemMob_Restart_to_string(int value)
{
    switch (value) {
        case 0: return "None";
        case 1: return "Reboot";
        default: return "???";
    }
}

const char* SystemMob_Database_to_string(int value)
{
    switch (value) {
        case 0: return "None";
        case 1: return "Clear";
        default: return "???";
    }
}

const char* SystemMob_LampTest_to_string(int value)
{
    switch (value) {
        case 0: return "Off";
        case 1: return "On";
        default: return "???";
    }
}

