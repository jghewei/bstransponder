#include "Attribute.h"

void BaseMob::UpdateAlarm(AlarmId::Enum aid, bool active)
{
    if (active)
    {
        RaiseAlarm(aid);
    }
    else
    {
        ClearAlarm(aid);
    }
}

void BaseMob::RaiseAlarm(AlarmId::Enum aid)
{
    if (mAlarmMap[aid]->raise())
    {
        ReportAlarm();
    }
}

void BaseMob::ClearAlarm(AlarmId::Enum aid)
{
    if (mAlarmMap[aid]->clear())
    {
        ReportAlarm();
    }
}

void BaseMob::AddAlarm(AlarmId::Enum aid, Alarm *alm)
{
    mAlarmMap[aid] = alm;
}

void BaseMob::ResetAllAlarm()
{
    std::map<AlarmId::Enum, Alarm*>::iterator it;        
    for (it = mAlarmMap.begin(); it != mAlarmMap.end(); ++it)
    {
        it->second->clear();
    }
    ReportAlarm();
}

void BaseMob::UpdateAllAlarm()
{
    bool changed = false;
    std::map<AlarmId::Enum, Alarm*>::iterator it;        
    for (it = mAlarmMap.begin(); it != mAlarmMap.end(); ++it)
    {
        if (it->second->update())
        {
            changed = true;
        }
    }
    if (changed)
    {
        ReportAlarm();
    }
}

void BaseMob::ReportAlarm()
{
    AlarmId::Enum cur_report = AlarmId::INVALID, exp_report = AlarmId::INVALID;
    
    std::map<AlarmId::Enum, Alarm*>::iterator it;
    for (it = mAlarmMap.begin(); it != mAlarmMap.end(); ++it)
    {
        if (it->second->reported())
        {
            cur_report = it->first;
            break;
        }
    }

    std::map<AlarmId::Enum, Alarm*>::reverse_iterator rit;
    for (rit = mAlarmMap.rbegin(); rit != mAlarmMap.rend(); ++rit)
    {
        if (rit->second->active())
        {
            exp_report = rit->first;
        }
    }

    if (cur_report == exp_report)
    {
        return;
    }

    if (cur_report != AlarmId::INVALID)
    {
        mAlarmMap[cur_report]->set_report(false);
    }

    if (exp_report != AlarmId::INVALID)
    {
        mAlarmMap[exp_report]->set_report(true);
    }
}

void BaseMob::AddCounter(int cid, Counter *cnt)
{
    mCounterMap[cid] = cnt;
}

void BaseMob::RotatePM(uint8_t mask)
{
    std::map<int, Counter*>::iterator it;
    for (it = mCounterMap.begin(); it != mCounterMap.end(); ++it)
    {
        it->second->rotate(mask);
    }
}

void BaseMob::PrintPM()
{
    std::map<int, Counter*>::iterator it;
    for (it = mCounterMap.begin(); it != mCounterMap.end(); ++it)
    {
        it->second->print();
    }
}

void BaseMob::ClearPM()
{
    std::map<int, Counter*>::iterator it;
    for (it = mCounterMap.begin(); it != mCounterMap.end(); ++it)
    {
        it->second->clear();
    }
}

void BaseMob::SetCounter(int cid, uint64_t val)
{
    mCounterMap[cid]->set_value(val);
}

int BaseMob::GetPmNum(void)
{
    return (int)mCounterMap.size();
}

bool check_cfg_name(std::string &name, const char *line)
{
    int size = name.size();
    int len = strlen(line);

    if ((len >= (size + 1)) && strncmp(name.c_str(), line, size) == 0)
    {
        return true;
    }
    return false;
}

