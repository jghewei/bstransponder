#include "DanOS.h"
#include "OchMob.h"
#include "Web.h"

void WebOchGet(Webs *wp)
{
    int inst = 0, num = OchMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        OchMob *pMob = OchMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("FecMode", pMob->FecMode.Get());
        webs.AddValue("ChanNum", pMob->ChanNum.Get());
        webs.AddValue("MinLaserFreq", pMob->MinLaserFreq.Get());
        webs.AddValue("MaxLaserFreq", pMob->MaxLaserFreq.Get());
        webs.AddValue("GridSpacing", pMob->GridSpacing.Get());
        webs.AddValue("MaxChans", pMob->MaxChans.Get());
        webs.AddValue("CurTxLaserFreq", pMob->CurTxLaserFreq.Get());
        webs.AddValue("CurRxLaserFreq", pMob->CurRxLaserFreq.Get());
        webs.AddValue("DGD", pMob->DGD.Get());
        webs.AddValue("CD", pMob->CD.Get());
        webs.AddValue("OSNR", pMob->OSNR.Get());
        webs.AddValue("Qfactor", pMob->Qfactor.Get());

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

void WebOchSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;
    std::string resp = WebFormResponse(true, "command complete");

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    OchMob *pMob = OchMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "FecMode", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->FecMode.Set(atoi(ptr))) goto error_exit;
    }

    ptr = websGetVar(wp, "ChanNum", "");
    if (strlen(ptr) > 0)
    {
        if (!pMob->ChanNum.Set(atoi(ptr))) goto error_exit;
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

void WebOchAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebOchSet(wp);
    else
        WebOchGet(wp);
}

