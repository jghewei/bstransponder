#include "Web.h"

extern void WebSystemAction(Webs *wp);
extern void WebFileMgmtAction(Webs *wp);
extern void WebShelfAction(Webs *wp);
extern void WebCardAction(Webs *wp);
extern void WebPsuAction(Webs *wp);
extern void WebOpticsAction(Webs *wp);
extern void WebFanAction(Webs *wp);
extern void WebNetworkAction(Webs *wp);
extern void WebPortAction(Webs *wp);
extern void WebEthAction(Webs *wp);
extern void WebOtuAction(Webs *wp);
extern void WebOchAction(Webs *wp);
extern void WebStmAction(Webs *wp);
extern void WebInventoryAction(Webs *wp);
extern void WebUserAdminAction(Webs *wp);
extern void WebSnmpAction(Webs *wp);
extern void WebCounterAction(Webs *wp);
extern void WebAlarmAction(Webs *wp);

void webRegisterAction(void)
{
    websDefineAction("System", (void*)WebSystemAction);
    websDefineAction("FileMgmt", (void*)WebFileMgmtAction);
    websDefineAction("Shelf", (void*)WebShelfAction);
    websDefineAction("Card", (void*)WebCardAction);
    websDefineAction("Psu", (void*)WebPsuAction);
    websDefineAction("Optics", (void*)WebOpticsAction);
    websDefineAction("Fan", (void*)WebFanAction);
    websDefineAction("Network", (void*)WebNetworkAction);
    websDefineAction("Port", (void*)WebPortAction);
    websDefineAction("Eth", (void*)WebEthAction);
    websDefineAction("Otu", (void*)WebOtuAction);
    websDefineAction("Och", (void*)WebOchAction);
    websDefineAction("Stm", (void*)WebStmAction);
    websDefineAction("Inventory", (void*)WebInventoryAction);
    websDefineAction("UserAdmin", (void*)WebUserAdminAction);
    websDefineAction("Snmp", (void*)WebSnmpAction);
    websDefineAction("Counter", (void*)WebCounterAction);
    websDefineAction("Alarm", (void*)WebAlarmAction);
}
