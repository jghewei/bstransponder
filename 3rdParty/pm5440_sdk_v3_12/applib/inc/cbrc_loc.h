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
#ifndef _CBRC_LOC_H
#define _CBRC_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include <stdio.h>
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"

#include "cbrc.h"    
#include "util_global.h"
#include "mpma.h"
#include "mpmo.h"
#include "scbs3.h"
#include "cfc.h"
#include "sdh_pmg.h"
#include "cbr_fsgm.h"
#include "fc800_pmg.h"
#include "fc1200_pmon.h"

#include "cbrc_mtsb_map.h"
#include "cbrc_sdh_pmg_io.h"
#include "cbrc_sdh_pmg_mtsb_map.h"
#include "mldprbs_pcbi_io.h"
#include "fc1200_pcs_rx_io.h"
#include "fc1200_pcs_tx_io.h"
#include "fc_tttd_io.h"
#include "fc_tttm_io.h"
#include "cbr_int_io.h"
#include "cbr_dint_io.h"
#include "cbrc_fc_ilvr_io.h"
#include "cbrc_fc_dlvr_io.h"
#include "pkt_int_io.h"
#include "pkt_dint_io.h"
#include "cbrc_sfis_rx_io.h"
#include "cbrc_sfis_tx_io.h"
#include "cbrc_sfi51_io.h"
#include "mldprbs_pcbi_io.h"
#include "prgm_io.h"
#include "stli_768_io.h"
#include "stl256_io.h"


/*
** Enumerated Types
*/

/*
** Constants
*/

#define CBRC_SDH_PMG_MAX_INDEX               4
#define CBRC_SDH_PMG_MAX_SLICE               3
#define CBRC_CBR_FSGM_MAX_SLICE              3
#define CBRC_CBR_FSGM_MAX_INDEX              4
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: cbrc_prbs_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for defining CBRC RX and TX PRBS context.
*
* ELEMENTS:
*   rx_cfg_active - when TRUE RX configuration is valid and can be used
*   rx_cfg        - RX PRBS saved configuration
*   tx_cfg_active - when TRUE TX configuration is valid and can be used
*   tx_cfg        - TX PRBS saved configuration
*   tx_pn11_lof   - status of PN11 generator before PRBS generator activation
*                   PN11 generator will be restored after TX PRBS deactivation
*
*******************************************************************************/
typedef struct cbrc_prbs_ctxt_t
{
    BOOL8 rx_cfg_active;
    sdh_pmg_prbs_cfg_t rx_cfg;
    BOOL8 tx_cfg_active;
    sdh_pmg_prbs_cfg_t tx_cfg;    
    sdh_pmg_pn11_insert_t tx_pn11_lof;
} cbrc_prbs_ctxt_t;


/*******************************************************************************
* STRUCTURE: cbrc_link_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure representing a bidirectional 10G/40G link through the CBRC.
*
* ELEMENTS:
*   head        - Pointer to the primary link in a logical group of 4 links 
*                 for 40G ports
*   available   - TRUE if the link has not been provisioned
*   tx_active   - TRUE if the TX direction has been activated.
*   rx_active   - TRUE if the RX direction has been activated.
*   serdes      - Specifies 10G/40G serdes type
*   mode        - Configured mode for the link (e.g. OC-192, CPRI, etc).
*
*******************************************************************************/
typedef struct cbrc_link_info_t
{
    struct cbrc_link_info_t *head; /*!< See title block for description */
    
    BOOL                     available;
    BOOL                     tx_active;
    BOOL                     rx_active;
   
    cbrc_serdes_type_t       serdes;
    cbrc_port_mode_t         mode;
} cbrc_link_info_t;



/*******************************************************************************
* STRUCTURE: cbrc_schd_use_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SCBS3 configuration for CBRC schedulers
*
* ELEMENTS:
*   alloc_mode          - Scheduler type
*   db_id               - database id
*   db_client_handle    - Handle to database
*
*******************************************************************************/
typedef struct {

    util_schd_alloc_method_t    alloc_mode;
    util_gen_db_id_t            db_id;
    UINT32                      db_client_handle;

} cbrc_schd_use_ctxt_t;



/*******************************************************************************
* STRUCTURE: cbrc_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for inital values.
*
* ELEMENTS:
*   schd_use_ctxt   - SCBS3 configuration
*
*******************************************************************************/
typedef struct
{
    cbrc_schd_use_ctxt_t     schd_use_ctxt[CBRC_NUM_SCHD];
} cbrc_cfg_t;



/*******************************************************************************
* STRUCTURE: cbrc_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*   links               - Pool of links that can be provisioned 
*   cbrc_sdh_link_mask  - Bitmask tracking provisioned SDH links by slice and direction
*                         Used to be able to quickly determine which of the three
*                         slices within the SDH_PMG can be powered down.
*   prbs_ctxt           - Context for PRBS
*   loopback            - Tracks loopbacks within the CBRC
*   cbrc_start_state    - TRUE if all resources in start state
*
*******************************************************************************/
typedef struct
{
    cbrc_link_info_t    links[CBRC_MAX_LINK];
    UINT32              cbrc_sdh_link_mask; 
    cbrc_prbs_ctxt_t    prbs_ctxt[CBRC_MAX_LINK];
    cbrc_loopback_t     loopback[CBRC_MAX_LINK];
    BOOL8               cbrc_start_state;
} cbrc_var_t;




/*******************************************************************************
* STRUCTURE: cbrc_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context for warm/crash recovery
*   
*
* ELEMENTS: 
*   var         - Dynamic CBRC state
*   is_40g_dp   - TRUE if link is 40G
*   mode_fuzzy  - TRUE when cbrc_ctxt_reg_recover() is not able to determine
*                 the exact channel mode from the register space.
*                 In this case, cbrc_ctxt_audit() will accept channel
*                 configuration that does not match.
*   scbs3_page  - SCBS3 page contents
*
*******************************************************************************/    
typedef struct {
    cbrc_var_t      var;
    BOOL8           is_40g_dp[CBRC_MAX_LINK];
    BOOL8           mode_fuzzy[CBRC_MAX_LINK];
    scbs3_page_t    scbs3_page;

} cbrc_recover_var_t;




/*******************************************************************************
* STRUCTURE: cbrc_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining CBRC energy state
*
* ELEMENTS: 
*   See typedef.
*
*******************************************************************************/    
typedef struct cbrc_energy_state_struct_t {  
    pmc_energy_state_t mld_prbs[CBRC_MAX_LINK];   
    pmc_energy_state_t sdh[CBRC_MAX_LINK];
    pmc_energy_state_t fc800[CBRC_MAX_LINK];
    pmc_energy_state_t fc1200[CBRC_MAX_LINK];
    
} cbrc_energy_state_t;





/*******************************************************************************
* STRUCTURE: cbrc_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining CBRC handle information.
*
* ELEMENTS: 
*   base                - Handle base information
*   
*   cfg                 - Static config structure
*   var                 - Variable config structure
*
*   sdh_pmg_rx_ptr      - Handle for SDH_PMG (RX direction)
*   sdh_pmg_tx_ptr      - Handle for SDH PMG (TX direction)
*   cbr_fsgm_rx_ptr     - Handle for CBR FSGM (RX direction)
*   cbr_fsgm_tx_ptr     - Handle for CBR FSGM (TX direction)
*   fc800_pmg_rx_ptr    - Handle for FC800 PMG (RX direction)
*   fc800_pmg_tx_ptr    - Handle for FC800 PMG (TX direction)
*   fc1200_pmon_rx_ptr  - Handle for FC1200 PMON (RX direction)
*   fc1200_pmon_tx_ptr  - Handle for FC1200 PMON (TX direction)
*   mpma_ptr            - Handle for MPMA (RX direction)
*   mpmo_ptr            - Handle for MPMO (TX direction)
*   pkt_int_scbs_ptr    - Handle for SCBS3 (TX direction)
*
*******************************************************************************/    
struct cbrc_handle_struct_t {
    pmc_handle_t base;
    
    cbrc_cfg_t cfg;
    cbrc_var_t var;
       
    /* Handles for the 6 SDH PMG slices (3 RX, 3 TX) */
    sdh_pmg_handle_t          *sdh_pmg_rx_ptr[CBRC_SDH_PMG_MAX_SLICE];
    sdh_pmg_handle_t          *sdh_pmg_tx_ptr[CBRC_SDH_PMG_MAX_SLICE];

    /* Handles for 6 CBR_FSGM slices in (3 RX, 3 TX)*/
    cbr_fsgm_handle_t         *cbr_fsgm_rx_ptr[CBRC_CBR_FSGM_MAX_SLICE];
    cbr_fsgm_handle_t         *cbr_fsgm_tx_ptr[CBRC_CBR_FSGM_MAX_SLICE];

    /* Handles for RX and TX FC800_PMG */
    fc800_pmg_handle_t        *fc800_pmg_rx_ptr;
    fc800_pmg_handle_t        *fc800_pmg_tx_ptr;

    /* Handles for RX and TX FC1200_PMON */
    fc1200_pmon_handle_t      *fc1200_pmon_rx_ptr;
    fc1200_pmon_handle_t      *fc1200_pmon_tx_ptr;

    
    mpma_handle_t             *mpma_ptr;
    mpmo_handle_t             *mpmo_ptr;

    scbs3_handle_t            *pkt_int_scbs_ptr;
};



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR cbrc_link_get(cbrc_handle_t     *cbrc_handle_ptr,
                               UINT32             link,
                               cbrc_link_info_t **link_ptr);

PUBLIC PMC_ERROR cbrc_mld_prbs_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link);

PUBLIC PMC_ERROR cbrc_mld_prbs_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                        cbrc_port_dir_t   dir,
                                        UINT32 link);

PUBLIC PMC_ERROR cbrc_mld_prbs_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                          cbrc_port_dir_t   dir,
                                          UINT32 link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link);

#ifdef __cplusplus
}
#endif

#endif /* _CBRC_LOC_H */

/* 
** end of file 
*/

