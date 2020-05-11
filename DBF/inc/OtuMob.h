#ifndef _OTUMOB_H_
#define _OTUMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int OtuMob_FecMode_Disabled = 0;
const int OtuMob_FecMode_GFEC = 1;
const int OtuMob_FecMode_I4 = 2;
const int OtuMob_FecMode_I7 = 3;
const int OtuMob_TestSignalType_Disabled = 0;
const int OtuMob_TestSignalType_PRBS = 1;
const int OtuMob_TestSignalStatus_Disabled = 0;
const int OtuMob_TestSignalStatus_InSync = 1;
const int OtuMob_TestSignalStatus_OutSync = 2;
const int OtuMob_TestSignalStatus_ErrorSync = 3;

const char* OtuMob_FecMode_to_string(int value);
const char* OtuMob_TestSignalType_to_string(int value);
const char* OtuMob_TestSignalStatus_to_string(int value);

// AID - alarm id
const char* OtuAlm_desp_string(AlarmId::Enum aid);

// CID - Counter id
const int OtuCnt_CorrectBits = 0;
const int OtuCnt_UncorrectBits = 1;
const int OtuCnt_FrameCount = 2;
const int OtuCnt_Bip8 = 3;
const int OtuCnt_Bei = 4;

const char* OtuCnt_name_string(int cid);
const char* OtuCnt_unit_string(int cid);

class OtuMob : public BaseMob
{
public:
    static OtuMob* AddInst(KeyType::Enum type, int id);
    static OtuMob* GetInst(int inst);
    static OtuMob* GetInstByKey(Key &key);
    static OtuMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    OtuMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, OtuMob> FecMode;
    Attribute<int, OtuMob> TestSignalType;
    Attribute<int, OtuMob> TestSignalStatus;

    Key key;

private:
    static std::vector<OtuMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

