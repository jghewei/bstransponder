#include "DanOS.h"
#include "Counter.h"
#include "Attribute.h"
#include "Web.h"

void WebCounterGet(Webs *wp)
{
    CJson webs;
    
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        unsigned int inst = (unsigned int)atoi(pid);
        std::vector<BaseMob*> mobList = ActivePM::GetInst().GetPmList();
        if (inst < mobList.size())
        {
            BaseMob *mob = mobList[inst];
            std::map<int, Counter*> cntList = mob->GetCounterList();
            std::map<int, Counter*>::iterator it;
            webs.AddEntry();
            webs.AddValue("Key", mob->GetKeyName());
            webs.AddValue("Name", ActivePM::GetInst().tss());
            webs.AddValue("Unit", "timestamp");
            webs.AddValue("Data1m", ActivePM::GetInst().tss_1m());
            webs.AddValue("Data15m", ActivePM::GetInst().tss_15m());
            webs.AddValue("Data1h", ActivePM::GetInst().tss_1h());
            webs.AddValue("Data24h", ActivePM::GetInst().tss_24h());
            webs.EndEntry();
            for (it = cntList.begin(); it != cntList.end(); ++it)
            {
                Counter *cnt = it->second;
                webs.AddEntry();
                webs.AddValue("Key", mob->GetKeyName());
                webs.AddValue("Name", cnt->name());
                webs.AddValue("Unit", cnt->unit());
                webs.AddValue("Data1m", to_string(cnt->data_1m()));
                webs.AddValue("Data15m", to_string(cnt->data_15m()));
                webs.AddValue("Data1h", to_string(cnt->data_1h()));
                webs.AddValue("Data24h", to_string(cnt->data_24h()));
                webs.EndEntry();
            }            
        }
    }
    else
    {
        std::vector<BaseMob*> mobList = ActivePM::GetInst().GetPmList();
        std::vector<BaseMob*>::iterator it;
        for (it = mobList.begin(); it != mobList.end(); ++it)
        {
            webs.AddEntry();
            webs.AddValue("Key", (*it)->GetKeyName());
            webs.AddValue("Num", (*it)->GetPmNum());
            webs.EndEntry();
        }
    }
    
    webs.End();

    websSetStatus(wp, 200);
    websWriteHeaders(wp, webs.GetLength(), 0);
    websWriteEndHeaders(wp);
    websWriteBlock(wp, (char*)webs.GetBuffer(), webs.GetLength());
    websDone(wp);
}

void WebCounterSet(Webs *wp)
{
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        unsigned int inst = (unsigned int)atoi(pid);
        std::vector<BaseMob*> mobList = ActivePM::GetInst().GetPmList();
        if (inst < mobList.size())
        {
            mobList[inst]->ClearPM();
        }
    }

    std::string resp = WebFormResponse(true, "Counter clear");

    websSetStatus(wp, 200);
    websWriteHeaders(wp, resp.size(), 0);
    websWriteEndHeaders(wp);
    websWrite(wp, (char*)resp.c_str());
    websDone(wp);
}

void WebCounterAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebCounterSet(wp);
    else
        WebCounterGet(wp);
}

