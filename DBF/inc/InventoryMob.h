#ifndef _INVENTORYMOB_H_
#define _INVENTORYMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int InventoryMob_Present_No = 0;
const int InventoryMob_Present_Yes = 1;

const char* InventoryMob_Present_to_string(int value);

class InventoryMob : public BaseMob
{
public:
    static InventoryMob* AddInst(KeyType::Enum type, int id);
    static InventoryMob* GetInst(int inst);
    static InventoryMob* GetInstByKey(Key &key);
    static InventoryMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    InventoryMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, InventoryMob> Present;
    Attribute<std::string, InventoryMob> ModuleType;
    Attribute<std::string, InventoryMob> PartNumber;
    Attribute<std::string, InventoryMob> SerialNumber;
    Attribute<std::string, InventoryMob> HwVersion;
    Attribute<std::string, InventoryMob> VendorName;
    Attribute<std::string, InventoryMob> ProduceDate;
    Attribute<std::string, InventoryMob> FwVersion;

    Key key;

private:
    static std::vector<InventoryMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

