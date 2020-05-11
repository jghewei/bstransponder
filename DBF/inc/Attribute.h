#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <string>
#include <map>
#include "Alarm.h"
#include "Counter.h"
#include "DanOS.h"

class BaseMob
{
public:
    virtual ~BaseMob() {}
    virtual std::string GetKeyName() { return ""; }
    
    void UpdateAlarm(AlarmId::Enum aid, bool active);
    void RaiseAlarm(AlarmId::Enum aid);
    void ClearAlarm(AlarmId::Enum aid);
    void AddAlarm(AlarmId::Enum aid, Alarm *alm);
    void ResetAllAlarm();
    void UpdateAllAlarm();
    void ReportAlarm();

    void AddCounter(int cid, Counter *cnt);
    virtual void UpdatePM() {}
    void RotatePM(uint8_t mask);
    void PrintPM();
    void ClearPM();
    void SetCounter(int cid, uint64_t val);
    int GetPmNum(void);
    std::map<int, Counter*> GetCounterList() { return mCounterMap; }

private:
    std::map<AlarmId::Enum, Alarm*> mAlarmMap;
    std::map<int, Counter*> mCounterMap;
};

typedef bool (*onSetCallback)(BaseMob* mob, std::string& atrb);
typedef bool (*onGetCallback)(BaseMob* mob, std::string& atrb);
typedef bool (*onChgCallback)(BaseMob* mob, std::string& atrb);

template<typename AT, typename PT>
class Attribute
{
public:
    Attribute(const std::string& name, PT *const parent)
    {
        name_ = name;
        parent_ = parent;
    }

    std::string& name() { return name_; }

    AT& Get()
    {
        parent_->GetCallback(parent_, name_);
        return value_;
    }

    bool Set(const AT& value)
    {
        value_ = value;
        return parent_->SetCallback(parent_, name_);
    }

    bool Exe()
    {
        return parent_->SetCallback(parent_, name_);
    }

    AT& GetValue() { return value_; }
    void SetValue(const AT& value) { value_ = value; }
    
private:
    std::string name_;
    PT* parent_;
    AT value_;
};

bool check_cfg_name(std::string &name, const char *line);

#endif

