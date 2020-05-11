#ifndef _SYSTEMMOB_H_
#define _SYSTEMMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int SystemMob_Restart_None = 0;
const int SystemMob_Restart_Reboot = 1;
const int SystemMob_Database_None = 0;
const int SystemMob_Database_Clear = 1;
const int SystemMob_LampTest_Off = 0;
const int SystemMob_LampTest_On = 1;

const char* SystemMob_Restart_to_string(int value);
const char* SystemMob_Database_to_string(int value);
const char* SystemMob_LampTest_to_string(int value);

class SystemMob : public BaseMob
{
public:
    static SystemMob* AddInst(KeyType::Enum type, int id);
    static SystemMob* GetInst(int inst);
    static SystemMob* GetInstByKey(Key &key);
    static SystemMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    SystemMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, SystemMob> Name;
    Attribute<std::string, SystemMob> Contact;
    Attribute<std::string, SystemMob> Location;
    Attribute<std::string, SystemMob> Description;
    Attribute<int, SystemMob> UpTime;
    Attribute<int, SystemMob> CpuUsage;
    Attribute<int, SystemMob> MemUsage;
    Attribute<std::string, SystemMob> CurrentDateTime;
    Attribute<std::string, SystemMob> SetDateTime;
    Attribute<std::string, SystemMob> SwVersion;
    Attribute<int, SystemMob> Restart;
    Attribute<int, SystemMob> Database;
    Attribute<int, SystemMob> LampTest;

    Key key;

private:
    static std::vector<SystemMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

