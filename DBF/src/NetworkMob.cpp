#include "NetworkMob.h"

std::vector<NetworkMob*> NetworkMob::mInst;
int NetworkMob::mInstNum = 0;
onSetCallback NetworkMob::mSetCb = NULL;
onGetCallback NetworkMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/NetworkMob.";
#else
static const char *DBFile = "/tmp/NetworkMob.";
#endif

NetworkMob* NetworkMob::AddInst(KeyType::Enum type, int id)
{
    NetworkMob *inst = new NetworkMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

NetworkMob* NetworkMob::GetInst(int inst)
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

NetworkMob* NetworkMob::GetInstByKey(Key &key)
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

NetworkMob* NetworkMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int NetworkMob::GetInstNum()
{
    return mInstNum;
}

NetworkMob::NetworkMob(KeyType::Enum type, int id) :
    IpAddress("IpAddress", this),
    Netmask("Netmask", this),
    Gateway("Gateway", this),
    Mode("Mode", this),
    key(type, id)
{
}

void NetworkMob::Save()
{
    std::string buffer;

    buffer += "IpAddress=";
    buffer += IpAddress.GetValue();
    buffer += "\n";

    buffer += "Netmask=";
    buffer += Netmask.GetValue();
    buffer += "\n";

    buffer += "Gateway=";
    buffer += Gateway.GetValue();
    buffer += "\n";

    buffer += "Mode=";
    buffer += to_string(Mode.GetValue());
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

void NetworkMob::Load()
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
        else if (check_cfg_name(IpAddress.name(), linebuf))
            IpAddress.SetValue((const char*)&linebuf[IpAddress.name().size()+1]);
        else if (check_cfg_name(Netmask.name(), linebuf))
            Netmask.SetValue((const char*)&linebuf[Netmask.name().size()+1]);
        else if (check_cfg_name(Gateway.name(), linebuf))
            Gateway.SetValue((const char*)&linebuf[Gateway.name().size()+1]);
        else if (check_cfg_name(Mode.name(), linebuf))
            Mode.SetValue(atoi((const char*)&linebuf[Mode.name().size()+1]));
    }

    fclose(fp);
}

const char* NetworkMob_Mode_to_string(int value)
{
    switch (value) {
        case 0: return "Route";
        case 1: return "Bridge";
        default: return "???";
    }
}

