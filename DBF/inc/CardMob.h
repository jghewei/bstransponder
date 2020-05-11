#ifndef _CARDMOB_H_
#define _CARDMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int CardMob_OperStatus_Normal = 0;
const int CardMob_OperStatus_Minor = 1;
const int CardMob_OperStatus_Major = 2;
const int CardMob_OperStatus_Critical = 3;

const char* CardMob_OperStatus_to_string(int value);

// AID - alarm id
const char* CardAlm_desp_string(AlarmId::Enum aid);

class CardMob : public BaseMob
{
public:
    static CardMob* AddInst(KeyType::Enum type, int id);
    static CardMob* GetInst(int inst);
    static CardMob* GetInstByKey(Key &key);
    static CardMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    CardMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, CardMob> BoardTemp;
    Attribute<int, CardMob> LowTempThd;
    Attribute<int, CardMob> HighTempThd;
    Attribute<int, CardMob> OperStatus;

    Key key;

private:
    static std::vector<CardMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

