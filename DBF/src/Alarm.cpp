#include "Alarm.h"

extern void SnmpSendTrap(const char *key, const char *name, const char *type,
                         const char *severity, const char *time, const char *desp,
                         const char *action);

void Alarm::set_report(bool report)
{
    reported_ = report;
    if (report)
    {
        ActiveAlarm::GetInst().InsertAlarm(this);
    }
    else
    {
        ActiveAlarm::GetInst().DeleteAlarm(this);
    }
}

bool Alarm::clear()
{
    // ToDo add soaking
#if 0
    if (state_ == AlarmState::Raised)
    {
        state_ = AlarmState::Soaking;
        time(&clear_time_);
    }
#endif
    if (state_ == AlarmState::Raised)
    {
        state_ = AlarmState::Cleared;
        time(&clear_time_);
        printf("%s %s %s\n", name(), "cleared", desp_);
        return true;
    }
    return false;
}

bool Alarm::raise()
{
    if (state_ == AlarmState::Cleared)
    {
        state_ = AlarmState::Raised;
        time(&raise_time_);
        // ToDo do reporting
        printf("%s %s %s\n", name(), "raised", desp_);
        return true;
    }
    else if (state_ == AlarmState::Soaking)
    {
        state_ = AlarmState::Raised;
    }
    return false;
}

bool Alarm::update()
{
    if (state_ == AlarmState::Soaking)
    {
        time_t mCurTime;
        time(&mCurTime);
        if ((mCurTime - clear_time_) > ALARM_SOAK_TIME)
        {
            state_ = AlarmState::Cleared;
            // ToDo do clear reporting
            printf("%s %s %s\n", name(), "cleared", desp_);
            return true;
        }
    }
    return false;
}

std::string Alarm::raise_time()
{
    char buf[64] = {0};
    tm* local = localtime(&raise_time_);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);  
    return buf;
}

ActiveAlarm* ActiveAlarm::mInst = NULL;

ActiveAlarm& ActiveAlarm::GetInst()
{
    if (mInst == NULL)
    {
        mInst = new ActiveAlarm();
    }
    return *mInst;
}

ActiveAlarm::ActiveAlarm() : mCrCnt(0), mMjCnt(0), mMnCnt(0)
{

}

void ActiveAlarm::InsertAlarm(Alarm *alm)
{
    CMutexLock lock(mLock);
    mList.push_back(alm);
    UpdateAlarmCnt(alm, true);
    SendSnmpTrap(alm, true);
}

void ActiveAlarm::DeleteAlarm(Alarm *alm)
{
    std::vector<Alarm*>::iterator it;
    CMutexLock lock(mLock);
    for (it = mList.begin(); it != mList.end(); ++it)
    {
        if (*it == alm)
        {
            UpdateAlarmCnt(alm, false);
            SendSnmpTrap(alm, false);
            mList.erase(it);
            break;
        }
    }
}

void ActiveAlarm::PrintAlarms()
{
    unsigned int cr, mj, mn;
    GetAlarmCount(&cr, &mj, &mn);
    printf("CR: %d, MJ: %d, MN: %d\n", cr, mj, mn);

    std::vector<Alarm*>::iterator it;
    CMutexLock lock(mLock);
    for (it = mList.begin(); it != mList.end(); ++it)
    {
        Alarm *alm = *it;
        printf("%s, %d, %s, %s, %s, %s, %s\n", alm->key().name().c_str(),
            alm->aid(), alm->name(), alm->raise_time().c_str(), alm->desp(),
            alm->service_affert(), alm->severity());
    }
}

void ActiveAlarm::SendSnmpTrap(Alarm *alm, bool raise)
{
    SnmpSendTrap(alm->key().name().c_str(), alm->name(), alm->service_affert(),
                 alm->severity(), alm->raise_time().c_str(), alm->desp(),
                 raise ? "raise" : "clear");
}

void ActiveAlarm::UpdateAlarmCnt(Alarm *alm, bool raise)
{
    unsigned int *pCnt = NULL;
    
    if (alm->severity_level() == AlarmSeverity::ALM_CR)
    {
        pCnt = &mCrCnt;
    }
    else if (alm->severity_level() == AlarmSeverity::ALM_MJ)
    {
        pCnt = &mMjCnt;
    }
    else if (alm->severity_level() == AlarmSeverity::ALM_MN)
    {
        pCnt = &mMnCnt;
    }

    if (pCnt != NULL)
    {
        if (raise)
        {
            *pCnt = *pCnt + 1;
        }
        else
        {
            *pCnt = *pCnt - 1;
        }

        // light system LED
    }
}

void ActiveAlarm::GetAlarmCount(unsigned int *cr, unsigned int *mj, unsigned int *mn)
{
    CMutexLock lock(mLock);
    *cr = mCrCnt;
    *mj = mMjCnt;
    *mn = mMnCnt;
}

std::vector<Alarm*> ActiveAlarm::GetAlarmList()
{
    CMutexLock lock(mLock);
    return mList;
}

#define ALARM_NAME_ENTRY(x)     case AlarmId::x: return #x

const char* GetAlarmName(AlarmId::Enum aid)
{
    switch(aid)
    {
        ALARM_NAME_ENTRY(TEMP_LT);
        ALARM_NAME_ENTRY(TEMP_HT);
        ALARM_NAME_ENTRY(FAN_FAULT);
        ALARM_NAME_ENTRY(PWR_FAULT);
            
        ALARM_NAME_ENTRY(ETH_LOS);
        ALARM_NAME_ENTRY(ETH_LOSYNC);
        ALARM_NAME_ENTRY(ETH_LF);
        ALARM_NAME_ENTRY(ETH_RF);
        
        ALARM_NAME_ENTRY(OTU_LOS);
        ALARM_NAME_ENTRY(OTU_LOL);
        ALARM_NAME_ENTRY(OTU_LOF);
        ALARM_NAME_ENTRY(OTU_LOM);
        ALARM_NAME_ENTRY(OTU_TIM);
        ALARM_NAME_ENTRY(OTU_BDI);
        ALARM_NAME_ENTRY(OTU_BERSD);
        
        ALARM_NAME_ENTRY(ODU_BERSD);
        ALARM_NAME_ENTRY(ODU_LCK);
        ALARM_NAME_ENTRY(ODU_OCI);
        ALARM_NAME_ENTRY(ODU_AIS);
        ALARM_NAME_ENTRY(ODU_BDI);
        ALARM_NAME_ENTRY(ODU_TIM);
        ALARM_NAME_ENTRY(ODU_PLM);
        ALARM_NAME_ENTRY(ODU_LOOMFI);
        ALARM_NAME_ENTRY(ODU_MSIM);
        ALARM_NAME_ENTRY(ODU_LOFLOM);
        
        ALARM_NAME_ENTRY(OPU_CSF);

        ALARM_NAME_ENTRY(STM_LOS);
        ALARM_NAME_ENTRY(STM_LOF);
        ALARM_NAME_ENTRY(STM_TIM_R);
        ALARM_NAME_ENTRY(STM_AIS_L);
        ALARM_NAME_ENTRY(STM_RFI_L);

        default: return "UNKNOWN";
    }
}
