#include "FileMgmtMob.h"

std::vector<FileMgmtMob*> FileMgmtMob::mInst;
int FileMgmtMob::mInstNum = 0;
onSetCallback FileMgmtMob::mSetCb = NULL;
onGetCallback FileMgmtMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/FileMgmtMob.";
#else
static const char *DBFile = "/tmp/FileMgmtMob.";
#endif

FileMgmtMob* FileMgmtMob::AddInst(KeyType::Enum type, int id)
{
    FileMgmtMob *inst = new FileMgmtMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

FileMgmtMob* FileMgmtMob::GetInst(int inst)
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

FileMgmtMob* FileMgmtMob::GetInstByKey(Key &key)
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

FileMgmtMob* FileMgmtMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int FileMgmtMob::GetInstNum()
{
    return mInstNum;
}

FileMgmtMob::FileMgmtMob(KeyType::Enum type, int id) :
    ServerIpAddress("ServerIpAddress", this),
    ServerProtocol("ServerProtocol", this),
    FilePathName("FilePathName", this),
    Action("Action", this),
    State("State", this),
    key(type, id)
{
}

void FileMgmtMob::Save()
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

void FileMgmtMob::Load()
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

const char* FileMgmtMob_ServerProtocol_to_string(int value)
{
    switch (value) {
        case 2: return "Tftp";
        default: return "???";
    }
}

const char* FileMgmtMob_Action_to_string(int value)
{
    switch (value) {
        case 0: return "None";
        case 1: return "ExportDatabase";
        case 2: return "ImportDatabase";
        case 3: return "UpgradeSoftware";
        default: return "???";
    }
}

const char* FileMgmtMob_State_to_string(int value)
{
    switch (value) {
        case 0: return "None";
        case 1: return "FileTransfer";
        case 2: return "FileTransferFailed";
        case 3: return "Execute";
        case 4: return "ExecuteFailed";
        case 5: return "Finished";
        default: return "???";
    }
}

