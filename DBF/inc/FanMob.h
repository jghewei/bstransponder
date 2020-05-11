#ifndef _FANMOB_H_
#define _FANMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int FanMob_State_Normal = 0;
const int FanMob_State_Failure = 1;
const int FanMob_State_NotPresent = 2;
const int FanMob_ForceSpeed_Auto = 0;
const int FanMob_ForceSpeed_FullSpeed = 1;

const char* FanMob_State_to_string(int value);
const char* FanMob_ForceSpeed_to_string(int value);

// AID - alarm id
const char* FanAlm_desp_string(AlarmId::Enum aid);

class FanMob : public BaseMob
{
public:
    static FanMob* AddInst(KeyType::Enum type, int id);
    static FanMob* GetInst(int inst);
    static FanMob* GetInstByKey(Key &key);
    static FanMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    FanMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, FanMob> State;
    Attribute<int, FanMob> Speed;
    Attribute<int, FanMob> ForceSpeed;

    Key key;

private:
    static std::vector<FanMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

