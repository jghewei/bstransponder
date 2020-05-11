#ifndef _PSUMOB_H_
#define _PSUMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int PsuMob_State_Normal = 0;
const int PsuMob_State_Failure = 1;
const int PsuMob_State_NoPower = 2;
const int PsuMob_State_NotPresent = 3;

const char* PsuMob_State_to_string(int value);

// AID - alarm id
const char* PsuAlm_desp_string(AlarmId::Enum aid);

class PsuMob : public BaseMob
{
public:
    static PsuMob* AddInst(KeyType::Enum type, int id);
    static PsuMob* GetInst(int inst);
    static PsuMob* GetInstByKey(Key &key);
    static PsuMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    PsuMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, PsuMob> State;

    Key key;

private:
    static std::vector<PsuMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

