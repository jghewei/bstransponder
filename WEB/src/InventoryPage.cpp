#include "DanOS.h"
#include "InventoryMob.h"
#include "Web.h"

void WebInventoryGet(Webs *wp)
{
    int inst = 0, num = InventoryMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        InventoryMob *pMob = InventoryMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("Present", pMob->Present.Get());
        webs.AddValue("ModuleType", pMob->ModuleType.Get());
        webs.AddValue("PartNumber", pMob->PartNumber.Get());
        webs.AddValue("SerialNumber", pMob->SerialNumber.Get());
        webs.AddValue("HwVersion", pMob->HwVersion.Get());
        webs.AddValue("VendorName", pMob->VendorName.Get());
        webs.AddValue("ProduceDate", pMob->ProduceDate.Get());
        webs.AddValue("FwVersion", pMob->FwVersion.Get());

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

void WebInventorySet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    InventoryMob *pMob = InventoryMob::GetInst(inst);
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

void WebInventoryAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebInventorySet(wp);
    else
        WebInventoryGet(wp);
}

