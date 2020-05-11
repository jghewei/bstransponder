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

#ifndef _DIGI_ENET_LOC_H
#define _DIGI_ENET_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/


#include "enet.h"
#include "etime.h"
#include "eclass.h"
#include "gsup43_c73.h"
#include "exco3m.h"
#include "exco4m.h"

/*
** Enumerated Types
*/


/*
** Constants
*/

#define DIGI_ENET_CHAN_0_PAUSED         0x1

/* GSUP43 7.3 data and ordered set UPI values */
#define DIGI_ENET_GSUP43_C73_PKT_TYPE  0x13
#define DIGI_ENET_GSUP43_C73_OS_TYPE   0x14

/* GSUP43 7.3 legacy data and ordered set UPI values */
#define DIGI_ENET_GSUP43_C73_LEGACY_PKT_TYPE  0xfd
#define DIGI_ENET_GSUP43_C73_LEGACY_OS_TYPE   0xfe

#define DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID          24
#define DIGI_MGMT_FEGE_TO_CPU_EGR_DPI_ID          3
#define DIGI_CPU_TO_MGMT_FEGE_ING_DPI_ID          24
#define DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID          3

#define DIGI_ENET_MCPB_10GE_LARGE_FIFO_NUM_BLOCKS  30
#define DIGI_ENET_MCPB_10GE_NORMAL_FIFO_NUM_BLOCKS 12

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* ENUM: digi_enet_pmon_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET PMON. 
*
* ELEMENTS:
*   DIGI_NO_PON         - PMON not required.
*   DIGI_ENET_LINE_PMON - PMON is using ENET_LINE side resources.
*   DIGI_ENET_SYS_PMON  - PMON is using ENET_SYS side resources.
*                           
*
*******************************************************************************/
typedef enum
{
    DIGI_NO_PMON = 0,
    DIGI_ENET_LINE_PMON,
    DIGI_ENET_SYS_PMON,

    LAST_DIGI_ENET_PMON    /* this is for out of range values error handling */
} digi_enet_pmon_type_t;

/*******************************************************************************
* ENUM: digi_enet_res_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET resource usage.
*
* ELEMENTS:
    DIGI_ENET_RES_UNUSED  - ENET resource is currently unused.
*   DIGI_ENET_RES_SERDES  - ENET resource is being used as either LINE or SYSTEM
*                           SERDES
*   DIGI_ENET_RES_PMON    - ENET resource is being used as a PMON resource.
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_RES_UNUSED = 0,
    DIGI_ENET_RES_SERDES,
    DIGI_ENET_RES_PMON,

    LAST_DIGI_ENET_RES     /* this is for out of range values error handling */
} digi_enet_res_type_t;

/*******************************************************************************
* STRUCTURE: digi_enet_client_chnl_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common enet output parameters
*
* ELEMENTS:
* 
*       switch_data     - a util_global_switch_data_def_t structure.
*       serdes_mode     - Configuration mode for serdes port
*       mode            - Configuration mode for enet
*       dsi_base_lane   - post Mux DSI base lane transferring data between
*                         Line Serdes and ENET_LINE subsystem
*
*******************************************************************************/
typedef struct digi_enet_client_chnl_def_t
{
    util_global_switch_data_def_t switch_data;
    digi_serdes_port_mode_t serdes_mode;
    digi_enet_mode_t mode;
    digi_enet_tx_timing_mode_t timing_mode;
    UINT32 dsi_base_lane;
} digi_enet_client_chnl_def_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC PMC_ERROR digi_enet_channel_num_get(digi_handle_t            *digi_handle,
                                           digi_enet_client_chnl_t  *chnl_handle,
                                           UINT32                   *chnl_num_ptr);

PUBLIC PMC_ERROR digi_enet_sys_pmon_map_validate_and_mode_set(digi_handle_t *digi_handle,
                                                              digi_mapping_mode_t map_mode, 
                                                              digi_oduk_t odu_data,
                                                              digi_enet_mode_t enet_line_mode,
                                                              enet_datapath_t* enet_mode, 
                                                              digi_enet_mode_t* digi_enet_mode);
    
PUBLIC PMC_ERROR digi_enet_pmon_ilkn_validate_and_mode_set(digi_handle_t *digi_handle,
                                                           UINT32 chnl_link, 
                                                           digi_enet_pmon_type_t enet_pmon,
                                                           enet_datapath_t* enet_mode, 
                                                           digi_enet_mode_t* digi_enet_mode);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_demap(digi_handle_t* digi_handle,
                                               util_global_switch_data_t* input_pc_ptr);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_activate(digi_handle_t* digi_handle,
                                                  util_global_switch_data_t* input_pc_ptr);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_line_local_activate(digi_handle_t* digi_handle,
                                                             util_global_switch_data_def_t* in_util_ptr);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_deactivate(digi_handle_t* digi_handle,
                                                    util_global_switch_data_t* output_pc_ptr);

PMC_ERROR digi_enet_pmon_is_available_and_case_determine(digi_handle_t* digi_handle,
                                                                digi_mapping_mode_t mode,
                                                                BOOL8 enet_pmon,
                                                                mapotn_src_dest_t mapotn_dest,
                                                                BOOL8* enet_line_as_pmon,
                                                                digi_enet_pmon_type_t* enet_pmon_type_ptr,
                                                                util_global_cpb_port_t* cpb_port_type_ptr);

PUBLIC digi_enet_client_chnl_def_t* digi_enet_enhanced_pmon_ptr_get(util_global_switch_data_def_t* util_sw_ptr);

PUBLIC digi_enet_client_chnl_def_t* digi_enet_client_chnl_get(digi_handle_t* digi_handle,
                                                              digi_serdes_port_mode_t serdes_mode,
                                                              digi_enet_mode_t mode,
                                                              UINT32 enet_link, 
                                                              util_global_cpb_port_t port_type,
                                                              digi_enet_res_type_t res_type);

PUBLIC BOOL8 digi_enet_does_state_match(digi_handle_t* digi_handle,
                                        UINT32 enet_chnl,
                                        BOOL8 rx_input,
                                        util_global_cpb_port_t port_type,
                                        util_global_chnl_state_t state);

PUBLIC PMC_ERROR digi_enet_irig_configuration_internal_get(digi_handle_t *digi_handle,
                                                           util_global_switch_data_t *chnl_handle,
                                                           BOOL8 *is_ingress_enable,
                                                           digi_enet_irig_mode_t* ingress_mode,
                                                           BOOL8 *is_egress_enable,
                                                           digi_enet_irig_mode_t* egress_mode);

PUBLIC PMC_ERROR digi_enet_insert_crc_internal_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL8 *is_enabled);

PUBLIC PMC_ERROR digi_enet_crc_forwarding_internal_get(digi_handle_t *digi_handle,
                                                       util_global_switch_data_t *chnl_handle,
                                                       BOOL8 *is_enabled);

PUBLIC PMC_ERROR digi_enet_phy_internal_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 *is_enabled);


PUBLIC PMC_ERROR digi_enet_is_link_mac_block_without_transparent_pmon(digi_handle_t *digi_handle,
                                                                      util_global_switch_data_t *chnl_handle,
                                                                      BOOL *is_mac_block);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper(digi_handle_t *digi_handle);  

PUBLIC BOOL8 digi_enet_is_multi_lane_serdes_get(digi_handle_t               *digi_handle,
                                                digi_enet_client_chnl_def_t *enet_chnl_ptr);

PUBLIC PMC_ERROR digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle_t         *digi_handle,
                                                                 digi_int_type_t        int_type, 
                                                                 BOOL8                  has_epmm_rx_lobl_en,
                                                                 BOOL8                  has_etrans_rx_lobl_en,
                                                                 enet_int_chnl_t       *filt_table_ptr,
                                                                 enet_int_chnl_t       *int_table_ptr);


PUBLIC PMC_ERROR digi_enet_fw_enet_client_source_conditional_update(digi_handle_t              *digi_handle,
                                                                    util_global_switch_data_t  *chnl_handle);

PUBLIC void digi_enet_update_dsi_base_lane(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_enet_lf_force_set(digi_handle_t *digi_handle,
                                        digi_enet_client_chnl_def_t *enet_chnl_ptr,
                                        BOOL8 fw_activated,
                                        BOOL8 fw_enable);

/* 
** Deprecated Functions 
*/

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR digi_enet_egress_insert_rate_internal_get(digi_handle_t *digi_handle,
                                                           util_global_switch_data_t *chnl_handle,
                                                           UINT32 *insert_quantum, 
                                                           UINT32 *system_quantum);

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */
 
#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ENET_LOC_H */

/* 
** end of file 
*/

