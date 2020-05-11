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
*      Defines ODU_RCP structures and function protypes used by upper 
*      layer block.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_RCP_H
#define _ODU_RCP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "odu_rcp_api.h"
#include "enet_fege.h"
#include "util_gen_db.h"
#include "cfc.h"
#include "cfc_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: odu_rcp_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ODU RCP ENET FEGE instance.
*
* ELEMENTS:
*   ODU_RCP_RI_INST    - RI RCP instance index.
*   ODU_RCP_DI _INST   - DI RCP instance index.
*
*******************************************************************************/
typedef enum
{
    ODU_RCP_RI_INST = 0,
    ODU_RCP_DI_INST = 1,
    LAST_ODU_RCP_INST
} odu_rcp_inst_t;

/*******************************************************************************
* ENUM: odu_rcp_inst_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ODU_RCP channel state definition.
*
* ELEMENTS:
*   ODU_RCP_STATE_START         - start state
*   ODU_RCP_STATE_EQUIPPED      - provisioned state
*   ODU_RCP_STATE_OPERATIONAL   - activated and operational state
*           
*******************************************************************************/
typedef enum
{
    ODU_RCP_STATE_START        = 0,
    ODU_RCP_STATE_EQUIPPED     = 1,
    ODU_RCP_STATE_OPERATIONAL  = 2,
    LAST_ODU_RCP_STATE
} odu_rcp_inst_state_t;


/*
** Constants
*/
/* Number of ODU_RCP FEGE instantiate in ODU RCP */
#define ODU_RCP_NUM_INST_MAX 2
/* Number of CFC instances */
#define ODU_RI_RCP_NUM_CFC_MAX 12
#define ODU_RI_RCP_NUM_FRAMER_MAX 6

/*
** Macro Definitions
*/
#define ODU_DI_RCP_INT_TABLE_DEFINE(tail)                               \
    ODU_DI_RCP_INT(odu_rcp_handle, di_rcp, DFEC_TX_DONE, dfec_tx_done_i); \
    ODU_DI_RCP_INT(odu_rcp_handle, di_rcp, PMON_TX_DONE, pmon_tx_done_i);   \

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: odu_rcp_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct odu_rcp_handle_struct_t odu_rcp_handle_t;

/*******************************************************************************
* STRUCTURE: odu_rcp_init_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef void odu_rcp_init_cfg_t;

/*******************************************************************************
* STRUCTURE: odu_di_rcp_d_rcohm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*     Structure defining RCOHM Defect status configuration.
*
* ELEMENTS:   
*   stg_31_00_drcohm_msk   - Channels mask for Stg 1, or N or 2 RCOHM defects 
*                            monitoring.\n
*                            For stage 1 and N, only channels 0 to 11 are valid.
*                            For stage 2, it is for channels 0 to 31.
*   stg_63_32_drcohm_msk   - Channels mask for Stg 2 RCOHM defects monitoring
*                            for channels 32 to 63.
*                            Only bit 0 to 63 can be set.
*   stg_95_64_d_rcohm_msk  - Channels mask for Stg 2 RCOHM defects monitoring
*                            for channels 95 to 64.
*******************************************************************************/
typedef struct odu_di_rcp_d_rcohm_cfg_struct_t
{
    UINT32                 stg_31_00_d_rcohm_msk;
    UINT32                 stg_63_32_d_rcohm_msk;
    UINT32                 stg_95_64_d_rcohm_msk;
}odu_di_rcp_d_rcohm_cfg_t;

/*******************************************************************************
* STRUCTURE: odu_rcp_di_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for DI RCP context 
*
* ELEMENTS:
*   state                 - DI RCP state
*   pdu_msk               - PDU mask
*   sdh_cnt_sel           - SONET counters select mask
*   drcohm_cfg            - RCOHM defects configuration
* 
*******************************************************************************/
typedef struct odu_rcp_di_ctxt_t
{
    odu_rcp_inst_state_t state;
    odu_di_rcp_cfg_t     cfg;
    UINT32               pdu_msk;
    UINT16               sdh_cnt_sel;
    odu_di_rcp_d_rcohm_cfg_t   drcohm_cfg[2];
}
odu_rcp_di_ctxt_t;

/*******************************************************************************
* STRUCTURE: odu_rcp_ri_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for RI RCP context 
*
* ELEMENTS:
*    state                - RI RCP state
*    extract_rx_enable    - indicate if extraction is enabled on a datapath
*                           for each framer * each channel 
* 
*******************************************************************************/
typedef struct odu_rcp_ri_ctxt_t
{
    odu_rcp_inst_state_t state;
    BOOL8 extract_rx_enable[ODU_RI_RCP_NUM_FRAMER_MAX][96];
    BOOL8 insert_backward_defect_enable[ODU_RI_RCP_NUM_FRAMER_MAX][96];
    BOOL8 insert_aps_enable[ODU_RI_RCP_NUM_FRAMER_MAX][96];

    UINT32 db_handle_rx;
    UINT32 db_handle_tx;
    BOOL8 cal_enable[ODU_RI_RCP_NUM_FRAMER_MAX];
    BOOL8 cal_select[ODU_RI_RCP_NUM_FRAMER_MAX];
}
odu_rcp_ri_ctxt_t;

/******************************************************************************
* STRUCTURE: odu_rcp_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RCP configuration
*
* ELEMENTS:
*   See Typedef
* 
*******************************************************************************/
typedef struct 
{
    digi_otn_odu_ri_rcp_cfg_t ri_rcp_port_cfg;
    UINT32*   db_dev_addr;
} odu_rcp_cfg_t;

/******************************************************************************
* STRUCTURE: odu_rcp_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dynamic context
*
* ELEMENTS:
*   See Typedef
* 
*******************************************************************************/
typedef struct
{
    odu_rcp_ri_ctxt_t ri_rcp_ctxt;
    odu_rcp_di_ctxt_t di_rcp_ctxt;      
} odu_rcp_var_t;


/******************************************************************************
* STRUCTURE: odu_rcp_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RCP handle 
*
* ELEMENTS:
*   See Typedef
* 
*******************************************************************************/
typedef struct odu_rcp_handle_struct_t
{
    pmc_handle_t base;
    
    odu_rcp_cfg_t cfg;
    odu_rcp_var_t var;

    cfc_handle_t    **cfc_handle;
    enet_fege_handle_t **enet_fege_handle; 
} odu_rcp_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC odu_rcp_handle_t *odu_rcp_ctxt_create(pmc_handle_t *parent, 
                                             UINT32 base_address, 
                                             pmc_sys_handle_t *sys_handle, 
                                             const char *tsb_name);

PUBLIC void odu_rcp_ctxt_destroy(odu_rcp_handle_t *odu_rcp_handle);

PUBLIC void odu_rcp_handle_init(odu_rcp_handle_t *odu_rcp_handle);

PUBLIC void odu_rcp_alloc_init(odu_rcp_handle_t *odu_rcp_handle);

PUBLIC void odu_rcp_handle_restart_init(odu_rcp_handle_t *odu_rcp_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                        pmc_energy_state_t top_energy_state);
    
PUBLIC UINT32 odu_rcp_base_address_get(odu_rcp_handle_t *odu_rcp_handle);

PUBLIC PMC_ERROR odu_rcp_init(odu_rcp_handle_t   *hndl,
                              odu_rcp_inst_t      odu_rcp_id,
                              odu_rcp_init_cfg_t *rcp_cfg_ptr);

PUBLIC PMC_ERROR odu_rcp_uninit(odu_rcp_handle_t *hndl,
                                odu_rcp_inst_t odu_rcp_id);

PUBLIC PMC_ERROR odu_rcp_init_get(odu_rcp_handle_t   *hndl,
                                  odu_rcp_inst_t      odu_rcp_id,
                                  odu_rcp_init_cfg_t *rcp_cfg_ptr,
                                  BOOL               *enable);
           
PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_cfg(odu_rcp_handle_t *hndl,         
                                            odu_rcp_frm_id_t rcp_frm_id,
                                            UINT32 chnl,
                                            digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                            UINT8 aps_pcc_mon_level,
                                            UINT32 *ch_entries,
                                            BOOL8 enable);
PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_cfg_get(odu_rcp_handle_t *hndl,         
                                                odu_rcp_frm_id_t rcp_frm_id,
                                                UINT32 chnl,
                                                digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                UINT8 *aps_pcc_mon_level,
                                                BOOL8 *enable);

PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_resize(odu_rcp_handle_t *hndl,         
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 *ch_entries);
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_pcc_mon_level_cfg(odu_rcp_handle_t *hndl,       
                                                      odu_rcp_frm_id_t rcp_frm_id,
                                                      UINT32 chnl,
                                                      UINT8 aps_pcc_mon_level);

PUBLIC PMC_ERROR otn_odu_ri_rcp_dest_get(odu_rcp_handle_t *hndl,       
                                         odu_rcp_frm_id_t rcp_frm_id,
                                         UINT32 chnl,
                                         digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr);

PUBLIC PMC_ERROR otn_odu_ri_rcp_bdi_bei_biae_insert_cfg(odu_rcp_handle_t *hndl,       
                                                        odu_rcp_frm_id_t rcp_frm_id,
                                                        UINT32 chnl,
                                                        UINT32 num_cal_entries,
                                                        BOOL8 enable);
PUBLIC void otn_odu_ri_rcp_bdi_bei_biae_insert_get(odu_rcp_handle_t *hndl,       
                                                        odu_rcp_frm_id_t rcp_frm_id,
                                                        UINT32 chnl,
                                                        BOOL8 *source);
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_insert_cfg(odu_rcp_handle_t *hndl,    
                                               odu_rcp_frm_id_t rcp_frm_id,
                                               UINT32 chnl,   
                                               UINT32 num_cal_entries, 
                                               BOOL8 source);
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_insert_en_get( odu_rcp_handle_t *hndl,    
                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                   UINT32 chnl,
                                                   BOOL8 *source);

PUBLIC PMC_ERROR odu_di_rcp_cfg(odu_rcp_handle_t *hndl, 
                                UINT32            pdu_msk,
                                BOOL8             enable);

PUBLIC PMC_ERROR odu_rcp_di_drcohm_cfg(odu_rcp_handle_t           *hndl, 
                                       odu_rcp_frm_id_t            stg,
                                       UINT32                      chnl_id,
                                       BOOL8                       status);

PUBLIC void odu_rcp_di_pdu_msk_get(odu_rcp_handle_t           *hndl,
                                   UINT32                     *pdu_msk_ptr,
                                   UINT16                     *sdh_cnt_sel_ptr);

PUBLIC void odu_rcp_di_sdh_cnt_sel_set(odu_rcp_handle_t         *hndl,
                                       UINT16                    sdh_cnt_sel);
PUBLIC BOOL odu_rcp_start_state_test(odu_rcp_handle_t   *hndl,
                                     odu_rcp_inst_t      odu_rcp_id);
PUBLIC void odu_rcp_chnl_clean(odu_rcp_handle_t   *hndl,
                               BOOL8 is_rx_frm,                               
                               odu_rcp_frm_id_t rcp_frm_id,
                               UINT32 *ch_entries,
                               UINT32 *chnl,
                               UINT32 num_chnl);

PUBLIC PMC_ERROR odu_rcp_int_enable(odu_rcp_handle_t    *odu_rcp_handle,
                                    odu_rcp_inst_t       rcp_inst,
                                    odu_rcp_int_t       *int_table_ptr,
                                    odu_rcp_int_t       *int_en_table_ptr,
                                    BOOL                 enable);

PUBLIC PMC_ERROR odu_rcp_int_retrieve(odu_rcp_handle_t    *odu_rcp_handle,
                                      odu_rcp_inst_t       rcp_inst,
                                      odu_rcp_int_t       *filt_table_ptr,
                                      odu_rcp_int_t       *int_table_ptr);


PUBLIC PMC_ERROR odu_rcp_int_clear(odu_rcp_handle_t    *odu_rcp_handle,
                                   odu_rcp_inst_t       rcp_inst,
                                   odu_rcp_int_t       *int_table_ptr,
                                   odu_rcp_int_t       *int_en_table_ptr);

PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_enable(odu_rcp_handle_t       *ri_rcp_handle,
                                            UINT32                  chnl_id,
                                            odu_ri_rcp_int_chnl_t  *int_table_ptr,
                                            odu_ri_rcp_int_chnl_t  *int_en_table_ptr,
                                            BOOL                    enable);

PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_retrieve(odu_rcp_handle_t        *ri_rcp_handle,
                                              UINT32                   chnl_id,
                                              odu_ri_rcp_int_chnl_t   *filt_table_ptr,
                                              odu_ri_rcp_int_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_clear(odu_rcp_handle_t      *ri_rcp_handle,
                                           UINT32                 chnl_id,
                                           odu_ri_rcp_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_enabled_check(odu_rcp_handle_t       *ri_rcp_handle,
                                                   UINT32                  chnl_id,
                                                   odu_ri_rcp_int_chnl_t  *int_en_table_ptr,
                                                   BOOL8                  *int_found_ptr);

PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_sel_set(odu_rcp_handle_t        *ri_rcp_handle,
                                            odu_rcp_frm_id_t         framer_id,
                                            util_global_direction_t  src_dir);
    
PUBLIC void odu_ri_rcp_cfc_int_sel_get(odu_rcp_handle_t        *ri_rcp_handle,
                                       odu_rcp_frm_id_t        *framer_id_ptr,
                                       util_global_direction_t *src_dir_ptr);

PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_type_sel_set(odu_rcp_handle_t          *ri_rcp_handle,
                                                 UINT32                     chnl,
                                                 odu_ri_rcp_cfc_int_type_t  int_type);

PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_type_sel_get(odu_rcp_handle_t          *ri_rcp_handle,
                                                 UINT32                     chnl,
                                                 odu_ri_rcp_cfc_int_type_t *int_type_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ODU_RCP_H */

/*
** end of file
*/
