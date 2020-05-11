#ifndef _STMMOB_H_
#define _STMMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"



// AID - alarm id
const char* StmAlm_desp_string(AlarmId::Enum aid);

class StmMob : public BaseMob
{
public:
    static StmMob* AddInst(KeyType::Enum type, int id);
    static StmMob* GetInst(int inst);
    static StmMob* GetInstByKey(Key &key);
    static StmMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    StmMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:

    Key key;

private:
    static std::vector<StmMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

