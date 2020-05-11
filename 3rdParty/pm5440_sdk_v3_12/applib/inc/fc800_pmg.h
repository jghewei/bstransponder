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

#ifndef _FC800_PMG_H
#define _FC800_PMG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "fc800_pmg_api.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define FC800_PMG_RESET                          0x1
#define FC800_PMG_ACTIVE                         0x0

#define FC800_PMG_ENABLE                         0x1
#define FC800_PMG_DISABLE                        0x0

#define FC800_PMG_MAX_INDEX                      12

#define FC800_PMG_FC_CBR_MODE_FC                 0x0
#define FC800_PMG_FC_CBR_MODE_ESCON              0x1
#define FC800_PMG_FC_CBR_BUS_WIDTH               0x5
#define FC800_PMG_FC_CBR_CONT_ALIGN_MODE         0x0
   #define FC800_PMG_FC_CBR_DLOLB_INT_PER        0x7
#define FC800_PMG_FC_CBR_LCV_DLOLB_THRSH         0xFFFF
#define FC800_PMG_FC_CBR_XDET_INTERVAL           0xFF
#define FC800_PMG_FC_CBR_XDET_DEASSERT_THRSH     0xFF
   #define FC800_PMG_FC_CBR_CLK_DIV_FC800        0x32A9F9
   #define FC800_PMG_FC_CBR_CLK_DIV_CPRI_5G      0x1D4C00
   #define FC800_PMG_FC_CBR_CLK_DIV_CPRI_6G      0x249F00 
   #define FC800_PMG_FC_CBR_CLK_DIV_CPRI_10G     0x3A9800
   #define FC800_PMG_FC_CBR_CLK_DIV_GDPS_5G      0x1DCD65
   #define FC800_PMG_FC_CBR_CLK_DIV_GDPS_10G     0x3B9ACA


/*
** Macro Definitions
*/

/* FC800_PMG_INT(handle, tsb_name, tsb_field, struct_field) */
#define FC800_PMG_INT_TABLE_DEFINE() \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_DISP_INVERT, rx_disp_invert_i); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_XDET       , rx_xdet_i       ); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_SOF_DET    , rx_sof_det_i    ); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_DLOLB      , rx_dlolb_i      ); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_ASD        , rx_asd_i        ); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_SYNC       , rx_sync_i       ); \
    FC800_PMG_INT(fc800_pmg_handle, fc_cbr, RX_LCV        , rx_lcv_i        ); \

#define FC800_PMG_STATUS_TABLE_DEFINE() \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_DISP_INVERT_V, rx_disp_invert_v); \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_XDET_V       , rx_xdet_v       ); \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_DLOLB_V      , rx_dlolb_v      ); \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_ASD_V        , rx_asd_v        ); \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_SYNC_V       , rx_sync_v       ); \
    FC800_PMG_STATUS(fc800_pmg_handle, fc_cbr, RX_LCV_V        , rx_lcv_v        ); \




/*
** Structures and Unions
*/


#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* ENUM: fc800_pmg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the client traffic modes that can be provisioned
*   through the fc800_pmg.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    FC800_PMG_MODE_NONE = 0,

    FC800_PMG_MODE_40G_STS768,
    FC800_PMG_MODE_40G_STM256,
   
    FC800_PMG_MODE_10G_STS192,
    FC800_PMG_MODE_10G_STM64,
    FC800_PMG_MODE_10G_FC1200_ODU1F,
    FC800_PMG_MODE_10G_FC1200_ODU2F,
    FC800_PMG_MODE_10G_FC1200_GFP_ODU2E,
    FC800_PMG_MODE_10G_FC800,
    FC800_PMG_MODE_10G_CPRI_5G,
    FC800_PMG_MODE_10G_CPRI_6G,
    FC800_PMG_MODE_10G_CPRI_10G,
    FC800_PMG_MODE_10G_GDPS_5G,
    FC800_PMG_MODE_10G_GDPS_10G,
    FC800_PMG_MODE_10G_STS192_PRBS,
    FC800_PMG_MODE_10G_STM64_PRBS,
    FC800_PMG_MODE_10G_8B_10B_IDLE,
    FC800_PMG_MODE_10G_64B_66B_IDLE,
    FC800_PMG_MODE_10G_8B_10B_ARB,    
    LAST_FC800_PMG_MODE,
} fc800_pmg_mode_t;
#endif /* DOXYGEN_PUBLIC_ONLY */




/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC fc800_pmg_handle_t *fc800_pmg_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void fc800_pmg_ctxt_destroy(fc800_pmg_handle_t *fc800_pmg_handle);
PUBLIC void fc800_pmg_handle_init(fc800_pmg_handle_t *fc800_pmg_handle);
PUBLIC BOOL fc800_pmg_start_state_test(fc800_pmg_handle_t *fc800_pmg_handle);
PUBLIC PMC_ERROR fc800_pmg_init(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                pmc_energy_state_t     energy_state);
PUBLIC PMC_ERROR fc800_pmg_mode_cfg(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                    UINT32                 index,
                                    fc800_pmg_mode_t       mode,
                                    BOOL8                  is_ilvr);
PUBLIC PMC_ERROR fc800_pmg_reset(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                 UINT32                 index,
                                 BOOL                   reset);
PUBLIC PMC_ERROR fc800_pmg_user_pattern_set(fc800_pmg_handle_t    *fc800_pmg_handle,
                                            UINT32                 index,
                                            UINT32                 user_pattern);
PUBLIC PMC_ERROR fc800_pmg_user_pattern_get(fc800_pmg_handle_t    *fc800_pmg_handle,
                                            UINT32                 index,
                                            UINT32                *user_pattern_ptr);
PUBLIC PMC_ERROR fc800_pmg_pattern_ins_set(fc800_pmg_handle_t           *fc800_pmg_handle,
                                           UINT32                        index,
                                           fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg);
PUBLIC PMC_ERROR fc800_pmg_pattern_ins_get(fc800_pmg_handle_t           *fc800_pmg_handle,
                                           UINT32                        index,
                                           fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr);
PUBLIC PMC_ERROR fc800_pmg_pattern_force(fc800_pmg_handle_t    *fc800_pmg_handle,
                                         UINT32                 index,
                                         BOOL                   force);
PUBLIC PMC_ERROR fc800_pmg_pattern_force_get(fc800_pmg_handle_t    *fc800_pmg_handle,
                                             UINT32                 index,
                                             BOOL                  *force_ptr);
PUBLIC PMC_ERROR fc800_pmg_dlolb_gen_set(fc800_pmg_handle_t           *fc800_pmg_handle,
                                         UINT32                        index,
                                         fc800_pmg_dlolb_gen_t        *dlolb_gen_cfg_ptr);
PUBLIC PMC_ERROR fc800_pmg_dlolb_gen_get(fc800_pmg_handle_t           *fc800_pmg_handle,
                                         UINT32                        index,
                                         fc800_pmg_dlolb_gen_t        *dlolb_gen_cfg_ptr);
PUBLIC PMC_ERROR fc800_pmg_int_chnl_enable(fc800_pmg_handle_t    *fc800_pmg_handle,
                                           UINT32                 index,
                                           fc800_pmg_int_chnl_t  *int_table_ptr,
                                           fc800_pmg_int_chnl_t  *int_en_table_ptr,
                                           BOOL                   enable);
PUBLIC PMC_ERROR fc800_pmg_int_chnl_clear(fc800_pmg_handle_t    *fc800_pmg_handle,
                                          UINT32                 index,
                                          fc800_pmg_int_chnl_t  *int_table_ptr,
                                          fc800_pmg_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR fc800_pmg_int_chnl_retrieve(fc800_pmg_handle_t    *fc800_pmg_handle,
                                             UINT32                 index,
                                             fc800_pmg_int_chnl_t  *filt_table_ptr,
                                             fc800_pmg_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR fc800_pmg_int_chnl_enabled_check(fc800_pmg_handle_t     *fc800_pmg_handle,
                                                  UINT32                 index,
                                                  fc800_pmg_int_chnl_t   *int_en_table_ptr,
                                                  BOOL                   *int_found_ptr);

PUBLIC void fc800_pmg_mode_get(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                               UINT32                 index,
                               fc800_pmg_mode_t      *mode_ptr);
    
PUBLIC BOOL8 fc800_pmg_status_get(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                  UINT32                 index);

#ifdef __cplusplus
}
#endif

#endif /* _FC800_PMG_H */

/*
** end of file
*/
