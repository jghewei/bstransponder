#include "DanOS.h"
#include "Snmp.h"
#include "InventoryMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

static netsnmp_table_data_set *table_set = NULL;

static int handle_Inventory(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    int inst = SnmpGetRequestInstance(requests);
    InventoryMob *pMob = InventoryMob::GetInst(inst);
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
                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->Present.Get()), sizeof(pMob->Present.Get()));
                    break;
                case 3:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->ModuleType.Get().c_str(), pMob->ModuleType.Get().size());
                    break;
                case 4:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->PartNumber.Get().c_str(), pMob->PartNumber.Get().size());
                    break;
                case 5:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->SerialNumber.Get().c_str(), pMob->SerialNumber.Get().size());
                    break;
                case 6:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->HwVersion.Get().c_str(), pMob->HwVersion.Get().size());
                    break;
                case 7:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->VendorName.Get().c_str(), pMob->VendorName.Get().size());
                    break;
                case 8:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->ProduceDate.Get().c_str(), pMob->ProduceDate.Get().size());
                    break;
                case 9:
                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->FwVersion.Get().c_str(), pMob->FwVersion.Get().size());
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

void InitInventoryMib(void)
{
    static oid Inventory_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 14, 1 };
    table_set = netsnmp_create_table_data_set("Inventory");
    table_set->allow_creation = 1;
    netsnmp_table_set_add_indexes(table_set, ASN_INTEGER, 0);
    netsnmp_table_set_multi_add_default_row(table_set,
                                            1, ASN_OCTET_STR, 0, NULL, 0,
                                            2, ASN_INTEGER, 0, NULL, 0,
                                            3, ASN_OCTET_STR, 0, NULL, 0,
                                            4, ASN_OCTET_STR, 0, NULL, 0,
                                            5, ASN_OCTET_STR, 0, NULL, 0,
                                            6, ASN_OCTET_STR, 0, NULL, 0,
                                            7, ASN_OCTET_STR, 0, NULL, 0,
                                            8, ASN_OCTET_STR, 0, NULL, 0,
                                            9, ASN_OCTET_STR, 0, NULL, 0,
                                            0);
    netsnmp_register_table_data_set(netsnmp_create_handler_registration("Inventory", handle_Inventory, Inventory_oid, OID_LENGTH(Inventory_oid), HANDLER_CAN_RWRITE), table_set, NULL);

    {
        int inst = 0, num = InventoryMob::GetInstNum();
        for (; inst < num; inst++)
        {
            InventoryMob *pMob = InventoryMob::GetInst(inst);
            if (pMob == NULL) continue;

            netsnmp_table_row *prow = netsnmp_create_table_data_row();
            netsnmp_table_row_add_index(prow, ASN_INTEGER, &inst, sizeof(inst));

            netsnmp_set_row_column(prow, 1, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());
            netsnmp_mark_row_column_writable(prow, 1, 0);
            netsnmp_set_row_column(prow, 2, ASN_INTEGER, &pMob->Present.Get(), sizeof(pMob->Present.Get()));
            netsnmp_mark_row_column_writable(prow, 2, 0);
            netsnmp_set_row_column(prow, 3, ASN_OCTET_STR, pMob->ModuleType.Get().c_str(), pMob->ModuleType.Get().size());
            netsnmp_mark_row_column_writable(prow, 3, 0);
            netsnmp_set_row_column(prow, 4, ASN_OCTET_STR, pMob->PartNumber.Get().c_str(), pMob->PartNumber.Get().size());
            netsnmp_mark_row_column_writable(prow, 4, 0);
            netsnmp_set_row_column(prow, 5, ASN_OCTET_STR, pMob->SerialNumber.Get().c_str(), pMob->SerialNumber.Get().size());
            netsnmp_mark_row_column_writable(prow, 5, 0);
            netsnmp_set_row_column(prow, 6, ASN_OCTET_STR, pMob->HwVersion.Get().c_str(), pMob->HwVersion.Get().size());
            netsnmp_mark_row_column_writable(prow, 6, 0);
            netsnmp_set_row_column(prow, 7, ASN_OCTET_STR, pMob->VendorName.Get().c_str(), pMob->VendorName.Get().size());
            netsnmp_mark_row_column_writable(prow, 7, 0);
            netsnmp_set_row_column(prow, 8, ASN_OCTET_STR, pMob->ProduceDate.Get().c_str(), pMob->ProduceDate.Get().size());
            netsnmp_mark_row_column_writable(prow, 8, 0);
            netsnmp_set_row_column(prow, 9, ASN_OCTET_STR, pMob->FwVersion.Get().c_str(), pMob->FwVersion.Get().size());
            netsnmp_mark_row_column_writable(prow, 9, 0);

            netsnmp_table_dataset_add_row(table_set, prow);
        }
    }
}

