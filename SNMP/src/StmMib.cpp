#include "DanOS.h"
#include "Snmp.h"
#include "StmMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static netsnmp_table_data_set *table_set = NULL;

static int handle_Stm(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    int inst = SnmpGetRequestInstance(requests);
    StmMob *pMob = StmMob::GetInst(inst);
    if (pMob == NULL) return SNMP_ERR_GENERR;

    netsnmp_table_request_info *table_info = netsnmp_extract_table_info(requests);

    switch (reqinfo->mode)
    {
        case MODE_GET:
            switch (table_info->colnum)
            {
                case 1:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());
                    break;
                default:
                    break;
            }
            break;

        case MODE_SET_ACTION:
            switch (table_info->colnum)
            {
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitStmMib(void)
{
    static oid Stm_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 13, 1 };
    table_set = netsnmp_create_table_data_set("Stm");
    table_set->allow_creation = 1;
    netsnmp_table_set_add_indexes(table_set, ASN_INTEGER, 0);
    netsnmp_table_set_multi_add_default_row(table_set,
                                            1, ASN_OCTET_STR, 0, NULL, 0,
                                            0);
    netsnmp_register_table_data_set(netsnmp_create_handler_registration("Stm", handle_Stm, Stm_oid, OID_LENGTH(Stm_oid), HANDLER_CAN_RWRITE), table_set, NULL);

    {
        int inst = 0, num = StmMob::GetInstNum();
        for (; inst < num; inst++)
        {
            StmMob *pMob = StmMob::GetInst(inst);
            if (pMob == NULL) continue;

            netsnmp_table_row *prow = netsnmp_create_table_data_row();
            netsnmp_table_row_add_index(prow, ASN_INTEGER, &inst, sizeof(inst));

            netsnmp_set_row_column(prow, 1, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());
            netsnmp_mark_row_column_writable(prow, 1, 0);

            netsnmp_table_dataset_add_row(table_set, prow);
        }
    }
}

