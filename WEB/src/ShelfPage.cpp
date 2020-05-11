#include "DanOS.h"
#include "ShelfMob.h"
#include "Web.h"

void WebShelfGet(Webs *wp)
{
    int inst = 0, num = ShelfMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        ShelfMob *pMob = ShelfMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("InletTemp", pMob->InletTemp.Get());
        webs.AddValue("OutletTemp", pMob->OutletTemp.Get());
        webs.AddValue("TempLowThd", pMob->TempLowThd.Get());
        webs.AddValue("TempHighThd", pMob->TempHighThd.Get());

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

void WebShelfSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    ShelfMob *pMob = ShelfMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "TempLowThd", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TempLowThd.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "TempHighThd", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TempHighThd.Set(atoi(ptr))) goto error_exit;
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

void WebShelfAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebShelfSet(wp);
    else
        WebShelfGet(wp);
}

