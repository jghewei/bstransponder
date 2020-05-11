#include "DanOS.h"
#include "Snmp.h"
#include "FileMgmtMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static int handle_ServerIpAddress(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    FileMgmtMob *pMob = FileMgmtMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->ServerIpAddress.Get().c_str(), pMob->ServerIpAddress.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->ServerIpAddress.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_ServerProtocol(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    FileMgmtMob *pMob = FileMgmtMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->ServerProtocol.Get()), sizeof(pMob->ServerProtocol.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->ServerProtocol.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_FilePathName(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    FileMgmtMob *pMob = FileMgmtMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->FilePathName.Get().c_str(), pMob->FilePathName.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->FilePathName.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Action(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    FileMgmtMob *pMob = FileMgmtMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Action.Get()), sizeof(pMob->Action.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->Action.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_State(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    FileMgmtMob *pMob = FileMgmtMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->State.Get()), sizeof(pMob->State.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->State.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitFileMgmtMib(void)
{
    static oid ServerIpAddress_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 2, 1 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("ServerIpAddress", handle_ServerIpAddress, ServerIpAddress_oid, OID_LENGTH(ServerIpAddress_oid), HANDLER_CAN_RWRITE));

    static oid ServerProtocol_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 2, 2 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("ServerProtocol", handle_ServerProtocol, ServerProtocol_oid, OID_LENGTH(ServerProtocol_oid), HANDLER_CAN_RWRITE));

    static oid FilePathName_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 2, 3 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("FilePathName", handle_FilePathName, FilePathName_oid, OID_LENGTH(FilePathName_oid), HANDLER_CAN_RWRITE));

    static oid Action_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 2, 4 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Action", handle_Action, Action_oid, OID_LENGTH(Action_oid), HANDLER_CAN_RWRITE));

    static oid State_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 2, 5 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("State", handle_State, State_oid, OID_LENGTH(State_oid), HANDLER_CAN_RONLY));

}
