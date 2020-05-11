#ifndef _Alarm_H_
#define _Alarm_H_

#include <time.h>
#include <vector>
#include "Key.h"
#include "CMutex.h"
#include "AlarmId.h"

#define ALARM_SOAK_TIME     5       /* delay 5 seconds */

namespace AlarmSeverity
{
    enum Enum
    {
        ALM_NA,
        ALM_CR,
        ALM_MJ,
        ALM_MN,
        ALM_NR,
    };
}

#define AlarmSeverity_to_String(x) \
        ((x) == AlarmSeverity::ALM_NA   ? "NONE"        : \
		 (x) == AlarmSeverity::ALM_CR   ? "Critical"    : \
         (x) == AlarmSeverity::ALM_MJ   ? "Major"       : \
         (x) == AlarmSeverity::ALM_MN   ? "Minor"       : \
         (x) == AlarmSeverity::ALM_NR   ? "NotReport"   : "?" )


namespace AlarmState
{
    enum Enum
    {
        Cleared,
        Raised,
        Soaking,
    };
}

const char* AlarmSeverity_to_string(int id);

class Alarm
{
public:
    Alarm(Key &key, AlarmId::Enum aid, const char *desp, bool sa, AlarmSeverity::Enum severity)
        : key_(key), aid_(aid), desp_(desp),
          service_affert_(sa), severity_level_(severity),
          state_(AlarmState::Cleared), reported_(false)
    {
    }

    bool clear();
    bool raise();
    bool update();

    Key& key() { return key_; }
    AlarmId::Enum aid() { return aid_; }
    const char *name() { return GetAlarmName(aid_); }
    const char *desp() { return desp_; }
    std::string raise_time();
    const char *service_affert() { return service_affert_ ? "SA" : "NSA"; }
    const char *severity() { return AlarmSeverity_to_String(severity_level_); }
    AlarmSeverity::Enum severity_level() { return severity_level_; }

    bool reported() { return reported_; }
    void set_report(bool report);

    bool active() { return (state_ == AlarmState::Raised || state_ == AlarmState::Soaking); }

private:
    Key key_;
    AlarmId::Enum aid_;
    const char *desp_;
    bool service_affert_;
    AlarmSeverity::Enum severity_level_;
    AlarmState::Enum state_;
    bool reported_;
    time_t raise_time_;
    time_t clear_time_;
};

class ActiveAlarm
{
public:
    static ActiveAlarm& GetInst();

    ActiveAlarm();

    void InsertAlarm(Alarm *alm);
    void DeleteAlarm(Alarm *alm);
    void PrintAlarms();
    std::vector<Alarm*> GetAlarmList();
    void GetAlarmCount(unsigned int *cr,
                       unsigned int *mj,
                       unsigned int *mn);

private:
    static ActiveAlarm *mInst;
    std::vector<Alarm*> mList;
    CMutex mLock;
    void SendSnmpTrap(Alarm *alm, bool raise);
    void UpdateAlarmCnt(Alarm *alm, bool raise);
    unsigned int mCrCnt;
    unsigned int mMjCnt;
    unsigned int mMnCnt;
};

#endif

