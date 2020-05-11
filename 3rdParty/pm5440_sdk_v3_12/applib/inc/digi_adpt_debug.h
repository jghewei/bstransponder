/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION:
*     The file describes the debug functionality of DIGI Adapter.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_DEBUG_H
#define _DIGI_ADPT_DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_util_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_sgmii_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SGMII port type.  The 
*   element terms have a syntax:
*      DIGI_PORT_<application>
*
* ELEMENTS:
*   DIGI_PORT_RI_RCP      - RI_RCP port
*   DIGI_PORT_DI_RCP      - DI_RCP port
*   DIGI_PORT_OHP1        - OHP1 port
*   DIGI_PORT_OHP2        - OHP2 port
*   DIGI_PORT_OHP3        - OHP3 port
*   DIGI_PORT_MGMT        - MGMT(management) port
*
*******************************************************************************/
typedef enum
{
    DIGI_PORT_RI_RCP  = 0x0,
    DIGI_PORT_DI_RCP  = 0x1,
    DIGI_PORT_OHP1    = 0x2,
    DIGI_PORT_OHP2    = 0x3,
    DIGI_PORT_OHP3    = 0x4,
    DIGI_PORT_MGMT    = 0x5,

    LAST_DIGI_PORT_TYPE,       /* this is for out of range values error handling */
} digi_sgmii_port_type_t;

/*
** Structures and Unions
*/
typedef struct 
{
    UINT32 offset;
    char   item_name[40];
}port_pmon_data_t;

/*
** Function Prototypes
*/
PUBLIC void adpt_help();
PUBLIC PMC_ERROR adpt_dump_all(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_basic(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_reg(UINT32 dev_id, UINT32 start_addr, UINT32 end_addr);
PUBLIC PMC_ERROR adpt_dump_data(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_odu_mux(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_cfg(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_tcm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_tti(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_alm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_alm_i(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_pmon(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_otn_int(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sdh_cfg(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sdh_alm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sdh_alm_i(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sdh_pmon(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sdh_int(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_gfp_cfg(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_gfp_alm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_gfp_alm_i(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_gfp_pmon(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_gfp_int(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_enet_cfg(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_enet_alm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_enet_alm_i(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_enet_pmon(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_enet_int(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_fc_alm(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_fc_alm_i(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_fc_pmon(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_fc_int(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_ptp(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_sifd_alm(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_sifd_alm_i(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_sifd_pmon(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_loopback(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_serdes(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_dump_serdes_st(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_sdk(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_fw(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_fw_log(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_pmon_cfg(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_pmon_data(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_int_tree(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_port_cfg(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_port_pmon(UINT32 dev_id, digi_sgmii_port_type_t port_type);
PUBLIC PMC_ERROR adpt_dump_ri_rcp_pmon(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_di_rcp_pmon(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_mgmt_alm_i(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_ohp_alm_i(UINT32 dev_id, odu_ohp_inst_t ohp_inst);
PUBLIC PMC_ERROR adpt_dump_di_rcp_alm_i(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_ri_rcp_alm_i(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_dump_dcsu(UINT32 dev_id);

/* SDK Prototypes */
PUBLIC void digi_debug_dsi_alloc_config_print(digi_handle_t* digi_handle, const char *config_name);
PUBLIC void digi_debug_enet_line_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_enet_sys_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ilkn1_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ilkn2_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_mapper_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ho_odu_struct_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_mo_odu_struct_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_lo_odu_struct_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_mapotn_chnl_print(digi_handle_t* digi_handle);
PUBLIC void digi_fw_show_debug_log(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_mapper_channel_num_get(digi_handle_t *digi_handle,
                                          digi_mapper_chnl_t *chnl_ctxt_pptr, 
                                          UINT32 *mapotn_chnl_ptr, 
                                          UINT32 *fo2_chnl_ptr, 
                                          UINT32 *enet_sys_link_ptr, 
                                          UINT32 *enet_line_link_ptr);
PUBLIC PMC_ERROR digi_enet_channel_num_get(digi_handle_t          *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        UINT32                    *chnl_num_ptr);
PUBLIC UINT32 digi_cbr_channel_num_get(digi_handle_t            *digi_handle,
                                       digi_cbr_client_chnl_t   *chnl_handle);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_DEBUG_H */

/* 
** end of file 
*/

