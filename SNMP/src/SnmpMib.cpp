#include "DanOS.h"
#include "Snmp.h"
#include "SnmpMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static int handle_Enable(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Enable.Get()), sizeof(pMob->Enable.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->Enable.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_GetCommunity(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->GetCommunity.Get().c_str(), pMob->GetCommunity.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->GetCommunity.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_SetCommunity(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->SetCommunity.Get().c_str(), pMob->SetCommunity.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->SetCommunity.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_TrapReceiver1(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->TrapReceiver1.Get().c_str(), pMob->TrapReceiver1.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->TrapReceiver1.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_TrapReceiver2(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->TrapReceiver2.Get().c_str(), pMob->TrapReceiver2.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->TrapReceiver2.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_TrapReceiver3(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SnmpMob *pMob = SnmpMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->TrapReceiver3.Get().c_str(), pMob->TrapReceiver3.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->TrapReceiver3.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitSnmpMib(void)
{
    static oid Enable_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 1 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Enable", handle_Enable, Enable_oid, OID_LENGTH(Enable_oid), HANDLER_CAN_RWRITE));

    static oid GetCommunity_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 2 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("GetCommunity", handle_GetCommunity, GetCommunity_oid, OID_LENGTH(GetCommunity_oid), HANDLER_CAN_RWRITE));

    static oid SetCommunity_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 3 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("SetCommunity", handle_SetCommunity, SetCommunity_oid, OID_LENGTH(SetCommunity_oid), HANDLER_CAN_RWRITE));

    static oid TrapReceiver1_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 4 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("TrapReceiver1", handle_TrapReceiver1, TrapReceiver1_oid, OID_LENGTH(TrapReceiver1_oid), HANDLER_CAN_RWRITE));

    static oid TrapReceiver2_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 5 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("TrapReceiver2", handle_TrapReceiver2, TrapReceiver2_oid, OID_LENGTH(TrapReceiver2_oid), HANDLER_CAN_RWRITE));

    static oid TrapReceiver3_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 16, 6 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("TrapReceiver3", handle_TrapReceiver3, TrapReceiver3_oid, OID_LENGTH(TrapReceiver3_oid), HANDLER_CAN_RWRITE));

}
