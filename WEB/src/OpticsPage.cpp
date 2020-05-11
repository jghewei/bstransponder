#include "DanOS.h"
#include "OpticsMob.h"
#include "Web.h"

void WebOpticsGet(Webs *wp)
{
    int inst = 0, num = OpticsMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        OpticsMob *pMob = OpticsMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("Type", pMob->Type.Get());
        webs.AddValue("Laser", pMob->Laser.Get());
        webs.AddValue("Los", pMob->Los.Get());
        webs.AddValue("TxPower", pMob->TxPower.Get());
        webs.AddValue("RxPower", pMob->RxPower.Get());
        webs.AddValue("Temp", pMob->Temp.Get());

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

void WebOpticsSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    OpticsMob *pMob = OpticsMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

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

void WebOpticsAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebOpticsSet(wp);
    else
        WebOpticsGet(wp);
}

