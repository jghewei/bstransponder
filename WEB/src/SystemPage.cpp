#include "DanOS.h"
#include "SystemMob.h"
#include "Web.h"

void WebSystemGet(Webs *wp)
{
    int inst = 0, num = SystemMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        SystemMob *pMob = SystemMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("Name", pMob->Name.Get());
        webs.AddValue("Contact", pMob->Contact.Get());
        webs.AddValue("Location", pMob->Location.Get());
        webs.AddValue("Description", pMob->Description.Get());
        webs.AddValue("UpTime", pMob->UpTime.Get());
        webs.AddValue("CpuUsage", pMob->CpuUsage.Get());
        webs.AddValue("MemUsage", pMob->MemUsage.Get());
        webs.AddValue("CurrentDateTime", pMob->CurrentDateTime.Get());
        webs.AddValue("SetDateTime", pMob->SetDateTime.Get());
        webs.AddValue("SwVersion", pMob->SwVersion.Get());
        webs.AddValue("Restart", pMob->Restart.Get());
        webs.AddValue("Database", pMob->Database.Get());
        webs.AddValue("LampTest", pMob->LampTest.Get());

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

void WebSystemSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    SystemMob *pMob = SystemMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "Name", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Name.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Contact", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Contact.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Location", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Location.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Description", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Description.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "SetDateTime", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->SetDateTime.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Restart", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Restart.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "Database", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Database.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "LampTest", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->LampTest.Set(atoi(ptr))) goto error_exit;
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

void WebSystemAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebSystemSet(wp);
    else
        WebSystemGet(wp);
}

