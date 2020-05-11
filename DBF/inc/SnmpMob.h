#ifndef _SNMPMOB_H_
#define _SNMPMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int SnmpMob_Enable_No = 0;
const int SnmpMob_Enable_Yes = 1;

const char* SnmpMob_Enable_to_string(int value);

class SnmpMob : public BaseMob
{
public:
    static SnmpMob* AddInst(KeyType::Enum type, int id);
    static SnmpMob* GetInst(int inst);
    static SnmpMob* GetInstByKey(Key &key);
    static SnmpMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    SnmpMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, SnmpMob> Enable;
    Attribute<std::string, SnmpMob> GetCommunity;
    Attribute<std::string, SnmpMob> SetCommunity;
    Attribute<std::string, SnmpMob> TrapReceiver1;
    Attribute<std::string, SnmpMob> TrapReceiver2;
    Attribute<std::string, SnmpMob> TrapReceiver3;

    Key key;

private:
    static std::vector<SnmpMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

