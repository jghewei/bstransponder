#ifndef _NETWORKMOB_H_
#define _NETWORKMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int NetworkMob_Mode_Route = 0;
const int NetworkMob_Mode_Bridge = 1;

const char* NetworkMob_Mode_to_string(int value);

class NetworkMob : public BaseMob
{
public:
    static NetworkMob* AddInst(KeyType::Enum type, int id);
    static NetworkMob* GetInst(int inst);
    static NetworkMob* GetInstByKey(Key &key);
    static NetworkMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    NetworkMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, NetworkMob> IpAddress;
    Attribute<std::string, NetworkMob> Netmask;
    Attribute<std::string, NetworkMob> Gateway;
    Attribute<int, NetworkMob> Mode;

    Key key;

private:
    static std::vector<NetworkMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

