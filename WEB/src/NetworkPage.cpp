#include "DanOS.h"
#include "NetworkMob.h"
#include "Web.h"

void WebNetworkGet(Webs *wp)
{
    int inst = 0, num = NetworkMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        NetworkMob *pMob = NetworkMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name().substr(0, pMob->key.name().size() - 2));
        webs.AddValue("IpAddress", pMob->IpAddress.Get());
        webs.AddValue("Netmask", pMob->Netmask.Get());
        webs.AddValue("Gateway", pMob->Gateway.Get());
        webs.AddValue("Mode", pMob->Mode.Get());

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

void WebNetworkSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    NetworkMob *pMob = NetworkMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "IpAddress", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->IpAddress.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Netmask", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Netmask.Set(ptr)) goto error_exit;
    }

    ptr = websGetVar(wp, "Gateway", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->Gateway.Set(ptr)) goto error_exit;
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

void WebNetworkAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebNetworkSet(wp);
    else
        WebNetworkGet(wp);
}

