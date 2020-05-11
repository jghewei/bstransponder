#include "DanOS.h"
#include "Snmp.h"
#include "SystemMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static int handle_Name(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Name.Get().c_str(), pMob->Name.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->Name.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Contact(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Contact.Get().c_str(), pMob->Contact.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->Contact.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Location(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Location.Get().c_str(), pMob->Location.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->Location.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Description(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Description.Get().c_str(), pMob->Description.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->Description.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_UpTime(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->UpTime.Get()), sizeof(pMob->UpTime.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->UpTime.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_CpuUsage(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->CpuUsage.Get()), sizeof(pMob->CpuUsage.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->CpuUsage.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_MemUsage(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->MemUsage.Get()), sizeof(pMob->MemUsage.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->MemUsage.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_CurrentDateTime(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->CurrentDateTime.Get().c_str(), pMob->CurrentDateTime.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->CurrentDateTime.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_SetDateTime(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->SetDateTime.Get().c_str(), pMob->SetDateTime.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->SetDateTime.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_SwVersion(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->SwVersion.Get().c_str(), pMob->SwVersion.Get().size());
            break;

        case MODE_SET_ACTION:
            pMob->SwVersion.Set((char*)requests->requestvb->buf);
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Restart(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Restart.Get()), sizeof(pMob->Restart.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->Restart.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_Database(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Database.Get()), sizeof(pMob->Database.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->Database.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

static int handle_LampTest(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    SystemMob *pMob = SystemMob::GetInst(0);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    switch (reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->LampTest.Get()), sizeof(pMob->LampTest.Get()));
            break;

        case MODE_SET_ACTION:
            {
                int val = 0;
                memcpy(&val, requests->requestvb->buf, sizeof(int));
                pMob->LampTest.Set(val);
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitSystemMib(void)
{
    static oid Name_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 1 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Name", handle_Name, Name_oid, OID_LENGTH(Name_oid), HANDLER_CAN_RWRITE));

    static oid Contact_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 2 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Contact", handle_Contact, Contact_oid, OID_LENGTH(Contact_oid), HANDLER_CAN_RWRITE));

    static oid Location_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 3 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Location", handle_Location, Location_oid, OID_LENGTH(Location_oid), HANDLER_CAN_RWRITE));

    static oid Description_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 4 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Description", handle_Description, Description_oid, OID_LENGTH(Description_oid), HANDLER_CAN_RWRITE));

    static oid UpTime_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 5 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("UpTime", handle_UpTime, UpTime_oid, OID_LENGTH(UpTime_oid), HANDLER_CAN_RONLY));

    static oid CpuUsage_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 6 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("CpuUsage", handle_CpuUsage, CpuUsage_oid, OID_LENGTH(CpuUsage_oid), HANDLER_CAN_RONLY));

    static oid MemUsage_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 7 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("MemUsage", handle_MemUsage, MemUsage_oid, OID_LENGTH(MemUsage_oid), HANDLER_CAN_RONLY));

    static oid CurrentDateTime_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 8 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("CurrentDateTime", handle_CurrentDateTime, CurrentDateTime_oid, OID_LENGTH(CurrentDateTime_oid), HANDLER_CAN_RONLY));

    static oid SetDateTime_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 9 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("SetDateTime", handle_SetDateTime, SetDateTime_oid, OID_LENGTH(SetDateTime_oid), HANDLER_CAN_RWRITE));

    static oid SwVersion_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 10 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("SwVersion", handle_SwVersion, SwVersion_oid, OID_LENGTH(SwVersion_oid), HANDLER_CAN_RONLY));

    static oid Restart_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 11 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Restart", handle_Restart, Restart_oid, OID_LENGTH(Restart_oid), HANDLER_CAN_RWRITE));

    static oid Database_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 12 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("Database", handle_Database, Database_oid, OID_LENGTH(Database_oid), HANDLER_CAN_RWRITE));

    static oid LampTest_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 1, 13 };
    netsnmp_register_scalar(netsnmp_create_handler_registration("LampTest", handle_LampTest, LampTest_oid, OID_LENGTH(LampTest_oid), HANDLER_CAN_RWRITE));

}
