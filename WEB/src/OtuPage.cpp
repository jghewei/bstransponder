#include "DanOS.h"
#include "OtuMob.h"
#include "Web.h"

void WebOtuGet(Webs *wp)
{
    int inst = 0, num = OtuMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        OtuMob *pMob = OtuMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("FecMode", pMob->FecMode.Get());
        webs.AddValue("TestSignalType", pMob->TestSignalType.Get());
        webs.AddValue("TestSignalStatus", pMob->TestSignalStatus.Get());

        webs.AddValue("id", to_string(inst));
        webs.EndEntry();
    }
    webs.End();

    websSetStatus(wp, 200);
    websWriteHeaders(wp, webs.GetLength(), 0);
    websWriteEndHeaders(wp);
    websWriteBlock(wp, (char*)webs.GetBuffer(), webs.GetLength());
    websDone(wp);
}

void WebOtuSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    OtuMob *pMob = OtuMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "FecMode", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->FecMode.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "TestSignalType", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TestSignalType.Set(atoi(ptr))) goto error_exit;
    }

    pMob->Save();
    goto set_end;

error_exit:
    resp = WebFormResponse(true, "command failed");

set_end:
    websSetStatus(wp, 200);
    websWriteHeaders(wp, resp.size(), 0);
    websWriteEndHeaders(wp);
    websWrite(wp, (char*)resp.c_str());
    websDone(wp);
}

void WebOtuAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebOtuSet(wp);
    else
        WebOtuGet(wp);
}

