#ifndef _OPTICSMOB_H_
#define _OPTICSMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int OpticsMob_Laser_Off = 0;
const int OpticsMob_Laser_On = 1;
const int OpticsMob_Los_On = 0;
const int OpticsMob_Los_Off = 1;

const char* OpticsMob_Laser_to_string(int value);
const char* OpticsMob_Los_to_string(int value);

class OpticsMob : public BaseMob
{
public:
    static OpticsMob* AddInst(KeyType::Enum type, int id);
    static OpticsMob* GetInst(int inst);
    static OpticsMob* GetInstByKey(Key &key);
    static OpticsMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    OpticsMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, OpticsMob> Type;
    Attribute<int, OpticsMob> Laser;
    Attribute<int, OpticsMob> Los;
    Attribute<float, OpticsMob> TxPower;
    Attribute<float, OpticsMob> RxPower;
    Attribute<float, OpticsMob> Temp;

    Key key;

private:
    static std::vector<OpticsMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

