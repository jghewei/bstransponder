#ifndef _ETHMOB_H_
#define _ETHMOB_H_

#include <vector>
#include "Key.h"
#include "Attribute.h"
#include "Alarm.h"



// AID - alarm id
const char* EthAlm_desp_string(AlarmId::Enum aid);

// CID - Counter id
const int EthCnt_RxOctets = 0;
const int EthCnt_RxPkts = 1;
const int EthCnt_RxUcastPkts = 2;
const int EthCnt_RxMulticastPkts = 3;
const int EthCnt_RxBroadcastPkts = 4;
const int EthCnt_RxPkts64Octets = 5;
const int EthCnt_RxPkts65to127Octets = 6;
const int EthCnt_RxPkts128to255Octets = 7;
const int EthCnt_RxPkts256to511Octets = 8;
const int EthCnt_RxPkts512to1023Octets = 9;
const int EthCnt_RxPkts1024to1518Octets = 10;
const int EthCnt_RxPkts1519toMaxOctets = 11;
const int EthCnt_RxCRCAlignErrors = 12;
const int EthCnt_RxUndersizePkts = 13;
const int EthCnt_RxOversizePkts = 14;
const int EthCnt_RxFragments = 15;
const int EthCnt_RxJabbers = 16;
const int EthCnt_TxOctets = 17;
const int EthCnt_TxPkts = 18;
const int EthCnt_TxUcastPkts = 19;
const int EthCnt_TxMulticastPkts = 20;
const int EthCnt_TxBroadcastPkts = 21;

const char* EthCnt_name_string(int cid);
const char* EthCnt_unit_string(int cid);

class EthMob : public BaseMob
{
public:
    static EthMob* AddInst(KeyType::Enum type, int id);
    static EthMob* GetInst(int inst);
    static EthMob* GetInstByKey(Key &key);
    static EthMob* GetInstByKey(KeyType::Enum type, int id);
    static int GetInstNum();
    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }
    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }
    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }
    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }

    EthMob(KeyType::Enum type, int id);
    void Save();
    void Load();
    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }
    std::string GetKeyName() { return key.name(); }

public:

    Key key;

private:
    static std::vector<EthMob*> mInst;
    static int mInstNum;
    static onSetCallback mSetCb;
    static onGetCallback mGetCb;
};

#endif

