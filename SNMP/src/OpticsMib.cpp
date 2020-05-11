#include "DanOS.h"
#include "Snmp.h"
#include "OpticsMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static netsnmp_table_data_set *table_set = NULL;

static int handle_Optics(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    int inst = SnmpGetRequestInstance(requests);
    OpticsMob *pMob = OpticsMob::GetInst(inst);
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
                case 2:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Type.Get().c_str(), pMob->Type.Get().size());
                    break;
                case 3:
                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Laser.Get()), sizeof(pMob->Laser.Get()));
                    break;
                case 4:
                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Los.Get()), sizeof(pMob->Los.Get()));
                    break;
                case 5:
                    {
                        char fval[32] = {0};
                        sprintf(fval, "%.2f", pMob->TxPower.Get());
                        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, fval, strlen(fval));
                    }
                    break;
                case 6:
                    {
                        char fval[32] = {0};
                        sprintf(fval, "%.2f", pMob->RxPower.Get());
                        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, fval, strlen(fval));
                    }
                    break;
                case 7:
                    {
                        char fval[32] = {0};
                        sprintf(fval, "%.2f", pMob->Temp.Get());
                        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, fval, strlen(fval));
                    }
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

void InitOpticsMib(void)
{
    static oid Optics_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 6, 1 };
    table_set = netsnmp_create_table_data_set("Optics");
    table_set->allow_creation = 1;
    netsnmp_table_set_add_indexes(table_set, ASN_INTEGER, 0);
    netsnmp_table_set_multi_add_default_row(table_set,
                                            1, ASN_OCTET_STR, 0, NULL, 0,
                                            2, ASN_OCTET_STR, 0, NULL, 0,
                                            3, ASN_INTEGER, 0, NULL, 0,
                                            4, ASN_INTEGER, 0, NULL, 0,
                                            5, ASN_INTEGER, 0, NULL, 0,
                                            6, ASN_INTEGER, 0, NULL, 0,
                                            7, ASN_INTEGER, 0, NULL, 0,
                                            0);
    netsnmp_register_table_data_set(netsnmp_create_handler_registration("Optics", handle_Optics, Optics_oid, OID_LENGTH(Optics_oid), HANDLER_CAN_RWRITE), table_set, NULL);

    {
        int inst = 0, num = OpticsMob::GetInstNum();
        for (; inst < num; inst++)
        {
            OpticsMob *pMob = OpticsMob::GetInst(inst);
            if (pMob == NULL) continue;

            netsnmp_table_row *prow = netsnmp_create_table_data_row();
            netsnmp_table_row_add_index(prow, ASN_INTEGER, &inst, sizeof(inst));

            netsnmp_set_row_column(prow, 1, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());
            netsnmp_mark_row_column_writable(prow, 1, 0);
            netsnmp_set_row_column(prow, 2, ASN_OCTET_STR, pMob->Type.Get().c_str(), pMob->Type.Get().size());
            netsnmp_mark_row_column_writable(prow, 2, 0);
            netsnmp_set_row_column(prow, 3, ASN_INTEGER, &pMob->Laser.Get(), sizeof(pMob->Laser.Get()));
            netsnmp_mark_row_column_writable(prow, 3, 0);
            netsnmp_set_row_column(prow, 4, ASN_INTEGER, &pMob->Los.Get(), sizeof(pMob->Los.Get()));
            netsnmp_mark_row_column_writable(prow, 4, 0);
            netsnmp_set_row_column(prow, 5, ASN_INTEGER, &pMob->TxPower.Get(), sizeof(pMob->TxPower.Get()));
            netsnmp_mark_row_column_writable(prow, 5, 0);
            netsnmp_set_row_column(prow, 6, ASN_INTEGER, &pMob->RxPower.Get(), sizeof(pMob->RxPower.Get()));
            netsnmp_mark_row_column_writable(prow, 6, 0);
            netsnmp_set_row_column(prow, 7, ASN_INTEGER, &pMob->Temp.Get(), sizeof(pMob->Temp.Get()));
            netsnmp_mark_row_column_writable(prow, 7, 0);

            netsnmp_table_dataset_add_row(table_set, prow);
        }
    }
}

