#include "Attribute.h"
#include "Counter.h"
#include "CThread.h"

void Counter::set_value(uint64_t val)
{
    if (!valid_)
    {
        last_ = val;
        data_1m_ = 0;
        data_15m_ = 0;
        data_1h_ = 0;
        data_24h_ = 0;     
        valid_ = true;
        return;
    }

    uint64_t offset = 0;
    if (val >= last_)
    {
        offset = val - last_;
    }
    else
    {
        offset = ~(uint64_t)0 - last_ + val;
    }

    data_1m_ += offset;
    data_15m_ += offset;
    data_1h_ += offset;
    data_24h_ += offset;

    last_ = val;
}

void Counter::rotate(uint8_t period_mask)
{
    // ToDo to save bin to file
    
    if (period_mask & PERIOD_MASK_1M)
    {
        data_1m_ = 0;
    }

    if (period_mask & PERIOD_MASK_15M)
    {
        data_15m_ = 0;
    }

    if (period_mask & PERIOD_MASK_1H)
    {
        data_1h_ = 0;
    }

    if (period_mask & PERIOD_MASK_24H)
    {
        data_24h_ = 0;
    }
}

void Counter::print()
{
    printf("%s, %d, %s, %s, %d, %s, %s, %s, %s, %s\n",
            key_.name().c_str(), cid_, name_, unit_, valid_,
            to_string(last_).c_str(), to_string(data_1m_).c_str(),
            to_string(data_15m_).c_str(), to_string(data_1h_).c_str(),
            to_string(data_24h_).c_str());
}

void Counter::clear()
{
    set_valid(false);
}

ActivePM* ActivePM::mInst = NULL;

ActivePM& ActivePM::GetInst()
{
    if (mInst == NULL)
    {
        mInst = new ActivePM();
    }
    return *mInst;
}

void ActivePM::set_enable(BaseMob *mob, bool enable)
{
    std::vector<BaseMob*>::iterator it;
    CMutexLock lock(mLock);

    if (enable)
    {
        for (it = mList.begin(); it != mList.end(); ++it)
        {
            if (*it == mob)
            {
                return;
            }
        }
        mList.push_back(mob);
    }
    else
    {
        for (it = mList.begin(); it != mList.end(); ++it)
        {
            if (*it == mob)
            {
                mList.erase(it);
                return;
            }
        }
    }
}

void ActivePM::PrintPM()
{
    std::vector<BaseMob*>::iterator it;
    printf("timestamp: %s(1m), %s(15m), %s(1h), %s(24h)\n",
            tss_1m().c_str(), tss_15m().c_str(), tss_1h().c_str(), tss_24h().c_str());
    CMutexLock lock(mLock);
    for (it = mList.begin(); it != mList.end(); ++it)
    {
        (*it)->PrintPM();
    }
}

std::vector<BaseMob*> ActivePM::GetPmList()
{
    CMutexLock lock(mLock);
    return mList;
}

std::string ActivePM::tss()
{
    char s[32] = {0};
    sprintf(s, "%04d-%02d-%02d %02d:%02d", ts_year + 1900, ts_month + 1, ts_24h, ts_1h, ts_1m);
    return s;
}

std::string ActivePM::tss_1m()
{
    char s[8] = {0};
    sprintf(s, "%02d:%02d", ts_1h, ts_1m);
    return s;
}

std::string ActivePM::tss_15m()
{
    char s[8] = {0};
    sprintf(s, "%02d:%02d", ts_1h, ts_15m);
    return s;
}

std::string ActivePM::tss_1h()
{
    char s[8] = {0};
    sprintf(s, "%02d:00", ts_1h);
    return s;
}

std::string ActivePM::tss_24h()
{
    char s[8] = {0};
    sprintf(s, "%02d-%02d", ts_month + 1, ts_24h);
    return s;
}

void* PmThread(void *param)
{
    uint8_t period_mask = 0;
    ActivePM &apm = ActivePM::GetInst();

    std::vector<BaseMob*> mobList = apm.GetPmList();
    std::vector<BaseMob*>::iterator it;

    time_t t;
    struct tm tt;
    time(&t);  
    localtime_r(&t, &tt);

    int ts_1m = tt.tm_min;
    int ts_15m = (tt.tm_min / 15) * 15;
    int ts_1h = tt.tm_hour;
    int ts_24h = tt.tm_mday;
    int ts_year = tt.tm_year;
    int ts_month = tt.tm_mon;

    if (ts_1m != apm.ts_1m)
    {
        apm.ts_1m = ts_1m;
        period_mask |= PERIOD_MASK_1M;
    }
    if (ts_15m != apm.ts_15m)
    {
        apm.ts_15m = ts_15m;
        period_mask |= PERIOD_MASK_15M;
    }
    if (ts_1h != apm.ts_1h)
    {
        apm.ts_1h = ts_1h;
        period_mask |= PERIOD_MASK_1H;
    }
    if (ts_24h != apm.ts_24h)
    {
        apm.ts_24h = ts_24h;
        period_mask |= PERIOD_MASK_24H;
    }

    // need lock here?
    
    if (period_mask != 0)
    {
        apm.ts_year = ts_year;
        apm.ts_month = ts_month;
        
        for (it = mobList.begin(); it != mobList.end(); ++it)
        {
            (*it)->RotatePM(period_mask);
        }
    }

    for (it = mobList.begin(); it != mobList.end(); ++it)
    {
        (*it)->UpdatePM();
    }

    sleep(1);

    return NULL;
}

void PMInit(void)
{
    ActivePM::GetInst();
    new CThread("PM", PmThread, NULL);
}

