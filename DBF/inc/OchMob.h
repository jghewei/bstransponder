#ifndef _OCHMOB_H_
#define _OCHMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"

const int OchMob_FecMode_SDFEC_ENHANCE = 0;
const int OchMob_FecMode_SDFEC_COMP = 1;
const int OchMob_FecMode_GFEC = 2;
const int OchMob_FecMode_Transparent = 3;

const char* OchMob_FecMode_to_string(int value);

// CID - Counter id
const int OchCnt_CorrectBits = 0;
const int OchCnt_UncorrectBits = 1;
const int OchCnt_Bip8 = 2;
const int OchCnt_Bei = 3;

const char* OchCnt_name_string(int cid);
const char* OchCnt_unit_string(int cid);

class OchMob : public BaseMob
{
public:
    static OchMob* AddInst(KeyType::Enum type, int id);
    static OchMob* GetInst(int inst);
    static OchMob* GetInstByKey(Key &key);
    static OchMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    OchMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:
    Attribute<int, OchMob> FecMode;
    Attribute<int, OchMob> ChanNum;
    Attribute<std::string, OchMob> MinLaserFreq;
    Attribute<std::string, OchMob> MaxLaserFreq;
    Attribute<std::string, OchMob> GridSpacing;
    Attribute<std::string, OchMob> MaxChans;
    Attribute<std::string, OchMob> CurTxLaserFreq;
    Attribute<std::string, OchMob> CurRxLaserFreq;
    Attribute<std::string, OchMob> DGD;
    Attribute<std::string, OchMob> CD;
    Attribute<std::string, OchMob> OSNR;
    Attribute<std::string, OchMob> Qfactor;

    Key key;

private:
    static std::vector<OchMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

