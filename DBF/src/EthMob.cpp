#include "EthMob.h"

std::vector<EthMob*> EthMob::mInst;
int EthMob::mInstNum = 0;
onSetCallback EthMob::mSetCb = NULL;
onGetCallback EthMob::mGetCb = NULL;
#ifdef __TARGET__
static const char *DBFile = "/mnt/sysfs/EthMob.";
#else
static const char *DBFile = "/tmp/EthMob.";
#endif

EthMob* EthMob::AddInst(KeyType::Enum type, int id)
{
    EthMob *inst = new EthMob(type, id);
    mInst.push_back(inst);
    mInstNum++;
    return inst;
}

EthMob* EthMob::GetInst(int inst)
{
    if (inst < GetInstNum())
    {
        return mInst[inst];
    }
    else
    {
        return NULL;
    }
}

EthMob* EthMob::GetInstByKey(Key &key)
{
    for (int i = 0; i < GetInstNum(); i++)
    {
        if (mInst[i]->key == key)
        {
            return mInst[i];
        }
    }
    return NULL;
}

EthMob* EthMob::GetInstByKey(KeyType::Enum type, int id)
{
    Key key(type, id);
    return GetInstByKey(key);
}

int EthMob::GetInstNum()
{
    return mInstNum;
}

EthMob::EthMob(KeyType::Enum type, int id) :
    key(type, id)
{
    AddAlarm(AlarmId::ETH_LOS, new Alarm(key, AlarmId::ETH_LOS, EthAlm_desp_string(AlarmId::ETH_LOS), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::ETH_LOSYNC, new Alarm(key, AlarmId::ETH_LOSYNC, EthAlm_desp_string(AlarmId::ETH_LOSYNC), true, AlarmSeverity::ALM_CR));
    AddAlarm(AlarmId::ETH_LF, new Alarm(key, AlarmId::ETH_LF, EthAlm_desp_string(AlarmId::ETH_LF), false, AlarmSeverity::ALM_NA));
    AddAlarm(AlarmId::ETH_RF, new Alarm(key, AlarmId::ETH_RF, EthAlm_desp_string(AlarmId::ETH_RF), false, AlarmSeverity::ALM_NA));
    AddCounter(EthCnt_RxOctets, new Counter(key, EthCnt_RxOctets, EthCnt_name_string(EthCnt_RxOctets), EthCnt_unit_string(EthCnt_RxOctets)));
    AddCounter(EthCnt_RxPkts, new Counter(key, EthCnt_RxPkts, EthCnt_name_string(EthCnt_RxPkts), EthCnt_unit_string(EthCnt_RxPkts)));
    AddCounter(EthCnt_RxUcastPkts, new Counter(key, EthCnt_RxUcastPkts, EthCnt_name_string(EthCnt_RxUcastPkts), EthCnt_unit_string(EthCnt_RxUcastPkts)));
    AddCounter(EthCnt_RxMulticastPkts, new Counter(key, EthCnt_RxMulticastPkts, EthCnt_name_string(EthCnt_RxMulticastPkts), EthCnt_unit_string(EthCnt_RxMulticastPkts)));
    AddCounter(EthCnt_RxBroadcastPkts, new Counter(key, EthCnt_RxBroadcastPkts, EthCnt_name_string(EthCnt_RxBroadcastPkts), EthCnt_unit_string(EthCnt_RxBroadcastPkts)));
    AddCounter(EthCnt_RxPkts64Octets, new Counter(key, EthCnt_RxPkts64Octets, EthCnt_name_string(EthCnt_RxPkts64Octets), EthCnt_unit_string(EthCnt_RxPkts64Octets)));
    AddCounter(EthCnt_RxPkts65to127Octets, new Counter(key, EthCnt_RxPkts65to127Octets, EthCnt_name_string(EthCnt_RxPkts65to127Octets), EthCnt_unit_string(EthCnt_RxPkts65to127Octets)));
    AddCounter(EthCnt_RxPkts128to255Octets, new Counter(key, EthCnt_RxPkts128to255Octets, EthCnt_name_string(EthCnt_RxPkts128to255Octets), EthCnt_unit_string(EthCnt_RxPkts128to255Octets)));
    AddCounter(EthCnt_RxPkts256to511Octets, new Counter(key, EthCnt_RxPkts256to511Octets, EthCnt_name_string(EthCnt_RxPkts256to511Octets), EthCnt_unit_string(EthCnt_RxPkts256to511Octets)));
    AddCounter(EthCnt_RxPkts512to1023Octets, new Counter(key, EthCnt_RxPkts512to1023Octets, EthCnt_name_string(EthCnt_RxPkts512to1023Octets), EthCnt_unit_string(EthCnt_RxPkts512to1023Octets)));
    AddCounter(EthCnt_RxPkts1024to1518Octets, new Counter(key, EthCnt_RxPkts1024to1518Octets, EthCnt_name_string(EthCnt_RxPkts1024to1518Octets), EthCnt_unit_string(EthCnt_RxPkts1024to1518Octets)));
    AddCounter(EthCnt_RxPkts1519toMaxOctets, new Counter(key, EthCnt_RxPkts1519toMaxOctets, EthCnt_name_string(EthCnt_RxPkts1519toMaxOctets), EthCnt_unit_string(EthCnt_RxPkts1519toMaxOctets)));
    AddCounter(EthCnt_RxCRCAlignErrors, new Counter(key, EthCnt_RxCRCAlignErrors, EthCnt_name_string(EthCnt_RxCRCAlignErrors), EthCnt_unit_string(EthCnt_RxCRCAlignErrors)));
    AddCounter(EthCnt_RxUndersizePkts, new Counter(key, EthCnt_RxUndersizePkts, EthCnt_name_string(EthCnt_RxUndersizePkts), EthCnt_unit_string(EthCnt_RxUndersizePkts)));
    AddCounter(EthCnt_RxOversizePkts, new Counter(key, EthCnt_RxOversizePkts, EthCnt_name_string(EthCnt_RxOversizePkts), EthCnt_unit_string(EthCnt_RxOversizePkts)));
    AddCounter(EthCnt_RxFragments, new Counter(key, EthCnt_RxFragments, EthCnt_name_string(EthCnt_RxFragments), EthCnt_unit_string(EthCnt_RxFragments)));
    AddCounter(EthCnt_RxJabbers, new Counter(key, EthCnt_RxJabbers, EthCnt_name_string(EthCnt_RxJabbers), EthCnt_unit_string(EthCnt_RxJabbers)));
    AddCounter(EthCnt_TxOctets, new Counter(key, EthCnt_TxOctets, EthCnt_name_string(EthCnt_TxOctets), EthCnt_unit_string(EthCnt_TxOctets)));
    AddCounter(EthCnt_TxPkts, new Counter(key, EthCnt_TxPkts, EthCnt_name_string(EthCnt_TxPkts), EthCnt_unit_string(EthCnt_TxPkts)));
    AddCounter(EthCnt_TxUcastPkts, new Counter(key, EthCnt_TxUcastPkts, EthCnt_name_string(EthCnt_TxUcastPkts), EthCnt_unit_string(EthCnt_TxUcastPkts)));
    AddCounter(EthCnt_TxMulticastPkts, new Counter(key, EthCnt_TxMulticastPkts, EthCnt_name_string(EthCnt_TxMulticastPkts), EthCnt_unit_string(EthCnt_TxMulticastPkts)));
    AddCounter(EthCnt_TxBroadcastPkts, new Counter(key, EthCnt_TxBroadcastPkts, EthCnt_name_string(EthCnt_TxBroadcastPkts), EthCnt_unit_string(EthCnt_TxBroadcastPkts)));
}

void EthMob::Save()
{
    std::string buffer;

    if (buffer.size() == 0) return;
    APP_LOG("%s", buffer.c_str());
    std::string fname = DBFile;
    fname += to_string(key.id());
    FILE *fp = fopen(fname.c_str(), "w");
    if (fp == NULL) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);
}

void EthMob::Load()
{
    std::string fname = DBFile;
    fname += to_string(key.id());
    FILE *fp = fopen(fname.c_str(), "r");
    if (fp == NULL) return;
    char linebuf[256] = {0};

    while (fgets(linebuf, 256, fp) != NULL)
    {
        int len = strlen(linebuf);
        if (len < 3) continue;
        if (linebuf[len-1] == '\n') linebuf[len-1] = 0;

        if (0) {}
    }

    fclose(fp);
}

const char* EthAlm_desp_string(AlarmId::Enum aid)
{
    switch (aid) {
        case AlarmId::ETH_LOS: return "Loss Of Signal";
        case AlarmId::ETH_LOSYNC: return "Loss Of Synchronization";
        case AlarmId::ETH_LF: return "Local Fault";
        case AlarmId::ETH_RF: return "Remote Fault";
        default: return "???";
    }
}

const char* EthCnt_name_string(int cid)
{
    switch (cid) {
        case EthCnt_RxOctets: return "RxOctets";
        case EthCnt_RxPkts: return "RxPkts";
        case EthCnt_RxUcastPkts: return "RxUcastPkts";
        case EthCnt_RxMulticastPkts: return "RxMulticastPkts";
        case EthCnt_RxBroadcastPkts: return "RxBroadcastPkts";
        case EthCnt_RxPkts64Octets: return "RxPkts64Octets";
        case EthCnt_RxPkts65to127Octets: return "RxPkts65to127Octets";
        case EthCnt_RxPkts128to255Octets: return "RxPkts128to255Octets";
        case EthCnt_RxPkts256to511Octets: return "RxPkts256to511Octets";
        case EthCnt_RxPkts512to1023Octets: return "RxPkts512to1023Octets";
        case EthCnt_RxPkts1024to1518Octets: return "RxPkts1024to1518Octets";
        case EthCnt_RxPkts1519toMaxOctets: return "RxPkts1519toMaxOctets";
        case EthCnt_RxCRCAlignErrors: return "RxCRCAlignErrors";
        case EthCnt_RxUndersizePkts: return "RxUndersizePkts";
        case EthCnt_RxOversizePkts: return "RxOversizePkts";
        case EthCnt_RxFragments: return "RxFragments";
        case EthCnt_RxJabbers: return "RxJabbers";
        case EthCnt_TxOctets: return "TxOctets";
        case EthCnt_TxPkts: return "TxPkts";
        case EthCnt_TxUcastPkts: return "TxUcastPkts";
        case EthCnt_TxMulticastPkts: return "TxMulticastPkts";
        case EthCnt_TxBroadcastPkts: return "TxBroadcastPkts";
        default: return "???";
    }
}

const char* EthCnt_unit_string(int cid)
{
    switch (cid) {
        case EthCnt_RxOctets: return "Packets";
        case EthCnt_RxPkts: return "Packets";
        case EthCnt_RxUcastPkts: return "Packets";
        case EthCnt_RxMulticastPkts: return "Packets";
        case EthCnt_RxBroadcastPkts: return "Packets";
        case EthCnt_RxPkts64Octets: return "Packets";
        case EthCnt_RxPkts65to127Octets: return "Packets";
        case EthCnt_RxPkts128to255Octets: return "Packets";
        case EthCnt_RxPkts256to511Octets: return "Packets";
        case EthCnt_RxPkts512to1023Octets: return "Packets";
        case EthCnt_RxPkts1024to1518Octets: return "Packets";
        case EthCnt_RxPkts1519toMaxOctets: return "Packets";
        case EthCnt_RxCRCAlignErrors: return "Packets";
        case EthCnt_RxUndersizePkts: return "Packets";
        case EthCnt_RxOversizePkts: return "Packets";
        case EthCnt_RxFragments: return "Packets";
        case EthCnt_RxJabbers: return "Packets";
        case EthCnt_TxOctets: return "Packets";
        case EthCnt_TxPkts: return "Packets";
        case EthCnt_TxUcastPkts: return "Packets";
        case EthCnt_TxMulticastPkts: return "Packets";
        case EthCnt_TxBroadcastPkts: return "Packets";
        default: return "???";
    }
}

