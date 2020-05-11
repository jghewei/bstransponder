#ifndef _SHELFMOB_H_
#define _SHELFMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"



// AID - alarm id
const char* ShelfAlm_desp_string(AlarmId::Enum aid);

class ShelfMob : public BaseMob
{
public:
    static ShelfMob* AddInst(KeyType::Enum type, int id);
    static ShelfMob* GetInst(int inst);
    static ShelfMob* GetInstByKey(Key &key);
    static ShelfMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    ShelfMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, ShelfMob> InletTemp;
    Attribute<int, ShelfMob> OutletTemp;
    Attribute<int, ShelfMob> TempLowThd;
    Attribute<int, ShelfMob> TempHighThd;

    Key key;

private:
    static std::vector<ShelfMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

