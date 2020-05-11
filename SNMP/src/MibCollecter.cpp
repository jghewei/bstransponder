#include "Snmp.h"

extern void InitSystemMib();
extern void InitFileMgmtMib();
extern void InitShelfMib();
extern void InitCardMib();
extern void InitPsuMib();
extern void InitOpticsMib();
extern void InitFanMib();
extern void InitNetworkMib();
extern void InitPortMib();
extern void InitEthMib();
extern void InitOtuMib();
extern void InitOchMib();
extern void InitStmMib();
extern void InitInventoryMib();
extern void InitUserAdminMib();
extern void InitSnmpMib();

void InitMibs(void)
{
    InitSystemMib();
    InitFileMgmtMib();
    InitShelfMib();
    InitCardMib();
    InitPsuMib();
    InitOpticsMib();
    InitFanMib();
    InitNetworkMib();
    InitPortMib();
    InitEthMib();
    InitOtuMib();
    InitOchMib();
    InitStmMib();
    InitInventoryMib();
    InitUserAdminMib();
    InitSnmpMib();
}
