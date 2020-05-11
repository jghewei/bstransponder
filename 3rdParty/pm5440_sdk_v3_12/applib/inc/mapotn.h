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

#ifndef _MAPOTN_H
#define _MAPOTN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "mapotn_api.h"
#include "util_global.h"
#include "ohfs_remove.h"
#include "ohfs_insert.h"
#include "cfc.h"
#include "mpma.h"
#include "mpmo.h"
#include "rxjc_proc.h"
#include "mp_mgen.h"
#include "cpact_obuf.h"


#define MAPOTN_NUM_CHANL                        96 


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: mapotn_src_dest_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify MAPOTN source/destionation of data stream.
*
* ELEMENTS:
*   MAPOTN_DEST_CPB           - CPB subsystem
*   MAPOTN_DEST_ENET          - ENET subsystem
*   MAPOTN_DEST_CPB_ENET      - both CPB and ENET subsystem
*   LAST_MAPOTN_DEST          - for out of range values error handling
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_DEST_CPB  = 0,    
    MAPOTN_DEST_ENET, 
    MAPOTN_DEST_CPB_ENET,    
    
    LAST_MAPOTN_DEST /* this is for out of range values error handling  */     
} mapotn_src_dest_t; 


/*******************************************************************************
* ENUM: mapotn_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify MAPOTN channel state definition.
*
* ELEMENTS:
*   MAPOTN_CHNL_START         - start state
*   MAPOTN_CHNL_EQUIPPED      - provisioned state
*   MAPOTN_CHNL_OPERATIONAL   - activated and operational state
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_CHNL_START        = 0,
    MAPOTN_CHNL_EQUIPPED     = 1,
    MAPOTN_CHNL_OPERATIONAL  = 2,
} mapotn_chnl_state_t;


/*******************************************************************************
* ENUM: mapotn_path_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify MAPOTN RX or TX path.
*
* ELEMENTS:
*   MAPOTN_RX                 - RX path
*   MAPOTN_TX                 - TX path

*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_RX  = 0,
    MAPOTN_TX  = 1,
} mapotn_path_t;


/* RGFPF */

/*******************************************************************************
* ENUM: mapotn_rgfpf_frame_field_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify possible mask values for frame field mask.
*
* ELEMENTS:
*   MAPOTN_FLD_MASK_PTI       - Field PTI mask
*   MAPOTN_FLD_MASK_UPI       - Field UPI mask
*   MAPOTN_FLD_MASK_EXI       - Field EXI mask
*   MAPOTN_FLD_MASK_PFI       - Field PFI mask
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_FLD_MASK_PTI = 1,
    MAPOTN_FLD_MASK_UPI = 2,     
    MAPOTN_FLD_MASK_EXI = 4,
    MAPOTN_FLD_MASK_PFI = 8,
} mapotn_rgfpf_frame_field_mask_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_match_ind_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify match indicator masks for RGFPF filter.
*
* ELEMENTS:
*   MAPOTN_USE_EXI_SET        - EXI set
*   MAPOTN_USE_PTI_SET        - PTI set
*   MAPOTN_USE_UPI_SET        - UPI set
*   MAPOTN_USE_ECSF           - ESCF
*
*******************************************************************************/
typedef enum {
    MAPOTN_USE_EXI_SET = 0x8,
    MAPOTN_USE_PTI_SET = 0x4,
    MAPOTN_USE_UPI_SET = 0x2,
    MAPOTN_USE_ECSF    = 0x1,
} mapotn_rgfpf_match_ind_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_pti_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify GFP PTI frame types
*
* ELEMENTS:
*   MAPOTN_GFP_CLIENT         - GFP client frame
*   MAPOTN_GFP_CMF            - GFP CMF frame
*   MAPOTN_GFP_MCF            - GFP MCF frame
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_GFP_CLIENT = 0,
    MAPOTN_GFP_CMF    = 4,
    MAPOTN_GFP_MCF    = 5,
} mapotn_rgfpf_pti_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_filter_comp_ops_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify RGFPF filter comparison operations.
*
* ELEMENTS:
*   MAPOTN_GEN_EQ             - Generic filter comparison equal to
*   MAPOTN_GEN_GT             - Generic filter comparison greater than
*   MAPOTN_GEN_LT             - Generic filter comparison less than
*   MAPOTN_GEN_NQ             - Generic filter comparison not equal to
*   MAPOTN_EXI_EQ             - EXI comparison equal to
*   MAPOTN_EXI_GT             - EXI comparison greater than
*   MAPOTN_EXI_LT             - EXI comparison less than
*   MAPOTN_EXI_NQ             - EXI comparison not equal to
*   MAPOTN_PTI_EQ             - PTI comparison equal to     
*   MAPOTN_PTI_GT             - PTI comparison greater than 
*   MAPOTN_PTI_LT             - PTI comparison less than    
*   MAPOTN_PTI_NQ             - PTI comparison not equal to 
*   MAPOTN_UPI_EQ             - UPI comparison equal to    
*   MAPOTN_UPI_GT             - UPI comparison greater than
*   MAPOTN_UPI_LT             - UPI comparison less than   
*   MAPOTN_UPI_NQ             - UPI comparison not equal to         
*
*******************************************************************************/
/* filter operations */
typedef enum {
    MAPOTN_GEN_EQ= 0x00, MAPOTN_GEN_GT=0x01,      MAPOTN_GEN_LT=0x02,   MAPOTN_GEN_NQ=0x03,
    MAPOTN_UPI_EQ= 0x00, MAPOTN_UPI_GT=0x01,      MAPOTN_UPI_LT=0x02,   MAPOTN_UPI_NQ=0x03,
    MAPOTN_PTI_EQ= 0x00, MAPOTN_PTI_GT=0x01 << 2, MAPOTN_PTI_LT=0x02<<2,MAPOTN_PTI_NQ=0x03<<2,
    MAPOTN_EXI_EQ= 0x00, MAPOTN_EXI_GT=0x01 << 4, MAPOTN_EXI_LT=0x02<<4,MAPOTN_EXI_NQ=0x03<<4,
} mapotn_rgfpf_filter_comp_ops_t;


/*******************************************************************************
* ENUM: mapotn_filter_action_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify actions that can be taken towards RGFPF filter.
*
* ELEMENTS:
*   MAPOTN_SEND_TO_CPU                  - send frame to CPU
*   MAPOTN_SEND_TO_EGRESS               - send frame to egress
*   MAPOTN_DROP_THE_FRAME               - drop the frame
*   MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF - store the UPI CMF frame and then drop it
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_SEND_TO_CPU                                    = 0,
    MAPOTN_SEND_TO_EGRESS                                 = 1,
    MAPOTN_DROP_THE_FRAME                                 = 2,
    MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF                   = 3,  
} mapotn_filter_action_type_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_cpu_fifo_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify the cpu fifo status
*
* ELEMENTS:
*   MAPOTN_CPU_FIFO_EMPTY               - No data available 
*   MAPOTN_CPU_FIFO_FRAME_AVL           - Received a complete frame
*   MAPOTN_CPU_FIFO_ERROR               - Error status bit set (overflow)
*   MAPOTN_CPU_FIFO_MISSING_EOF         - Missing EOF 
*   MAPOTN_CPU_FIFO_MISSING_SOF         - Missing SOF 
*   MAPOTN_CPU_FIFO_SOF_WITH_NO_DATA    - Received a SOF with num bypes = 0 (Shouldn't happen)
*   MAPOTN_CPU_FIFO_READ_ERROR          - Frame length > maximum buffer size (shouldn't happen)
*
*******************************************************************************/
typedef enum {
    MAPOTN_CPU_FIFO_EMPTY                                    = 0,
    MAPOTN_CPU_FIFO_FRAME_AVL                                = 0x01,
    MAPOTN_CPU_FIFO_ERROR                                    = 0x04,
    MAPOTN_CPU_FIFO_MISSING_EOF                              = 0x08,
    MAPOTN_CPU_FIFO_MISSING_SOF                              = 0x10,
    MAPOTN_CPU_FIFO_SOF_WITH_NO_DATA                         = 0x20,
    MAPOTN_CPU_FIFO_READ_ERROR                               = 0x40,
} mapotn_rgfpf_cpu_fifo_status_t;


/*******************************************************************************
* ENUM: mapotn_db_method_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify the MAPOTN database method access.
*
* ELEMENTS:
*           MAPOTN_DB_MODE_LOCAL      - Generate data to the database locally
*           MAPOTN_DB_MODE_EXTERNAL   - Generate data to the database externally
*           LAST_MAPOTN_DB_MODE       - for out of range values error handling
*
*
*******************************************************************************/
typedef enum
{
    MAPOTN_DB_MODE_LOCAL      = 0,
    MAPOTN_DB_MODE_EXTERNAL   = 1,
    LAST_MAPOTN_DB_MODE /* this is for out of range values error handling  */
} mapotn_db_method_t;

/*
** Constants
*/



/*
** Macro Definitions
*/
/* MAPOTN_INT(handle, tsb_name, tsb_field, struct_field) */
#define MAPOTN_INT_TABLE_DEFINE()                                       \
    MAPOTN_INT(mapotn_handle, mapotn_rx,    FIFO_FULL                , dci_dpi_async_fifo_full_i); \
    MAPOTN_INT(mapotn_handle, mapotn_tx,    ENET_FIFO_OVFL           , enet_fifo_ovfl_i         ); \
    MAPOTN_INT(mapotn_handle, mapotn_rgfpf, UNPACK_AFIFO_OVF         , unpack_afifo_ovf_i       ); \
    MAPOTN_INT(mapotn_handle, mapotn_rgfpf, CPU_RDRPT_AFIFO_OVF      , cpu_rdrpt_afifo_ovf_i    ); \
    MAPOTN_INT(mapotn_handle, mapotn_rgfpf, CPU_WRRPT_AFIFO_OVF         , cpu_wrrpt_afifo_ovf_i    ); \
    
/* MAPOTN_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
/* MAPOTN_INT_CHNL_ARRAY(handle, tsb_name, tsb_field, start_i, end_i, struct_field) */
#define MAPOTN_INT_CHNL_TABLE_DEFINE()                                                     \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, LOFD                , lofd_i                 ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, IDLE                , idle_i                 ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CTRLFRM             , ctrlfrm_i              ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, UNPACK_FIFO_DROP    , unpack_fifo_drop_i     ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, UCHEC               , uchec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CCHEC               , cchec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, UTHEC               , uthec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CTHEC               , cthec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, UEHEC               , uehec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CEHEC               , cehec_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, EXI                 , exi_i                  ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, PTI_UPI_MSMCH       , pti_upi_msmch_i        ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, GENERIC             , generic_i              ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CSF                 , csf_i                  ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, DROP                , drop_i                 ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, DATAFRM             , datafrm_i              ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, PAUSE               , pause_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, MAX_FRM_LEN         , max_frm_len_i          ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, MIN_FRM_LEN         , min_frm_len_i          ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, PFCSE               , pfcse_i                ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, DWNSTRM_FRM         , dwnstrm_frm_i          ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, HAO_STATUS_AVL      , hao_status_avl_i       ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, LEN_ERR_INT         , len_err_int_i          ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, PAUSE_FRAME_TX_INT  , pause_frame_tx_int_i   ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, MGMT_FRAME_TX_INT   , mgmt_frame_tx_int_i    ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, OVERSIZE_FRM_RX_INT , oversize_frm_rx_int_i  ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, UNDERSIZE_FRM_RX_INT, undersize_frm_rx_int_i ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_tgfpf, lfield_range, ORD_SET_LEN_ERR_INT , ord_set_len_err_int_i  ); \
    MAPOTN_INT_CHNL_ARRAY(mapotn_handle, mapotn_rgfpf, lfield_range, 7, FRM_LEN_STAT_INTR,  frm_len_stat_intr_i); \

#define MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE()                                           \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, SSF_SF_RCVD         , ssf_sf_rcvd_i          ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, DPLM                , dplm_i                 ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, OPU_CSF             , opu_csf_i              ); \
    MAPOTN_INT_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, IN_SFAIL            , in_sfail_i             ); \

#define MAPOTN_STATUS_CHNL_TABLE_DEFINE()                                           \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, LOFD_V         , lofd_v         );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, IDLE_V         , idle_v         );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, CSF_V          , csf_v          );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, EXI_V          , exi_v          );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, PTI_UPI_MSMCH_V, pti_upi_msmch_v);    \

#define MAPOTN_STATUS_CHNL_TABLE_REV_B_DEFINE()                                           \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, SSF_SF_RCVD_V  , ssf_sf_rcvd_v  );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, DPLM_V         , dplm_v         );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, OPU_CSF_V      , opu_csf_v      );    \
    MAPOTN_STATUS_CHNL(mapotn_handle, mapotn_rgfpf, lfield_range, IN_SFAIL_V     , in_sfail_v     );    \

/* Code associated to MAPTON RGFP FCS check mode */ 
/* MAPOTN_RX_FCS_CHK_MODE_SKIP_N_PREAMBLE: eFCS check after skipping programmed number of */
#define MAPOTN_RX_FCS_CHK_MODE_SKIP_N_PREAMBLE    0x2
/* MAPOTN_RX_FCS_CHK_MODE_NO_PREAMBLE_SKIP eFCS check from 1st byte onwards (no preamble  skipping)*/
#define MAPOTN_RX_FCS_CHK_MODE_NO_PREAMBLE_SKIP   0x3

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: mapotn_rgfpf_fltr_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining RGFP filter properties.
*
* ELEMENTS:
*       select_generic   - defining if standard GFP fields are to be compared 
*                          filter fields or using common Generic filter 
*                          configuration
*       generic_ofs      - defining Generic offset to be used in generic 
*                          filtering
*       generic_msk      - defining Generic Mask to be used in generic filtering
*       generic_val      - defining Generic Value to be used in generic 
*                          filtering
*       exi_msk          - defining the mask to be applied to the extracted EXI 
*                          from the frame
*       pti_msk          - defining the mask to be applied to the extracted PTI 
*                          from the frame
*       upi_msk          - defining the mask to be applied to the extracted UPI
*                          from the frame
*       exi_val          - defining EXI value with which the masked extracted 
*                          EXI is compared 
*       pti_val          - defining PTI value with which the masked extracted 
*                          PTI is compared 
*       upi_val          - defining UPI value with which the masked extracted
*                          UPI is compared 
*       match_indicator  - defining match indicator to be applied when a filter 
*                          set passes
*       next_fltr_ofs    - defining the next filter set number for the 
*                          subsequent filter sets to be referred
*       cont_operator    - defining whether to merge the current action with the
*                          previous filter sets action or override the action
*       action           - defining the action to be taken if the comparison of 
*                          this filter set passes
*       comp_operator    - defining compare operator selection bit for each of 
*                          the 3 type header fields( UPI, PTI, EXI)
*
*******************************************************************************/
typedef struct mapotn_rgfpf_fltr_struct_t {  
    UINT32 select_generic; 
    UINT32 generic_ofs;
    UINT32 generic_msk; 
    UINT32 generic_val; 
    UINT32 exi_msk; 
    UINT32 pti_msk; 
    UINT32 upi_msk; 
    UINT32 exi_val; 
    UINT32 pti_val;
    UINT32 upi_val;
    UINT32 match_indicator;
    UINT32 next_fltr_ofs;
    UINT32 cont_operator; 
    UINT32 action; 
    UINT32 comp_operator;
    
} mapotn_rgfpf_fltr_t;


/*
** Global variables
*/


/*
** Function Prototypes
*/
PUBLIC mapotn_handle_t *mapotn_ctxt_create                     ( pmc_handle_t                    *parent, 
                                                                 UINT32                           base_address, 
                                                                 pmc_sys_handle_t                *sys_handle, 
                                                                 const char                      *tsb_name );
                                                                 
PUBLIC PMC_ERROR mapotn_ctxt_destroy                           ( mapotn_handle_t                 *mapotn_handle );


PUBLIC void mapotn_handle_init                                 ( mapotn_handle_t                 *mapotn_handle, 
                                                                 mapotn_db_method_t               db_mode);

PUBLIC void mapotn_alloc_init(mapotn_handle_t *mapotn_handle);

PUBLIC PMC_ERROR mapotn_handle_restart_init(                     mapotn_handle_t *mapotn_handle,
                                                                 util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                                                 pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR mapotn_db_entry_set                           ( mapotn_handle_t                 *mapotn_handle, 
                                                                UINT32                            schd_addr,
                                                                UINT32                            chnl );

PUBLIC PMC_ERROR mapotn_db_entry_clear                         ( mapotn_handle_t                 *mapotn_handle, 
                                                                 UINT32                           chnl );
                                                                                                                                            
PUBLIC PMC_ERROR mapotn_db_entry_get                           ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           schd_addr, 
                                                                 UINT32                           *chnl );

PUBLIC PMC_ERROR mapotn_db_entry_all_get                       ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                          chnl[MAPOTN_NUM_CHANL] );

             
PUBLIC PMC_ERROR mapotn_init                                   ( mapotn_handle_t *mapotn_handle, 
                                                                 pmc_energy_state_request_t energy_state_request,
                                                                 util_global_map_adapt_func_t adaptation_function) ;
PUBLIC BOOL8 mapotn_start_state_test                            ( mapotn_handle_t                 *mapotn_handle);

PUBLIC PMC_ERROR mapotn_num_cal_entry_req_get                  ( mapotn_handle_t                 *mapotn_handle,
                                                                 util_global_odukp_type_t         odukp_type, 
                                                                 UINT32                           oduflex_cbr_rate,
                                                                 UINT32                           oduflex_gfp_rate,
                                                                 UINT32                          *num_cal_entry );

PUBLIC PMC_ERROR mapotn_unused_chnl_get                        ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                          *chnl );

PUBLIC PMC_ERROR mapotn_chnl_avail_chk                         ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_head_tail_size_get                     ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                          *head_byte_ptr, 
                                                                 BOOL8                            *tail_ins_ptr );

PUBLIC PMC_ERROR mapotn_demapper_prov                          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 util_global_map_adapt_func_t     adaptation_function,
                                                                 util_global_odukp_type_t         odukp_type, 
                                                                 UINT32                           oduflex_cbr_rate,
                                                                 UINT32                           oduflex_gfp_rate, 
                                                                 UINT32                           cbr_client_rate, 
                                                                 mapotn_src_dest_t                demapper_destination );

PUBLIC BOOL8  mapotn_demapper_prov_get                      ( mapotn_handle_t                 *mapotn_handle,
                                                              UINT32                           chnl,
                                                              util_global_map_adapt_func_t     adaptation_function);

PUBLIC PMC_ERROR mapotn_demapper_unprov                        ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC void      mapotn_demapper_clean(                          mapotn_handle_t *mapotn_handle,
                                                                 BOOL8 has_gfp,
                                                                 BOOL8 has_gmp,
                                                                 UINT32 *chnl,   
                                                                 UINT32 num_chnl);

PUBLIC PMC_ERROR mapotn_demapper_activate                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_demapper_deactivate                    ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );   

PUBLIC PMC_ERROR mapotn_mapper_prov                            ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 util_global_map_adapt_func_t     adaptation_function,
                                                                 util_global_odukp_type_t         odukp_type, 
                                                                 UINT32                           oduflex_cbr_rate,
                                                                 UINT32                           oduflex_gfp_rate,  
                                                                 UINT32                           cbr_client_rate, 
                                                                 mapotn_src_dest_t                enet_mapper_source );    

PUBLIC BOOL8 mapotn_mapper_prov_get                             ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 util_global_map_adapt_func_t     adaptation_function);   

PUBLIC PMC_ERROR mapotn_mapping_mode_get                        ( mapotn_handle_t *mapotn_handle, 
                                                                  util_global_map_adapt_func_t adaptation_function, 
                                                                  util_global_mapping_mode_t *mapotn_mapping_mode); 
PUBLIC PMC_ERROR mapotn_tgfpf_cfc_thresh_reprov                ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate );

PUBLIC PMC_ERROR mapotn_mapper_mpmo_deprov                     ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_bwr_ind_ramp_start_delay_get           ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                          *bwr_ind_ramp_start_delay_us );

PUBLIC PMC_ERROR mapotn_mapper_unprov                          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_mapper_activate                        ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_mapper_deactivate                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl ); 

PUBLIC void      mapotn_mapper_clean(                            mapotn_handle_t *mapotn_handle,
                                                                 BOOL8 has_gfp,
                                                                 BOOL8 has_gmp,
                                                                 UINT32 *chnl,   
                                                                 UINT32 num_chnl);    
PUBLIC void      mapotn_rgfpf_volatile_ctxt_init               ( mapotn_handle_t                 *mapotn_handle, 
                                                                 UINT32                           enable );
                                                                 
                                                                 


PUBLIC PMC_ERROR mapotn_mapper_mpmo_cfg_get                    ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 BOOL8                            *mpmo_cfg_req );

PUBLIC PMC_ERROR mapotn_mpma_data_get                          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                          *pkt_size_ptr,
                                                                 UINT32                          *pkt_per_int_ptr,
                                                                 UINT32                          *pkt_per_n_ptr,
                                                                 UINT32                          *pkt_per_d_ptr, 
                                                                 DOUBLE                          *epsilon_ptr );

PUBLIC PMC_ERROR mapotn_mapper_mpmo_prov                       ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           pkt_size,
                                                                 UINT32                           pkt_per_int,
                                                                 UINT32                           pkt_per_n,
                                                                 UINT32                           pkt_per_d,
                                                                 BOOL8                            is_sonet);

/* LCOV_EXCL_START */
PUBLIC PMC_ERROR mapotn_mpma_pkt_period_calc                   ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl, 
                                                                 UINT32                          *pkt_size_ptr,
                                                                 UINT32                          *pkt_per_int_ptr,
                                                                 UINT32                          *pkt_per_n_ptr,
                                                                 UINT32                          *pkt_per_d_ptr,
                                                                 DOUBLE                          *epsilon_ptr ); 
/* LCOV_EXCL_STOP */                                                             

PUBLIC PMC_ERROR mapotn_cn_frame_period_get                    ( mapotn_handle_t                 *mapotn_handle,
                                                                 util_global_odukp_type_t         odukp_type, 
                                                                 util_opsa_cn_frame_period_t     *cn_frame_period );


PUBLIC PMC_ERROR mapotn_mapper_tgmp_cfc_xoff_cfg               ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_mpmo_ch_activate                       ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl);

PUBLIC PMC_ERROR mapotn_mpmo_ch_deactivate                     ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                          chnl);

                                         
PUBLIC PMC_ERROR mapotn_server_rate_cal                        (mapotn_handle_t                  *mapotn_handle, 
                                                                util_global_odukp_type_t         odukp_type, 
                                                                UINT32                           oduflex_cbr_rate,
                                                                UINT32                           oduflex_gfp_rate,  
                                                                UINT32                           *server_rate);

PUBLIC PMC_ERROR mapotn_mapper_tgmp_cfc_xoff_cfg_super_prov     (mapotn_handle_t                  *mapotn_handle,
                                                                 UINT32                           chnl);

/* RGFPF */  

PUBLIC PMC_ERROR mapotn_rgfpf_init                             ( mapotn_handle_t                *mapotn_handle, 
                                                                 pmc_energy_state_t              energy_state );

PUBLIC BOOL8 mapotn_rgfpf_start_state_test                      ( mapotn_handle_t                 *mapotn_handle ); 

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_cpb_zone_thrld_cfg          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl, 
                                                                 UINT32                           val);

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_default_init                ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 pmc_block_reg_access_state_enum  state );

/* LCOV_EXCL_START */
PUBLIC PMC_ERROR mapotn_rgfpf_basic_fltr_cfg                   ( mapotn_handle_t                 *mapotn_handle, 
                                                                 UINT32                           chnl );
/* LCOV_EXCL_STOP */

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_activate                    ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_deactivate                  ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC void mapotn_rgfpf_chnl_clean                            ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                          *chnl,
                                                                 UINT32                           num_chnl );  

PUBLIC PMC_ERROR mapotn_rgfpf_block_non_default_cfg            ( mapotn_handle_t                 *mapotn_handle );


PUBLIC PMC_ERROR mapotn_rgfpf_chnl_prov                        ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_deprov                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_reset_cfg                   ( mapotn_handle_t                 *mapotn_handle, 
                                                                 UINT32                           chnl, 
                                                                 pmc_block_reg_access_state_enum  chnl_state );


PUBLIC PMC_ERROR mapotn_rgfpf_extn_hdr_extrct_cfg              ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           ehec_chk,
                                                                 UINT32                           ehec_ecc,
                                                                 UINT32                           exi_val1, 
                                                                 UINT32                           exi_len1,
                                                                 UINT32                           exi_val2, 
                                                                 UINT32                           exi_len2 );

PUBLIC PMC_ERROR mapotn_rgfpf_pause_frm_detect_cfg             ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           pause_fltr,
                                                                 UINT32                           pause_enable, 
                                                                 UINT32                           pause_mode,
                                                                 UINT32                           pause_upi, 
                                                                 UINT32                           pause_pti,
                                                                 UINT32                           auto_flow_ctrl_en );

PUBLIC PMC_ERROR mapotn_rgfpf_pause_frm_detect_cfg_get             ( mapotn_handle_t                 *mapotn_handle,
                                                                     UINT32                           chnl,
                                                                     BOOL8                           *pause_fltr,
                                                                     BOOL8                           *pause_enable, 
                                                                     BOOL8                           *pause_mode,
                                                                     BOOL8                           *pause_upi, 
                                                                     BOOL8                           *pause_pti,
                                                                     BOOL8                           *auto_flow_ctrl_en );


PUBLIC PMC_ERROR mapotn_rgfpf_xoff_fifo_thrld_cfg              ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           num_cal_entries );


PUBLIC PMC_ERROR mapotn_rgfpf_egress_frm_len_chk_cfg           ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT16                           max_frm_len, 
                                                                 UINT16                           min_frm_len );

PUBLIC PMC_ERROR mapotn_rgfpf_frm_len_stat_cfg                 ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           bucket_no,
                                                                 UINT32                           max_frm_len, 
                                                                 UINT32                           min_frm_len );

PUBLIC PMC_ERROR mapotn_rgfpf_frm_len_stat_cfg_get             ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,           
                                                                 UINT32                           bucket_no,
                                                                 UINT32                          *max_frm_len, 
                                                                 UINT32                          *min_frm_len );

PUBLIC PMC_ERROR mapotn_rgfpf_preamble_sfd_mode_cfg            ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           preamble_size );
                                                                                                 
PUBLIC PMC_ERROR mapotn_rgfpf_fcs_chk_cfg                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           cnt_udf_fcs,
                                                                 UINT32                           gfp_fcs_chk_mode );


PUBLIC PMC_ERROR mapotn_rgfpf_enet_mac_dest_addr_cfg           ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           mac_da_lsb, 
                                                                 UINT32                           mac_da_msb );

PUBLIC PMC_ERROR mapotn_rgfpf_ordered_set_upi_cfg              ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           ord_set_upi );

/* LCOV_EXCL_START */
PUBLIC UINT32 mapotn_rgfpf_ordered_set_upi_get                 ( mapotn_handle_t                 *mapotn_handle);
/* LCOV_EXCL_STOP */


PUBLIC PMC_ERROR mapotn_rgfpf_ch_defects_en                    ( mapotn_handle_t                 *mapotn_handle,  
                                                                 UINT32                           chnl,
                                                                 UINT32                           dfld_en,
                                                                 UINT32                           dcsf_en,
                                                                 UINT32                           dump_en,
                                                                 UINT32                           dexm_en );
                                                                 


PUBLIC PMC_ERROR mapotn_rgfpf_cpu_fifo_reset                    ( mapotn_handle_t                *mapotn_handle, 
                                                                  UINT32                          chnl,
                                                                  UINT32                          reset_val );


PUBLIC PMC_ERROR mapotn_rgfpf_csf_stat                          ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                         *valid,
                                                                  UINT32                         *chid, 
                                                                  UINT32                         *upi );


PUBLIC PMC_ERROR mapotn_rgfpf_extn_hdr_extrct_get               ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                         *ehec_chk, 
                                                                  UINT32                         *ehec_ecc,
                                                                  UINT32                         *exi_val1, 
                                                                  UINT32                         *exi_len1, 
                                                                  UINT32                         *exi_val2, 
                                                                  UINT32                         *exi_len2 );  

PUBLIC PMC_ERROR mapotn_rgfpf_fcs_chk_mode_get                  ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl, 
                                                                  UINT32                         *gfp_fcs_chk_mode );

PUBLIC void mapotn_rgfpf_activated_chnl_get(mapotn_handle_t *mapotn_handle,
                                            BOOL8 *actv_chnl);

/* TGFPF */ 

PUBLIC PMC_ERROR mapotn_tgfpf_init                              ( mapotn_handle_t                *mapotn_handle, 
                                                                  pmc_energy_state_t              energy_state ); 

PUBLIC BOOL8 mapotn_tgfpf_start_state_test                      ( mapotn_handle_t                 *mapotn_handle );


PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prov                         ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_deprov                       ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_activate                     ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_deactivate                   ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl );

PUBLIC void mapotn_tgfpf_chnl_clean                             ( mapotn_handle_t                *mapotn_handle,
                                                                 UINT32                          *chnl,
                                                                 UINT32                           num_chnl );

PUBLIC PMC_ERROR mapotn_tgfpf_block_non_default_cfg             ( mapotn_handle_t                *mapotn_handle );

PUBLIC PMC_ERROR mapotn_tgfpf_enbl                              ( mapotn_handle_t                *mapotn_handle, 
                                                                  UINT32                          enable );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_reset                        ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  pmc_block_reg_access_state_enum chnl_state ); 

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prefetch_thrld_cfg           ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl, 
                                                                  UINT32                          thrld );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_packer_thrld_cfg             ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                          low_thrld, 
                                                                  UINT32                          high_thrld );
                                                                  

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pause_frm_cfg                ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl, 
                                                                  UINT32                          enable,
                                                                  UINT32                          priority,
                                                                  mapotn_tgfpf_pause_frm_memory_t   xon_pause,
                                                                  mapotn_tgfpf_pause_frm_memory_t   xoff_pause );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pause_frm_cfg_get            ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl, 
                                                                  BOOL8                          *enable,
                                                                  BOOL8                          *priority,
                                                                  mapotn_tgfpf_pause_frm_memory_t   *xon_pause,
                                                                  mapotn_tgfpf_pause_frm_memory_t   *xoff_pause );
                                                                  

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_hdr_cfg              ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl,
                                                                  UINT32                           ext_hdr_enbl,
                                                                  UINT32                           ext_hdr_len,
                                                                  UINT32                           pti_value,
                                                                  UINT32                           exi_value,
                                                                  UINT32                           upi_value,
                                                                  UINT32                           upi_sel );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_fcs_cfg              ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl, 
                                                                  mapotn_fcs_mode_t                fcs_mode,     
                                                                  UINT32                           pfcs_rev_obyte,    
                                                                  UINT32                           pfcs_rev_obit,     
                                                                  UINT32                           pfcs_rev_ibit,     
                                                                  UINT32                           pfcs_rev_crpt_inv, 
                                                                  UINT32                           pfcs_rev_inv,      
                                                                  UINT32                           pfcs_rev_init );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_frm_scrmbl_cfg               ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl, 
                                                                  UINT32                           payload_scrmbl, 
                                                                  UINT32                           core_hdr_scrmbl );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_client_data_frm_len_cfg      ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl,
                                                                  UINT32                           max_frm_len, 
                                                                  UINT32                           min_frm_len );

PUBLIC PMC_ERROR mapotn_tgfpf_order_set_upi_cfg                 ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           upi_value );

PUBLIC UINT32 mapotn_tgfpf_order_set_upi_get                    ( mapotn_handle_t                 *mapotn_handle);


PUBLIC PMC_ERROR mapotn_tgfpf_ohfs_insert_csf_cfg_get(mapotn_handle_t *mapotn_handle,
                                                      UINT32 chnl,
                                                      UINT32 *csf_insrt);
                                                                  
PUBLIC PMC_ERROR mapotn_tgfpf_ext_hdr_mem_cfg                   ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT32                            ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT] );


PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ext_hdr_cfg                  ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT32                            ext_hdr_enbl,
                                                                  UINT32                            ext_hdr_len, 
                                                                  UINT32                            ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT] );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ext_hdr_get                  ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT32                           *ext_hdr_enbl,
                                                                  UINT32                           *ext_hdr_len ); 

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_fcs_get              ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  mapotn_fcs_mode_t                *fcs_mode );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pti_set                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                            pti_value );

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pti_get                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                           *pti_value);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_set                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                            upi_value);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_get                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                           *upi_value);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_sel_set(mapotn_handle_t *mapotn_handle,
                                               UINT32           chnl, 
                                               BOOL8            upi_sel);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_sel_get(mapotn_handle_t *mapotn_handle,
                                               UINT32           chnl, 
                                               BOOL8           *upi_sel);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_exi_set                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                            exi_value);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_exi_get                      ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl, 
                                                                  UINT32                           *exi_value);

PUBLIC PMC_ERROR mapotn_ohfs_insert_chnl_payload_type_set       ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT8                             pt );

PUBLIC PMC_ERROR mapotn_ohfs_insert_chnl_payload_type_get       ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT8                            *pt );

PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_payload_type_set       ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT8                             pt );

PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_accpt_payload_type_get ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT8                            *accpt_pt );

PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_exp_payload_type_get   ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                            chnl,
                                                                  UINT8                            *exp_pt );

PUBLIC PMC_ERROR mapotn_chnl_count_get                      ( mapotn_handle_t                  *mapotn_handle, 
                                                              UINT32                           *gfp_chnl_count, 
                                                              UINT32                           *gmp_chnl_count);

PUBLIC PMC_ERROR mapotn_adjust_dci_clock                        ( mapotn_handle_t                  *mapotn_handle,
                                                                  UINT32                           num_mult,
                                                                  UINT32                           denom_mult);
            
PUBLIC PMC_ERROR mapotn_int_enable(mapotn_handle_t    *mapotn_handle,
                                   mapotn_int_t       *int_table_ptr,
                                   mapotn_int_t       *int_en_table_ptr,
                                   BOOL8               enable);

PUBLIC PMC_ERROR mapotn_int_clear(mapotn_handle_t    *mapotn_handle,
                                  mapotn_int_t       *int_table_ptr,
                                  mapotn_int_t       *int_en_table_ptr);

PUBLIC PMC_ERROR mapotn_int_retrieve(mapotn_handle_t    *mapotn_handle,
                                     mapotn_int_t       *filt_table_ptr,
                                     mapotn_int_t       *int_table_ptr);

PUBLIC PMC_ERROR mapotn_int_enabled_check(mapotn_handle_t   *mapotn_handle,
                                          mapotn_int_t      *int_en_table_ptr,
                                          BOOL8             *int_found_ptr);

PUBLIC PMC_ERROR mapotn_int_chnl_enable(mapotn_handle_t    *mapotn_handle,
                                        UINT32              chnl_id,
                                        mapotn_int_chnl_t  *int_table_ptr,
                                        mapotn_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable);

PUBLIC PMC_ERROR mapotn_int_chnl_clear(mapotn_handle_t    *mapotn_handle,
                                       UINT32              chnl_id,
                                       mapotn_int_chnl_t  *int_table_ptr,
                                       mapotn_int_chnl_t  *int_en_table_ptr);

PUBLIC void mapotn_retrieve_chnl_registers(mapotn_handle_t *mapotn_handle);

PUBLIC PMC_ERROR mapotn_int_chnl_retrieve(mapotn_handle_t    *mapotn_handle,
                                          UINT32              chnl_id,
                                          mapotn_int_chnl_t  *filt_table_ptr,
                                          mapotn_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR mapotn_int_chnl_enabled_check(mapotn_handle_t   *mapotn_handle,
                                               UINT32             chnl_id,
                                               mapotn_int_chnl_t *int_en_table_ptr,
                                               BOOL8             *int_found_ptr);

PUBLIC PMC_ERROR mapotn_map_to_ssf_set(mapotn_handle_t *mapotn_handle, 
                                       UINT32 chnl, 
                                       ohfs_remove_map_ssf_type_t map_ssf_type,
                                       UINT32 enable);

PUBLIC PMC_ERROR mapotn_map_to_ssf_get(mapotn_handle_t *mapotn_handle, 
                                       UINT32 chnl, 
                                       ohfs_remove_map_ssf_type_t map_ssf_type,
                                       UINT32 *enable);

PUBLIC PMC_ERROR mapotn_null_frame_cfg(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       util_global_direction_t dir,
                                       UINT32 *null_frame_en,
                                       UINT32 *prbs_tx_null_frame_en);

PUBLIC PMC_ERROR mapotn_null_frame_get(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       BOOL8 *tx_null_frame_en,
                                       BOOL8 *rx_null_frame_en);

PUBLIC BOOL mapotn_is_etrans_in_path(mapotn_handle_t *mapotn_handle);

/* RGFPF */

PUBLIC PMC_ERROR mapotn_rgfpf_client_classify_prov      ( mapotn_handle_t                 *mapotn_handle, 
                                                          UINT32                           chnl, 
                                                          mapotn_rgfpf_cmf_dest_t          cmf_dest, 
                                                          mapotn_rgfpf_cmf_upi_filt_mode_t cmf_upi_filt_mode,
                                                          UINT32                           upi_range_1_start, 
                                                          UINT32                           upi_range_1_end, 
                                                          UINT32                           upi_range_2_start, 
                                                          UINT32                           upi_range_2_en );

PUBLIC PMC_ERROR mapotn_rgfpf_classify_unprov           ( mapotn_handle_t                 *mapotn_handle, 
                                                          UINT32                           chnl );    

PUBLIC PMC_ERROR mapotn_rgfpf_user_client_classify_prov ( mapotn_handle_t                 *mapotn_handle, 
                                                          UINT32                           chnl, 
                                                          UINT32                           frame_field_mask,
                                                          mapotn_rgfpf_comparison_op_t     pti_comp, 
                                                          UINT32                           pti_value,
                                                          mapotn_rgfpf_comparison_op_t     upi_comp, 
                                                          UINT32                           upi_value,
                                                          mapotn_rgfpf_comparison_op_t     exi_comp, 
                                                          UINT32                           exi_value,
                                                          mapotn_rgfpf_comparison_op_t     pfi_comp, 
                                                          UINT32                           pfi_value,
                                                          mapotn_rgfpf_proc_seq_t          proc_seq_enum, 
                                                          mapotn_rgfpf_classify_actions_t  action );

PUBLIC PMC_ERROR mapotn_rgfpf_classify_cfg(mapotn_handle_t *mapotn_handle, 
                                           UINT32 chnl, 
                                           UINT32 filter_no,
                                           UINT32 frame_field_mask, 
                                           mapotn_rgfpf_comparison_op_t pti_comp, 
                                           UINT32 pti_value,
                                           mapotn_rgfpf_comparison_op_t upi_comp, 
                                           UINT32 upi_value,
                                           mapotn_rgfpf_comparison_op_t exi_comp, 
                                           UINT32 exi_value,
                                           mapotn_rgfpf_classify_actions_t action);

PUBLIC PMC_ERROR mapotn_rgfpf_classify_get(mapotn_handle_t *mapotn_handle, 
                                           UINT32 chnl, 
                                           UINT32 filter_no,
                                           UINT32 *frame_field_mask_ptr, 
                                           mapotn_rgfpf_comparison_op_t *pti_comp_ptr, 
                                           UINT32 *pti_value_ptr,
                                           mapotn_rgfpf_comparison_op_t *upi_comp_ptr, 
                                           UINT32 *upi_value_ptr,
                                           mapotn_rgfpf_comparison_op_t *exi_comp_ptr, 
                                           UINT32 *exi_value_ptr,
                                           mapotn_rgfpf_classify_actions_t *action_ptr);

PUBLIC PMC_ERROR mapotn_rgfpf_pmon_latch_trig_set(mapotn_handle_t *mapotn_handle);


PUBLIC PMC_ERROR mapotn_rgfpf_egress_pfcs_efcs_strip_cfg(mapotn_handle_t *mapotn_handle,
                                                         UINT32 chnl,
                                                         UINT32 efcs_strip, 
                                                         UINT32 pfcs_strip);

/* TGFPF */

PUBLIC PMC_ERROR mapotn_tgfpf_pause_frm_insrt          ( mapotn_handle_t                  *mapotn_handle,
                                                         UINT32                            chnl );

PUBLIC PMC_ERROR mapotn_tgfpf_pause_frm_mem_cfg        ( mapotn_handle_t                  *mapotn_handle,
                                                         UINT32                            pause_time[MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT],
                                                         UINT32                            pause_frame_mem[MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT] );

PUBLIC PMC_ERROR mapotn_tgfpf_pmon_latch_trig_set(mapotn_handle_t *mapotn_handle);


PUBLIC void mapotn_tgfpf_fcs_corrupt_set(mapotn_handle_t    *mapotn_handle,
                                         UINT32              chnl,
                                         BOOL8               enable);

PUBLIC BOOL8 mapotn_tgfpf_fcs_corrupt_get(mapotn_handle_t    *mapotn_handle,
                                          UINT32              chnl);

PUBLIC void mapotn_tgfpf_activated_chnl_get(mapotn_handle_t *mapotn_handle,
                                            BOOL8 *actv_chnl);    
PUBLIC mapotn_src_dest_t mapotn_tgfpf_chnl_mapping_get(mapotn_handle_t *mapotn_handle,
                                                       UINT32 chnl);


PUBLIC PMC_ERROR mapotn_ghao_db_chnl_rec_update                ( mapotn_handle_t                 *mapotn_handle,
                                                                 util_gen_db_id_t                 db_id,
                                                                 UINT32                           chnl, 
                                                                 UINT32                           new_num_cal_entries,
                                                                 UINT32                           old_cal_mask[3],
                                                                 UINT32                           new_cal_mask[3],
                                                                 UINT32                          *cal_slots_to_add_remove_num,
                                                                 UINT32                           cal_slots_to_add_remove[79] );

PUBLIC PMC_ERROR mapotn_ghao_db_chnl_get_masks                 ( mapotn_handle_t                 *mapotn_handle,
                                                                 util_gen_db_id_t                 db_id,
                                                                 UINT32                           chnl, 
                                                                 UINT32                           new_num_cal_entries,
                                                                 UINT32                           old_cal_mask[3],
                                                                 UINT32                           new_cal_mask[3],
                                                                 UINT32                          *cal_slots_to_add_remove_num,
                                                                 UINT32                           cal_slots_to_add_remove[79] );

PUBLIC PMC_ERROR mapotn_db_chnl_num_entries_get                ( mapotn_handle_t                 *mapotn_handle, 
                                                                 UINT32                          chnl,
                                                                 util_gen_db_id_t                db_id,
                                                                 UINT32                          *num_cal_entries);

PUBLIC PMC_ERROR mapotn_rgfpf_cfc_thresh_reprov                ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate );

PUBLIC PMC_ERROR mapotn_mpma_scbs3_reprov                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate );

PUBLIC PMC_ERROR mapotn_tgfpf_cfc_thresh_reprov                ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate );

PUBLIC PMC_ERROR mapotn_oduksc_scbs3_cgen_reprov               ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate );

PUBLIC PMC_ERROR mapotn_oduksc_ramp_prov                       ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           oduflex_gfp_rate,
                                                                 BOOL                             end_for_bwr_ind_250us );

PUBLIC PMC_ERROR mapotn_oduksc_ramp_start                      ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 BOOL                             set_bwr_ind );

PUBLIC PMC_ERROR mapotn_oduksc_ramp_poll_end_deprov            ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 BOOL                             clear_bwr_ind,
                                                                 UINT32                           poll_delay_us );

PUBLIC PMC_ERROR mapotn_oduksc_ramp_done_get                   ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                          *ramp_done );

PUBLIC PMC_ERROR mapotn_oduksc_ramp_done_isr                   ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl );

PUBLIC PMC_ERROR mapotn_bwr_ind_ramp_start_delay_get           ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                          *bwr_ind_ramp_start_delay_us );

/* RGFPF */  

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_unpack_fifo_reprov          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           new_num_cal_entries);

PUBLIC PMC_ERROR mapotn_rgfpf_chnl_egress_fifo_reprov          ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           new_num_cal_entries);

PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_get                          ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                         *rcoh );

PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_ncs_get                      ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                         *ncs );

PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_bwr_ind_get                  ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                         *bwr_ind );

PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_ncs_poll                     ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                          ncs,
                                                                  UINT32                          poll_delay_us );

PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_bwr_ind_poll                 ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                          bwr_ind,
                                                                  UINT32                          poll_delay_us );

/* TGFPF */ 

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prefetch_fifo_reprov         ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                          new_num_cal_entries);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov      ( mapotn_handle_t                *mapotn_handle,
                                                                  UINT32                          chnl,
                                                                  UINT32                          new_num_cal_entries);

PUBLIC PMC_ERROR mapotn_tgfpf_chnl_packing_fifo_reprov         ( mapotn_handle_t                 *mapotn_handle,
                                                                 UINT32                           chnl,
                                                                 UINT32                           new_num_cal_entries);

PUBLIC PMC_ERROR mapotn_tgfpf_rcoh_ncs_set                      ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl,
                                                                  UINT32                           ncs );

PUBLIC PMC_ERROR mapotn_tgfpf_rcoh_bwr_ind_set                  ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT32                           chnl,
                                                                  UINT32                           bwr_ind );

/* LCOV_EXCL_START */
PUBLIC PMC_ERROR mapotn_ordered_set_upi_cfg                     ( mapotn_handle_t                 *mapotn_handle,
                                                                  UINT8                            tgfpf_ordered_set_upi,
                                                                  UINT8                            rgfpf_ordered_set_upi);
/* LCOV_EXCL_STOP */
PUBLIC PMC_ERROR mapotn_db_link_num_entries_get(mapotn_handle_t *mapotn_handle,
                                                UINT32         chnl,              
                                                UINT32        *num_of_recs_ptr);

PUBLIC void mapotn_cm_status_get(mapotn_handle_t *mapotn_handle,
                                 UINT32 chnl,              
                                 UINT32 *value);


#ifdef __cplusplus
}
#endif

#endif /* _MAPOTN_H */

/* 
** end of file 
*/
