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
#ifndef _MAPOTN_LOC_H
#define _MAPOTN_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "mapotn.h"
#include "ohfs_remove_api.h"
#include "ohfs_insert_api.h"
#include "cfc.h"
#include "oduksc.h"
#include "mpma.h"
#include "mpmo.h"
#include "rgmp.h"
#include "tgmp.h"
#include "mapotn_rx_io.h"
#include "mapotn_rx_io_inline.h"
#include "mapotn_rx_regs.h"
#include "mapotn_map.h"
#include "mapotn_tx_io.h"
#include "mapotn_tx_io_inline.h"
#include "mapotn_rgfpf_io.h"
#include "mapotn_rgfpf_io_inline.h"
#include "mapotn_tgfpf_io.h"
#include "mapotn_tgfpf_io_inline.h"
#include "mapotn_tx_regs.h"
#include "rgmp_regs.h"
#include "rgmp_io.h"
#include "rgmp_io_inline.h"
#include "tgmp_regs.h"
#include "tgmp_io.h"
#include "tgmp_io_inline.h"
#include "cfc_io.h"
#include "cfc_io_inline.h"
#include "oduksc_io.h"
#include "oduksc_io_inline.h"
#include "util_gen_db.h"


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: mapotn_rgfpf_fifo_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify FIFO state.
*
* ELEMENTS:
*   MAPOTN_FIFO_NON_INIT      - FIFO not initialized
*   MAPOTN_FIFO_IDLE          - FIFO is Idle (in reset)
*   MAPOTN_FIFO_SOF           - FIFO start of frame
*           
*
*******************************************************************************/ 
typedef enum {
    MAPOTN_FIFO_NON_INIT,
    MAPOTN_FIFO_IDLE,
    MAPOTN_FIFO_SOF,
} mapotn_rgfpf_fifo_state_t; 


/*
** Constants
*/

/* Number of supported channels */
#define MAPOTN_PER_SLOT_BW                              1250
#define MAPOTN_CORE_TYPE_HDR_BYTE_LEN                   8                                                        
#define MAPOTN_ODUKSC_USERBITS_DEL                      0xC                                                        
#define MAPOTN_NULL_DB_REC                              0xFF

#define MAPOTN_NUM_CAL_ENTRIES_IN_ODU0                  1
#define MAPOTN_NUM_CAL_ENTRIES_IN_ODU1                  2
#define MAPOTN_NUM_CAL_ENTRIES_IN_10G                   8
#define MAPOTN_NUM_CAL_ENTRIES_IN_40G                   32
#define MAPOTN_NUM_CAL_ENTRIES_IN_100G                  80

/* RGFPF */
#define MAPOTN_RGFPF_CPB_ZONE_THRLD_CHNL_NUM_BITS       2
#define MAPOTN_RGFPF_CHNL_NUM_FILTERS                   10
#define MAPOTN_FRM_LEN_STAT_COUNTER_CONFIG_N            7
/* cpu data fifo - maximum buffer length =  4 buffers * 144 bytes each */
#define MAPOTN_RGFPF_MAX_CPU_MSG_LEN                    (144 * 4)

/* Page size per FIFO channel */       
#define MAPOTN_RGFPF_CFC_PAGE_SIZE                      2 

/* TGFPF */
/*#define MAPOTN_TGFPF_ERR_MGMT_FRAME_TX_IN_PROGRESS      1
#define MAPOTN_TGFPF_ERR_MGMT_FRAME_TX_NOT_COMPLETED    2
#define MAPOTN_TGFPF_ERR_PAUSE_FRAME_TX_IN_PROGRESS     3
#define MAPOTN_TGFPF_ERR_PAUSE_FRAME_TX_NOT_COMPLETED   4*/

#define MAPOTN_MPMA_PKT_SIZE                            144
#define MAPOTN_MPMA_PKT_SIZE_RANGE                      96

/* Default Ordered set UPI value */
#define MAPOTN_DEFAULT_ORDERED_SET_UPI                  0x14
#define MAPOTN_DUAL_OHFS_REMOVE_MODE                    FALSE


/*
** Macro Definitions
*/        



#define MAPOTN_MAPPING_MODE_IS_GMP(handle, chnl_id) \
    ((UTIL_GLOBAL_GMP == handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) || \
     (UTIL_GLOBAL_AMP == handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) || \
     (UTIL_GLOBAL_BMP == handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))

/*
** Structures and Unions
*/




/*******************************************************************************
* STRUCTURE: mapotn_oduksc_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining ODUKSC configuration parameters for each instantiation.
*
* ELEMENTS:
*       max_chnl         - defining maximum number of channels in the context of
*                          a parent
*       userbits_del     - defining userbits 
*
*******************************************************************************/
typedef struct mapotn_oduksc_cfg_struct_t {   
    UINT32 max_chnl;   
    UINT32 userbits_del;  
}  mapotn_oduksc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: mapotn_cfc_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining CFC configuration parameters for each instantiation.
*
* ELEMENTS:
*       max_fifo_num     - defining nummber of FIFOs in CFC. Equivalent to 
*                          channel count
*       page_size        - defining number of entries per page
*
*******************************************************************************/
typedef struct mapotn_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;  
    UINT32 page_size;      
}  mapotn_cfc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: mapotn_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing basic configuration information for any MAPOTN 
*   instance.
*
* ELEMENTS:
*       num_mapotn_chanl  - defining maximim nummber of MAPOTN channels
*       cfc_cfg           - defining structure to CFC configuration parameters
*       oduksc_cfg        - defining structure to ODUKSC configuration parameters
*       db_mode           - defining database read and write access mode
*
*******************************************************************************/
typedef struct mapotn_cfg_struct_t {  
    UINT32                        num_mapotn_chanl;  
    mapotn_cfc_cfg_struct_t       cfc_cfg;
    mapotn_oduksc_cfg_struct_t    oduksc_cfg;
    mapotn_db_method_t            db_mode;    
} mapotn_cfg_t;

/*******************************************************************************
* STRUCTURE: mapotn_chnl_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining channel context information.
*
* ELEMENTS:
*       state             - defining the state of the channel
*       adaptation_func   - defining operational mode of resource see
*                           util_global_map_adapt_func_t 
*       odukp_type        - defining ODUkp or ODUjp type see odukp_type_t
*       client_rate       - defining client data rate of signal in kbits/sec
*       num_cal_entries   - defining number of calendar entries required for the 
*                           channel
*       mapping_mode      - defining mapping mode of the channel(GMP, GFP, AMP, 
*                           BMP)
*       src_dest          - defining source/destination of data stream
*       ghao_ramp_oduflex_gfp_rate  - defining oduflex rate before ramp
*       ghao_start_oduflex_gfp_rate  - defining whether we are ramping up/down
*                           until 250us before final rate to clear BWR_IND
*                           (MAPOTN_TX)
*
*******************************************************************************/
typedef struct mapotn_chnl_ctxt_struct_t {
    
    mapotn_chnl_state_t            state; 
    util_global_map_adapt_func_t   adaptation_func;
    util_global_odukp_type_t       odukp_type;
    DOUBLE                         client_rate;
    UINT32                         num_cal_entries;
    util_global_mapping_mode_t     mapping_mode;
    mapotn_src_dest_t              src_dest;  
    UINT32                         ghao_start_oduflex_gfp_rate;
    BOOL8                          doing_ghao_ramp_until_bwr_ind;
} mapotn_chnl_ctxt_t;                                   
        
                                                 
/*******************************************************************************
* STRUCTURE: mapotn_rgfpf_chnl_fltr_mgmt_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining filter managment context information.
*
* ELEMENTS: 
*       next_filter       - defining index of next filter to be configured
*
*******************************************************************************/                                         
typedef struct mapotn_rgfpf_chnl_fltr_mgmt_struct_t {      
                                                                                                                            
    BOOL8 client_classify_prov;
    UINT32 next_filter;  
                                                  
} mapotn_rgfpf_chnl_fltr_mgmt_t;
  

/*******************************************************************************
* STRUCTURE: mapotn_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining MAPOTN energy state
*
* ELEMENTS: 
*   rgfpf96 - RGFPF96 energy state
*   tgfpf96 - TGFPF96 energy state
*   rgmp96 - RGMP96 energy state
*   tgmp96 - TGMP96 energy state
*   mpmo   - MPMO energy state
*   mpma   - MPMA energy state
*   oduksc - ODUKSC energy state 
*   rx_top - RX top energy state 
*   tx_top - TX top energy state
*
*******************************************************************************/    
typedef struct mapotn_energy_state_struct_t { 
    pmc_energy_state_t rgfpf96;
    pmc_energy_state_t tgfpf96;
    pmc_energy_state_t rgmp96;
    pmc_energy_state_t tgmp96;
    pmc_energy_state_t mpmo;    
    pmc_energy_state_t mpma;
    pmc_energy_state_t oduksc;
    pmc_energy_state_t rx_top;
    pmc_energy_state_t tx_top;   
} mapotn_energy_state_t;

/*******************************************************************************
* STRUCTURE: mapotn_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS: 
*       rx_chnl_ctxt                 - defining the RGFPF channel context
*       tx_chnl_ctxt                 - defining the TGFPF channel context
*       mapotn_rgfpf_chnl_fltr_mgmt  - defining the filter managment context
*       mapotn_start_state           - defining MAPOTN start state
*
*******************************************************************************/
typedef struct mapotn_var_struct_t {
    
    mapotn_chnl_ctxt_t                rx_chnl_ctxt[MAPOTN_NUM_CHANL];
    mapotn_chnl_ctxt_t                tx_chnl_ctxt[MAPOTN_NUM_CHANL];
    
    mapotn_rgfpf_chnl_fltr_mgmt_t     mapotn_rgfpf_chnl_fltr_mgmt[MAPOTN_NUM_CHANL];
    BOOL                              mapotn_start_state;
    UINT8                             tgfpf_ordered_set_upi;
    UINT8                             rgfpf_ordered_set_upi;
} mapotn_var_t;

/*******************************************************************************
* STRUCTURE: mapotn_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context for warm/crash recovery
*
* ELEMENTS: 
*       var            - the var context 
*       rx_fuzzy_flag  - this RX channel based flag indicates indicates
*                         how to compare a recovered context 
*       tx_fuzzy_flag  - this TX channel based flag indicates indicates
*                         how to compare a recovered context 
*
*******************************************************************************/
typedef struct mapotn_recover_var_struct_t {
    mapotn_var_t var;
    BOOL8        rx_fuzzy_flag[MAPOTN_NUM_CHANL];
    BOOL8        tx_fuzzy_flag[MAPOTN_NUM_CHANL];
    BOOL8        has_rx_gfp_chnl;
    BOOL8        has_rx_gmp_chnl;
    BOOL8        has_tx_gfp_chnl;
    BOOL8        has_tx_gmp_chnl;
    scbs3_page_t mpma_scbs3_page;
    scbs3_page_t oduksc_scbs3_page;   
} mapotn_recover_var_t;

/*******************************************************************************
* STRUCTURE: mapotn_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining MAPOTN handle information.
*
* ELEMENTS: 
*  
*
*******************************************************************************/
struct mapotn_handle_struct_t {
    pmc_handle_t           base;
    mapotn_cfg_t           cfg;
    mapotn_var_t           var;
    
    /* Child handles needed for MAPOTN RX */ 
    ohfs_remove_handle_t  *ohfs_remove_handle;
    cfc_handle_t          *cfc_unpack_handle;
    cfc_handle_t          *cfc_egress_handle;
    mpma_handle_t         *mpma_handle;
    rgmp_handle_t         *rgmp_handle;
    
    /* Child handles needed for MAPOTN TX*/
    ohfs_insert_handle_t  *ohfs_insert_handle;
    cfc_handle_t          *cfc_packing_handle;
    cfc_handle_t          *cfc_prefetch_handle;
    mpmo_handle_t         *mpmo_handle;
    tgmp_handle_t         *tgmp_handle;
    
    oduksc_handle_t       *oduksc_handle; 
    
    /* Database handle */
    UINT32                 oduksc_db_handle;
    UINT32                 mpmo_db_handle;
    UINT32                 mpma_db_handle;
    
};




/*
** Global variables
*/

/*
** Function Prototypes
*/ 

/* LCOV_EXCL_START */
PUBLIC PMC_ERROR mapotn_db_chnl_rec_add(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl, 
                                        UINT32 num_cal_entries, 
                                        util_gen_db_id_t db_id);
                                         
PUBLIC PMC_ERROR mapotn_db_chnl_rec_rem(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl,
                                        util_gen_db_id_t db_id); 
/* LCOV_EXCL_END */                                                                                     
                                         
PUBLIC PMC_ERROR mapotn_db_empty_rec_get(mapotn_handle_t *mapotn_handle, 
                                         UINT32 num_cal_entries,
                                         util_gen_db_id_t db_id,
                                         UINT32 db_handle, 
                                         UINT32 *available_rec_array);                                        
PUBLIC PMC_ERROR mapotn_db_chnl_num_entries_get(mapotn_handle_t *mapotn_handle, 
                                                UINT32 chnl,                                                  
                                                util_gen_db_id_t db_id,
                                                UINT32 *num_cal_entries);

/* RGFPF */


/* TGFPF */

PUBLIC PMC_ERROR mapotn_tgfpf_prefetch_threshold_get(mapotn_handle_t *mapotn_handle,
                                                     UINT32 chnl,
                                                     UINT32 num_cal_entries,
                                                     UINT32 *prefetch_thresh);

PUBLIC PMC_ERROR mapotn_tgfpf_packing_threshold_get(mapotn_handle_t *mapotn_handle,
                                                    UINT32 chnl,
                                                    UINT32 num_cal_entries,
                                                    UINT32 *packing_low_thresh,
                                                    UINT32 *packing_high_thresh);
                                                        
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_mapping_cfg            ( mapotn_handle_t  *mapotn_handle,
                                                            UINT32            chnl,
                                                            mapotn_src_dest_t mapper_source );


#ifdef __cplusplus
}
#endif

#endif /* _MAPOTN_LOC_H */

/* 
** end of file 
*/

