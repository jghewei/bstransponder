#include "DanOS.h"
#include "Snmp.h"
#include "ShelfMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static int handle_InletTemp(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    ShelfMob *pMob = ShelfMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->InletTemp.Get()), sizeof(pMob->InletTemp.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->InletTemp.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_OutletTemp(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    ShelfMob *pMob = ShelfMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->OutletTemp.Get()), sizeof(pMob->OutletTemp.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->OutletTemp.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_TempLowThd(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    ShelfMob *pMob = ShelfMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->TempLowThd.Get()), sizeof(pMob->TempLowThd.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->TempLowThd.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_TempHighThd(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    ShelfMob *pMob = ShelfMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->TempHighThd.Get()), sizeof(pMob->TempHighThd.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->TempHighThd.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitShelfMib(void)
{
    static oid InletTemp_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 3, 1 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("InletTemp", handle_InletTemp, InletTemp_oid, OID_LENGTH(InletTemp_oid), HANDLER_CAN_RONLY));

    static oid OutletTemp_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 3, 2 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("OutletTemp", handle_OutletTemp, OutletTemp_oid, OID_LENGTH(OutletTemp_oid), HANDLER_CAN_RONLY));

    static oid TempLowThd_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 3, 3 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("TempLowThd", handle_TempLowThd, TempLowThd_oid, OID_LENGTH(TempLowThd_oid), HANDLER_CAN_RWRITE));

    static oid TempHighThd_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 3, 4 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("TempHighThd", handle_TempHighThd, TempHighThd_oid, OID_LENGTH(TempHighThd_oid), HANDLER_CAN_RWRITE));

}
