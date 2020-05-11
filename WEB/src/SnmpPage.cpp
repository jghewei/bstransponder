#include "DanOS.h"
#include "SnmpMob.h"
#include "Web.h"

void WebSnmpGet(Webs *wp)
{
    int inst = 0, num = SnmpMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        SnmpMob *pMob = SnmpMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("Enable", pMob->Enable.Get());
        webs.AddValue("GetCommunity", pMob->GetCommunity.Get());
        webs.AddValue("SetCommunity", pMob->SetCommunity.Get());
        webs.AddValue("TrapReceiver1", pMob->TrapReceiver1.Get());
        webs.AddValue("TrapReceiver2", pMob->TrapReceiver2.Get());
        webs.AddValue("TrapReceiver3", pMob->TrapReceiver3.Get());

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

void WebSnmpSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    SnmpMob *pMob = SnmpMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "Enable", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Enable.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "GetCommunity", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->GetCommunity.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "SetCommunity", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->SetCommunity.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "TrapReceiver1", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TrapReceiver1.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "TrapReceiver2", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TrapReceiver2.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "TrapReceiver3", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->TrapReceiver3.Set(ptr)) goto error_exit;
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

void WebSnmpAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebSnmpSet(wp);
    else
        WebSnmpGet(wp);
}

