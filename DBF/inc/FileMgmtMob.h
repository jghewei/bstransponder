#ifndef _FILEMGMTMOB_H_
#define _FILEMGMTMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int FileMgmtMob_ServerProtocol_Tftp = 2;
const int FileMgmtMob_Action_None = 0;
const int FileMgmtMob_Action_ExportDatabase = 1;
const int FileMgmtMob_Action_ImportDatabase = 2;
const int FileMgmtMob_Action_UpgradeSoftware = 3;
const int FileMgmtMob_State_None = 0;
const int FileMgmtMob_State_FileTransfer = 1;
const int FileMgmtMob_State_FileTransferFailed = 2;
const int FileMgmtMob_State_Execute = 3;
const int FileMgmtMob_State_ExecuteFailed = 4;
const int FileMgmtMob_State_Finished = 5;

const char* FileMgmtMob_ServerProtocol_to_string(int value);
const char* FileMgmtMob_Action_to_string(int value);
const char* FileMgmtMob_State_to_string(int value);

class FileMgmtMob : public BaseMob
{
public:
    static FileMgmtMob* AddInst(KeyType::Enum type, int id);
    static FileMgmtMob* GetInst(int inst);
    static FileMgmtMob* GetInstByKey(Key &key);
    static FileMgmtMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    FileMgmtMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, FileMgmtMob> ServerIpAddress;
    Attribute<int, FileMgmtMob> ServerProtocol;
    Attribute<std::string, FileMgmtMob> FilePathName;
    Attribute<int, FileMgmtMob> Action;
    Attribute<int, FileMgmtMob> State;

    Key key;

private:
    static std::vector<FileMgmtMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

