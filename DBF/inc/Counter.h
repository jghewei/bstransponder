#ifndef _Counter_H_
#define _Counter_H_

#include <stdint.h>
#include <time.h>
#include <vector>
#include "Key.h"
#include "CMutex.h"

#define PERIOD_MASK_1M      0x01
#define PERIOD_MASK_15M     0x02
#define PERIOD_MASK_1H      0x04
#define PERIOD_MASK_24H     0x08

class Counter
{
public:
    Counter(Key &key, int cid, const char *name, const char *unit)
        : key_(key), cid_(cid), name_(name), unit_(unit), valid_(false),
          last_(0), data_1m_(0), data_15m_(0), data_1h_(0), data_24h_(0)
    {
    }

    void set_valid(bool valid) { valid_ = valid; }
    void set_value(uint64_t val);
    void rotate(uint8_t period_mask);
    void print();
    void clear();

    Key& key() { return key_; }
    int cid() { return cid_; }
    const char *name() { return name_; }
    const char *unit() { return unit_; }
    bool valie() { return valid_; }
    uint64_t last() { return last_; }
    uint64_t data_1m() { return data_1m_; }
    uint64_t data_15m() { return data_15m_; }
    uint64_t data_1h() { return data_1h_; }
    uint64_t data_24h() { return data_24h_; }

private:
    Key key_;
    int cid_;
    const char *name_;
    const char *unit_;
    bool valid_;
    uint64_t last_;
    uint64_t data_1m_;
    uint64_t data_15m_;
    uint64_t data_1h_;
    uint64_t data_24h_;
};

class BaseMob;
class ActivePM
{
public:
    static ActivePM& GetInst();

    void set_enable(BaseMob *mob, bool enable);
    void PrintPM();
    std::vector<BaseMob*> GetPmList();

    int ts_1m;
    int ts_15m;
    int ts_1h;
    int ts_24h;
    int ts_year;
    int ts_month;

    std::string tss();
    std::string tss_1m();
    std::string tss_15m();
    std::string tss_1h();
    std::string tss_24h();

private:
    static ActivePM *mInst;
    std::vector<BaseMob*> mList;
    CMutex mLock;
};

void PMInit(void);

#endif

