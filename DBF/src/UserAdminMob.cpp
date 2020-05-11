#include "UserAdminMob.h"

std::vector<UserAdminMob*> UserAdminMob::mInst;
int UserAdminMob::mInstNum = 0;
onSetCallback UserAdminMob::mSetCb = NULL;
onGetCallback UserAdminMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/UserAdminMob.";
#else
static const char *DBFile = "/tmp/UserAdminMob.";
#endif

UserAdminMob* UserAdminMob::AddInst(KeyType::Enum type, int id)
{
    UserAdminMob *inst = new UserAdminMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

UserAdminMob* UserAdminMob::GetInst(int inst)
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

UserAdminMob* UserAdminMob::GetInstByKey(Key &key)
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

UserAdminMob* UserAdminMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int UserAdminMob::GetInstNum()
{
    return mInstNum;
}

UserAdminMob::UserAdminMob(KeyType::Enum type, int id) :
    UserName("UserName", this),
    Password("Password", this),
    Privilege("Privilege", this),
    key(type, id)
{
}

void UserAdminMob::Save()
{
    std::string buffer;

    buffer += "UserName=";
    buffer += UserName.GetValue();
    buffer += "\n";

    buffer += "Password=";
    buffer += Password.GetValue();
    buffer += "\n";

    buffer += "Privilege=";
    buffer += to_string(Privilege.GetValue());
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

void UserAdminMob::Load()
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
        else if (check_cfg_name(UserName.name(), linebuf))
            UserName.SetValue((const char*)&linebuf[UserName.name().size()+1]);
        else if (check_cfg_name(Password.name(), linebuf))
            Password.SetValue((const char*)&linebuf[Password.name().size()+1]);
        else if (check_cfg_name(Privilege.name(), linebuf))
            Privilege.SetValue(atoi((const char*)&linebuf[Privilege.name().size()+1]));
    }

    fclose(fp);
}

const char* UserAdminMob_Privilege_to_string(int value)
{
    switch (value) {
        case 0: return "Administrator";
        case 1: return "Service";
        case 2: return "Monitor";
        default: return "???";
    }
}

