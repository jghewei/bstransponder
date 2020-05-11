#include "DanOS.h"
#include "UserAdminMob.h"
#include "Web.h"
#include "CThread.h"

static int webFinished = 0;     // for thread terminate

bool isWebPostMethod(Webs *wp)
{
    if (strcmp(websGetMethod(wp), "POST") == 0)
        return true;
    else
        return false;
}

std::string WebFormResponse(bool success, const char *message)
{
    std::string resp;

    resp += "{\"success\":";
    if (success)
        resp += "true";
    else
        resp += "false";
    resp += ",\"message\":\"";
    resp += message;
    resp += "\"}";

    return resp;
}

/////////////////////////////////////////////////////////
// Web Server
/////////////////////////////////////////////////////////

void* WebServiceThread(void *param)
{
    APP_LOG("Web service started");
    websServiceEvents(&webFinished);
    return NULL;
}

void WebLoginAction(Webs *wp)
{
    int inst = 0, num = UserAdminMob::GetInstNum();
    char *username = websGetVar(wp, "username", "");
    char *password = websGetVar(wp, "password", "");
    bool auth = false;

    APP_LOG("username: %s, password: %s", username, password);
    
    for (; inst < num; inst++)
    {
        if (strlen(username) < 3 || strlen(password) < 3) continue;

        if (strcmp(username, "root") == 0 &&
            strcmp(password, "root") == 0)
        {
            auth = true;
            break;
        }        
        
        UserAdminMob *pMob = UserAdminMob::GetInst(inst);
        if (pMob == NULL) continue;

        APP_LOG("Username: %s, Password: %s", pMob->UserName.Get().c_str(), pMob->Password.Get().c_str());

        if (strcmp(username, pMob->UserName.Get().c_str()) == 0 &&
            strcmp(password, pMob->Password.Get().c_str()) == 0)
        {
            auth = true;
            break;
        }
    }

    std::string resp = WebFormResponse(auth, "command complete");
    websSetStatus(wp, 200);
    websWriteHeaders(wp, resp.size(), 0);
    websWriteEndHeaders(wp);
    websWrite(wp, (char*)resp.c_str());
    websDone(wp);
}

extern void webRegisterAction(void);

int WebInit(void)
{    
    if (websOpen((char*)"html", (char*)"route.txt") < 0) {
        APP_LOG("websOpen failed");
        return -1;
    }

    if (websListen((char*)":80") < 0) {
        APP_LOG("Can't listen on port 80");
        return -2;
    }

    webRegisterAction();
    websDefineAction("Login", (void*)WebLoginAction);

    new CThread("web", WebServiceThread, NULL);
    
    return 0;
}

void WebExit(void)
{
    webFinished = 1;
    websClose();
}

