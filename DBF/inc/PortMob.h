#ifndef _PORTMOB_H_
#define _PORTMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int PortMob_PortMode_NONE = 0;
const int PortMob_PortMode_ETH10G = 1;
const int PortMob_PortMode_ETH100G = 2;
const int PortMob_PortMode_STM64 = 3;
const int PortMob_PortMode_OTU2 = 5;
const int PortMob_PortMode_OTU2E = 6;
const int PortMob_PortMode_OTU4 = 7;
const int PortMob_PortMode_ETH10G_GFP = 10;
const int PortMob_AdminStatus_Off = 0;
const int PortMob_AdminStatus_On = 1;
const int PortMob_OperStatus_Off = 0;
const int PortMob_OperStatus_Up = 1;
const int PortMob_OperStatus_Down = 2;
const int PortMob_LoopbackType_Disabled = 0;
const int PortMob_LoopbackType_Facility = 1;
const int PortMob_LoopbackType_Terminal = 2;

const char* PortMob_PortMode_to_string(int value);
const char* PortMob_AdminStatus_to_string(int value);
const char* PortMob_OperStatus_to_string(int value);
const char* PortMob_LoopbackType_to_string(int value);

class PortMob : public BaseMob
{
public:
    static PortMob* AddInst(KeyType::Enum type, int id);
    static PortMob* GetInst(int inst);
    static PortMob* GetInstByKey(Key &key);
    static PortMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    PortMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, PortMob> AliasName;
    Attribute<int, PortMob> PortMode;
    Attribute<int, PortMob> AdminStatus;
    Attribute<int, PortMob> OperStatus;
    Attribute<int, PortMob> LoopbackType;

    Key key;

private:
    static std::vector<PortMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

