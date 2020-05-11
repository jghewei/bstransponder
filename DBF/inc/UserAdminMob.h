#ifndef _USERADMINMOB_H_
#define _USERADMINMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int UserAdminMob_Privilege_Administrator = 0;
const int UserAdminMob_Privilege_Service = 1;
const int UserAdminMob_Privilege_Monitor = 2;

const char* UserAdminMob_Privilege_to_string(int value);

class UserAdminMob : public BaseMob
{
public:
    static UserAdminMob* AddInst(KeyType::Enum type, int id);
    static UserAdminMob* GetInst(int inst);
    static UserAdminMob* GetInstByKey(Key &key);
    static UserAdminMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    UserAdminMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<std::string, UserAdminMob> UserName;
    Attribute<std::string, UserAdminMob> Password;
    Attribute<int, UserAdminMob> Privilege;

    Key key;

private:
    static std::vector<UserAdminMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

