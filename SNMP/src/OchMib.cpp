#include "DanOS.h"
#include "Snmp.h"
#include "OchMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static netsnmp_table_data_set *table_set = NULL;

static int handle_Och(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    int inst = SnmpGetRequestInstance(requests);
    OchMob *pMob = OchMob::GetInst(inst);
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
                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->FecMode.Get()), sizeof(pMob->FecMode.Get()));
                    break;
                case 3:
                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->ChanNum.Get()), sizeof(pMob->ChanNum.Get()));
                    break;
                case 4:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->MinLaserFreq.Get().c_str(), pMob->MinLaserFreq.Get().size());
                    break;
                case 5:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->MaxLaserFreq.Get().c_str(), pMob->MaxLaserFreq.Get().size());
                    break;
                case 6:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->GridSpacing.Get().c_str(), pMob->GridSpacing.Get().size());
                    break;
                case 7:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->MaxChans.Get().c_str(), pMob->MaxChans.Get().size());
                    break;
                case 8:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->CurTxLaserFreq.Get().c_str(), pMob->CurTxLaserFreq.Get().size());
                    break;
                case 9:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->CurRxLaserFreq.Get().c_str(), pMob->CurRxLaserFreq.Get().size());
                    break;
                case 10:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->DGD.Get().c_str(), pMob->DGD.Get().size());
                    break;
                case 11:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->CD.Get().c_str(), pMob->CD.Get().size());
                    break;
                case 12:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->OSNR.Get().c_str(), pMob->OSNR.Get().size());
                    break;
                case 13:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->Qfactor.Get().c_str(), pMob->Qfactor.Get().size());
                    break;
                default:
                    break;
            }
            break;

        case MODE_SET_ACTION:
            switch (table_info->colnum)
            {
                case 2:
                    {
                        int val = 0;
                        memcpy(&val, requests->requestvb->buf, sizeof(int));
                        pMob->FecMode.Set(val);
                    }
                    break;
                case 3:
                    {
                        int val = 0;
                        memcpy(&val, requests->requestvb->buf, sizeof(int));
                        pMob->ChanNum.Set(val);
                    }
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return SNMP_ERR_NOERROR;
}

void InitOchMib(void)
{
    static oid Och_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 12, 1 };
    table_set = netsnmp_create_table_data_set("Och");
    table_set->allow_creation = 1;
    netsnmp_table_set_add_indexes(table_set, ASN_INTEGER, 0);
    netsnmp_table_set_multi_add_default_row(table_set,
                                            1, ASN_OCTET_STR, 0, NULL, 0,
                                            2, ASN_INTEGER, 1, NULL, 0,
                                            3, ASN_INTEGER, 1, NULL, 0,
                                            4, ASN_OCTET_STR, 0, NULL, 0,
                                            5, ASN_OCTET_STR, 0, NULL, 0,
                                            6, ASN_OCTET_STR, 0, NULL, 0,
                                            7, ASN_OCTET_STR, 0, NULL, 0,
                                            8, ASN_OCTET_STR, 0, NULL, 0,
                                            9, ASN_OCTET_STR, 0, NULL, 0,
                                            10, ASN_OCTET_STR, 0, NULL, 0,
                                            11, ASN_OCTET_STR, 0, NULL, 0,
                                            12, ASN_OCTET_STR, 0, NULL, 0,
                                            13, ASN_OCTET_STR, 0, NULL, 0,
                                            0);
    netsnmp_register_table_data_set(netsnmp_create_handler_registration("Och", handle_Och, Och_oid, OID_LENGTH(Och_oid), HANDLER_CAN_RWRITE), table_set, NULL);

    {
        int inst = 0, num = OchMob::GetInstNum();
        for (; inst < num; inst++)
        {
            OchMob *pMob = OchMob::GetInst(inst);
            if (pMob == NULL) continue;

            netsnmp_table_row *prow = netsnmp_create_table_data_row();
            netsnmp_table_row_add_index(prow, ASN_INTEGER, &inst, sizeof(inst));

            netsnmp_set_row_column(prow, 1, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());
            netsnmp_mark_row_column_writable(prow, 1, 0);
            netsnmp_set_row_column(prow, 2, ASN_INTEGER, &pMob->FecMode.Get(), sizeof(pMob->FecMode.Get()));
            netsnmp_mark_row_column_writable(prow, 2, 1);
            netsnmp_set_row_column(prow, 3, ASN_INTEGER, &pMob->ChanNum.Get(), sizeof(pMob->ChanNum.Get()));
            netsnmp_mark_row_column_writable(prow, 3, 1);
            netsnmp_set_row_column(prow, 4, ASN_OCTET_STR, pMob->MinLaserFreq.Get().c_str(), pMob->MinLaserFreq.Get().size());
            netsnmp_mark_row_column_writable(prow, 4, 0);
            netsnmp_set_row_column(prow, 5, ASN_OCTET_STR, pMob->MaxLaserFreq.Get().c_str(), pMob->MaxLaserFreq.Get().size());
            netsnmp_mark_row_column_writable(prow, 5, 0);
            netsnmp_set_row_column(prow, 6, ASN_OCTET_STR, pMob->GridSpacing.Get().c_str(), pMob->GridSpacing.Get().size());
            netsnmp_mark_row_column_writable(prow, 6, 0);
            netsnmp_set_row_column(prow, 7, ASN_OCTET_STR, pMob->MaxChans.Get().c_str(), pMob->MaxChans.Get().size());
            netsnmp_mark_row_column_writable(prow, 7, 0);
            netsnmp_set_row_column(prow, 8, ASN_OCTET_STR, pMob->CurTxLaserFreq.Get().c_str(), pMob->CurTxLaserFreq.Get().size());
            netsnmp_mark_row_column_writable(prow, 8, 0);
            netsnmp_set_row_column(prow, 9, ASN_OCTET_STR, pMob->CurRxLaserFreq.Get().c_str(), pMob->CurRxLaserFreq.Get().size());
            netsnmp_mark_row_column_writable(prow, 9, 0);
            netsnmp_set_row_column(prow, 10, ASN_OCTET_STR, pMob->DGD.Get().c_str(), pMob->DGD.Get().size());
            netsnmp_mark_row_column_writable(prow, 10, 0);
            netsnmp_set_row_column(prow, 11, ASN_OCTET_STR, pMob->CD.Get().c_str(), pMob->CD.Get().size());
            netsnmp_mark_row_column_writable(prow, 11, 0);
            netsnmp_set_row_column(prow, 12, ASN_OCTET_STR, pMob->OSNR.Get().c_str(), pMob->OSNR.Get().size());
            netsnmp_mark_row_column_writable(prow, 12, 0);
            netsnmp_set_row_column(prow, 13, ASN_OCTET_STR, pMob->Qfactor.Get().c_str(), pMob->Qfactor.Get().size());
            netsnmp_mark_row_column_writable(prow, 13, 0);

            netsnmp_table_dataset_add_row(table_set, prow);
        }
    }
}

