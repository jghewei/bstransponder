#include "DanOS.h"
#include "Alarm.h"
#include "Web.h"

void WebAlarmGet(Webs *wp)
{
    std::vector<Alarm*> alarms = ActiveAlarm::GetInst().GetAlarmList();

    CJson webs;
    for (unsigned int inst = 0; inst < alarms.size(); inst++)
    {
        Alarm *pAlm = alarms[inst];
        if (pAlm == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pAlm->key().name());
        webs.AddValue("Name", pAlm->name());
        webs.AddValue("Type", pAlm->service_affert());
        webs.AddValue("Severity", pAlm->severity());
        webs.AddValue("Time", pAlm->raise_time());
        webs.AddValue("Desp", pAlm->desp());
        webs.EndEntry();	
    }
    webs.End();

    websSetStatus(wp, 200);
    websWriteHeaders(wp, webs.GetLength(), 0);
    websWriteEndHeaders(wp);
    websWriteBlock(wp, (char*)webs.GetBuffer(), webs.GetLength());
    websDone(wp);
}

void WebAlarmAction(Webs *wp)
{
    WebAlarmGet(wp);
}

