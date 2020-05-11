/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
* --------------------------------------------------------------------------
*  This software embodies materials and concepts which are proprietary and
*  confidential to PMC-Sierra, Inc.
*  PMC-Sierra distributes this software to its customers pursuant to the
*  terms and conditions of the Software License Agreement
*  contained in the text file software.lic that is distributed along with
*  the software. This software can only be utilized if all
*  terms and conditions of the Software License Agreement are
*  accepted. If there are any questions, concerns, or if the
*  Software License Agreement text file, software.lic, is missing please
*  contact PMC-Sierra for assistance.
* --------------------------------------------------------------------------
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ECLASS_H
#define _ECLASS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "eclass_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/
/*!< eclass packet type */
typedef enum
{
  ECLASS_EXTRACT_NO_PKTS       = 0x0,
  ECLASS_EXTRACT_ALL_PKTS_MGMT = 0x1,
  ECLASS_EXTRACT_ALL_PKTS_CPU  = 0x2,
  ECLASS_EXTRACT_PTP_PKTS      = 0x3,
  ECLASS_G8021_CLAUSE_8_3      = 0x4
} eclass_extract_pkt_t;

/*******************************************************************************
* ENUM: eclass_vlan_tci_check_filter_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ECLASS VLAN tci check filter.
*
* ELEMENTS:
*   ECLASS_VLAN_TCI_CHECK_OVER_ETHERNET  - 0 vlan tci check for Ethernet
*   ECLASS_VLAN_TCI_CHECK_OVER_IPV4      - 1 vlan tci check for ipv4
*
*******************************************************************************/
typedef enum
{
  ECLASS_VLAN_TCI_CHECK_OVER_ETHERNET   = 0x0,
  ECLASS_VLAN_TCI_CHECK_OVER_IPV4       = 0x1,
} eclass_vlan_tci_check_filter_type_t;

/*******************************************************************************
* ENUM: eclass_vlan_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ECLASS VLAN config.
*
* ELEMENTS:
*   ECLASS_VLAN_CFG_NO_VLAN      - No VLAN
*   ECLASS_VLAN_CFG_1VLAN        - 1 VLAN tags
*   ECLASS_VLAN_CFG_2VLAN        - 2 VALN tags
*
*******************************************************************************/
typedef enum
{
  ECLASS_VLAN_CFG_NO_VLAN      = 0x0,
  ECLASS_VLAN_CFG_1VLAN        = 0x1,
  ECLASS_VLAN_CFG_2VLAN        = 0x2,
} eclass_vlan_cfg_t;

/*
** Constants
*/
#define ECLASS_NUM_LINKS 12

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: eclass_pkt_extract_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains packet forwarding context parameters.
*
* ELEMENTS:
*  valid        - entry status
*  pkt_type     - packet type
*  is_vlan      - when TRUE vlan filter is valid
*  vlan_entry   - VLAN filter
*
*******************************************************************************/
typedef struct eclass_pkt_extract_cfg_struct_t
{   
    BOOL8                   valid;
    util_global_pkt_type_t  pkt_type;
    BOOL8                   is_vlan;
    util_global_vlan_cfg_t  vlan_entry;        
}eclass_pkt_extract_cfg_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC eclass_handle_t *eclass_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void eclass_ctxt_destroy(eclass_handle_t *eclass_handle);
PUBLIC void eclass_handle_init(eclass_handle_t *eclass_handle);
PUBLIC PMC_ERROR eclass_handle_restart_init(eclass_handle_t *eclass_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR eclass_init(eclass_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL eclass_start_state_test(eclass_handle_t *hndl);

PUBLIC UINT32 eclass_base_address_get(eclass_handle_t *eclass_handle);

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
PUBLIC PMC_ERROR eclass_extract_packet(eclass_handle_t *hndl, UINT32 link, eclass_extract_pkt_t pkt_type);
PUBLIC PMC_ERROR eclass_combiner_cfg(eclass_handle_t *hndl, UINT32 link, eclass_extract_pkt_t pkt_type );
PUBLIC PMC_ERROR eclass_routing_filter_cfg(eclass_handle_t *hndl, UINT32 link, eclass_extract_pkt_t pkt_type );
PUBLIC PMC_ERROR eclass_nomatch_cfg(eclass_handle_t *hndl, UINT32 link, eclass_extract_pkt_t pkt_type );
PUBLIC PMC_ERROR eclass_activate(eclass_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR eclass_deactivate(eclass_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR eclass_general_config_filter_cfg(eclass_handle_t *hndl, UINT32 link, eclass_extract_pkt_t pkt_type );
PUBLIC PMC_ERROR eclass_ptp_unicastdacheck_over_eth_filter_cfg(eclass_handle_t *hndl, 
                                                               UINT32 link, 
                                                               UINT32 da_msb,
                                                               UINT32 da_lsb,
                                                               BOOL enable);
PUBLIC PMC_ERROR eclass_ptp_unicastdacheck_over_ipv4_filter_cfg(eclass_handle_t *hndl, 
                                                                UINT32 link, 
                                                                UINT32 ipv4_da,
                                                                BOOL enable);
PUBLIC PMC_ERROR eclass_ptp_vlancheck_tci_filter_cfg(eclass_handle_t *hndl, 
                                                               UINT32 link, 
                                                               eclass_vlan_tci_check_filter_type_t filter_type,
                                                               eclass_vlan_cfg_t vlan_cfg_type,
                                                               UINT16 tci_value1_inner,
                                                               UINT16 tci_value2_outer,
                                                               BOOL enable);
PUBLIC PMC_ERROR eclass_vlan_cfg(eclass_handle_t *hndl, UINT32 link, eclass_vlan_cfg_t vlan_cfg_type );
PUBLIC PMC_ERROR eclass_vlan_custom_tpid_cfg(eclass_handle_t *hndl, UINT32 link, UINT32 custom_tpid_inner, UINT32 custom_tpid_outer );
PUBLIC PMC_ERROR eclass_filt20_cfg(eclass_handle_t *hndl, UINT32 link, UINT32 tag, UINT32 value, UINT32 mask, UINT32 addr, UINT32 inv, UINT32 type );
PUBLIC void eclass_var_default_init(eclass_handle_t *hndl);
PUBLIC void eclass_block_non_default_cfg(eclass_handle_t *hndl);

PUBLIC PMC_ERROR eclass_pkt_extract_cfg(eclass_handle_t *hndl, 
                                        UINT32 link, 
                                        BOOL8 enable,
                                        eclass_pkt_extract_cfg_t *ext_cfg_ptr);

PUBLIC BOOL8 eclass_pkt_extract_status_get(eclass_handle_t *hndl, 
                                           UINT32 link, 
                                           eclass_pkt_extract_cfg_t *extract_cfg_ptr);

PUBLIC void eclass_pkt_extract_cfg_get(eclass_handle_t *hndl, 
                                       UINT32 link, 
                                       UINT32 *array_size_ptr,
                                       UINT32 *array_used_size_ptr,
                                       eclass_pkt_extract_cfg_t *extract_cfg_ptr);

PUBLIC BOOL8 eclass_pkt_extract_all_cfg_disabled_get(eclass_handle_t *hndl);


PUBLIC BOOL8 eclass_pkt_extract_link_cfg_disabled_get(eclass_handle_t *hndl,
                                                      UINT32 link);

PUBLIC PMC_ERROR eclass_intr_enable_set(eclass_handle_t *hndl, BOOL enable);

PUBLIC void eclass_activated_link_get(eclass_handle_t   *hndl,
                                      BOOL8             *actv_link);

PUBLIC void eclass_link_pkt_extract_cleanup(eclass_handle_t            *hndl, 
                                            UINT32                      link);

#endif /* DOXYGEN_PUBLIC_ONLY */



#ifdef __cplusplus
}
#endif

#endif /* _ECLASS_H */

/*
** end of file
*/
