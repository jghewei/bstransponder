#include "DanOS.h"
#include "Snmp.h"
#include "CardMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static int handle_BoardTemp(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    CardMob *pMob = CardMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->BoardTemp.Get()), sizeof(pMob->BoardTemp.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->BoardTemp.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_LowTempThd(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    CardMob *pMob = CardMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->LowTempThd.Get()), sizeof(pMob->LowTempThd.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->LowTempThd.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_HighTempThd(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    CardMob *pMob = CardMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->HighTempThd.Get()), sizeof(pMob->HighTempThd.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->HighTempThd.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_OperStatus(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    CardMob *pMob = CardMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->OperStatus.Get()), sizeof(pMob->OperStatus.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->OperStatus.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitCardMib(void)
{
    static oid BoardTemp_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 4, 1 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("BoardTemp", handle_BoardTemp, BoardTemp_oid, OID_LENGTH(BoardTemp_oid), HANDLER_CAN_RONLY));

    static oid LowTempThd_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 4, 2 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("LowTempThd", handle_LowTempThd, LowTempThd_oid, OID_LENGTH(LowTempThd_oid), HANDLER_CAN_RWRITE));

    static oid HighTempThd_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 4, 3 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("HighTempThd", handle_HighTempThd, HighTempThd_oid, OID_LENGTH(HighTempThd_oid), HANDLER_CAN_RWRITE));

    static oid OperStatus_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 4, 4 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("OperStatus", handle_OperStatus, OperStatus_oid, OID_LENGTH(OperStatus_oid), HANDLER_CAN_RONLY));

}
