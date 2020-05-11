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
*   DESCRIPTION:
*     This file contains functions related to the CBRC subsystem for handle
*     creation, datapath provisioning and activation, and block run time
*     configuration and OAMP.
*
*   NOTES:
*
*
*******************************************************************************/
/*
** Include Files
*/
#include "cbrc_loc.h"
#include "mld_prbs_ind.h"
#include "mldprbs_pcbi_io.h"
#include "prbs_mg_io.h"
#include "prgm_io.h"
#include "trmp_io.h"
#include "sdh_pmg_io.h"
#include "cbrc_sfis_rx_io.h"
#include "cbrc_sfis_tx_io.h"
#include "cbrc_sfi51_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define CBRC_TSB_NAME_LEN               20
#define CBRC_PRGM_MG_TIP_POLL_MAX_CNT   10000
#define CBRC_PRGM_MG_TIP_POLL_DELAY     10
#define CBRC_PRGM_PRBS_LFSR_NUM_STAGES  23
#define CBRC_NUM_STS_1_PATHS_MAX        12
#define CBRC_PKT_INT_ID_UNUSED          0xF



/*
** Local Macro Definitions
*/
#define CBRC_BASE_ADDR_SDH_PMG(A,B)     (BASE_ADDR_CBRC_CBRC_SDH_PMG_SDH_PMG_MTSB(0) + (A)*0x2000 + (B)*0x800)
#define CBRC_BASE_ADDR_CBR_FSGM_RX(A)   (BASE_ADDR_CBRC_CBRC_FC_ILVR_CBR_FSGM(A))
#define CBRC_BASE_ADDR_CBR_FSGM_TX(A)   (BASE_ADDR_CBRC_CBRFSGM_CBR_FSGM(A))


/*
** SDH_PMG Link Mask: tracks provisioned sdh links by direction and slice so that
** slices can be powered up and down as needed. There are 4 links per slice,
** 3 slices per direction, and 2 directions in the sdh_pmg macro. 
**
** Dir:          TX   TX   TX         RX   RX   RX
** Slice:         2    1    0          2    1    0
** Link:  ---- ---- ---- ----  ---- ---- ---- ----
** Bit:   (32)           (16)  (15)            (0)
** 
*/
#define LINK_MASK_DIR_OFFSET                                        16
#define LINK_MASK_SLICE_OFFSET                                      4

#define CBRC_SDH_LINK_MASK_SET(cbrc_sdh_link_mask, dir, slice, link) do { \
        cbrc_sdh_link_mask |=                                           \
            (1 << (dir * LINK_MASK_DIR_OFFSET)                          \
             << (slice * LINK_MASK_SLICE_OFFSET)                        \
             << link);                                                  \
    } while(0)

#define CBRC_SDH_LINK_MASK_CLR(cbrc_sdh_link_mask, dir, slice, link) do { \
        cbrc_sdh_link_mask &=                                           \
            ~(1 << (dir * LINK_MASK_DIR_OFFSET)                         \
              << (slice * LINK_MASK_SLICE_OFFSET)                       \
              << link);                                                 \
    } while(0)

#define IS_CBRC_SDH_SLICE_PROV(cbrc_handle, dir, slice) \
    (cbrc_handle->var.cbrc_sdh_link_mask &              \
     (0xF << (dir * LINK_MASK_DIR_OFFSET)               \
      << (slice * LINK_MASK_SLICE_OFFSET)))

#define IS_CBRC_SDH_PROV(cbrc_handle)           \
    (cbrc_handle->var.cbrc_sdh_link_mask)




/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS
/* 
* The following creates the LOG/ERR strings from above by pulling out the text
* portion of the LOG_ERR_TABLE above 
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char CBRC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    CBRC_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char CBRC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
PRIVATE PMC_ERROR cbrc_block_energy_state_set(cbrc_handle_t *cbrc_handle_ptr, 
                                              pmc_block_energy_state_enum block_energy_state) UNUSED;

PRIVATE PMC_ERROR cbrc_port_set(cbrc_handle_t       *cbrc_handle_ptr,
                                cbrc_link_info_t    *link_ptr,
                                cbrc_link_info_t    *link_info_ptr);


PRIVATE BOOL8 cbrc_port_is_avail(cbrc_handle_t      *cbrc_handle_ptr,
                                 cbrc_link_info_t   *link_ptr,
                                 cbrc_serdes_type_t  serdes_type);


PRIVATE BOOL8 cbrc_port_is_prov(cbrc_handle_t      *cbrc_handle_ptr,
                                cbrc_link_info_t   *link_ptr);

PRIVATE BOOL8 cbrc_link_is_head(cbrc_handle_t    *cbrc_handle_ptr,
                                cbrc_link_info_t *link_ptr);

PRIVATE PMC_ERROR cbrc_port_is_active(cbrc_handle_t      *cbrc_handle_ptr,
                                      cbrc_link_info_t   *link_ptr,
                                      cbrc_port_dir_t     dir);

PRIVATE BOOL8 cbrc_port_is_sonet(cbrc_port_mode_t  mode);
PRIVATE BOOL8 cbrc_port_is_fc800(cbrc_port_mode_t  mode);

PRIVATE cbrc_link_info_t* cbrc_link_get_next(cbrc_handle_t      *cbrc_handle_ptr,
                                             cbrc_link_info_t   *link_ptr);
                                             
PRIVATE void cbrc_scbs3_init(scbs3_cfg_t* scbs_cfg);                                             

PRIVATE void cbrc_db_all_init(cbrc_handle_t *cbrc_handle_ptr,
                              scbs3_cfg_t* scbs_cfg,
                              cbrc_handle_init_cfg_t cbrc_handle_init_cfg_ptr);

PRIVATE PMC_ERROR cbrc_sdh_pmg_get_handle(cbrc_handle_t      *cbrc_handle_ptr,
                                          cbrc_port_dir_t     dir,
                                          UINT32              link,
                                          sdh_pmg_handle_t  **sdh_handle_pptr,
                                          UINT32             *index_ptr);

PRIVATE PMC_ERROR cbrc_ctxt_reg_recover(cbrc_handle_t        *cbrc_handle, 
                                        pmc_energy_state_t    top_energy_state_reg,         
                                        cbrc_recover_var_t   *cbrc_var_reg,              
                                        cbrc_energy_state_t  *cbrc_energy_state_reg);

PRIVATE PMC_ERROR cbrc_ctxt_audit(cbrc_handle_t        *cbrc_handle,
                                  cbrc_recover_var_t   *cbrc_var_reg,
                                  cbrc_energy_state_t  *cbrc_energy_state_reg,
                                  cbrc_var_t           *cbrc_var_clean);

PRIVATE PMC_ERROR cbrc_ctxt_cleanup(cbrc_handle_t         *cbrc_handle,
                                    cbrc_recover_var_t    *cbrc_var_reg,
                                    cbrc_energy_state_t   *cbrc_energy_state_reg,
                                    cbrc_var_t            *cbrc_var_clean);
PRIVATE void cbrc_ctxt_mpmo_cleanup(cbrc_handle_t         *hndl,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PRIVATE void cbrc_serdes_type_get(cbrc_handle_t      *cbrc_handle,
                                  cbrc_serdes_type_t *serdes_type_ptr);

PRIVATE cbrc_loopback_t cbrc_loopback_mode_get(cbrc_handle_t            *cbrc_handle,
                                               UINT32                   link,
                                               BOOL8                    is_40g_dp);
PRIVATE PMC_ERROR cbrc_db_link_entry_audit(cbrc_handle_t     *cbrc_handle_ptr,
                                           cbrc_db_id_t       cbrc_db_id,
                                           scbs3_page_t      *scbs3_page_ptr,
                                           UINT32             link);

PRIVATE PMC_ERROR cbrc_sfis_slice_get(cbrc_handle_t  *cbrc_handle,
                                      UINT32          link,
                                      UINT8          *slice_id_ptr);

PRIVATE void cbrc_sfis_odd_parity_reset(cbrc_handle_t    *cbrc_handle,
                                        UINT8             slice);

/*
** Public Functions
*/

PUBLIC PMC_ERROR cbrc_mld_prbs_mode_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                        UINT32               link,
                                        cbrc_serdes_type_t   serdes_type);

PUBLIC PMC_ERROR cbrc_mld_prbs_clk_en(cbrc_handle_t       *cbrc_handle_ptr,
                                      UINT32              link,
                                      cbrc_serdes_type_t  serdes_type,
                                      BOOL8                enable);

PUBLIC PMC_ERROR cbrc_mld_prbs_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                    UINT32            link);

PUBLIC PMC_ERROR cbrc_mld_prbs_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link);

PUBLIC PMC_ERROR cbrc_mld_prbs_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                        cbrc_port_dir_t   dir,
                                        UINT32            link);

PUBLIC PMC_ERROR cbrc_mld_prbs_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                          cbrc_port_dir_t   dir,
                                          UINT32            link);

PRIVATE void cbrc_mld_prbs_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            num_link_to_clean,
                                   UINT32           *links);

PUBLIC PMC_ERROR cbrc_sdh_pmg_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link);
PUBLIC PMC_ERROR cbrc_sdh_pmg_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);
    
PUBLIC PMC_ERROR cbrc_fc_ilvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_ilvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link);

PRIVATE void cbrc_fc_ilvr_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                  UINT32            num_link_to_clean,
                                  UINT32           *links);

PUBLIC PMC_ERROR cbrc_fc_dlvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);

PUBLIC PMC_ERROR cbrc_fc_dlvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link);

PRIVATE void cbrc_fc_dlvr_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                  UINT32            num_link_to_clean,
                                  UINT32           *links);

PUBLIC DOUBLE cbrc_client_rate_get(cbrc_handle_t    *cbrc_handle_ptr,
                                   cbrc_port_mode_t  mode);

PRIVATE PMC_ERROR cbrc_sdh_prbs_mon_cfg(cbrc_handle_t         *handle,
                                        UINT8                 link,
                                        sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                        BOOL8                 prbs_enable);
PRIVATE PMC_ERROR cbrc_sdh_prbs_gen_cfg(cbrc_handle_t         *handle,
                                        UINT8                 link,
                                        sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                        BOOL8                 prbs_enable);
PRIVATE PMC_ERROR cbrc_link_and_dir_validate(cbrc_handle_t         *handle,
                                             cbrc_port_dir_t       dir,
                                             UINT32                link);

PRIVATE PMC_ERROR cbrc_prbs_rx_ctxt_push(cbrc_handle_t     *handle,
                                         UINT8             link,
                                         sdh_pmg_prbs_cfg_t *cfg_ptr);
PRIVATE PMC_ERROR cbrc_prbs_rx_ctxt_pop(cbrc_handle_t     *handle,
                                        UINT8             link,
                                        BOOL8 *are_all_prbs_off);
PRIVATE PMC_ERROR cbrc_prbs_tx_ctxt_push(cbrc_handle_t     *handle,
                                         UINT8             link,
                                         sdh_pmg_prbs_cfg_t *cfg_ptr);
PRIVATE PMC_ERROR cbrc_prbs_tx_ctxt_pop(cbrc_handle_t     *handle,
                                        UINT8             link,
                                        BOOL8 *are_all_prbs_off);

PRIVATE PMC_ERROR cbrc_sdh_prbs_err_cnt_get(cbrc_handle_t         *handle,
                                            UINT8                  link,
                                            UINT32                *err_cnt);

PRIVATE PMC_ERROR cbrc_prbs_enable_validate(cbrc_handle_t     *handle,
                                            BOOL8              is_tx_dir,
                                            BOOL8              oc_768_mode);

PRIVATE PMC_ERROR cbrc_prbs_rx_bringup_down(cbrc_handle_t         *handle,
                                            sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                            UINT32                 chnl_id,
                                            BOOL8                  oc768_mode,
                                            BOOL8                  enable);


PRIVATE PMC_ERROR cbrc_prbs_tx_bringup_down(cbrc_handle_t         *handle,
                                            sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                            UINT32                 chnl_id,
                                            BOOL8                  oc768_mode,
                                            BOOL8                  enable);

PRIVATE PMC_ERROR cbrc_int_init(cbrc_handle_t    *cbrc_handle,
                                BOOL8             enable);

PRIVATE PMC_ERROR cbrc_int_params_get(cbrc_handle_t        *cbrc_handle,
                                      cbrc_serdes_type_t    serdes_type,
                                      UINT32                link,
                                      BOOL8                *has_stl256,
                                      UINT32               *stl256_idx,
                                      BOOL8                *has_sfis,
                                      UINT32               *sfis_idx,
                                      BOOL8                *has_sfi51,
                                      UINT32               *sfi51_idx);

PRIVATE PMC_ERROR cbrc_stl256_int_chnl_enable(cbrc_handle_t          *cbrc_handle,
                                              UINT32                  stl256_link,
                                              cbrc_stl256_int_chnl_t *int_table_ptr,
                                              cbrc_stl256_int_chnl_t *int_en_table_ptr,
                                              BOOL                    enable);

PRIVATE PMC_ERROR cbrc_sfis_int_chnl_enable(cbrc_handle_t        *cbrc_handle,
                                            UINT32                sfis_link,
                                            cbrc_sfis_int_chnl_t *int_table_ptr,
                                            cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                            BOOL                  enable);

PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_enable(cbrc_handle_t         *cbrc_handle,
                                             UINT32                 sfi51_link,
                                             cbrc_sfi51_int_chnl_t *int_table_ptr,
                                             cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                             BOOL                   enable);

PRIVATE PMC_ERROR cbrc_stl256_int_chnl_clear(cbrc_handle_t          *cbrc_handle,
                                             UINT32                  stl256_link,
                                             cbrc_stl256_int_chnl_t *int_en_table_ptr,
                                             cbrc_stl256_int_chnl_t *int_table_ptr);

PRIVATE PMC_ERROR cbrc_sfis_int_chnl_clear(cbrc_handle_t        *cbrc_handle,
                                           UINT32                sfis_link,
                                           cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                           cbrc_sfis_int_chnl_t *int_table_ptr);

PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_clear(cbrc_handle_t         *cbrc_handle,
                                            UINT32                 sfi51_link,
                                            cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                            cbrc_sfi51_int_chnl_t *int_table_ptr);

PRIVATE PMC_ERROR cbrc_stl256_int_chnl_retrieve(cbrc_handle_t           *cbrc_handle,
                                                UINT32                   stl256_link,
                                                cbrc_stl256_int_chnl_t  *filt_table_ptr,
                                                cbrc_stl256_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR cbrc_sfis_int_chnl_retrieve(cbrc_handle_t         *cbrc_handle,
                                              UINT32                 sfis_link,
                                              cbrc_sfis_int_chnl_t  *filt_table_ptr,
                                              cbrc_sfis_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_retrieve(cbrc_handle_t          *cbrc_handle,
                                               UINT32                  sfi51_link,
                                               cbrc_sfi51_int_chnl_t  *filt_table_ptr,
                                               cbrc_sfi51_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR cbrc_stl256_int_chnl_enabled_check(cbrc_handle_t          *cbrc_handle,
                                                     UINT32                  stl256_link,
                                                     cbrc_stl256_int_chnl_t *int_en_table_ptr,
                                                     BOOL                   *int_found_ptr);

PRIVATE PMC_ERROR cbrc_sfis_int_chnl_enabled_check(cbrc_handle_t        *cbrc_handle,
                                                   UINT32                sfis_link,
                                                   cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                                   BOOL                 *int_found_ptr);

PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_enabled_check(cbrc_handle_t        *cbrc_handle,
                                                    UINT32                sfi51_link,
                                                    cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                                    BOOL                  *int_found_ptr);

PRIVATE void cbrc_prbs_deprov(cbrc_handle_t     *handle,
                              UINT8             link);

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* cbrc_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a CBRC block instance.
*
*
* INPUTS:
*   *parent             - pointer to parent handle.  If no parent pass in
*                         NULL
*   base_address        - base address of the CBRC subsystem relative to 
*                         the device memory space
*   *sys_handle         - pointer to user system handle
*   *tsb_name           - Name of subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cbrc_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC cbrc_handle_t *cbrc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    cbrc_handle_t   *cbrc_handle_ptr;

    const char      *CBRC_SDH_PMG_NAME_TX_1      = "SDH PMG TX 1";
    const char      *CBRC_SDH_PMG_NAME_TX_2      = "SDH PMG TX 2";
    const char      *CBRC_SDH_PMG_NAME_TX_3      = "SDH PMG TX 3";
    const char      *CBRC_SDH_PMG_NAME_RX_1      = "SDH PMG RX 1";
    const char      *CBRC_SDH_PMG_NAME_RX_2      = "SDH PMG RX 2";
    const char      *CBRC_SDH_PMG_NAME_RX_3      = "SDH PMG RX 3";
    
    const char      *CBRC_CBR_FSGM_NAME_TX_1     = "CBR FSGM TX 1";
    const char      *CBRC_CBR_FSGM_NAME_TX_2     = "CBR FSGM TX 2";
    const char      *CBRC_CBR_FSGM_NAME_TX_3     = "CBR FSGM TX 3";
    const char      *CBRC_CBR_FSGM_NAME_RX_1     = "CBR FSGM RX 1";
    const char      *CBRC_CBR_FSGM_NAME_RX_2     = "CBR FSGM RX 2";
    const char      *CBRC_CBR_FSGM_NAME_RX_3     = "CBR FSGM RX 3";
    
    const char      *CBRC_FC800_PMG_NAME_TX      = "FC800 PMG TX";
    const char      *CBRC_FC800_PMG_NAME_RX      = "FC800 PMG RX";
    
    const char      *CBRC_FC1200_PMON_NAME_TX    = "FC1200 PMON TX";
    const char      *CBRC_FC1200_PMON_NAME_RX    = "FC1200 PMON RX";
    
    PMC_ENTRY();

    cbrc_handle_ptr = (cbrc_handle_t*)PMC_CTXT_CALLOC(sizeof(cbrc_handle_t), parent);

    pmc_handle_init(parent, cbrc_handle_ptr, sys_handle, PMC_MID_DIGI_CBRC, tsb_name, base_address); /* consider bring out for DIGI variants 60G/META) */

    /* Create RX SDH_PMG contexts */
    cbrc_handle_ptr->sdh_pmg_rx_ptr[0] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(0, 0),
                            sys_handle, CBRC_SDH_PMG_NAME_RX_1);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_rx_ptr[0], parent);
    cbrc_handle_ptr->sdh_pmg_rx_ptr[1] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(0, 1),
                            sys_handle, CBRC_SDH_PMG_NAME_RX_2);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_rx_ptr[1], parent);
    cbrc_handle_ptr->sdh_pmg_rx_ptr[2] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(0, 2),
                            sys_handle, CBRC_SDH_PMG_NAME_RX_3);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_rx_ptr[2], parent);

    /* Create TX SDH_PMG contexts */
    cbrc_handle_ptr->sdh_pmg_tx_ptr[0] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(1, 0),
                            sys_handle, CBRC_SDH_PMG_NAME_TX_1);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_tx_ptr[0], parent);
    cbrc_handle_ptr->sdh_pmg_tx_ptr[1] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(1, 1),
                            sys_handle, CBRC_SDH_PMG_NAME_TX_2);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_tx_ptr[1], parent);
    cbrc_handle_ptr->sdh_pmg_tx_ptr[2] =
        sdh_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_SDH_PMG(1, 2),
                            sys_handle, CBRC_SDH_PMG_NAME_TX_3);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->sdh_pmg_tx_ptr[2], parent);

    /* Create RX CBR FSGM contexts */
    cbrc_handle_ptr->cbr_fsgm_rx_ptr[0] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_RX(0),
                             sys_handle, CBRC_CBR_FSGM_NAME_RX_1);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_rx_ptr[0], parent);
    cbrc_handle_ptr->cbr_fsgm_rx_ptr[1] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_RX(1),
                             sys_handle, CBRC_CBR_FSGM_NAME_RX_2);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_rx_ptr[1], parent);
    cbrc_handle_ptr->cbr_fsgm_rx_ptr[2] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_RX(2),
                             sys_handle, CBRC_CBR_FSGM_NAME_RX_3);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_rx_ptr[2], parent);
    
    
    /* Create TX CBR FSGM contexts */
    cbrc_handle_ptr->cbr_fsgm_tx_ptr[0] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_TX(0),
                             sys_handle, CBRC_CBR_FSGM_NAME_TX_1);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_tx_ptr[0], parent);
    cbrc_handle_ptr->cbr_fsgm_tx_ptr[1] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_TX(1),
                             sys_handle, CBRC_CBR_FSGM_NAME_TX_2);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_tx_ptr[1], parent);
    cbrc_handle_ptr->cbr_fsgm_tx_ptr[2] =
        cbr_fsgm_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+CBRC_BASE_ADDR_CBR_FSGM_TX(2),
                             sys_handle, CBRC_CBR_FSGM_NAME_TX_3);    
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->cbr_fsgm_tx_ptr[2], parent);


    /* Create RX/TX FC800_PMG contexts */
    cbrc_handle_ptr->fc800_pmg_rx_ptr = 
        fc800_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_ILVR_FC800_PMG_MTSB(0),
                              sys_handle, CBRC_FC800_PMG_NAME_RX);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->fc800_pmg_rx_ptr, parent);
    cbrc_handle_ptr->fc800_pmg_tx_ptr =
        fc800_pmg_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_DLVR_FC800_PMG_MTSB(0),
                              sys_handle, CBRC_FC800_PMG_NAME_TX);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->fc800_pmg_tx_ptr, parent);


    /* Create RX/TX FC1200_PMON contexts */
    cbrc_handle_ptr->fc1200_pmon_rx_ptr = 
        fc1200_pmon_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_ILVR_FC1200_PTE_FC1200_PMON(0),
                                sys_handle, CBRC_FC1200_PMON_NAME_RX);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->fc1200_pmon_rx_ptr, parent);
    cbrc_handle_ptr->fc1200_pmon_tx_ptr =
        fc1200_pmon_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC1200_PMON(0),
                                sys_handle, CBRC_FC1200_PMON_NAME_TX);
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->fc1200_pmon_tx_ptr, parent);



    /* Create MPMA/MPMO contexts */
    cbrc_handle_ptr->mpma_ptr = mpma_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_ILVR_MPMA_MTSB, sys_handle, "MPMA");
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->mpma_ptr, parent);
    cbrc_handle_ptr->mpmo_ptr = mpmo_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_DLVR_MPMO_MTSB, sys_handle, "MPMO");
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->mpmo_ptr, parent);
    

    /* Create SCBS3 context */
    cbrc_handle_ptr->pkt_int_scbs_ptr = scbs3_ctxt_create((pmc_handle_t *)cbrc_handle_ptr, base_address+BASE_ADDR_CBRC_CBRC_FC_ILVR_PKT_INT_SCBS3, sys_handle, "PKT_INT SCBS2");
    PMC_CTXT_REGISTER_PTR(&cbrc_handle_ptr->pkt_int_scbs_ptr, parent);


    /* Register log strings */
    pmc_log_block_strings_register(CBRC_LOG_ERR_STRINGS[0], CBRC_LOG_ERR_TABLE_BASE, CBRC_LOG_ERR_COUNT);

    PMC_RETURN(cbrc_handle_ptr);

} /* cbrc_ctxt_create */

 
/*******************************************************************************
* cbrc_ctxt_destroy
* _____________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an CBRC block instance.
*
*
* INPUTS:
*   *cbrc_handle_ptr         - pointer to CBRC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void cbrc_ctxt_destroy(cbrc_handle_t *cbrc_handle_ptr)
{
    UINT32  i;

    PMC_ENTRY();
    
    pmc_handle_mid_verify(cbrc_handle_ptr, PMC_MID_DIGI_CBRC);
    cbrc_handle_ptr->base.tsb_mid = PMC_MID_INVALID;

    /* detach from database if database used */
    for (i = 0; i < CBRC_NUM_SCHD; i++) /* so only scbs3 db is init here. MPMO db is taken care in mpmo_handle_init() */
    {
        if(cbrc_handle_ptr->cfg.schd_use_ctxt[i].alloc_mode == UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB ||
           cbrc_handle_ptr->cfg.schd_use_ctxt[i].alloc_mode == UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
        {
            gen_db_detach(&(cbrc_handle_ptr->base), 
                          cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_id,
                          cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_client_handle);
        }
    }

    /* Free SDH_PMG slices */
    for (i = 0; i <= MAX_CBRC_CBRC_SDH_PMG_SDH_PMG_MTSB; i++)
    {
        sdh_pmg_ctxt_destroy(cbrc_handle_ptr->sdh_pmg_rx_ptr[i]);
        sdh_pmg_ctxt_destroy(cbrc_handle_ptr->sdh_pmg_tx_ptr[i]);
    }
        
    /* Free CBR_FSGM contexts */
    for (i = 0; i < CBRC_CBR_FSGM_MAX_SLICE; i++)
    {
        cbr_fsgm_ctxt_destroy(cbrc_handle_ptr->cbr_fsgm_rx_ptr[i]);
        cbr_fsgm_ctxt_destroy(cbrc_handle_ptr->cbr_fsgm_tx_ptr[i]);
    } 
        
    /* Free FC800_PMG contexts */
    fc800_pmg_ctxt_destroy(cbrc_handle_ptr->fc800_pmg_rx_ptr);
    fc800_pmg_ctxt_destroy(cbrc_handle_ptr->fc800_pmg_tx_ptr);
        
    /* Free FC1200_PMON contexts */
    fc1200_pmon_ctxt_destroy(cbrc_handle_ptr->fc1200_pmon_rx_ptr);
    fc1200_pmon_ctxt_destroy(cbrc_handle_ptr->fc1200_pmon_tx_ptr);

    mpma_ctxt_destroy(cbrc_handle_ptr->mpma_ptr);
    mpmo_ctxt_destroy(cbrc_handle_ptr->mpmo_ptr);

   
    scbs3_ctxt_destroy(cbrc_handle_ptr->pkt_int_scbs_ptr);

    PMC_CTXT_FREE(&cbrc_handle_ptr, cbrc_handle_ptr);
    PMC_RETURN();
} /* cbrc_ctxt_destroy */


/*******************************************************************************
* FUNCTION: cbrc_var_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the CBRC dynamic context to the starting state after reset. 
*
* INPUTS:
*   *cbrc_handle_ptr - pointer to CBRC handle instance
*   var_ptr          - Pointer to CBRC variable struct to initialize
*    do_register     - when TRUE, link head is registered in context, otherwise it is not.                  
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_var_init(cbrc_handle_t  *cbrc_handle_ptr,
                                cbrc_var_t     *var_ptr,
                                BOOL8           do_register)
{
    PMC_ERROR     result = PMC_SUCCESS;
    UINT8         i;


    PMC_ENTRY();

    /* Initialize link information */
    for (i = 0; i < CBRC_MAX_LINK; i++)
    {
        if (TRUE == do_register)
        {
            PMC_CTXT_REGISTER_PTR(&var_ptr->links[i].head, cbrc_handle_ptr);
        }

        var_ptr->links[i].tx_active  = FALSE; 
        var_ptr->links[i].rx_active  = FALSE; 
        var_ptr->links[i].available  = TRUE;
        var_ptr->links[i].head       = NULL;
        var_ptr->links[i].mode       = CBRC_PORT_MODE_NONE;
        var_ptr->links[i].serdes     = CBRC_SERDES_TYPE_NONE;
        var_ptr->loopback[i] = CBRC_DISABLE_LOOPBACK;
        var_ptr->prbs_ctxt[i].tx_pn11_lof = LAST_SDH_PMG_PN11_INSERT;    
    }

    PMC_RETURN(result);
} /* cbrc_var_init */



/*******************************************************************************
* cbrc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CBRC block handle instance.
*
*
* INPUTS:
*   *cbrc_handle_ptr            - pointer to CBRC handle instance
*   *cbrc_handle_init_cfg_ptr   - defining the DCS scheduler mode for CBRC schedulers 
*                                 0: scheduler mode for scbs3
*                                 1: scheduler mode for mpmo
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void cbrc_handle_init(cbrc_handle_t              *cbrc_handle_ptr,
                             cbrc_handle_init_cfg_t     *cbrc_handle_init_cfg_ptr)
{
    UINT32         i;
    scbs3_cfg_t    scbs_cfg;
    
    PMC_ENTRY();

    /*initializes the CBRC schedulers with local/db mode */
    cbrc_db_all_init(cbrc_handle_ptr, &scbs_cfg, *cbrc_handle_init_cfg_ptr);

    /* Initialize CBRC dynamic context info */
    cbrc_var_init(cbrc_handle_ptr, &cbrc_handle_ptr->var, TRUE);

    /* Initialize SDH_PMG slices */
    for (i = 0; i <= MAX_CBRC_CBRC_SDH_PMG_SDH_PMG_MTSB; i++)
    {
        sdh_pmg_handle_init(cbrc_handle_ptr->sdh_pmg_tx_ptr[i]);
        sdh_pmg_handle_init(cbrc_handle_ptr->sdh_pmg_rx_ptr[i]);
    }
   
    /* Initialize CBR_FSGM contexts */
    for (i = 0; i < CBRC_CBR_FSGM_MAX_SLICE; i++)
    {
        cbr_fsgm_handle_init(cbrc_handle_ptr->cbr_fsgm_rx_ptr[i]);
        cbr_fsgm_handle_init(cbrc_handle_ptr->cbr_fsgm_tx_ptr[i]);
    } 


    /* Initialize FC800_PMG contexts */
    fc800_pmg_handle_init(cbrc_handle_ptr->fc800_pmg_rx_ptr);
    fc800_pmg_handle_init(cbrc_handle_ptr->fc800_pmg_tx_ptr);

    /* Initialize FC1200_PMON contexts */
    fc1200_pmon_handle_init(cbrc_handle_ptr->fc1200_pmon_rx_ptr);
    fc1200_pmon_handle_init(cbrc_handle_ptr->fc1200_pmon_tx_ptr);



    /* Initialize MPMA */
    mpma_handle_init(cbrc_handle_ptr->mpma_ptr, MPMA_CBRC, 0);  

    /* Initialize MPMO */   
    if(cbrc_handle_ptr->cfg.schd_use_ctxt[1].alloc_mode == UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
    {
        mpmo_handle_init(cbrc_handle_ptr->mpmo_ptr, MPMO_CBRC_DB, 0);
    } else {
        mpmo_handle_init(cbrc_handle_ptr->mpmo_ptr, MPMO_CBRC, 0);
    }

    /* Initialize SCBS3 */
    cbrc_scbs3_init(&scbs_cfg);
    
    scbs3_handle_init(cbrc_handle_ptr->pkt_int_scbs_ptr, &scbs_cfg);

    PMC_RETURN();
} /* cbrc_handle_init */


/*******************************************************************************
* cbrc_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate any local memory freed in ctxt_destroy
*
*
* INPUTS:
*   *cbrc_handle_ptr            - pointer to CBRC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void cbrc_alloc_init(cbrc_handle_t *cbrc_handle_ptr)
{
    PMC_ENTRY();

    mpma_alloc_init(cbrc_handle_ptr->mpma_ptr);
    mpmo_alloc_init(cbrc_handle_ptr->mpmo_ptr);

    scbs3_alloc_init(cbrc_handle_ptr->pkt_int_scbs_ptr);

    PMC_RETURN();
} /* cbrc_alloc_init */

/*******************************************************************************
* cbrc_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns CBRC link provisioning status.
*
*
* INPUTS:
*   *cbrc_handle_ptr            - pointer to CBRC handle instance
*   link                        - CBRC link
*   rx_dir                      - when TRUE, link RX proviosned status is returned, otherwise it is link TX status
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUe when link is provisioned, otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_prov_get(cbrc_handle_t *cbrc_handle_ptr,
                           UINT32         link,
                           BOOL8          rx_dir)
{
    BOOL8 rc;

    PMC_ENTRY();

    rc = cbrc_handle_ptr->var.links[link].available == FALSE;

    rc &= (TRUE == rx_dir ? cbrc_handle_ptr->var.links[link].rx_active : cbrc_handle_ptr->var.links[link].tx_active);

    PMC_RETURN(rc);
}/* cbrc_prov_get */

/*******************************************************************************
* cbrc_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an CBRC block handle instance.
*
*
* INPUTS:
*   *cbrc_handle_ptr            - pointer to CBRC handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state            - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   CBRC_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put CBRC in RESET mode
*              subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_handle_restart_init(cbrc_handle_t *cbrc_handle_ptr,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                          pmc_energy_state_t top_energy_state)
{
    cbrc_recover_var_t   *cbrc_var_reg   = NULL;
    cbrc_energy_state_t   cbrc_energy_state_reg;
    cbrc_var_t           *cbrc_var_clean = NULL;
    BOOL8          mismatch_channels;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(CBRC_LOG_ERR_STRINGS[0], CBRC_LOG_ERR_TABLE_BASE, CBRC_LOG_ERR_COUNT);
    }

    cbrc_var_reg = (cbrc_recover_var_t*) PMC_CALLOC(sizeof(cbrc_recover_var_t));
    
    if (NULL == cbrc_var_reg) 
    {
        rc = CBRC_ERR_NULL_PARAMETERS;
    }

    /* reinitialize MPMA */
    rc = mpma_handle_restart_init(cbrc_handle_ptr->mpma_ptr, restart_init_cfg_ptr, top_energy_state, TRUE, &mismatch_channels);  

    /* reinitialize MPMO */
    if (PMC_SUCCESS == rc)
    {
        rc = mpmo_handle_restart_init(cbrc_handle_ptr->mpmo_ptr, restart_init_cfg_ptr, top_energy_state, TRUE, &mismatch_channels);
    }

    if (PMC_SUCCESS == rc)
    {
        /* 
           Recover context from register space        
           - var context is partially recovered from register space (good enough to have a valid audit)
           - only OPERATIONAL channel configruation are recovered
           - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
           - detailed energy states are recovered
           - incoherency in register context means that the subsystem shall be shutdown
        */
        rc = cbrc_ctxt_reg_recover(cbrc_handle_ptr,
                                   top_energy_state,
                                   cbrc_var_reg,
                                   &cbrc_energy_state_reg);
        
        
        if (PMC_SUCCESS == rc)
        {
            cbrc_var_clean = (cbrc_var_t*) PMC_CALLOC(sizeof(cbrc_var_t));
        
            if(NULL == cbrc_var_clean) 
            {
                rc = CBRC_ERR_NULL_PARAMETERS;
            }
            
            if (PMC_SUCCESS == rc)
            {
                /* 
                 * Audit register context and memory context 
                 *   - Audit power and decide if we should just shut down the subsystem 
                 *   - Create a new context that represents the intersection between memory and register space (cbrc_var_clean)
                 */
                rc = cbrc_ctxt_audit(cbrc_handle_ptr, 
                                     cbrc_var_reg,
                                     &cbrc_energy_state_reg,
                                     cbrc_var_clean);                 

                PMC_LOG_TRACE("cbrc_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
                if (CBRC_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
                {
                    /* 
                     * Cleanup 
                     *   - At the end, cbrc_var_clean is the new context
                     *    - We keep OPERATIONAL channel defined in cbrc_var_clean
                     *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
                     *       - they may have be partially provisionned
                     *    - We ajust power setting based on cbrc_var_clean
                     */                                                
                    rc = cbrc_ctxt_cleanup(cbrc_handle_ptr, 
                                           cbrc_var_reg,
                                           &cbrc_energy_state_reg,
                                           cbrc_var_clean); 

                }
                /* 
                 * in second phase, we clean MPMO channels associated to OPERATIONAL channels 
                 * that are unconnected to the wwith shall be cleaned. MPMO are provisionned when we connect 
                 * the segment the switch 
                 *  
                 */
                if (PMC_SUCCESS == rc && restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_SECOND)
                {
                    cbrc_ctxt_mpmo_cleanup(cbrc_handle_ptr,restart_init_cfg_ptr);
                }                
                if ( NULL != cbrc_var_clean)
                {
                    PMC_FREE(&cbrc_var_clean);
                }
            }
        }
    }

    if (NULL != cbrc_var_reg)
    {
        PMC_FREE(&cbrc_var_reg);
    }

    PMC_RETURN(rc);
} /* cbrc_handle_restart_init */

/*******************************************************************************
*  cbrc_ctxt_mpmo_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure CBRC MPMO for OPERATIONAL channels that are 
*   unconnected to the switch 
*
* INPUTS:
*   *hndl                  - handle to the CBRC instance to be operated on containing 
*   *restart_init_cfg_ptr  - pointer to restart init config
* 
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if operation is successfully executed, 
*   otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_ctxt_mpmo_cleanup(cbrc_handle_t                  *hndl,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{

    UINT32 link_itr = 0;
    UINT32 link_clean[CBRC_MAX_LINK];
    UINT32 num_link_clean = 0;
    UINT32     egress_port = (UINT32) UTIL_GLOBAL_CPB_PORT_CBRC;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, CBRC_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(restart_init_cfg_ptr->dcpb_connect!=NULL, CBRC_ERR_INVALID_ARG, 0, 0);
        
    for(link_itr = 0;link_itr < CBRC_MAX_LINK;link_itr++)
    {
        if (hndl->var.links[link_itr].tx_active == TRUE)
        {
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->dcpb_connect->chnl[egress_port][link_itr].mst.state)
            {
                link_clean[num_link_clean] = link_itr;
                num_link_clean += 1;
            }
        }
    }
    if (num_link_clean != 0)
    {
        mpmo_ch_clean(hndl->mpmo_ptr, &link_clean[0],num_link_clean );
    }
    
    PMC_RETURN();
    
} /* cbrc_ctxt_mpmo_cleanup */








/*******************************************************************************
* cbrc_db_entry_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method for the datapath layer to write to the
*   databases that are in use by the schedulers in CBRC.
*
* INPUTS: nb
*   *cbrc_handle_ptr       - pointer to CBRC handle instance
*   cbrc_db_id             - CBRC_DB_ID_PKT_INT_MPMA or CBRC_DB_ID_MPMO
*   schd_addr              - Scheduler calendar slot to program channel_id
*   schd_channel_id        - Channel id 
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_db_entry_set(cbrc_handle_t *cbrc_handle_ptr,
                                   cbrc_db_id_t   cbrc_db_id,
                                   UINT32         schd_addr,
                                   UINT32         schd_channel_id)
{
    mapotn_db_rec rec;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT( cbrc_db_id < LAST_CBRC_DB_ID, CBRC_ERR_INVALID_ARG, cbrc_db_id, LAST_CBRC_DB_ID);
    PMC_ASSERT( schd_addr < 96 , CBRC_ERR_INVALID_ARG, 0 , 0);
    
    if (schd_channel_id > UTIL_GLOBAL_CBRC_MAX_SLOTS)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    } 

    switch(cbrc_db_id) 
    {
    case CBRC_DB_ID_PKT_INT_MPMA: /* set the scbs3 db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].alloc_mode != UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* write schd_addr and schd_channel_id to the data base for
               the specified dcs_inst */
            rec.channel_num = schd_channel_id;
            rec.calendar_entry = schd_addr;

            result = gen_db_add_record(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id, 
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_client_handle, 
                                       (void *)&rec);
        }
        break;
    case CBRC_DB_ID_MPMO: /* set the mpmo db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->mpmo_ptr->cfg.default_param.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* write schd_addr and schd_channel_id to the data base for
               the specified dcs_inst */
            rec.channel_num = schd_channel_id;
            rec.calendar_entry = schd_addr;

            result = gen_db_add_record(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_id , 
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_handle, 
                                       (void *)&rec);
        }
        break;
    default:
        break;
    }
    PMC_RETURN(result);

} /* cbrc_db_entry_set */




/*******************************************************************************
* cbrc_db_entry_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method for the datapath layer to clear the
*   entries of databases that are in use by the schedulers in CBRC.  The value 0xFFFFFFFF 
*   is the unused/cleared value for db content.
*
*
* INPUTS:
*   *cbrc_handle_ptr       - pointer to CBRC handle instance
*   cbrc_db_id             - CBRC_DB_ID_PKT_INT_MPMA or CBRC_DB_ID_MPMO
*   schd_channel_id        - Scheduler channel number to clear
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_db_entry_clear(cbrc_handle_t *cbrc_handle_ptr,
                                     cbrc_db_id_t   cbrc_db_id,
                                     UINT32         schd_channel_id )
{
    PMC_ERROR result = PMC_SUCCESS;
    mapotn_db_rec rec;
    
    PMC_ENTRY();    

    PMC_ASSERT( cbrc_db_id < LAST_CBRC_DB_ID, CBRC_ERR_INVALID_ARG, cbrc_db_id, LAST_CBRC_DB_ID);
    
    if (schd_channel_id > UTIL_GLOBAL_CBRC_MAX_SLOTS)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    } 
    
    switch(cbrc_db_id) 
    {
    case CBRC_DB_ID_PKT_INT_MPMA: /* set the scbs3 db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].alloc_mode != UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* clear the database entry for the rd_channel ID supplied */
            rec.channel_num = schd_channel_id; 
            rec.calendar_entry = DO_NOT_USE_KEY;

            result = gen_db_rem_record(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id, 
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_client_handle, 
                                       (void *)&rec);

        }
        break;
    case CBRC_DB_ID_MPMO: /* set the mpmo db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->mpmo_ptr->cfg.default_param.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* clear the database entry for the rd_channel ID supplied */
            rec.channel_num = schd_channel_id; 
            rec.calendar_entry = DO_NOT_USE_KEY;

            result = gen_db_rem_record(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_id , 
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_handle,  
                                       (void *)&rec);

        }
        break;
    default:
        break;
    }
    PMC_RETURN(result);

} /* cbrc_db_entry_clear */

/*******************************************************************************
* cbrc_db_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method for the datapath layer to retrieve the
*   entries of databases that are in use by the schedulers in CBRC.
*
*
* INPUTS:
*   *cbrc_handle_ptr       - pointer to CBRC handle instance
*   cbrc_db_id             - CBRC_DB_ID_PKT_INT_MPMA or CBRC_DB_ID_MPMO
*   schd_addr              - Scheduler calendar entry
*
* OUTPUTS:
*   schd_channel_id_ptr    - Channel number assigned to calendar entry
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_db_entry_get(cbrc_handle_t *cbrc_handle_ptr,
                                   cbrc_db_id_t   cbrc_db_id,
                                   UINT32         schd_addr,
                                   UINT32        *schd_channel_id_ptr)                                   
{
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 num_of_recs = 0;
    mapotn_db_key   key;
    mapotn_db_query_res *q_result = NULL;
        
    PMC_ENTRY();

    PMC_ASSERT( cbrc_db_id < LAST_CBRC_DB_ID, CBRC_ERR_INVALID_ARG, cbrc_db_id, LAST_CBRC_DB_ID);
    PMC_ASSERT( schd_addr < 96 , CBRC_ERR_INVALID_ARG, 0 , 0);
    
    switch(cbrc_db_id) 
    {
    case CBRC_DB_ID_PKT_INT_MPMA: /* set the scbs3 db */      
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].alloc_mode != UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* query schd_channel_id data for the schd_addr */
            key.calendar_entry = schd_addr;
            key.channel_num = DO_NOT_USE_KEY;
            num_of_recs = gen_db_query(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id, 
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_client_handle, 
                                       (void *)&key,
                                       (void **)&q_result);
            if(num_of_recs == 0)
            {
                PMC_RETURN(CBRC_ERR_DB_ENTRY_NOT_FOUND);
            }

            PMC_LOG_TRACE("CPB DB Element: schd_addr=0x%x,cal schd_channel_id=0x%x,\n",
                          q_result->p_recs[0]->calendar_entry, q_result->p_recs[0]->channel_num);          
                           
            *schd_channel_id_ptr = q_result->p_recs[0]->channel_num;                         
        }
        break;
    case CBRC_DB_ID_MPMO: /* set the mpmo db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->mpmo_ptr->cfg.default_param.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            /* query schd_channel_id data for the schd_addr */
            key.calendar_entry = schd_addr;
            key.channel_num = DO_NOT_USE_KEY;
            num_of_recs = gen_db_query(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_id , 
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_handle, 
                                       (void *)&key,
                                       (void **)&q_result);
            if(num_of_recs == 0)
            {
                PMC_RETURN(CBRC_ERR_DB_ENTRY_NOT_FOUND);
            }

            PMC_LOG_TRACE("CPB DB Element: schd_addr=0x%x,cal schd_channel_id=0x%x,\n",
                          q_result->p_recs[0]->calendar_entry, q_result->p_recs[0]->channel_num);          
                           
            *schd_channel_id_ptr = q_result->p_recs[0]->channel_num;                         
        }
        break;
    default:
        break;
    }
    PMC_RETURN(result);
} /* cbrc_db_entry_get */

/*******************************************************************************
* cbrc_db_entry_all_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method for the datapath layer to retrieve the
*   all the entries of databases that are in use by the schedulers in CBRC.
*
* INPUTS:
*   *cbrc_handle_ptr       - pointer to CBRC handle instance
*   cbrc_db_id             - CBRC_DB_ID_PKT_INT_MPMA or CBRC_DB_ID_MPMO
*
* OUTPUTS:
*   schd_channel_id_ptr     - 96 element array that function will populate with
*                           schd_channel_id
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_db_entry_all_get(cbrc_handle_t *cbrc_handle_ptr,
                                       cbrc_db_id_t   cbrc_db_id,
                                       UINT32         schd_channel_id_ptr[CALENDAR_ENTRIES_NUM])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, num_of_recs;
    mapotn_db_key   key;
    mapotn_db_query_res *q_result = NULL;
    
    PMC_ENTRY();
    
    switch(cbrc_db_id) 
    {
    case CBRC_DB_ID_PKT_INT_MPMA: /* set the scbs3 db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].alloc_mode != UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else 
        {
            key.calendar_entry = CALENDAR_ENTRIES_NUM;
            key.channel_num = DO_NOT_USE_KEY;
            num_of_recs = gen_db_query(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id, 
                                       cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_client_handle, 
                                       (void *)&key,
                                       (void **)&q_result);
            if(num_of_recs == 0)
            {
                PMC_RETURN(CBRC_ERR_DB_ENTRY_NOT_FOUND);
            }
                    
            for(i=0; i < 96; i++)
            {
                schd_channel_id_ptr[i] = q_result->p_recs[i]->channel_num;
            }
        }
        break;
    case CBRC_DB_ID_MPMO: /* set the mpmo db */
        /* if data base does not exist return an error */
        if(cbrc_handle_ptr->mpmo_ptr->cfg.default_param.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB)
        {
            result = CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE;
        } else
        {
            key.calendar_entry = CALENDAR_ENTRIES_NUM;
            key.channel_num = DO_NOT_USE_KEY;
            num_of_recs = gen_db_query(&(cbrc_handle_ptr->base),
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_id , 
                                       cbrc_handle_ptr->mpmo_ptr->cfg.default_param.db_handle, 
                                       (void *)&key,
                                       (void **)&q_result);

            if(num_of_recs == 0)
            {
                PMC_RETURN(CBRC_ERR_DB_ENTRY_NOT_FOUND);
            }
                
            for(i=0; i < 96; i++)
            {
                schd_channel_id_ptr[i] = q_result->p_recs[i]->channel_num;
                           
            }
        }
        break;
    default:
        break;
    }
    PMC_RETURN(result);
    
} /* cbrc_db_entry_all_get */



/*******************************************************************************
* FUNCTION: cbrc_sync_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The CBRC sync resets are by default deasserted. This function will put
*   the TSB sync resets into reset before low power is deasserted.
*   This function call will be followed by digi_cbrc_energy_state_reg_set() 
*   which will take the device out of low power. At this point, all TSBs
*   and macros will be in the "default" reset state.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   reset             - CBRC_RESET to put the sync bits into reset
*                       CBRC_ACTIVE to put the sync bits out of reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sync_reset(cbrc_handle_t       *cbrc_handle_ptr,
                                 BOOL8                 reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();

    /* Reset MLD_PRBS : By default MLD_PRBS is out of soft reset */ 
    mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set(NULL, cbrc_handle_ptr, reset); 

    /* Reset FC_ILVR : By default FC_ILVR is out of soft reset */ 
    cbrc_fc_ilvr_field_ILVR_SYNC_RST_set(NULL, cbrc_handle_ptr, reset);

    /* Reset FC_DLVR : By default FC_DLVR is out of soft reset */ 
    cbrc_fc_dlvr_field_DLVR_SYNC_RST_set(NULL, cbrc_handle_ptr, reset);

    PMC_RETURN(result);
} /* cbrc_sync_reset */





/*******************************************************************************
* cbrc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   For PMC_ENERGY_STATE_RUNNING, the function sets the register to a 
*   configurable state and puts the subblocks into the same state.
*   For PMC_ENERGY_STATE_RESET, the function configures the CBRC context
*   to the starting state for all dynamic context. All dynamic context will be
*   returned to the values that were generated during the call to
*   cbrc_handle_init().
*
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   energy_state      - Energy state to put CBRC into
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS      
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_init(cbrc_handle_t       *cbrc_handle_ptr,
                           pmc_energy_state_t   energy_state)
{
    PMC_ERROR   result = PMC_SUCCESS;
    UINT8       i;

    PMC_ENTRY();
    
    switch (energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        cbrc_sync_reset(cbrc_handle_ptr, CBRC_ACTIVE);


        /* Rose: initialize CBR_DINT channel entries to 0xF */ 
        for (i = 0; i < CBRC_MAX_LINK; i++)
        {
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, i, 0xf);
        }

        /* enable FC_CBR_PATT_INS if required*/
        for (i = 0; i < CBRC_CBR_FSGM_MAX_SLICE; i++)
        {
            if (TRUE == cbr_fsgm_start_state_test(cbrc_handle_ptr->cbr_fsgm_tx_ptr[i]))
            {
                cbr_fsgm_init(cbrc_handle_ptr->cbr_fsgm_tx_ptr[i], PMC_ENERGY_STATE_RUNNING);
            }
                
            if (TRUE == cbr_fsgm_start_state_test(cbrc_handle_ptr->cbr_fsgm_rx_ptr[i]))
            {
                cbr_fsgm_init(cbrc_handle_ptr->cbr_fsgm_rx_ptr[i], PMC_ENERGY_STATE_RUNNING);
            }
        } 

        /* enable MPMA/MPMO */
        if (TRUE == mpma_start_state_test(cbrc_handle_ptr->mpma_ptr))
        {
            mpma_init(cbrc_handle_ptr->mpma_ptr, PMC_ENERGY_STATE_RUNNING);
        }

        if (TRUE == mpmo_start_state_test(cbrc_handle_ptr->mpmo_ptr))
        {
            mpmo_init(cbrc_handle_ptr->mpmo_ptr, PMC_ENERGY_STATE_RUNNING);
        }

        if (TRUE == mpmo_start_state_test(cbrc_handle_ptr->mpmo_ptr))
        {
            mpmo_enable_cfg(cbrc_handle_ptr->mpmo_ptr, TRUE);
        }


        /* enable SCBS3 */
        if (TRUE == scbs3_start_state_test(cbrc_handle_ptr->pkt_int_scbs_ptr))
        {
            scbs3_schd_init(cbrc_handle_ptr->pkt_int_scbs_ptr, PMC_ENERGY_STATE_RUNNING);
        }
            
            
        pkt_int_field_SOFT_RST_set(NULL, cbrc_handle_ptr, CBRC_ACTIVE);
        cbr_int_field_CBR_INT_RESET_set(NULL, cbrc_handle_ptr, CBRC_ACTIVE);

        /* Enable interrupts to go to external pin */
        cbrc_int_init(cbrc_handle_ptr, CBRC_ENABLE);
        break;

    case PMC_ENERGY_STATE_RESET:
        cbrc_var_init(cbrc_handle_ptr, &cbrc_handle_ptr->var, TRUE);

        /* reset SDH_PMG */
        for (i = 0; i <= MAX_CBRC_CBRC_SDH_PMG_SDH_PMG_MTSB; i++)
        {
            sdh_pmg_init(cbrc_handle_ptr->sdh_pmg_tx_ptr[i], PMC_ENERGY_STATE_RESET);
            sdh_pmg_init(cbrc_handle_ptr->sdh_pmg_rx_ptr[i], PMC_ENERGY_STATE_RESET);
        }

        /* reset CBR FSGM */
        for (i = 0; i < CBRC_CBR_FSGM_MAX_SLICE; i++)
        {
            cbr_fsgm_init(cbrc_handle_ptr->cbr_fsgm_tx_ptr[i], PMC_ENERGY_STATE_RESET);
            cbr_fsgm_init(cbrc_handle_ptr->cbr_fsgm_rx_ptr[i], PMC_ENERGY_STATE_RESET);
        } 

        /* reset MPMA/MPMO */
        mpma_init(cbrc_handle_ptr->mpma_ptr, PMC_ENERGY_STATE_RESET);
        mpmo_init(cbrc_handle_ptr->mpmo_ptr, PMC_ENERGY_STATE_RESET);
        mpmo_enable_cfg(cbrc_handle_ptr->mpmo_ptr, FALSE);
           
        /* reset SCBS3 */
        scbs3_schd_init(cbrc_handle_ptr->pkt_int_scbs_ptr, PMC_ENERGY_STATE_RESET);
         
        cbr_int_field_CBR_INT_RESET_set(NULL, cbrc_handle_ptr, CBRC_RESET);
        pkt_int_field_SOFT_RST_set(NULL, cbrc_handle_ptr, CBRC_RESET);

        cbrc_sync_reset(cbrc_handle_ptr, CBRC_RESET);
            
        /* Disable interrupts from going to external pin */
        cbrc_int_init(cbrc_handle_ptr, CBRC_DISABLE);
        break;
        
    default:
        PMC_ASSERT(FALSE, CBRC_ERR_INVALID_ARG, energy_state, 0);
        break;
    } /* end switch */
    


    PMC_RETURN(result);
} /* cbrc_init */


/*******************************************************************************
* cbrc_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   CBRC instance.
*
*
* INPUTS:
*   *cbrc_handle      - pointer to cbrc handle instance to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE              - Resources are all in start state (not provisioned).
*   FALSE             - At least one resource is provisioned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_start_state_test(cbrc_handle_t *cbrc_handle)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    BOOL8                start_state = TRUE;

    /* Variable declaration */
    
    PMC_ENTRY();
    PMC_ASSERT(cbrc_handle!=NULL, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    result = cbrc_link_get(cbrc_handle, 0, &link_ptr);
    
    /* Traverse through all links to find one that has been allocated */ 
    while ((NULL != link_ptr) && (TRUE == start_state) && (PMC_SUCCESS == result))
    {
        /* Resource has been allocated: CBRC is not in start state */
        if (!link_ptr->available)
        {
            start_state = FALSE;
        }
        else 
        {
            link_ptr = cbrc_link_get_next(cbrc_handle, link_ptr);
        }
    } /* end while */


    PMC_RETURN(start_state);
    
} /* cbrc_start_state_test */







/*******************************************************************************
* cbrc_port_provision
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Attempts to allocate a logical port within the CBRC based on SERDES type,
*   desired post-mld link(s), and traffic type. A logical port consists of one
*   or more links in the CBRC. Both rx and tx directions will be configured
*   the same.
*   
*
* INPUTS:
*   cbrc_handle_ptr - Pointer to subsystem handle
*   link            - First post-MLD link to be part of the logical port (0..11)
*                     The serdes type will determine whether there are one
*                     or four links in the port.
*   serdes_type     - The ingress or egress serdes used by the logical port.
*                     The serdes type will determine whether the port is
*                     40G or 10G.
*   mode            - client signal to be transported by this port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS             - Port provisioning successful.
*   CBRC_ERR_INVALID_LINK   - Requested link out of acceptable range
*   CBRC_ERR_INVALID_ARG    - Serdes or port mode argument out of range
*   CBRC_ERR_LINK_NOT_AVAIL - Requested link is already provisioned
*   CBRC_ERR_LINK_NOT_HEAD  - Link is not at the head of a logical 40G port
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_port_provision(cbrc_handle_t      *cbrc_handle_ptr,
                                     UINT32              link,
                                     cbrc_serdes_type_t  serdes_type,
                                     cbrc_port_mode_t    mode)
{
    PMC_ERROR            result = PMC_SUCCESS;
    cbrc_link_info_t    *link_ptr = NULL;
    cbrc_link_info_t     link_info_set;
    cbrc_link_info_t    *link_info_set_ptr = &link_info_set;
 
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    } 

    if (serdes_type >= LAST_CBRC_SERDES_TYPE) 
    {
        PMC_RETURN(CBRC_ERR_INVALID_ARG); 
    }
    
    if (mode >= LAST_CBRC_PORT_MODE) 
    {
        PMC_RETURN(CBRC_ERR_INVALID_ARG); 
    }

    /* Retieve pointer to requested link element */
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);


    /* Check if link is valid for 40G ports */
    if ((PMC_SUCCESS == result) && cbrc_serdes_type_is_40g(serdes_type) && (link % 4 != 0))
    {
        result = CBRC_ERR_LINK_NOT_HEAD; 
    }



    /* Check if requested range of ports are available */
    if (PMC_SUCCESS == result)
    {
        if (cbrc_port_is_avail(cbrc_handle_ptr, link_ptr, serdes_type))
        {
            result = CBRC_ERR_LINK_NOT_AVAIL;
        }
    }


    /* Reserve configured port */
    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(link_info_set_ptr, 0, sizeof(cbrc_link_info_t));
        link_info_set.head      = link_ptr;
        link_info_set.available = FALSE; 
        link_info_set.serdes    = serdes_type;
        link_info_set.mode      = mode;
        link_info_set.tx_active = FALSE;
        link_info_set.rx_active = FALSE;
        result = cbrc_port_set(cbrc_handle_ptr, link_ptr, &link_info_set); 
    }


    /* Configure data path through CBRC */
    
    /* Configure MLDs */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_prov(cbrc_handle_ptr, link);
    }
   
    /* Configure SONET */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_prov(cbrc_handle_ptr, link);
    }

    /* Configure FC_ILVR */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc_ilvr_prov(cbrc_handle_ptr, link);
    }

    /* Configure FC_DLVR */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc_dlvr_prov(cbrc_handle_ptr, link);
    }


    if (PMC_SUCCESS != result)
    {
        /* Clean up */
    }

    PMC_RETURN(result);
} /* cbrc_port_provision */

/*******************************************************************************
* cbrc_port_unprovision
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Attempts to unprovision a logical port within the CBRC and return the links
*   back to the available pool.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - First post-MLD link of logical port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - Port unprovisioning successful.
*   CBRC_ERR_INVALID_LINK  - Requested link out of acceptable range
*   CBRC_ERR_LINK_NOT_HEAD - Link is not at the head of a logical 40G port
*   CBRC_ERR_PORT_NOT_PROV - Link is not provisioned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_port_unprovision(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link)
{
    PMC_ERROR            result = PMC_SUCCESS;
    cbrc_link_info_t    *link_ptr = NULL;
    cbrc_link_info_t     link_info_set;
    cbrc_link_info_t    *link_info_set_ptr = &link_info_set;
 
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    } 

    /* Retieve pointer to requested link element */
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }
   

    /* Check if requested link is a head element of a group */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_link_is_head(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_LINK_NOT_HEAD;
        }
    }

    /* Deactivate link first if needed */
    if ((PMC_SUCCESS == result) &&
        cbrc_port_is_active(cbrc_handle_ptr, link_ptr, CBRC_PORT_DIR_RX))
    {
        result = cbrc_port_deactivate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    }
    
    if ((PMC_SUCCESS == result) &&
        cbrc_port_is_active(cbrc_handle_ptr, link_ptr, CBRC_PORT_DIR_TX))
    {
        result = cbrc_port_deactivate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);
    }


    /* Configure MLDs */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_unprov(cbrc_handle_ptr, link);
    }


    /* Unconfigure data path through CBRC */
    /* Unconfigure SONET block */
    if (PMC_SUCCESS == result)
    {
        /* deprovision RX and TX PRBS if there are activated */
        cbrc_prbs_deprov(cbrc_handle_ptr, link);
    
        result = cbrc_sdh_pmg_unprov(cbrc_handle_ptr, link);
    }

    /* Configure FC_ILVR */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc_ilvr_unprov(cbrc_handle_ptr, link);
    }

    /* Configure FC_DLVR */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc_dlvr_unprov(cbrc_handle_ptr, link);
    }



    /* Release link(s) */
    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(link_info_set_ptr, 0, sizeof(cbrc_link_info_t));
        link_info_set.head      = NULL;
        link_info_set.available = TRUE; 
        link_info_set.serdes    = CBRC_SERDES_TYPE_NONE;
        link_info_set.mode      = CBRC_PORT_MODE_NONE;
        link_info_set.tx_active = FALSE;
        link_info_set.rx_active = FALSE;
        result = cbrc_port_set(cbrc_handle_ptr, link_ptr, &link_info_set); 
    }

    if (PMC_SUCCESS != result)
    {
        /* Clean up */
    }

    PMC_RETURN(result);
} /* cbrc_port_unprovision */





/*******************************************************************************
* cbrc_port_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Attempts to activate a provisioned port to send traffic.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - rx or tx to activate
*   link              - First post-MLD link of logical port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - Port activation successful.
*   CBRC_ERR_INVALID_LINK  - Requested link out of acceptable range
*   CBRC_ERR_LINK_NOT_HEAD - Link is not at the head of a logical 40G port
*   CBRC_ERR_PORT_NOT_PROV - Link is not provisioned
*   CBRC_ERR_INVALID_DIR   - Invalid direction
*   CBRC_ERR_LINK_DIR_ALREADY_ACTIVE - Requested dir is already active
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_port_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                    cbrc_port_dir_t   dir,
                                    UINT32            link)
{
    PMC_ERROR            result = PMC_SUCCESS;
    cbrc_link_info_t    *link_ptr = NULL;
    cbrc_link_info_t     link_info_set;
    cbrc_link_info_t    *link_info_set_ptr = &link_info_set;
 
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    }

    if (dir >= LAST_CBRC_PORT_DIR)
    {
        PMC_RETURN(CBRC_ERR_INVALID_DIR); 
    }



    /* Retieve pointer to requested link element */
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);

    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    /* Check if requested link is a head element of a group */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_link_is_head(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_LINK_NOT_HEAD;
        }
    }

    /* Check if requested is already active */
    if (PMC_SUCCESS == result)
    {
        if (cbrc_port_is_active(cbrc_handle_ptr, link_ptr, dir))
        {
            result = CBRC_ERR_LINK_DIR_ALREADY_ACTIVE; 
        }
    }

    /* Activate link(s) */
    if (PMC_SUCCESS == result)
    {
        PMC_MEMCPY(link_info_set_ptr, link_ptr, sizeof(cbrc_link_info_t));

        if (CBRC_PORT_DIR_TX == dir) 
        {
            link_info_set.tx_active = TRUE;
        }
        else
        {
            link_info_set.rx_active = TRUE;
        }
        result = cbrc_port_set(cbrc_handle_ptr, link_ptr, &link_info_set); 
    }


    /* Configure data path through CBRC */
    
    /* Configure MLDs */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_activate(cbrc_handle_ptr, dir, link);
    }


    if (PMC_SUCCESS == result)
    {
        /* Configure FC_ILVR for RX */
        if (CBRC_PORT_DIR_RX == dir)
        {
            result = cbrc_fc_ilvr_activate(cbrc_handle_ptr, link);
        }
        else
            /* Configure FC_DLVR for TX */
        {
            result = cbrc_fc_dlvr_activate(cbrc_handle_ptr, link);
        }
    }


    if (PMC_SUCCESS != result)
    {
        /* Clean up */
    }

    PMC_RETURN(result);
} /* cbrc_port_activate */



/*******************************************************************************
* cbrc_port_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Attempts to deactivate a provisioned port. On successful deactivation,
*   port will be in provisioned state.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - rx or tx to deactivate
*   link              - First post-MLD link of logical port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS              - Port deactivation successful.
*   CBRC_ERR_INVALID_LINK    - Requested link out of acceptable range
*   CBRC_ERR_LINK_NOT_HEAD   - Link is not at the head of a logical 40G port
*   CBRC_ERR_PORT_NOT_PROV   - Link is not provisioned
*   CBRC_ERR_PORT_NOT_ACTIVE - Link is not active
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_port_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                      cbrc_port_dir_t   dir,
                                      UINT32            link)
{
    PMC_ERROR            result = PMC_SUCCESS;
    cbrc_link_info_t    *link_ptr = NULL;
    cbrc_link_info_t     link_info_set;
    cbrc_link_info_t    *link_info_set_ptr = &link_info_set;
 
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    }

    if (dir >= LAST_CBRC_PORT_DIR)
    {
        PMC_RETURN(CBRC_ERR_INVALID_DIR); 
    }



    /* Retieve pointer to requested link element */
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    /* Check if requested link is a head element of a group */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_link_is_head(cbrc_handle_ptr, link_ptr))
        {
            result = CBRC_ERR_LINK_NOT_HEAD;
        }
    }

    /* Check if requested is already active */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_active(cbrc_handle_ptr, link_ptr, dir))
        {
            result = CBRC_ERR_LINK_DIR_NOT_ACTIVE; 
        }
    }

    /* Deactivate link(s) */
    if (PMC_SUCCESS == result)
    {
        PMC_MEMCPY(link_info_set_ptr, link_ptr, sizeof(cbrc_link_info_t));

        if (CBRC_PORT_DIR_TX == dir) 
        {
            link_info_set.tx_active = FALSE;
        }
        else
        {
            link_info_set.rx_active = FALSE;
        }
        result = cbrc_port_set(cbrc_handle_ptr, link_ptr, &link_info_set); 
    }


    /* Configure data path through CBRC */
    /* Configure MLDs */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_deactivate(cbrc_handle_ptr, dir, (cbrc_port_dir_t)link);
    }


    if (PMC_SUCCESS == result)
    {
        /* Configure FC_ILVR for RX */
        if (CBRC_PORT_DIR_RX == dir)
        {
            result = cbrc_fc_ilvr_deactivate(cbrc_handle_ptr, link);
        }
        else
            /* Configure FC_DLVR for TX */
        {
            result = cbrc_fc_dlvr_deactivate(cbrc_handle_ptr, link);
        }
    }

    if (PMC_SUCCESS != result)
    {
        /* Clean up */
    }

    
    PMC_RETURN(result);
} /* cbrc_port_deactivate */

/*******************************************************************************
*  cbrc_mpmo_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines if the MPMO configuration is required for a specific channel.
*
* INPUTS:
*  *cbrc_hndl        - pointer to CBRC handle instance to be operated on
*  link              - channel ID to be configured
*                      Valid range: 0 - 95 
*
* OUTPUTS:
*  mpmo_cfg_req      - MPMO configuration required or not
*                      TRUE  : MPMO configuration required
*                      FALSE : MPMO configuration not required
*
* RETURNS:
*  PMC_ERR_FAIL      - Internally generated error code
*             0      - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_cfg_get(cbrc_handle_t *cbrc_hndl,
                                   UINT32 link,
                                   BOOL8 *mpmo_cfg_req)
{   
 
    PMC_ENTRY();
    if(cbrc_hndl->var.links[link].serdes == CBRC_SERDES_TYPE_10G_XFI ||
       cbrc_hndl->var.links[link].serdes == CBRC_SERDES_TYPE_40G_STL ||
       cbrc_hndl->var.links[link].serdes == CBRC_SERDES_TYPE_40G_SFIS ||
       cbrc_hndl->var.links[link].serdes == CBRC_SERDES_TYPE_40G_SFI51)
    {
        *mpmo_cfg_req = TRUE;
    }
    else 
    {
        *mpmo_cfg_req = FALSE; 
    }
    
    if(*mpmo_cfg_req == TRUE)
    { 
        if(cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_40G_STS768 ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_40G_STM256 ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_STS192 ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_STM64 ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_FC800 ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_FC1200_ODU1F ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_FC1200_ODU2F ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_CPRI_5G ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_CPRI_6G ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_CPRI_10G ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_GDPS_5G ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_GDPS_10G ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_STS192_PRBS ||
           cbrc_hndl->var.links[link].mode == CBRC_PORT_MODE_10G_STM64_PRBS) 
        {
            *mpmo_cfg_req = TRUE;
        } 
        else 
        {
            *mpmo_cfg_req = FALSE; 
        }  
    }
        
    PMC_RETURN(PMC_SUCCESS);
    
} /* cbrc_mpmo_cfg_get */

/*******************************************************************************
*  cbrc_mpma_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMA packet size and packet period information for a channel.
*
* INPUTS:
*   *cbrc_hndl          - handle to the CBRC instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*   epsilon_ptr         - a pointer to the epsilon value.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpma_data_get(cbrc_handle_t *cbrc_hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr,
                                    DOUBLE *epsilon_ptr)
{   
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
        
    PMC_ENTRY();

    ret_val = mpma_pkt_period_var_get(cbrc_hndl->mpma_ptr,link,
                                      pkt_size_ptr,
                                      pkt_per_int_ptr,
                                      pkt_per_n_ptr,
                                      pkt_per_d_ptr,
                                      epsilon_ptr);
        
    if (0 == *pkt_size_ptr)
    {
        *pkt_size_ptr    = MPMA_DEFAULT_PKT_SIZE;
    }
    
    PMC_RETURN(ret_val);
    
} /* cbrc_mpma_data_get */


/*******************************************************************************
*  cbrc_mpmo_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMO packet size and packet period information for a channel.
*
* INPUTS:
*   *cbrc_hndl          - handle to the CBRC instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_data_get(cbrc_handle_t *cbrc_hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr)
{   
    /* Variable declaration */
    UINT32 ps_ptr; 
    UINT32 pp_int_ptr;  
    UINT32 pp_n_ptr;    
    UINT32 pp_d_ptr;  
    PMC_ERROR ret_val = PMC_SUCCESS;
        
    PMC_ENTRY();

    ret_val = mpmo_pkt_period_var_get(cbrc_hndl->mpmo_ptr,link,
                                      &ps_ptr,&pp_int_ptr,&pp_n_ptr,&pp_d_ptr);
        
    *pkt_size_ptr = ps_ptr; 
    *pkt_per_int_ptr = pp_int_ptr;                         
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr;
    
    PMC_RETURN(ret_val);
    
} /* cbrc_mpmo_data_get */


/*******************************************************************************
*  cbrc_port_is_sonet_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets if CBRC channel is SONET or or not.
*
* INPUTS:
*   *cbrc_hndl          - handle to the CBRC instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*
* OUTPUTS:
*
* RETURNS:
*   TRUE : CBRC stream is SONET
*   FALSE : CBRC stream is not sonet
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_port_is_sonet_get(cbrc_handle_t *cbrc_hndl,
                                    UINT32 link)
{   
    /* Variable declaration */
    BOOL8 is_sonet;
        
    PMC_ENTRY();

    is_sonet = cbrc_port_is_sonet(cbrc_hndl->var.links[link].mode);
    
    PMC_RETURN(is_sonet);
    
} /* cbrc_port_is_sonet_get */


/*******************************************************************************
*  cbrc_mpmo_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configures MPMO channel.
*
* INPUTS:
*   *cbrc_hndl          - pointer to CBRC handle instance to be operated on
*   link                - channel ID to be configured
*                         Valid range: 0 - 1
*   pkt_size            - packet size configured at the packet maker
*   pkt_per_int         - integer portion for packet period configured at the 
*                         packet maker
*   pkt_per_n           - numerator for packet period fraction configured at the 
*                         packet maker
*   pkt_per_d           - denominator for packet period fraction configured at  
*                         the packet maker
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_prov(cbrc_handle_t *cbrc_hndl,
                                UINT32 link,
                                UINT32 pkt_size,
                                UINT32 pkt_per_int,
                                UINT32 pkt_per_n,
                                UINT32 pkt_per_d)
{   
    /* Variable declaration */
    PMC_ERROR ret_val;
    DOUBLE client_rate;
    
    PMC_ENTRY();

    client_rate = cbrc_client_rate_get(cbrc_hndl, cbrc_hndl->var.links[link].mode);

    /*program MPMO */
    ret_val = mpmo_ch_prov(cbrc_hndl->mpmo_ptr,
                           link,
                           client_rate,
                           pkt_size,
                           pkt_per_int,
                           pkt_per_n,
                           pkt_per_d,
                           UTIL_OPSA_T_FRAME_UNUSED,
                           FALSE);

    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* cbrc_mpmo_prov */ 

/*******************************************************************************
*  cbrc_mpmo_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configures MPMO channel.
*
* INPUTS:
*   *cbrc_hndl          - pointer to CBRC handle instance to be operated on
*   link                - channel ID to be configured
*                         Valid range: 0 - 1
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL        - Internally generated error code
*   0                   - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_deprov(cbrc_handle_t *cbrc_hndl,
                                  UINT32 link)
{   
    /* Variable declaration */
    PMC_ERROR ret_val;
    
    PMC_ENTRY();

    /*program MPMO */
    ret_val = mpmo_ch_deprov(cbrc_hndl->mpmo_ptr,
                             link);

    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* cbrc_mpmo_deprov */ 


/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts individual sdh_pmg slices into reset and low power
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction
*   link              - 0..11
*   reset             - TRUE to put into low power and reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_reset(cbrc_handle_t    *cbrc_handle_ptr,
                                    cbrc_port_dir_t   dir,
                                    UINT32            link,
                                    BOOL8              reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         slice;

    PMC_ENTRY();
   
    slice = link / 4;

    switch (slice)
    {
    case 0:
        cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set(NULL, cbrc_handle_ptr, dir, reset);
        cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set(NULL, cbrc_handle_ptr, dir, reset);
        break;
        
    case 1:
        cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set(NULL, cbrc_handle_ptr, dir, reset);
        cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set(NULL, cbrc_handle_ptr, dir, reset);
        break;
        
    case 2:             
        cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set(NULL, cbrc_handle_ptr, dir, reset);
        cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set(NULL, cbrc_handle_ptr, dir, reset);
        break;
    
    default:
        PMC_ASSERT(0, CBRC_ERR_CODE_ASSERT, slice, 0);
        break;
    } /* end switch */
    


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_reset */





/*******************************************************************************
* cbrc_sdh_pmg_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to prov (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    sdh_pmg_handle_t   *sdh_tx_ptr, *sdh_rx_ptr;
    sdh_pmg_sts_mode_t  mode;
    UINT32              index;
    UINT32              slice;
    
    PMC_ENTRY();


    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);

   
    if ((PMC_SUCCESS == result) && 
        cbrc_port_is_sonet(link_ptr->mode))
    {
        /* Get SDH handle */
        index = link % CBRC_SDH_PMG_MAX_INDEX;
        slice = link / CBRC_SDH_PMG_MAX_INDEX;
        sdh_tx_ptr = cbrc_handle_ptr->sdh_pmg_tx_ptr[slice];
        sdh_rx_ptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[slice];



        /* Take macro out of reset if this is first link provisioned on macro */
        if (!IS_CBRC_SDH_PROV(cbrc_handle_ptr))
        {
            cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_RX, CBRC_ACTIVE);
            cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_TX, CBRC_ACTIVE);
        }
       

        /* Take macro out of low power if this is first link provisioned on slice */
        if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_RX, slice))
        {
            cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link, CBRC_ACTIVE);
        }
        if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_TX, slice))
        {
            cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link, CBRC_ACTIVE);
        }


        /* init SDH_PMG if required*/
        if (TRUE == sdh_pmg_start_state_test(sdh_tx_ptr))
        {
            sdh_pmg_init(sdh_tx_ptr, PMC_ENERGY_STATE_RUNNING);
        }

        if (TRUE == sdh_pmg_start_state_test(sdh_rx_ptr))
        {
            sdh_pmg_init(sdh_rx_ptr, PMC_ENERGY_STATE_RUNNING);
        }




        /* Set mode */
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_40G_STS768:
            mode = SDH_PMG_STS_MODE_40G_STS768;
            break;
        case CBRC_PORT_MODE_40G_STM256:
            mode = SDH_PMG_STS_MODE_40G_STM256;
            break;
        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_10GE:
            mode = SDH_PMG_STS_MODE_10G_STS192;
            break;
        case CBRC_PORT_MODE_10G_STM64:
            mode = SDH_PMG_STS_MODE_10G_STM64;
            break;
        case CBRC_PORT_MODE_10G_STS192_PRBS:
            mode = SDH_PMG_STS_MODE_10G_STS192_PRBS;
            break;
        case CBRC_PORT_MODE_10G_STM64_PRBS:
            mode = SDH_PMG_STS_MODE_10G_STM64_PRBS;
            break;

        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, 0);
            break;
        }


        /* Configure tx link */
        if (PMC_SUCCESS == result)
        {
            result = sdh_pmg_prov(sdh_tx_ptr, index, mode);
        }
        
        /* Configure rx link */
        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_prov(sdh_rx_ptr, index, mode);
        }
        
        /* Update link_mask */
        if (PMC_SUCCESS == result) 
        {
            CBRC_SDH_LINK_MASK_SET(cbrc_handle_ptr->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_TX, slice, index);
            CBRC_SDH_LINK_MASK_SET(cbrc_handle_ptr->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_RX, slice, index);
        }


        /* Clean up */
        if (PMC_SUCCESS != result)
        {
            /* Put macro into reset if no links were successfully provisioned */
            if (!IS_CBRC_SDH_PROV(cbrc_handle_ptr))
            {
                cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_RX, CBRC_RESET);
                cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_TX, CBRC_RESET);
            }

            /* Put slice into low power if no links were successfully provisioned */
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_RX, slice))
            {
                cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link, CBRC_RESET);
            }
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_TX, slice))
            {
                cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link, CBRC_RESET);
            }
        }
    }
    

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_prov */




/*******************************************************************************
* cbrc_sdh_pmg_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to unprov (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    sdh_pmg_handle_t   *sdh_tx_ptr, *sdh_rx_ptr;
    UINT32              index;
    UINT32              slice;

    PMC_ENTRY();
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if ((PMC_SUCCESS == result) && 
        cbrc_port_is_sonet(link_ptr->mode))
    {
        /* Get SDH handle */
        index = link % CBRC_SDH_PMG_MAX_INDEX;
        slice = link / CBRC_SDH_PMG_MAX_INDEX;
        sdh_tx_ptr = cbrc_handle_ptr->sdh_pmg_tx_ptr[slice];
        sdh_rx_ptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[slice];
        /* Update tx config */
        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_unprov(sdh_tx_ptr, index);
        }

        /* Update rx config */
        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_unprov(sdh_rx_ptr, index);
        }

        
        /* Update sdh link_mask */
        if (PMC_SUCCESS == result) 
        {
            CBRC_SDH_LINK_MASK_CLR(cbrc_handle_ptr->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_TX, slice, index);
            CBRC_SDH_LINK_MASK_CLR(cbrc_handle_ptr->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_RX, slice, index);
            /* init SDH_PMG if required*/
            if (TRUE == sdh_pmg_start_state_test(sdh_tx_ptr))
            {
                sdh_pmg_init(sdh_tx_ptr, PMC_ENERGY_STATE_RESET);
            }
            
            if (TRUE == sdh_pmg_start_state_test(sdh_rx_ptr))
            {
                sdh_pmg_init(sdh_rx_ptr, PMC_ENERGY_STATE_RESET);
            }

            /* Put slice into low power if no links are still provisioned */
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_RX, slice))
            {
                cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link, CBRC_RESET);
            }
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle_ptr, CBRC_PORT_DIR_TX, slice))
            {
                cbrc_sdh_pmg_reset(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link, CBRC_RESET);
            }
            /* Put macro into reset if no links are still provisioned */
            if (!IS_CBRC_SDH_PROV(cbrc_handle_ptr))
            {
                cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_RX, CBRC_RESET);
                cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, CBRC_PORT_DIR_TX, CBRC_RESET);
            }
        }
    }

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_unprov */




/*******************************************************************************
* cbrc_sdh_pmg_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activate or deactivates a given link/block in the sdh_pmg 
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to activate
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       cbrc_port_dir_t   dir,
                                       UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    sdh_pmg_handle_t   *sdh_handle_ptr;
    UINT32              index;
    
    PMC_ENTRY();

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if ((PMC_SUCCESS == result) && 
        cbrc_port_is_sonet(link_ptr->mode))
    {
        index = link%4;

        /* Get SDH handle */
        if (CBRC_PORT_DIR_TX == dir) {
            sdh_handle_ptr = cbrc_handle_ptr->sdh_pmg_tx_ptr[link/4];
        } else {
            sdh_handle_ptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[link/4];
        }

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_activate(sdh_handle_ptr, index);
        }
    }

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_activate */

/*******************************************************************************
* cbrc_sonet_los_v_workaround
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is sonet los_v workaround. It disables SDH_PMG_PN11_INSERT_LOS and enables
*   FC800PMG_CH. 
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   cbrc_link         - Post-mld link to activate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sonet_los_v_workaround(cbrc_handle_t *cbrc_handle_ptr, UINT32 cbrc_link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    sdh_pmg_handle_t   *sdh_handle_ptr;
    UINT32              index;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(cbrc_link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, cbrc_link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, cbrc_link, &link_ptr);
    
    if (PMC_SUCCESS == result) {
        if (link_ptr->mode == CBRC_PORT_MODE_40G_STS768 ||
            link_ptr->mode == CBRC_PORT_MODE_40G_STM256 ||
            link_ptr->mode == CBRC_PORT_MODE_10G_STS192 ||
            link_ptr->mode == CBRC_PORT_MODE_10G_STM64 ||
            link_ptr->mode == CBRC_PORT_MODE_10G_10GE)
        {
            index = cbrc_link%4;

            /* Get SDH handle */
            sdh_handle_ptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[cbrc_link/4];
            result = sdh_pmg_pn11_ins(sdh_handle_ptr, index, SDH_PMG_PN11_INSERT_LOS, 0);

            if (PMC_SUCCESS == result)
            {
                (void)cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, cbrc_link, CBRC_ENABLE);
            }
        }
    }

    PMC_RETURN(result);
}

/*******************************************************************************
* cbrc_cdh_pmg_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to deactivate
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         cbrc_port_dir_t   dir,
                                         UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    sdh_pmg_handle_t   *sdh_handle_ptr;
    UINT32              index;

    PMC_ENTRY();
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
   
    if ((PMC_SUCCESS == result) && 
        cbrc_port_is_sonet(link_ptr->mode))
    {
        index = link%4;

        /* Get SDH handle */
        if (CBRC_PORT_DIR_TX == dir) {
            sdh_handle_ptr = cbrc_handle_ptr->sdh_pmg_tx_ptr[link/4];
        } else {
            sdh_handle_ptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[link/4];
        }
        
        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_deactivate(sdh_handle_ptr, index);
        }
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_deactivate */





/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_pn11_ins()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures if PN11 is inserted into the datapath when an LOS or LOF occurs.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   lof_los           - Configure insertion PN11 on LOF and/or LOS
*   ins_enable        - Enable or disable PN11 insertion
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_pn11_ins(cbrc_handle_t            *cbrc_handle_ptr,
                                       cbrc_port_dir_t           dir,
                                       UINT32                    link,
                                       sdh_pmg_pn11_insert_t     lof_los,
                                       BOOL8                      ins_enable)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr=NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }

    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }

    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_pn11_ins(sdh_handle_ptr, index, lof_los, ins_enable);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_pn11_ins */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_pn11_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 is inserted into the datapath when an LOS or LOF occurs.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   lof_los           - Configure insertion PN11 on LOF and/or LOS
*
* OUTPUTS:
*  *ins_enable_ptr    - PN11 insertion is enabled or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_pn11_ins_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           cbrc_port_dir_t           dir,
                                           UINT32                    link,
                                           sdh_pmg_pn11_insert_t     lof_los,
                                           BOOL                     *ins_enable_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr=NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
   
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }

    
    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }

    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_pn11_ins_get(sdh_handle_ptr, index, lof_los, ins_enable_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_pn11_ins_get */


/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_oof_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Force an Out Of Frame into the next framing boundary.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_oof_force(cbrc_handle_t    *cbrc_handle_ptr,
                                        cbrc_port_dir_t   dir,
                                        UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr=NULL;
    sdh_pmg_handle_t   *sdh_handle_ptr=NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    } 
   
    if (PMC_SUCCESS == result)
    {
        if (cbrc_port_is_sonet(link_ptr->mode))
        {
            if (PMC_SUCCESS == result) 
            {
                result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
            }

            
            if (PMC_SUCCESS == result) 
            {
                result = sdh_pmg_oof_force(sdh_handle_ptr, index);
            }
        }
    }  


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_oof_force */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rrmp_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the RRMP:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   type              - RRMP config type
*  *rrmp_cfg_ptr      - Configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                       cbrc_port_dir_t           dir,
                                       UINT32                    link,
                                       sdh_pmg_rrmp_cfg_type_t   type,
                                       void                     *rrmp_cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr = NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
   

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_rrmp_cfg(sdh_handle_ptr, index, type, rrmp_cfg_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rrmp_cfg */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rrmp_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves RRMP configuration for:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   type              - RRMP config type
*
* OUTPUTS:
*  *rrmp_cfg_ptr      - RRMP configuration
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           cbrc_port_dir_t           dir,
                                           UINT32                    link,
                                           sdh_pmg_rrmp_cfg_type_t   type,
                                           void                     *rrmp_cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr = NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
   

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_rrmp_cfg_get(sdh_handle_ptr, index, type, rrmp_cfg_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rrmp_cfg_get */







/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rrmp_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves RRMP extracted bytes
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   type              - Byte to extract
*
* OUTPUTS:
*   byte_ptr          - Extracted byte
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_byte_get(cbrc_handle_t            *cbrc_handle_ptr,
                                            cbrc_port_dir_t           dir,
                                            UINT32                    link,
                                            sdh_pmg_rrmp_byte_type_t  type,
                                            UINT8                    *byte_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_rrmp_byte_get(sdh_handle_ptr, index, type, byte_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rrmp_byte_get */

/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_trmp_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the TRMP:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*   type              - RRMP config type
*  *trmp_cfg_ptr      - Configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_trmp_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                       UINT32                    link,
                                       sdh_pmg_trmp_cfg_type_t   type,
                                       sdh_pmg_trmp_alarm_cfg_t  *trmp_cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, 0, 0);
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }
   
    if (PMC_SUCCESS == result) 
    {
        if (trmp_cfg_ptr->mask & SDH_PMG_TRMP_ALARM_CFG_MASK_M0EN)
        {
            trmp_field_M0EN_set(NULL, cbrc_handle_ptr, link, trmp_cfg_ptr->M0_en);
        }
    }

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_trmp_cfg */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_trmp_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves TRMP configuration for:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*   type              - TRMP config type
*
* OUTPUTS:
*  *trmp_cfg_ptr      - TRMP configuration
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_trmp_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           UINT32                    link,
                                           sdh_pmg_trmp_cfg_type_t   type,
                                           sdh_pmg_trmp_alarm_cfg_t  *trmp_cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, 0, 0);
    

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }
    
    if (PMC_SUCCESS == result) 
    {

        trmp_cfg_ptr->mask = SDH_PMG_TRMP_ALARM_CFG_MASK_M0EN;
        trmp_cfg_ptr->M0_en           = trmp_field_M0EN_get(NULL, cbrc_handle_ptr, link);

    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_trmp_cfg_get */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_sber_byte_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the byte (B1 or B2) that the SDH SBER block will use to detect
*    signal degrade and signal fail. The SBER in the SDH_PMG can only monitor
*    either the B1 or B2 byte at one time. Changing this configuration will
*    affect all channels.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   byte              - Sets the byte that the BER will be calculated from
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_byte_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                            sdh_pmg_sber_byte_t       byte)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
   
    /* Set SBER byte config for all SDH_PMG slices */
    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_rx_ptr[0], byte);
    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_rx_ptr[1], byte);
    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_rx_ptr[2], byte);

    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_tx_ptr[0], byte);
    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_tx_ptr[1], byte);
    sdh_pmg_sber_byte_cfg(cbrc_handle_ptr->sdh_pmg_tx_ptr[2], byte);


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_sber_byte_cfg */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_sber_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves the configured byte that the SDH SBER block will use to detect
*    signal degrade and signal failure.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*
* OUTPUTS:
*   *byte_ptr         - Configured byte that the BER will be calculated from
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_byte_get(cbrc_handle_t            *cbrc_handle_ptr,
                                            sdh_pmg_sber_byte_t      *byte_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
   
    /* Set SBER byte config for all SDH_PMG slices */
    sdh_pmg_sber_byte_get(cbrc_handle_ptr->sdh_pmg_rx_ptr[0], byte_ptr);


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_sber_byte_get */


/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_sber_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables SD/SF monitoring and sets the threshold, or disables monitoring.
*   The byte from which the BER is calculated must be configured first
*   with digi_cbr_sdh_sber_byte_cfg().
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   alarm             - Enable/disable SD or SF alarm for given BER
*   ber               - Bit error rate, or disable monitoring
*                       Valid inputs are:\n
*                       B1 STS-768: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_12 \n
*                       B1 STS-192: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_11 \n    
*                       B2 STS-768: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 \n
*                       B2 STS-192: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_enable(cbrc_handle_t                *cbrc_handle_ptr,
                                          cbrc_port_dir_t               dir,
                                          UINT32                        link,
                                          sdh_pmg_sber_alarm_t          alarm,
                                          sdh_pmg_sber_ber_t            ber)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr = NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
  

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }



    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }

    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_sber_enable(sdh_handle_ptr, index, alarm, ber);
    }

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_sber_enable */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_sber_ber_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured BER rate for SD or SF.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to deactivate (0..11)
*   alarm             - Enable/disable SD or SF alarm for given BER
*
* OUTPUTS:
*   ber_ptr           - configured SD or SF bit error rate, or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_ber_get(cbrc_handle_t                *cbrc_handle_ptr,
                                           cbrc_port_dir_t               dir,
                                           UINT32                        link,
                                           sdh_pmg_sber_alarm_t          alarm,
                                           sdh_pmg_sber_ber_t           *ber_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr= NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
   

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }

    
    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_sber_ber_get(sdh_handle_ptr, index, alarm, ber_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_sber_ber_get */



/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rttp_len_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the length of the tail trace message for a specified channel
*   within the CBRC. Also configures the algorithm to be used.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to deactivate
*   link              - Post-mld link to deactivate (0..11)
*   length            - length of trace message
*   algo              - Disable trace message processing, or use specified
*                       algorithm
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_len_cfg(cbrc_handle_t           *cbrc_handle_ptr,
                                           cbrc_port_dir_t          dir,
                                           UINT32                   link,
                                           sdh_pmg_rttp_length_t    length,
                                           sdh_pmg_rttp_algo_t      algo)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr= NULL;
    UINT32              index;
    sdh_pmg_rttp_cfg_t  rttp_cfg;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
   
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        rttp_cfg.length = length;
        rttp_cfg.algo = algo;
        result = sdh_pmg_rttp_cfg(sdh_handle_ptr, index, &rttp_cfg);
    }

    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rttp_len_cfg */


/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rttp_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the connfigured the length and algo of the tail trace message for a 
*   specified channel within the CBRC.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to deactivate
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*   cfg_ptr           - configuration of trace message
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_cfg_get(cbrc_handle_t           *cbrc_handle_ptr,
                                           cbrc_port_dir_t          dir,
                                           UINT32                   link,
                                           sdh_pmg_rttp_cfg_t       *cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr= NULL;
    UINT32              index;
    sdh_pmg_rttp_cfg_t  rttp_cfg;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cfg_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_rttp_cfg_get(sdh_handle_ptr, index, &rttp_cfg);
    }

    if (PMC_SUCCESS == result)
    {
        cfg_ptr->length = rttp_cfg.length;
        cfg_ptr->algo = rttp_cfg.algo;
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rttp_cfg_get */

/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rttp_expect_trace_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the expected trace message.
*
*    cbrc_sdh_pmg_rttp_len_cfg() is required to be called first to configure
*    the length of the trace message and the algorithm to be used.
*   
*    16 byte trace messages will have a CRC inserted in the byte 16.
*    64 byte trace messages will have CR/LF inserted in byte 63 and 64.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to deactivate
*   link              - Post-mld link to deactivate (0..11)
*   *expect_trace     - pointer to trace message structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_expect_trace_set(cbrc_handle_t            *cbrc_handle_ptr,
                                                    cbrc_port_dir_t           dir,
                                                    UINT32                    link,
                                                    sdh_pmg_rttp_trace_t     *expect_trace)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr = NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        /* sdh_pmg blk will check that msg length and algorithm pairs
           are valid */
        result = sdh_pmg_rttp_expect_trace_set(sdh_handle_ptr, 
                                               index,
                                               expect_trace);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rttp_expect_trace_set */





/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_rttp_trace_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtains the caputured, accepted, or expected trace message for the channel.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - direction to deactivate
*   link              - Post-mld link to deactivate (0..11)
*   trace_type        - expected, accepted, or captured trace
*
* OUTPUTS:
*   trace_ptr         - Retrieved trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_trace_get(cbrc_handle_t              *cbrc_handle_ptr,
                                             cbrc_port_dir_t             dir,
                                             UINT32                      link,
                                             sdh_pmg_rttp_trace_type_t   trace_type,
                                             sdh_pmg_rttp_trace_t       *trace_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sdh_pmg_handle_t   *sdh_handle_ptr = NULL;
    UINT32              index;

    PMC_ENTRY();
    
    
    /* Check arguments */
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_SDH);    
    }


    if (PMC_SUCCESS == result) 
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle_ptr, dir, link, &sdh_handle_ptr, &index);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = sdh_pmg_rttp_trace_get(sdh_handle_ptr, 
                                        index, trace_type,
                                        trace_ptr);
    }


    PMC_RETURN(result);
} /* cbrc_sdh_pmg_rttp_trace_get */






/*******************************************************************************
* cbrc_mld_prbs_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure a MLD to pass traffic on a desired link.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to configure (0..11)
*   serdes_type       - Serdes type of incoming traffic
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   CBRC_ERR_INVALID_LINK
*   CBRC_ERR_INVALID_ARG
*   CBRC_ERR_NULL_HANDLE
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_mode_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                        UINT32               link,
                                        cbrc_serdes_type_t   serdes_type)
{
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              fifo_thresh;
    UINT8               i;
    UINT8               pre_mld_start_index = 0;
    UINT8               pre_mld_end_index = 0;
    UINT8               post_mld_start_index = 0;
    UINT8               post_mld_end_index = 0;
    UINT8               rx_mux_sel = 0;
    UINT8               tx_mux_sel = 0;
    UINT8               sfis_index;
    UINT8               stl_index =0;
    UINT8               prbs_tx_mux_sel = 0x0;  /* 0x0 - FC_DLVR
                                                 * 0x1 - PRBS
                                                 */
    UINT8               xoff_tx_mux_sel = 0x0;  /* configures the xoff multiplexer whose output is broadcast 
                                                 * to the CBRC_FC_DLVR for backpressuring
                                                 * 0x0 - STL256.4 
                                                 * 0x1 - SFI-S
                                                 * 0x2 - SF15.1
                                                 * 0x3 - XFI Fifo
                                                 */

    PMC_ENTRY();
   

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_type < LAST_CBRC_SERDES_TYPE, CBRC_ERR_INVALID_ARG, serdes_type, LAST_CBRC_SERDES_TYPE);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    
    if (PMC_SUCCESS == result)
    {
         
        /* Set RX and TX muxes for MLDs */
        switch (serdes_type)
        {
        case CBRC_SERDES_TYPE_10G_XFI:
            rx_mux_sel = CBRC_MLD_RX_XFI;
            tx_mux_sel = CBRC_MLD_TX_XFI;
            xoff_tx_mux_sel = CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_XFI;

            pre_mld_start_index  = link;
            pre_mld_end_index    = link + 1;
            post_mld_start_index = link;
            post_mld_end_index = link + 1;
            break;
            
        case CBRC_SERDES_TYPE_40G_STL:
            rx_mux_sel = CBRC_MLD_RX_STL;
            tx_mux_sel = CBRC_MLD_TX_STL;
            xoff_tx_mux_sel = CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_STL;

            pre_mld_start_index  = link;
            pre_mld_end_index    = link + 4;
            post_mld_start_index = link;
            post_mld_end_index = link + 4;

            if (0 == link)
            {
                stl_index = 0;
            }
            if (4 == link)
            {
                stl_index = 1;
            }
            if (8 == link)
            {
                stl_index = 2;
            }
                
            /* Set the AF_THRESH : determines the egress direction FIFO fill level at which 
            ** the egress STM256 interface is backpressured */
            stl256_field_AF_THRESH_set(NULL, cbrc_handle_ptr, stl_index, CBRC_MLD_STL256_AF_THRESH);

            break;
            
        case CBRC_SERDES_TYPE_40G_SFIS:
            rx_mux_sel = CBRC_MLD_RX_SFIS;
            tx_mux_sel = CBRC_MLD_TX_SFIS;
            xoff_tx_mux_sel = CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_SFIS;

            if (0 == link)
            {
                sfis_index = 0;
            }
            else
            {
                sfis_index = 1;
            }
                
            cbrc_sfis_rx_field_RX_NUM_LANES_set(NULL, cbrc_handle_ptr, sfis_index, 0x4);
            cbrc_sfis_tx_field_TX_NUM_LANES_set(NULL, cbrc_handle_ptr, sfis_index, 0x4);
                
            pre_mld_start_index  = sfis_index * 5;
            pre_mld_end_index    = pre_mld_start_index + 5;
            post_mld_start_index = link;
            post_mld_end_index = link + 4;
            break;
            
        case CBRC_SERDES_TYPE_40G_SFI51:
            rx_mux_sel = CBRC_MLD_RX_SFI51;
            xoff_tx_mux_sel = CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_SFI51;
               
            pre_mld_start_index  = 0; /* no need to run pre-mld config for SFI51 */
            pre_mld_end_index    = 0;
            post_mld_start_index = link;
            post_mld_end_index = link + 4;
            break;
        
        default:
            break;
        } /* end switch */
      
        /* Configure post-mld registers (e.g. 40G SFI-S: 0..3, 4..7) */
        for (i = post_mld_start_index; i < post_mld_end_index; i++)
        {
            mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set(NULL, cbrc_handle_ptr, i, rx_mux_sel);
            mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set(NULL, cbrc_handle_ptr, i, prbs_tx_mux_sel);
            mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set(NULL, cbrc_handle_ptr, i, xoff_tx_mux_sel);

            /* Disable diagnostic loopback */
            mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set(NULL, cbrc_handle_ptr, i, CBRC_DISABLE);
        }

        
        /* Configure pre-mld registers (e.g. 40G SFI-S: 0..4, 5..9) */
        for (i = pre_mld_start_index; i < pre_mld_end_index; i++)
        {
            mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set(NULL, cbrc_handle_ptr, i, tx_mux_sel);
        }


        /* Setup FIFO thresholds for TX MLD PRBS */ 
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:     
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_FC800;
            break;

        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:
        case CBRC_PORT_MODE_10G_64B_66B_IDLE:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_FC1200;
            break;

        case CBRC_PORT_MODE_10G_CPRI_5G:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_5G;
            break;

        case CBRC_PORT_MODE_10G_CPRI_6G:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_6G;
            break;

        case CBRC_PORT_MODE_10G_CPRI_10G:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_10G;
            break;

        case CBRC_PORT_MODE_10G_GDPS_5G:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_GDPS_5G;
            break;

        case CBRC_PORT_MODE_10G_GDPS_10G:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_GDPS_10G;
            break;

        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_10GE:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_STS192;
            break;

        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            fifo_thresh = CBRC_MLD_PRBS_TX_FIFO_THRESH_STS768;
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
            
        mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set(NULL, cbrc_handle_ptr, link, fifo_thresh);
    } /* end if */


    PMC_RETURN(result);
} /* cbrc_mld_prbs_mode_cfg */





/*******************************************************************************
* cbrc_mld_prbs_clk_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure a MLD clk enables and resets
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to configure (0..11)
*   serdes_type       - Serdes type of incoming traffic
*   enable            - value to be programmed

* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   CBRC_ERR_INVALID_LINK
*   CBRC_ERR_INVALID_ARG
*   CBRC_ERR_NULL_HANDLE
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_clk_en(cbrc_handle_t       *cbrc_handle_ptr,
                                      UINT32              link,
                                      cbrc_serdes_type_t  serdes_type,
                                      BOOL8                enable  )
{
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              clk_en_val;
    /* Initialize to 0 so it is set before used. */
    UINT32              set_mask = 0;
    UINT32              stl256_rst;

    PMC_ENTRY();
   

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_type < LAST_CBRC_SERDES_TYPE, CBRC_ERR_INVALID_ARG, serdes_type, LAST_CBRC_SERDES_TYPE);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if (PMC_SUCCESS == result)
    {
         
        /* Set RX and TX muxes for MLDs */
        switch (serdes_type)
        {
        case CBRC_SERDES_TYPE_10G_XFI:
                 
            break;
            
        case CBRC_SERDES_TYPE_40G_STL:
            if (0 == link)
            {
                if (enable == TRUE ){
                    set_mask = 0x1; 
                } 
                else {
                    set_mask = 0x6; 
                }
            }
            if (4 == link)
            {
                if ( enable == TRUE ){
                    set_mask = 0x2; 
                } 
                else {
                    set_mask = 0x5; 
                }
            }
            if (8 == link)
            {
                if (enable == TRUE) {
                    set_mask = 0x4;
                } 
                else {
                    set_mask = 0x3;
                }
            }

            clk_en_val = mldprbs_pcbi_field_STL256_CLK_EN_get(NULL,cbrc_handle_ptr);
            PMC_LOG_TRACE("[%s:%d] clk_en_val after read", __FUNCTION__, clk_en_val);
            stl256_rst = mldprbs_pcbi_field_STL256_SYNC_RST_get(NULL,cbrc_handle_ptr);
            PMC_LOG_TRACE("[%s:%d] stl256_rst after read", __FUNCTION__, stl256_rst);
            PMC_LOG_TRACE("[%s:%d] enable = ", __FUNCTION__, enable);
            PMC_LOG_TRACE("[%s:%d] clk_en_mask = ", __FUNCTION__, set_mask);

            if (enable == TRUE) 
            {
                stl256_rst = stl256_rst & (0x7 & ~set_mask); 
                clk_en_val = clk_en_val | set_mask;
            } 
            else 
            {
                stl256_rst = stl256_rst | (0x7 & ~set_mask);
                clk_en_val = clk_en_val & set_mask;
            }


            PMC_LOG_TRACE("[%s:%d] stl256_rst to set", __FUNCTION__, stl256_rst);
            mldprbs_pcbi_field_STL256_SYNC_RST_set(NULL, cbrc_handle_ptr, stl256_rst);
            mldprbs_pcbi_field_STL256_CLK_EN_set(NULL, cbrc_handle_ptr,clk_en_val);

            break;
            
        case CBRC_SERDES_TYPE_40G_SFIS:
            if (0 == link)
            {
                if (enable == TRUE ){
                    set_mask = 1; 
                } 
                else {
                    set_mask = 0x6; 
                }
            }
            else
            {
                if ( enable == TRUE ){
                    set_mask = 0x2; 
                } 
                else {
                    set_mask = 0x5; 
                }
            }
                
            clk_en_val = mldprbs_pcbi_field_SFIS_CLK_EN_get(NULL,cbrc_handle_ptr);
            PMC_LOG_TRACE("[%s:%d] clk_en_val after read", __FUNCTION__, clk_en_val);

            clk_en_val = TRUE == enable ? clk_en_val | set_mask :
                clk_en_val & set_mask;
            mldprbs_pcbi_field_SFIS_CLK_EN_set(NULL, cbrc_handle_ptr, clk_en_val);
                
            break;
            
        case CBRC_SERDES_TYPE_40G_SFI51:
            if (0 == link)
            {
                if (enable == TRUE ){
                    set_mask = 1; 
                } 
                else {
                    set_mask = 0x6; 
                }
            }
            else
            {
                if ( enable == TRUE ){
                    set_mask = 0x2; 
                } 
                else {
                    set_mask = 0x5; 
                }
            }
                
            clk_en_val = mldprbs_pcbi_field_SFI51_CLK_EN_get(NULL,cbrc_handle_ptr);

            clk_en_val = TRUE == enable ? clk_en_val | set_mask :
                clk_en_val & set_mask;
            mldprbs_pcbi_field_SFI51_CLK_EN_set(NULL, cbrc_handle_ptr, clk_en_val);
            break;
        
        default:
            break;
        } /* end switch */
      
    } /* end if */


    PMC_RETURN(result);
} /* cbrc_mld_prbs_clk_en */

/*******************************************************************************
* cbrc_mld_prbs_rx_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the ingress line side mux on the CBRC to choose traffic from either
*   the line or system.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   rx_sel            - MLD_PRBS_RX_MUX_LINE or MLD_PRBS_RX_MUX_SYSTEM
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_rx_mux_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                          UINT32               link,
                                          mld_prbs_rx_mux_t    rx_sel)
{
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);
    PMC_ASSERT(rx_sel < LAST_MLD_PRBS_RX_MUX, CBRC_ERR_INVALID_ARG, rx_sel, LAST_MLD_PRBS_RX_MUX);


    mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set(NULL, cbrc_handle_ptr, link, rx_sel);

    PMC_RETURN(PMC_SUCCESS);
} /* cbrc_mld_prbs_rx_mux_cfg*/


/*******************************************************************************
* cbrc_mld_prbs_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision post-mld link - configures client mode and brings block out of
*   reset and low power
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to provision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                    UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT8               i;
    UINT8               max_index = 1;
    cbrc_link_info_t   *link_ptr;
    UINT8               sfis_index;
    UINT32              slice;
    UINT32              index;
    cbr_fsgm_mode_t     cbr_fsgm_mode[LAST_CBRC_PORT_MODE];
   
    PMC_ENTRY();

    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    
    if (cbrc_serdes_type_is_40g(link_ptr->serdes))
    {
        max_index = 4;
    }
            
    
    if (0 == link)
    {
        sfis_index = 0;
    }
    else
    {
        sfis_index = 1;
    }



    /* Bring block into reset for configuration */
    switch (link_ptr->serdes)
    {
    case CBRC_SERDES_TYPE_10G_XFI:
        mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        break;
    
    case CBRC_SERDES_TYPE_40G_STL:
        for (i = 0; i < max_index; i++)
        {
            mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link + i, CBRC_RESET);
            mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link + i, CBRC_RESET);
        }
        break;
    
    case CBRC_SERDES_TYPE_40G_SFIS:

        cbrc_sfis_rx_field_RX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        cbrc_sfis_tx_field_TX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        cbrc_sfis_odd_parity_reset(cbrc_handle_ptr, sfis_index);
        break;
    
    case CBRC_SERDES_TYPE_40G_SFI51:
        cbrc_sfi51_field_DSC_FIFO_HI_RD_THRSH_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_SFI51_DSC_FIFO_HI_RD_THRSH);
        cbrc_sfi51_field_RX_SYNC_RESET_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        break;

    default:
        break;
    } /* end switch */

            

    /* Atma : RAM_SD bits to be pulled low for stl256 and tx_fifo */
    mldprbs_pcbi_field_TX_FIFO_RAM_SD_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE); 
    mldprbs_pcbi_field_STL256_RAM_SD_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 


    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, link_ptr->serdes, FALSE);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_mld_prbs_mode_cfg(cbrc_handle_ptr, link, link_ptr->serdes);
        PMC_LOG_TRACE("[%s:%d] 4", __FUNCTION__, __LINE__);
    }

    /* Adding CBR_FSGM configuration for egress path */
    if (PMC_SUCCESS == result)
    {
        /* Map CBRC mode to CBR_FSGM mode */
        cbr_fsgm_mode[CBRC_PORT_MODE_NONE]             = CBR_FSGM_MODE_NONE;
        cbr_fsgm_mode[CBRC_PORT_MODE_40G_STS768]       = CBR_FSGM_MODE_40G_STS768;
        cbr_fsgm_mode[CBRC_PORT_MODE_40G_STM256]       = CBR_FSGM_MODE_40G_STM256;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STS192]       = CBR_FSGM_MODE_10G_STS192;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STM64]        = CBR_FSGM_MODE_10G_STM64;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_ODU1F] = CBR_FSGM_MODE_10G_FC1200_ODU1F;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_ODU2F] = CBR_FSGM_MODE_10G_FC1200_ODU2F;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E] = CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC800]        = CBR_FSGM_MODE_10G_FC800;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_5G]      = CBR_FSGM_MODE_10G_CPRI_5G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_6G]      = CBR_FSGM_MODE_10G_CPRI_6G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_10G]     = CBR_FSGM_MODE_10G_CPRI_10G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_GDPS_5G]      = CBR_FSGM_MODE_10G_GDPS_5G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_GDPS_10G]     = CBR_FSGM_MODE_10G_GDPS_10G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STS192_PRBS]  = CBR_FSGM_MODE_10G_STS192_PRBS;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STM64_PRBS]   = CBR_FSGM_MODE_10G_STM64_PRBS;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_10GE]         = CBR_FSGM_MODE_10G_10GE_LF;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_8B_10B_IDLE]  = CBR_FSGM_MODE_10G_8B_10B_IDLE; 
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_64B_66B_IDLE] = CBR_FSGM_MODE_10G_64B_66B_IDLE; 
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_8B_10B_ARB] = CBR_FSGM_MODE_10G_8B_10B_ARB; 
       
        /* Find slice and index within CBR_FSGM */
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        /* Provision CBR_FSGM */
        result = cbr_fsgm_provision(cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice], index, cbr_fsgm_mode[link_ptr->mode]);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* cbrc_mld_prbs_prov */

/*******************************************************************************
* cbrc_mld_prbs_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovision post-mld link. Puts block back into reset and low power.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deprovision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link)
{
    PMC_ERROR   result = PMC_SUCCESS;
    UINT8       index, slice;

    PMC_ENTRY();
    
    if (PMC_SUCCESS == result)
    {
        /* Find slice and index within CBR_FSGM */
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        /* Provision CBR_FSGM */
        result = cbr_fsgm_unprovision(cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice], index);
        
    } /* end if */
    
    PMC_RETURN(result);
} /* cbrc_mld_prbs_unprov */

/*******************************************************************************
* cbrc_mld_prbs_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates post-mld link - enables channel on block.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*   dir               - Direction
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                        cbrc_port_dir_t   dir,
                                        UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    UINT8               sfis_index;
    UINT8               max_index = 1;
   
    PMC_ENTRY();

    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    if (cbrc_serdes_type_is_40g(link_ptr->serdes))
    {
        max_index = 4;
    }
    
    if (0 == link)
    {
        sfis_index = 0;
    }
    else
    {
        sfis_index = 1;
    }


    
    /* Bring block out of reset */
    switch (link_ptr->serdes)
    {
    case CBRC_SERDES_TYPE_10G_XFI:
        mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
        mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
        break;
        
    case CBRC_SERDES_TYPE_40G_STL:
        break;
        
    case CBRC_SERDES_TYPE_40G_SFIS:
        cbrc_sfis_rx_field_RX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_ACTIVE);
        cbrc_sfis_tx_field_TX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_ACTIVE);
        /* set RX alarms: OOA, OOF, LOS */
        cbrc_sfis_rx_field_RXOOA_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 1);
        cbrc_sfis_rx_field_RXOOF_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 1);
        cbrc_sfis_rx_field_RXLOS_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 1);
        break;
        
    case CBRC_SERDES_TYPE_40G_SFI51:
        cbrc_sfi51_field_RX_SYNC_RESET_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_ACTIVE);
        break;
    
    default:
        break;
    } /* end switch */
    
    
    /* Enable clocks */
    result = cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, link_ptr->serdes, TRUE);

    PMC_RETURN(result);
} /* cbrc_mld_prbs_activate */


/*******************************************************************************
* cbrc_mld_prbs_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates post-mld link - disables channel on block.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*   dir               - Direction
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mld_prbs_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                          cbrc_port_dir_t   dir,
                                          UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    UINT8               sfis_index;
    UINT8               max_index = 1;
    UINT8               i;
    UINT8               index, slice;
   
    PMC_ENTRY();

    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    if (cbrc_serdes_type_is_40g(link_ptr->serdes))
    {
        max_index = 4;
    }
            
    if (0 == link)
    {
        sfis_index = 0;
    }
    else
    {
        sfis_index = 1;
    }



    /* Bring block out back into reset for reconfiguration */
    switch (link_ptr->serdes)
    {
    case CBRC_SERDES_TYPE_10G_XFI:
        mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        break;
        
    case CBRC_SERDES_TYPE_40G_STL:
        for (i = 0; i < max_index; i++)
        {
            mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link + i, CBRC_RESET);
            mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link + i, CBRC_RESET);
        }
        break;
        
    case CBRC_SERDES_TYPE_40G_SFIS:
        cbrc_sfis_rx_field_RX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        cbrc_sfis_tx_field_TX_SW_RST_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        /* unset RX alarms: OOA, OOF, LOS */
        cbrc_sfis_rx_field_RXOOA_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 0);
        cbrc_sfis_rx_field_RXOOF_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 0);
        cbrc_sfis_rx_field_RXLOS_ALARM_EN_set(NULL, cbrc_handle_ptr, sfis_index, 0);
        break;
        
    case CBRC_SERDES_TYPE_40G_SFI51:
        cbrc_sfi51_field_RX_SYNC_RESET_set(NULL, cbrc_handle_ptr, sfis_index, CBRC_RESET);
        break;
    
    default:
        break;
    } /* end switch */

   
    /* Disable clocks */
    result = cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, link_ptr->serdes, FALSE);
    
    if (PMC_SUCCESS == result)
    {
        /* Find slice and index within CBR_FSGM */
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        /* Deactivate CBR_FSGM */
        result = cbr_fsgm_deactivate(cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice], index);
        
    } /* end if */


    PMC_RETURN(PMC_SUCCESS);
} /* cbrc_mld_prbs_deactivate */

/*******************************************************************************
* cbrc_mld_prbs_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup links MLD_PRBS register space associated to given list.
*
* INPUTS:
*   cbrc_handle_ptr     - Pointer to subsystem handle
*   num_link_to_clean   - Number of links to cleanup
*   links               - List of links to cleanup
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_mld_prbs_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            num_link_to_clean,
                                   UINT32           *links)
{
    PMC_ERROR   result = PMC_SUCCESS;
    UINT32 link;
    UINT32 itr;

    PMC_ENTRY();
    
    for(itr = 0; itr < num_link_to_clean; itr += 1)
    {
        link = links[itr];
        mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        /* possible SFIS lane */
        if (0 == (link % LINK_MASK_SLICE_OFFSET))
        {
            if (2 > (link / LINK_MASK_SLICE_OFFSET))
            {
                cbrc_sfis_rx_field_RX_SW_RST_set(NULL, cbrc_handle_ptr, link / LINK_MASK_SLICE_OFFSET, CBRC_RESET);
                cbrc_sfis_tx_field_TX_SW_RST_set(NULL, cbrc_handle_ptr, link / LINK_MASK_SLICE_OFFSET, CBRC_RESET);
                cbrc_sfis_odd_parity_reset(cbrc_handle_ptr, link / LINK_MASK_SLICE_OFFSET);
                (void)cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, CBRC_SERDES_TYPE_40G_SFIS, FALSE);
                (void)cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, CBRC_SERDES_TYPE_40G_SFI51, FALSE);
            }
            (void)cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, CBRC_SERDES_TYPE_40G_STL, FALSE);
        }

        /* Disable clocks */
        result = cbrc_mld_prbs_clk_en(cbrc_handle_ptr, link, CBRC_SERDES_TYPE_10G_XFI, FALSE);
    }
    
    PMC_RETURN();
} /* cbrc_mld_prbs_cleanup */

/*******************************************************************************
* cbrc_fc_ilvr_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures post-mld link to handle desired client traffic.   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to provision (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_ilvr_mode_cfg(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    UINT32                      slice;

    /* Initialize all array elements to 0 */
    fc800_pmg_mode_t            fc_cbr_mode[LAST_CBRC_PORT_MODE] = { FC800_PMG_MODE_NONE };
    
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);


    if (PMC_SUCCESS == result)
    {
        /* atma : REVIST */ 
        /* CBRC_FC_ILVR be brought out of RAM_SD modes*/
        cbrc_fc_ilvr_field_MPMA_RAM_SD_REG_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 
        cbrc_fc_ilvr_field_PKTINT_RAM_SD_REG_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 
    }


    if (PMC_SUCCESS == result)
    {
        /* Configure FC800_PMG */
        fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;

        /* Map mode to FC800_PMG input */
        fc_cbr_mode[CBRC_PORT_MODE_40G_STS768]       = FC800_PMG_MODE_40G_STS768;
        fc_cbr_mode[CBRC_PORT_MODE_40G_STM256]       = FC800_PMG_MODE_40G_STM256;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STS192]       = FC800_PMG_MODE_10G_STS192;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STM64]        = FC800_PMG_MODE_10G_STM64;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_ODU1F] = FC800_PMG_MODE_10G_FC1200_ODU1F;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_ODU2F] = FC800_PMG_MODE_10G_FC1200_ODU2F;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E] = FC800_PMG_MODE_10G_FC1200_GFP_ODU2E;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC800]        = FC800_PMG_MODE_10G_FC800;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_5G]      = FC800_PMG_MODE_10G_CPRI_5G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_6G]      = FC800_PMG_MODE_10G_CPRI_6G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_10G]     = FC800_PMG_MODE_10G_CPRI_10G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_GDPS_5G]      = FC800_PMG_MODE_10G_GDPS_5G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_GDPS_10G]     = FC800_PMG_MODE_10G_GDPS_10G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STS192_PRBS]  = FC800_PMG_MODE_10G_STS192_PRBS;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STM64_PRBS]   = FC800_PMG_MODE_10G_STM64_PRBS;
        fc_cbr_mode[CBRC_PORT_MODE_10G_10GE]         = FC800_PMG_MODE_10G_STS192;
        fc_cbr_mode[CBRC_PORT_MODE_10G_8B_10B_IDLE]  = FC800_PMG_MODE_10G_8B_10B_IDLE;
        fc_cbr_mode[CBRC_PORT_MODE_10G_64B_66B_IDLE] = FC800_PMG_MODE_10G_64B_66B_IDLE;
        fc_cbr_mode[CBRC_PORT_MODE_10G_8B_10B_ARB] =  FC800_PMG_MODE_10G_8B_10B_ARB;

        result = fc800_pmg_mode_cfg(fc_cbr_ptr, link, fc_cbr_mode[link_ptr->mode], TRUE);
        
    } /* end if */
         
  


    if (PMC_SUCCESS == result)
    {
        /* Set pkt/cbr int IDs */       
        slice = link / 4;
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_10GE:
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
        case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_64B_66B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:     
            cbr_int_field_MULTI_LANE_40G_SEL_set(NULL, cbrc_handle_ptr, slice, CBRC_DISABLE);
            cbr_int_field_CHN_ID_set(NULL, cbrc_handle_ptr, link, link);
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_INT_ID_UNUSED);
            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link, link);
            fc_tttm_field_OSIR_NUM_set(NULL, cbrc_handle_ptr, link, CBRC_FC_TTTM_OSIR_NUM);
            fc_tttm_field_OSIR_DENOM_set(NULL, cbrc_handle_ptr, link, CBRC_FC_TTTM_OSIR_DENOM);
            cbrc_fc_ilvr_field_range_GPO_REG_set(NULL, cbrc_handle_ptr, CBRC_FC_ILVR_48B_ENABLE_BIT, CBRC_FC_ILVR_48B_ENABLE_BIT, CBRC_DISABLE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbr_int_field_MULTI_LANE_40G_SEL_set(NULL, cbrc_handle_ptr, slice, CBRC_ENABLE);
            cbr_int_field_CHN_ID_set(NULL, cbrc_handle_ptr, link,   link); 
            cbr_int_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+1, link);
            cbr_int_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+2, link);
            cbr_int_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+3, link);
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link,   CBRC_PKT_INT_ID_UNUSED);
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link+1, CBRC_PKT_INT_ID_UNUSED);
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link+2, CBRC_PKT_INT_ID_UNUSED);
            pkt_int_field_ID_set(NULL, cbrc_handle_ptr, link+3, CBRC_PKT_INT_ID_UNUSED);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
    }


    PMC_RETURN(result);

} /* cbrc_fc_ilvr_mode_cfg */


/*******************************************************************************
* cbrc_fc_ilvr_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions post-mld link on FC_ILVR. Configures client mode and brings block
*   out of reset and low power.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to provision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_ilvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    DOUBLE                      client_rate = 0;
    UINT32                      pkt_size;
    util_opsa_cn_frame_period_t cn_frm_period;
    util_opsa_pkt_period_calc_t pkt_period_calc;
    UINT32                      scbs3_user_bits_2_1;
    UINT32                      cfg_pp_n = 0;
    UINT32                      cfg_pp_d = 0;
    UINT32                      cfg_pp_int = 0;
    DOUBLE                      epsilon = 0;
    BOOL8                        sch_only;
    UINT32                      scbs3_rate;
    UINT32                      scbs3_user_bits;
    UINT8                       slice, index;
    cbr_fsgm_mode_t             cbr_fsgm_mode[LAST_CBRC_PORT_MODE];
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc1200_pmon_handle_t       *fc1200_ptr;
    
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
 

    
    if (PMC_SUCCESS == result)
    {
        /* Provision FC800_PMG */
        fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;
        result = fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_RESET); 
    }


    if (PMC_SUCCESS == result)
    {
        fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_rx_ptr;
        result = fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_RESET); 
    }

    if (PMC_SUCCESS == result)
    {
        /* Bring block into reset for configuration */
        fc_tttm_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        fc1200_pcs_rx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
    }
    

    /* Provision CBR_FSGM */
    if (PMC_SUCCESS == result)
    {
        /* Map CBRC mode to CBR_FSGM mode */
        cbr_fsgm_mode[CBRC_PORT_MODE_NONE]             = CBR_FSGM_MODE_NONE;
        cbr_fsgm_mode[CBRC_PORT_MODE_40G_STS768]       = CBR_FSGM_MODE_40G_STS768;
        cbr_fsgm_mode[CBRC_PORT_MODE_40G_STM256]       = CBR_FSGM_MODE_40G_STM256;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STS192]       = CBR_FSGM_MODE_10G_STS192;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STM64]        = CBR_FSGM_MODE_10G_STM64;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_ODU1F] = CBR_FSGM_MODE_10G_FC1200_ODU1F;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_ODU2F] = CBR_FSGM_MODE_10G_FC1200_ODU2F;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E] = CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_FC800]        = CBR_FSGM_MODE_10G_FC800;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_5G]      = CBR_FSGM_MODE_10G_CPRI_5G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_6G]      = CBR_FSGM_MODE_10G_CPRI_6G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_CPRI_10G]     = CBR_FSGM_MODE_10G_CPRI_10G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_GDPS_5G]      = CBR_FSGM_MODE_10G_GDPS_5G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_GDPS_10G]     = CBR_FSGM_MODE_10G_GDPS_10G;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STS192_PRBS]  = CBR_FSGM_MODE_10G_STS192_PRBS;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_STM64_PRBS]   = CBR_FSGM_MODE_10G_STM64_PRBS;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_10GE]         = CBR_FSGM_MODE_10G_10GE_LF;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_8B_10B_IDLE]  = CBR_FSGM_MODE_10G_8B_10B_IDLE;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_64B_66B_IDLE] = CBR_FSGM_MODE_10G_64B_66B_IDLE;
        cbr_fsgm_mode[CBRC_PORT_MODE_10G_8B_10B_ARB]  = CBR_FSGM_MODE_10G_8B_10B_ARB;
    
        /* Find slice and index within CBR_FSGM */
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        /* Provision CBR_FSGM */
        result = cbr_fsgm_provision(cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice], index, cbr_fsgm_mode[link_ptr->mode]);
        
    } /* end if */



    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc_ilvr_mode_cfg(cbrc_handle_ptr, link);
    } /* end if */
        


    /* Configure MPMA if link is not FC1200 GFP */
    if ((PMC_SUCCESS == result) && !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
    {
        client_rate = cbrc_client_rate_get(cbrc_handle_ptr, link_ptr->mode);

        cn_frm_period       = UTIL_OPSA_T_FRAME_UNUSED; 
        pkt_period_calc     = UTIL_OPSA_EPS_MIN;
        sch_only            = FALSE;

        if ( CBRC_PORT_MODE_10G_10GE != link_ptr->mode)
        {
            pkt_size            = MPMA_DEFAULT_PKT_SIZE;
            scbs3_user_bits_2_1 = 0;
        }
        else
        {
            pkt_size            = MPMA_DEFAULT_PKT_SIZE + 1;
            scbs3_user_bits_2_1 = 0;
        }
        
        result = mpma_ch_prov(cbrc_handle_ptr->mpma_ptr, link, client_rate,
                              pkt_size, cn_frm_period, pkt_period_calc, 
                              scbs3_user_bits_2_1, sch_only,
                              cfg_pp_int, cfg_pp_n, cfg_pp_d, epsilon); 
    }
   

    if (PMC_SUCCESS == result)
    {
        /* Configure SCBS3 */
        if (cbrc_serdes_type_is_40g(link_ptr->serdes)) {
            scbs3_rate = 40;
        } else {
            scbs3_rate = 10;
        }
        
        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode)) {
            scbs3_user_bits = 0; /* Selects FC1200 path into SCBS3 */
        } else {
            scbs3_user_bits = 1; /* Selects MPMA path into SCBS3 */
        }

        result = scbs3_chnl_prov(cbrc_handle_ptr->pkt_int_scbs_ptr, link, scbs3_rate, UTIL_SCHD_1G_BIT_SEC, scbs3_user_bits); 
    }

    if (PMC_SUCCESS == result)
    {
        /* init FC800_PMG if required*/
        if (TRUE == fc800_pmg_start_state_test(cbrc_handle_ptr->fc800_pmg_rx_ptr))
        {
            fc800_pmg_init(cbrc_handle_ptr->fc800_pmg_rx_ptr, PMC_ENERGY_STATE_RUNNING);
        }
        
        /* init FC1200_PMON if required*/
        if (TRUE == fc1200_pmon_start_state_test(cbrc_handle_ptr->fc1200_pmon_rx_ptr))
        {
            fc1200_pmon_init(cbrc_handle_ptr->fc1200_pmon_rx_ptr, PMC_ENERGY_STATE_RUNNING);
        }


        /* enable MPMA if required*/
        if (FALSE == mpma_start_state_test(cbrc_handle_ptr->mpma_ptr))
        {
            mpma_enable_cfg(cbrc_handle_ptr->mpma_ptr, TRUE);
        }
    } /* end if */


    PMC_RETURN(result);
} /* cbrc_fc_ilvr_prov */



/*******************************************************************************
* cbrc_fc_ilvr_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions post-mld link on FC_ILVR. Puts block back into reset and
*   low power.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deprovision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_ilvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    UINT8               index, slice;
    
    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if ((PMC_SUCCESS == result) && !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
    {
        result = mpma_ch_deprov(cbrc_handle_ptr->mpma_ptr, link);
    }

    if (PMC_SUCCESS == result)
    {
        /* Find slice and index within CBR_FSGM */
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        /* Provision CBR_FSGM */
        result = cbr_fsgm_unprovision(cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice], index);
        
    } /* end if */




    if (PMC_SUCCESS == result)
    {
        result = scbs3_chnl_deprov(cbrc_handle_ptr->pkt_int_scbs_ptr, link);
    }

    if (PMC_SUCCESS == result)
    {
        /* init FC800_PMG if required*/
        if (TRUE == fc800_pmg_start_state_test(cbrc_handle_ptr->fc800_pmg_rx_ptr))
        {
            fc800_pmg_init(cbrc_handle_ptr->fc800_pmg_rx_ptr, PMC_ENERGY_STATE_RESET);
        }
        
        /* init FC1200_PMON if required*/
        if (TRUE == fc1200_pmon_start_state_test(cbrc_handle_ptr->fc1200_pmon_rx_ptr))
        {
            fc1200_pmon_init(cbrc_handle_ptr->fc1200_pmon_rx_ptr, PMC_ENERGY_STATE_RESET);
        }
        
        /* disable MPMA if required*/
        if (TRUE == mpma_start_state_test(cbrc_handle_ptr->mpma_ptr))
        {
            mpma_enable_cfg(cbrc_handle_ptr->mpma_ptr, FALSE);
        }
    } /* end if */

    PMC_RETURN(result);
} /* cbrc_fc_ilvr_unprov */


/*******************************************************************************
* cbrc_fc_ilvr_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates post-mld link - enables channel on block.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_ilvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    UINT8                       slice, index;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc1200_pmon_handle_t       *fc1200_ptr;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
   

    if (PMC_SUCCESS == result)
    {
        if (cbrc_port_mode_is_fc1200(link_ptr->mode))
        {
            /* Enable PCS for all FC1200 modes */
            fc1200_pcs_rx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
        }

        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            /* Enable FC1200 transcoder for only FC1200 GFP clients */
            fc_tttm_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
            
            fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_rx_ptr;
            result = fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_ACTIVE); 
        }
        else
        {
            /* Enable FC800 PMG */
            fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;
            fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_ACTIVE); 
            
            switch (link_ptr->mode)
            {
            case CBRC_PORT_MODE_10G_FC800:
            case CBRC_PORT_MODE_10G_CPRI_5G:
            case CBRC_PORT_MODE_10G_CPRI_6G:
            case CBRC_PORT_MODE_10G_CPRI_10G:
            case CBRC_PORT_MODE_10G_GDPS_5G:
            case CBRC_PORT_MODE_10G_GDPS_10G:
            case CBRC_PORT_MODE_10G_8B_10B_IDLE:
            case CBRC_PORT_MODE_10G_8B_10B_ARB:       
                cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
                break;

            default:
                break;
            }
            
           
            /* Find slice and index within CBR_FSGM */
            slice = link / CBRC_CBR_FSGM_MAX_INDEX;
            index = link % CBRC_CBR_FSGM_MAX_INDEX;
           
            /* Activate CBR_FSGM */
            result = cbr_fsgm_activate(cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice], index);
        } 
    }

    
    
    
    /* Enable channels */ 
    if (PMC_SUCCESS == result)
    {
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
        case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:
            cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            /* fall through */

        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_10GE:
            cbrc_fc_ilvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            pkt_int_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
            
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
		case CBRC_PORT_MODE_10G_64B_66B_IDLE:
            cbrc_fc_ilvr_field_FCPTE_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbrc_fc_ilvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            pkt_int_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbrc_fc_ilvr_field_FCPTE_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            pkt_int_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbrc_fc_ilvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE); 
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+1, CBRC_ENABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+2, CBRC_ENABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+3, CBRC_ENABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
    } 
    

    
    if (PMC_SUCCESS == result) 
    {
        result = scbs3_chnl_activate(cbrc_handle_ptr->pkt_int_scbs_ptr, link);
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = scbs3_page_udpate_and_swap(cbrc_handle_ptr->pkt_int_scbs_ptr);
    }
   


    /* RX dir - MPMA */
    if ((PMC_SUCCESS == result) && !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
    {
        result = mpma_ch_activate(cbrc_handle_ptr->mpma_ptr, link, TRUE);
    }


    PMC_RETURN(result);
} /* cbrc_fc_ilvr_activate */




/*******************************************************************************
* cbrc_fc_ilvr_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates post-mld link - disables channel on block.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_ilvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    UINT8                       slice, index;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc1200_pmon_handle_t       *fc1200_ptr;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);
    
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
   

    if (PMC_SUCCESS == result)
    {
        /* Disable channels */ 
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
        case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:
            cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            /* Fall through */

        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_10GE:
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
            
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
		case CBRC_PORT_MODE_10G_64B_66B_IDLE:
            cbrc_fc_ilvr_field_FCPTE_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            cbrc_fc_ilvr_field_FCPTE_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            pkt_int_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE); 
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+1, CBRC_DISABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+2, CBRC_DISABLE);
            cbr_int_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+3, CBRC_DISABLE);
            cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
        
        
        if (cbrc_port_mode_is_fc1200(link_ptr->mode))
        {
            /* Disable PCS for all FC1200 modes */
            fc1200_pcs_rx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        }
        
        /* Bring blocks into reset to allow configuration */
        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            fc_tttm_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
            
            fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_rx_ptr;
            result = fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_RESET); 
        } 
        else
        {
            fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;
            fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_RESET); 
            

            /* Find slice and index within CBR_FSGM */
            slice = link / CBRC_CBR_FSGM_MAX_INDEX;
            index = link % CBRC_CBR_FSGM_MAX_INDEX;
           
            /* Deactivate CBR_FSGM */
            result = cbr_fsgm_deactivate(cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice], index);
        } 
    }
    
    
    /* RX dir - MPMA */
    if ((PMC_SUCCESS == result) && !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
    {
        result = mpma_ch_deactivate(cbrc_handle_ptr->mpma_ptr, link);
    }

    if (PMC_SUCCESS == result) 
    {
        result = scbs3_chnl_deactivate(cbrc_handle_ptr->pkt_int_scbs_ptr, link);
    }

    if (PMC_SUCCESS == result) 
    {
        result = scbs3_page_udpate_and_swap(cbrc_handle_ptr->pkt_int_scbs_ptr);
    }

    PMC_RETURN(result);
} /* cbrc_fc_ilvr_deactivate */


/*******************************************************************************
* cbrc_fc_ilvr_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup links ILVR register space associated to given list.
*
* INPUTS:
*   cbrc_handle_ptr     - Pointer to subsystem handle
*   num_link_to_clean   - Number of links to cleanup
*   links               - List of links to cleanup
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_fc_ilvr_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                  UINT32            num_link_to_clean,
                                  UINT32           *links)
{
    cbrc_link_info_t   *link_ptr;
    UINT32 link;
    UINT32 itr;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    
    PMC_ENTRY();    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    for (itr = 0; itr < CBRC_CBR_FSGM_MAX_SLICE; itr += 1)
    {
        /* cleanup CBR_FSGM */
        cbr_fsgm_cleanup(cbrc_handle_ptr->cbr_fsgm_rx_ptr[itr], itr * CBRC_CBR_FSGM_MAX_INDEX, num_link_to_clean, links);

    }
    
    fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;

    for(itr = 0; itr < num_link_to_clean; itr+=1)
    {
        link = links[itr];
        link_ptr = &cbrc_handle_ptr->var.links[link];
        
        (void)mpma_ch_deprov(cbrc_handle_ptr->mpma_ptr, link);
                                      
        (void) scbs3_chnl_deprov(cbrc_handle_ptr->pkt_int_scbs_ptr, link);
        
        /* init FC800_PMG if required*/            
        cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);        
        cbrc_fc_ilvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
        cbrc_fc_ilvr_field_FCPTE_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
        cbr_int_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
               
        (void)fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_RESET);         
            
        /* Bring block into reset for configuration */
        fc1200_pmon_reset(cbrc_handle_ptr->fc1200_pmon_rx_ptr, link, FC1200_PMON_RESET);
        fc_tttm_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        fc1200_pcs_rx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);

        /* RX dir - MPMA */        
        (void)mpma_ch_deactivate(cbrc_handle_ptr->mpma_ptr, link);        
        (void)scbs3_chnl_deactivate(cbrc_handle_ptr->pkt_int_scbs_ptr, link);
    }

    (void)scbs3_page_udpate_and_swap(cbrc_handle_ptr->pkt_int_scbs_ptr);

    PMC_RETURN();
} /* cbrc_fc_ilvr_cleanup */
    
/*******************************************************************************
* cbrc_fc800_dlvr_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set channel FIFO threshold for a FC800 channel.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post MLD link to configure (0..11)
*   fifo_thresh       - Channel FIFO threshold
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_dlvr_mode_cfg(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link, 
                                       UINT32            fifo_thresh)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    UINT8                       slice;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc800_pmg_mode_t            fc_cbr_mode[LAST_CBRC_PORT_MODE] = { FC800_PMG_MODE_NONE };
    
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);
    PMC_ASSERT(fifo_thresh != 0xffffffff, CBRC_ERR_INVALID_ARG, link_ptr->mode, fifo_thresh);
    
    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if (PMC_SUCCESS == result)
    {
        /* Set pkt/cbr int IDs */       
        slice = link / 4;
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_8B_10B_ARB: 
            cbr_dint_field_MULTI_LANE_40G_SEL_set(NULL, cbrc_handle_ptr, slice, CBRC_DISABLE);
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link, link);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link, fifo_thresh);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_INT_ID_UNUSED);
            
            cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
            
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
    }
    
    PMC_RETURN(result);
} /* cbrc_fc800_dlvr_mode_cfg */

/*******************************************************************************
* cbrc_fc_dlvr_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-MLD link to configure (0..11)
*   
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_dlvr_mode_cfg(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    UINT8                       slice;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc800_pmg_mode_t            fc_cbr_mode[LAST_CBRC_PORT_MODE] = { FC800_PMG_MODE_NONE };
    UINT32                      fifo_thresh;
    
    PMC_ENTRY();
   

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);

    if (PMC_SUCCESS == result)
    {
        /* CBRC_FC_DLVR be brought out of RAM_SD modes*/
        cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 
        cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 
        cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set(NULL, cbrc_handle_ptr, CBRC_DISABLE); 
    }


    if (PMC_SUCCESS == result)
    {
        /* Configure FC800_PMG */
        fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_tx_ptr;

        /* Map mode to FC800_PMG input */
        fc_cbr_mode[CBRC_PORT_MODE_40G_STS768]       = FC800_PMG_MODE_40G_STS768;
        fc_cbr_mode[CBRC_PORT_MODE_40G_STM256]       = FC800_PMG_MODE_40G_STM256;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STS192]       = FC800_PMG_MODE_10G_STS192;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STM64]        = FC800_PMG_MODE_10G_STM64;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_ODU1F] = FC800_PMG_MODE_10G_FC1200_ODU1F;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_ODU2F] = FC800_PMG_MODE_10G_FC1200_ODU2F;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E] = FC800_PMG_MODE_10G_FC1200_GFP_ODU2E;
        fc_cbr_mode[CBRC_PORT_MODE_10G_FC800]        = FC800_PMG_MODE_10G_FC800;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_5G]      = FC800_PMG_MODE_10G_CPRI_5G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_6G]      = FC800_PMG_MODE_10G_CPRI_6G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_CPRI_10G]     = FC800_PMG_MODE_10G_CPRI_10G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_GDPS_5G]      = FC800_PMG_MODE_10G_GDPS_5G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_GDPS_10G]     = FC800_PMG_MODE_10G_GDPS_10G;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STS192_PRBS]  = FC800_PMG_MODE_10G_STS192_PRBS;
        fc_cbr_mode[CBRC_PORT_MODE_10G_STM64_PRBS]   = FC800_PMG_MODE_10G_STM64_PRBS;
        fc_cbr_mode[CBRC_PORT_MODE_10G_10GE]         = FC800_PMG_MODE_10G_STS192;
		fc_cbr_mode[CBRC_PORT_MODE_10G_8B_10B_IDLE]  = FC800_PMG_MODE_10G_8B_10B_IDLE;
		fc_cbr_mode[CBRC_PORT_MODE_10G_64B_66B_IDLE] = FC800_PMG_MODE_10G_64B_66B_IDLE;
        fc_cbr_mode[CBRC_PORT_MODE_10G_8B_10B_ARB]  = FC800_PMG_MODE_10G_8B_10B_ARB;

        result = fc800_pmg_mode_cfg(fc_cbr_ptr, link, fc_cbr_mode[link_ptr->mode], FALSE);
        
    } /* end if */

    /* Setup FIFO thresholds for non FC1200 GFP datapaths*/
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            switch (link_ptr->mode)
            {
            case CBRC_PORT_MODE_10G_FC800:
		    case CBRC_PORT_MODE_10G_8B_10B_IDLE:
            case CBRC_PORT_MODE_10G_8B_10B_ARB:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_FC800;
                break;

            case CBRC_PORT_MODE_10G_FC1200_ODU1F:
            case CBRC_PORT_MODE_10G_FC1200_ODU2F:
			case CBRC_PORT_MODE_10G_64B_66B_IDLE:				
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_FC1200;
                break;

            case CBRC_PORT_MODE_10G_CPRI_5G:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_CPRI_5G;
                break;

            case CBRC_PORT_MODE_10G_CPRI_6G:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_CPRI_6G;
                break;

            case CBRC_PORT_MODE_10G_CPRI_10G:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_CPRI_10G;
                break;
                
            case CBRC_PORT_MODE_10G_GDPS_5G:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_GDPS_5G;
                break;

            case CBRC_PORT_MODE_10G_GDPS_10G:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_GDPS_10G;
                break;

            case CBRC_PORT_MODE_10G_STS192:
            case CBRC_PORT_MODE_10G_STM64:
            case CBRC_PORT_MODE_10G_STS192_PRBS:
            case CBRC_PORT_MODE_10G_STM64_PRBS:
            case CBRC_PORT_MODE_10G_10GE:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_STS192;
                break;

            case CBRC_PORT_MODE_40G_STS768:
            case CBRC_PORT_MODE_40G_STM256:
                fifo_thresh = CBRC_PKT_DINT_FIFO_THRESH_STS768;
                break;
                 
            case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            default:
                PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
                break;
            } /* end switch */
        }
        else
        {
            fifo_thresh = 0xffffffff; /* set to invalid value for check below */
        }
    } /* end if */



    if (PMC_SUCCESS == result)
    {
        /* Set pkt/cbr int IDs */       
        slice = link / 4;
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_10GE:
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
		case CBRC_PORT_MODE_10G_8B_10B_IDLE:
		case CBRC_PORT_MODE_10G_64B_66B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:     
            PMC_ASSERT(fifo_thresh != 0xffffffff, CBRC_ERR_INVALID_ARG, link_ptr->mode, fifo_thresh);
            cbr_dint_field_MULTI_LANE_40G_SEL_set(NULL, cbrc_handle_ptr, slice, CBRC_DISABLE);
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link, link);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link, fifo_thresh);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_INT_ID_UNUSED);
            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            /* Set CORRUPT_BLK_MODE for transcoded FC1200*/
            result = cbrc_fc1200_tx_corrupt_blk_conv_enable(cbrc_handle_ptr,
                                                            link,
                                                            TRUE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbr_dint_field_MULTI_LANE_40G_SEL_set(NULL, cbrc_handle_ptr, slice, CBRC_ENABLE);
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link, link); 
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+1, link);
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+2, link);
            cbr_dint_field_CHN_ID_set(NULL, cbrc_handle_ptr, link+3, link);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link, fifo_thresh);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link+1, fifo_thresh);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link+2, fifo_thresh);
            cbr_dint_field_CHN_FIFO_TH_set(NULL, cbrc_handle_ptr, link+3, fifo_thresh);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_INT_ID_UNUSED);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link+1, CBRC_PKT_INT_ID_UNUSED);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link+2, CBRC_PKT_INT_ID_UNUSED);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link+3, CBRC_PKT_INT_ID_UNUSED);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
    }

    /* Setup PKT_DINT_XOFF threshold for all datapaths 
     * Operation section needs an update.. This register needs to be set to 
     * a non-zero value without which Xoff will never get de-asserted */
    pkt_dint_field_CFC_XOFF_THRESHOLD_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_DINT_CFC_XOFF_THRESHOLD);

    /* PKT_DINT's Zone Config Register bits */ 
    pkt_dint_field_UF_ZONE_MODE_set(NULL, cbrc_handle_ptr,link,CBRC_PKT_DINT_UF_ZONE_MODE);
    pkt_dint_field_ZONE_XOFF_SET_THRESHOLD_set(NULL, cbrc_handle_ptr,link,CBRC_PKT_DINT_ZONE_XOFF_SET_THRESHOLD);
    pkt_dint_field_ZONE_XOFF_CLEAR_THRESHOLD_set(NULL, cbrc_handle_ptr,link,CBRC_PKT_DINT_ZONE_XOFF_CLEAR_THRESHOLD);
    
    pkt_dint_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);


    PMC_RETURN(result);
} /* cbrc_fc_dlvr_mode_cfg */







/*******************************************************************************
* cbrc_fc_dlvr_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions post-mld link on FC_DLVR. Configures client mode and brings block
*   out of reset and low power.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to provision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_dlvr_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                   UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc1200_pmon_handle_t       *fc1200_ptr;



    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);


    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    
    if (PMC_SUCCESS == result)
    {
        /* Bring block into reset for configuration */
        fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_tx_ptr;
        fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_RESET); 
        
        fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_tx_ptr;
        fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_RESET); 
        
        fc_tttd_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        fc1200_pcs_tx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET);
        cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_RESET); 

        result = cbrc_fc_dlvr_mode_cfg(cbrc_handle_ptr, link);
    }


    if (PMC_SUCCESS == result)
    {
        /* init FC800_PMG if required*/
        if (TRUE == fc800_pmg_start_state_test(cbrc_handle_ptr->fc800_pmg_tx_ptr))
        {
            fc800_pmg_init(cbrc_handle_ptr->fc800_pmg_tx_ptr, PMC_ENERGY_STATE_RUNNING);
        }
        
        /* init FC1200_PMON if required*/
        if (TRUE == fc1200_pmon_start_state_test(cbrc_handle_ptr->fc1200_pmon_tx_ptr))
        {
            fc1200_pmon_init(cbrc_handle_ptr->fc1200_pmon_tx_ptr, PMC_ENERGY_STATE_RUNNING);
        }
    } /* end if */


    PMC_RETURN(result);
} /* cbrc_fc_dlvr_prov */

/*******************************************************************************
* cbrc_fc_dlvr_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions post-mld link on FC_DLVR. Puts block back into reset and
*   low power.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deprovision (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_dlvr_unprov(cbrc_handle_t    *cbrc_handle_ptr,
                                     UINT32            link)
{
    PMC_ERROR   result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);


    if (PMC_SUCCESS == result)
    {
        /* init FC800_PMG if required*/
        if (TRUE == fc800_pmg_start_state_test(cbrc_handle_ptr->fc800_pmg_tx_ptr))
        {
            fc800_pmg_init(cbrc_handle_ptr->fc800_pmg_tx_ptr, PMC_ENERGY_STATE_RESET);
        }
        
        /* init FC1200_PMON if required*/
        if (TRUE == fc1200_pmon_start_state_test(cbrc_handle_ptr->fc1200_pmon_tx_ptr))
        {
            fc1200_pmon_init(cbrc_handle_ptr->fc1200_pmon_tx_ptr, PMC_ENERGY_STATE_RESET);
        }
    } /* end if */

    PMC_RETURN(result);
} /* cbrc_fc_dlvr_unprov */



/*******************************************************************************
* cbrc_fc_dlvr_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates post-mld link - enables channel on block.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-MLD link
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_dlvr_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    UINT32                      slice, index;
    fc1200_pmon_handle_t       *fc1200_ptr;

    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);


    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
   
    if (PMC_SUCCESS == result)
    {
        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            /* Bring block out of reset because configuration is complete */
            fc_tttd_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
            fc1200_pcs_tx_field_SW_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
            
            fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_tx_ptr;
            result = fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_ACTIVE); 
        } 
        else
        {
            /* Enable FC800 PMG */

            fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_tx_ptr;
            fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_ACTIVE); 
        
        
            switch (link_ptr->mode)
            {
            case CBRC_PORT_MODE_10G_FC800:
            case CBRC_PORT_MODE_10G_CPRI_5G:
            case CBRC_PORT_MODE_10G_CPRI_6G:
            case CBRC_PORT_MODE_10G_CPRI_10G:
            case CBRC_PORT_MODE_10G_GDPS_5G:
            case CBRC_PORT_MODE_10G_GDPS_10G:
			case CBRC_PORT_MODE_10G_8B_10B_IDLE:
            case CBRC_PORT_MODE_10G_8B_10B_ARB:       
                cbrc_fc_dlvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
                break;

            default:
                break;
            }
        } 
    } 
   


    /* Activate TX MLD_PRBS CBR_FSGM */
    if (PMC_SUCCESS == result)
    {
        slice = link / CBRC_CBR_FSGM_MAX_INDEX;
        index = link % CBRC_CBR_FSGM_MAX_INDEX;
       
        result = cbr_fsgm_activate(cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice], index);
    }
       



    /* Enable channels */ 
    if (PMC_SUCCESS == result)
    {
        /* PKT Dinterlaver RST is required for both FC1200 and MPMA path*/
        pkt_dint_field_SOFT_RST_set(NULL, cbrc_handle_ptr, CBRC_ACTIVE);
        
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
        case CBRC_PORT_MODE_10G_10GE:
		case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_64B_66B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:     
            cbrc_fc_dlvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;

            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            cbrc_fc_dlvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbrc_fc_dlvr_field_FCPTD_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link, link);
            pkt_dint_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbrc_fc_dlvr_field_CBR_CH_EN_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link, CBRC_ACTIVE);
            cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link+1, CBRC_ACTIVE); 
            cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link+2, CBRC_ACTIVE); 
            cbr_dint_field_CHN_RESET_set(NULL, cbrc_handle_ptr, link+3, CBRC_ACTIVE); 
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_ENABLE); 
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+1, CBRC_ENABLE);
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+2, CBRC_ENABLE);
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+3, CBRC_ENABLE);
            cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
    }   

    PMC_RETURN(result);
} /* cbrc_fc_dlvr_activate */


/*******************************************************************************
* cbrc_mpmo_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates MPMO
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-MLD link
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                    UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
   
    /* TX dir - MPMO */
    if (PMC_SUCCESS == result)
    {
      if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
      {
            /* FC1200 GFP just requires the mpmo to be enabled */
            result = mpmo_ch_enable_cfg(cbrc_handle_ptr->mpmo_ptr, link, CBRC_ENABLE);
            if (PMC_SUCCESS == result)
            {
                result = scbs3_page_udpate_and_swap(cbrc_handle_ptr->mpmo_ptr->scbs3_handle);
            }
        }
        else
        {
            result = mpmo_ch_activate(cbrc_handle_ptr->mpmo_ptr, link);
        }
    }

    PMC_RETURN(result);
} /* cbrc_mpmo_activate */


/*******************************************************************************
* cbrc_fc_dlvr_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates post-mld link - disables channel on block.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc_dlvr_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;
    fc800_pmg_handle_t         *fc_cbr_ptr;
    fc1200_pmon_handle_t       *fc1200_ptr;

    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Disable channels */ 
        switch (link_ptr->mode)
        {
        case CBRC_PORT_MODE_10G_FC800:
        case CBRC_PORT_MODE_10G_CPRI_5G:
        case CBRC_PORT_MODE_10G_CPRI_6G:
        case CBRC_PORT_MODE_10G_CPRI_10G:
        case CBRC_PORT_MODE_10G_GDPS_5G:
        case CBRC_PORT_MODE_10G_GDPS_10G:
		case CBRC_PORT_MODE_10G_8B_10B_IDLE:
        case CBRC_PORT_MODE_10G_8B_10B_ARB:     
            cbrc_fc_dlvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            /* Fall through */

        case CBRC_PORT_MODE_10G_STS192:
        case CBRC_PORT_MODE_10G_STM64:
        case CBRC_PORT_MODE_10G_STS192_PRBS:
        case CBRC_PORT_MODE_10G_STM64_PRBS:
        case CBRC_PORT_MODE_10G_FC1200_ODU1F:
        case CBRC_PORT_MODE_10G_FC1200_ODU2F:
        case CBRC_PORT_MODE_10G_10GE:
		case CBRC_PORT_MODE_10G_64B_66B_IDLE:
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;

        case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
            cbrc_fc_dlvr_field_FCPTD_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
            pkt_dint_field_ID_set(NULL, cbrc_handle_ptr, link, CBRC_PKT_INT_ID_UNUSED);
            pkt_dint_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
        
        case CBRC_PORT_MODE_40G_STS768:
        case CBRC_PORT_MODE_40G_STM256:
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE); 
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+1, CBRC_DISABLE);
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+2, CBRC_DISABLE);
            cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link+3, CBRC_DISABLE);
            cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
            break;
             
        default:
            PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, link_ptr->mode, LAST_CBRC_PORT_MODE);
            break;
        } /* end switch */
        
        
        /* Bring blocks into reset to allow configuration */
        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            fc1200_ptr = cbrc_handle_ptr->fc1200_pmon_tx_ptr;
            result = fc1200_pmon_reset(fc1200_ptr, link, FC1200_PMON_RESET); 
        } 
        else
        {
            fc_cbr_ptr = cbrc_handle_ptr->fc800_pmg_tx_ptr;
            fc800_pmg_reset(fc_cbr_ptr, link, FC800_PMG_RESET); 
        } 
        
    }
    
    PMC_RETURN(result);
} /* cbrc_fc_dlvr_deactivate */

/*******************************************************************************
* cbrc_fc_dlvr_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup links DLVR register space associated to given list.
*   
* INPUTS:
*   cbrc_handle_ptr     - Pointer to subsystem handle
*   num_link_to_clean   - Number of links to cleanup
*   links               - List of links to cleanup
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_fc_dlvr_cleanup(cbrc_handle_t    *cbrc_handle_ptr,
                                  UINT32            num_link_to_clean,
                                  UINT32           *links)
{
    cbrc_link_info_t   *link_ptr;
    UINT32 link;
    UINT32 itr;
    
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    for (itr = 0; itr < CBRC_CBR_FSGM_MAX_SLICE; itr += 1)
    {
        /* cleanup CBR_FSGM */
        cbr_fsgm_cleanup(cbrc_handle_ptr->cbr_fsgm_tx_ptr[itr], itr * CBRC_CBR_FSGM_MAX_INDEX, num_link_to_clean, links);

    }
    
    for(itr = 0; itr < num_link_to_clean; itr+=1)
    {
        link = links[itr];
        link_ptr = &cbrc_handle_ptr->var.links[link];
        
        /* init FC800_PMG */            
        cbrc_fc_dlvr_field_FC800PMG_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
        cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);         
        cbrc_fc_dlvr_field_FCPTD_CH_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE);
        pkt_dint_field_ID_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);    
        cbr_dint_field_CHN_EN_set(NULL, cbrc_handle_ptr, link, CBRC_DISABLE); 
        cbr_dint_field_CH_UPDATE_set(NULL, cbrc_handle_ptr, CBRC_ENABLE);
           

        (void)fc800_pmg_reset(cbrc_handle_ptr->fc800_pmg_tx_ptr, link, FC800_PMG_RESET);         
            
        /* Bring block into reset for configuration */
        fc1200_pmon_reset(cbrc_handle_ptr->fc1200_pmon_rx_ptr, link, FC1200_PMON_RESET);
    }

    PMC_RETURN();
} /* cbrc_fc_dlvr_cleanup */


/*******************************************************************************
* cbrc_mpmo_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates MPMO
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link to deactivate (0..11)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link)
{
    PMC_ERROR                   result = PMC_SUCCESS;
    cbrc_link_info_t           *link_ptr;

    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);

    /* TX dir - MPMO */
    if (PMC_SUCCESS == result && !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
    {
        if (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            result = mpmo_ch_enable_cfg(cbrc_handle_ptr->mpmo_ptr, link, CBRC_DISABLE);
        }
        else
        {
            result = mpmo_ch_deactivate(cbrc_handle_ptr->mpmo_ptr, link);
        }
    }
    
    PMC_RETURN(result);
} /* cbrc_mpmo_deactivate */


/*******************************************************************************
* cbrc_serdes_type_is_40g
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if a serdes type is for 40G client traffic.
*   
*
* INPUTS:
*   serdes_type       - Serdes type - multiple 40G and single 10G
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the serdes type is 40G.
*   FALSE if the serdes type is not 40G.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_serdes_type_is_40g(cbrc_serdes_type_t   serdes_type)
{
    BOOL8    result = FALSE;
    
    PMC_ENTRY();

    switch (serdes_type)
    {
    case CBRC_SERDES_TYPE_40G_SFI51:
    case CBRC_SERDES_TYPE_40G_SFIS:
    case CBRC_SERDES_TYPE_40G_STL:
        result = TRUE;
        break;
        
    case CBRC_SERDES_TYPE_10G_XFI:
    case CBRC_SERDES_TYPE_NONE:
        result = FALSE;
        break;

    default:
        PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, serdes_type, LAST_CBRC_SERDES_TYPE);
        break;
    } /* end switch */
    
    
    PMC_RETURN(result);
} /* cbrc_serdes_type_is_40g */

/*******************************************************************************
* cbrc_port_is_sonet
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks if CBRC link is provisioned for SONET 
*
* INPUTS:
*   mode              - Client signal
*
* OUTPUTS:
*
* RETURNS:
*   TRUE               - Port mode is SONET/SDH
*   FALSE              - Port mode is not SONET/SDH
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 cbrc_port_is_sonet(cbrc_port_mode_t  mode)
{
    BOOL8    result;

    PMC_ENTRY();

    switch (mode)
    {
    case CBRC_PORT_MODE_40G_STS768:
    case CBRC_PORT_MODE_40G_STM256:
    case CBRC_PORT_MODE_10G_STS192:
    case CBRC_PORT_MODE_10G_STM64:
    case CBRC_PORT_MODE_10G_10GE:
        result = TRUE; 
        break;
         
    default:
        result = FALSE;
        break;
    } /* end switch */
     

    PMC_RETURN(result);
} /* cbrc_port_is_sonet */


/*******************************************************************************
* cbrc_port_is_fc800
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks if CBRC link is provisioned for FC800 
*
* INPUTS:
*   mode              - Client signal
*
* OUTPUTS:
*
* RETURNS:
*   TRUE               - Port mode is FC800
*   FALSE              - Port mode is not FC800
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 cbrc_port_is_fc800(cbrc_port_mode_t  mode)
{
    BOOL8    result;

    PMC_ENTRY();

    switch (mode)
    {
    case CBRC_PORT_MODE_10G_FC800:
        result = TRUE; 
        break;
         
    default:
        result = FALSE;
        break;
    } /* end switch */
     

    PMC_RETURN(result);
} /* cbrc_port_is_fc800 */





/*******************************************************************************
* cbrc_port_mode_is_fc1200
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if a port is configured for FC1200 client traffic
*   
*
* INPUTS:
*   port_mode   - Client traffic configured on port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the port mode type is FC1200
*   FALSE if the port mode type is not FC1200
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_port_mode_is_fc1200(cbrc_port_mode_t  port_mode)
{
    BOOL8    result = FALSE;
    
    PMC_ENTRY();

    switch (port_mode)
    {
    case CBRC_PORT_MODE_10G_FC1200_ODU1F:
    case CBRC_PORT_MODE_10G_FC1200_ODU2F:
    case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:
        result = TRUE;
        break;
        
    default:
        result = FALSE;
        break;
    } /* end switch */
    
    
    PMC_RETURN(result);
} /* cbrc_port_mode_is_fc1200 */



/*******************************************************************************
* cbrc_port_mode_is_fc1200_gfp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if a port is configured for FC1200 ODU2E GFP client traffic
*   
*
* INPUTS:
*   port_mode   - Client traffic configured on port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the port mode type is FC1200 ODU2E GFP
*   FALSE if the port mode type is not FC1200 ODU2E GFP
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbrc_port_mode_is_fc1200_gfp(cbrc_port_mode_t  port_mode)
{
    BOOL8    result = FALSE;
    
    PMC_ENTRY();

    switch (port_mode)
    {
    case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:
        result = TRUE;
        break;
        
    default:
        result = FALSE;
        break;
    } /* end switch */
    
    
    PMC_RETURN(result);
} /* cbrc_port_mode_is_fc1200_gfp */



/*******************************************************************************
*  cbrc_mpmo_ckctl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MPMO clock control ports in CBRC.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   ckctl_port        - clock control port number (0-25): \n
*                       Ports 0--11 are line side 10G SERDES. \n
*                       Ports 12--13 are line side 2.5G SERDES. \n
*                       Ports 14--25 are system side 10G SERDES. \n
*   chnl              - channel to map to the port specified by ckctl_port.
*   enable            - enables/disables CKCTL output
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   CBRC_ERR_NULL_HANDLE
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_mpmo_ckctl_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                     UINT32               ckctl_port, 
                                     UINT32               chnl,
                                     BOOL8                 enable)
{
    cbrc_link_info_t       *link_ptr        = NULL;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    ret_val = mpmo_ckctl_cfg(cbrc_handle_ptr->mpmo_ptr, ckctl_port, chnl, enable);

    if (PMC_SUCCESS == ret_val)
    {
        ret_val = cbrc_link_get(cbrc_handle_ptr, chnl, &link_ptr);
    }

    if (PMC_SUCCESS == ret_val)
    {
        if(cbrc_port_mode_is_fc1200_gfp(link_ptr->mode))
        {
            /* perform explicit setting of CKCTL_EN */
            ret_val = mpmo_ckctl_en_set(cbrc_handle_ptr->mpmo_ptr, ckctl_port, enable);
        }
    }

    PMC_RETURN(ret_val);

} /* cbrc_mpmo_ckctl_cfg */




/*
** Private Functions
*/

/*******************************************************************************
* FUNCTION: cbrc_sdh_pmg_get_handle()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SDH PMG context handle based on direction and link number.
*
* INPUTS:
*   cbrc_handle_ptr - Pointer to subsystem handle
*   dir             - Direction of traffic
*   link            - Post-mld link to deactivate (0..11)

*
* OUTPUTS:
*   sdh_handle_pptr - SDH_PMG handle pointer
*   index_ptr       - Index within SDH_PMG
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sdh_pmg_get_handle(cbrc_handle_t      *cbrc_handle_ptr,
                                          cbrc_port_dir_t     dir,
                                          UINT32              link,
                                          sdh_pmg_handle_t  **sdh_handle_pptr,
                                          UINT32             *index_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;    
    UINT32          slice;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != index_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != sdh_handle_pptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);


    *index_ptr = link % CBRC_SDH_PMG_MAX_INDEX;
    slice      = link / CBRC_SDH_PMG_MAX_INDEX;

    /* Get SDH handle */
    if (CBRC_PORT_DIR_TX == dir) {
        *sdh_handle_pptr = cbrc_handle_ptr->sdh_pmg_tx_ptr[slice];
    } else {
        *sdh_handle_pptr = cbrc_handle_ptr->sdh_pmg_rx_ptr[slice];
    }
        
    PMC_RETURN(result);
} /* cbrc_sdh_pmg_get_handle */




/*******************************************************************************
* FUNCTION: cbrc_link_and_dir_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Returns error when given link or direction are incorrect.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*   None.    
*
* RETURNS:
*   PMC_SUCCESS       - when dir and link are in possible ranges, 
*                        otherwise a descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_link_and_dir_validate(cbrc_handle_t         *handle,
                                             cbrc_port_dir_t       dir,
                                             UINT32                link)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK);
    }

    if (dir >= LAST_CBRC_PORT_DIR)
    {
        PMC_RETURN(CBRC_ERR_INVALID_DIR);
    }

    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* cbrc_port_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets link_info on the appropriate number of links
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link_ptr          - Pointer to first link to be part of the logical port 
*                       The serdes type will determine whether there are one
*                       or four links in the port.
*   link_info_ptr     - Information to set the link with
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*   CBRC_ERR_NULL_HANDLE
*   CBRC_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_port_set(cbrc_handle_t       *cbrc_handle_ptr,
                                cbrc_link_info_t    *link_ptr,
                                cbrc_link_info_t    *link_info_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              max_links;
    UINT32              count;
    cbrc_serdes_type_t  serdes_type = CBRC_SERDES_TYPE_NONE;

    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link_info_ptr->serdes < LAST_CBRC_SERDES_TYPE, CBRC_ERR_INVALID_ARG, link_info_ptr->serdes, LAST_CBRC_SERDES_TYPE);
    PMC_ASSERT(link_info_ptr->mode < LAST_CBRC_PORT_MODE, CBRC_ERR_INVALID_ARG, link_info_ptr->mode, LAST_CBRC_PORT_MODE);


    /* Determine the serdes type */
    if (CBRC_SERDES_TYPE_NONE != link_ptr->serdes)
    {
        serdes_type = link_ptr->serdes;
    }
    else if (CBRC_SERDES_TYPE_NONE != link_info_ptr->serdes)
    {
        serdes_type = link_info_ptr->serdes;
    }
    else 
    {
        PMC_ASSERT(0, CBRC_ERR_CODE_ASSERT, 0, 0);
    }


    /* Set number of links based on serdes type */
    if (cbrc_serdes_type_is_40g(serdes_type))
    {
        max_links = 4;
    }
    else
    {
        max_links = 1;
    }

    count = 0;
    while ((NULL != link_ptr) && (count < max_links))
    {
        /* Set link info */
        *link_ptr = *link_info_ptr;
        
        link_ptr = cbrc_link_get_next(cbrc_handle_ptr, link_ptr);
        count++; 
    } /* end while */

    PMC_RETURN(result);
} /* cbrc_port_set */





/*******************************************************************************
* cbrc_port_is_avail
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks to see if requested link(s) based on serdes type are avaible for
*   provisioning
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link_ptr          - Pointer to first link to be part of the logical port 
*                       The serdes type will determine whether there are one
*                       or four links in the port.
*   serdes_type       - The ingress or egress serdes used by the logical port.
*                       The serdes type will determine whether the port is
*                       40G or 10G.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the desired link(s) are available
*   FALSE if the desired link(s) are not available
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 cbrc_port_is_avail(cbrc_handle_t      *cbrc_handle_ptr,
                                 cbrc_link_info_t   *link_ptr,
                                 cbrc_serdes_type_t  serdes_type)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              max_links;
    UINT32              count;

    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_type < LAST_CBRC_SERDES_TYPE, CBRC_ERR_INVALID_ARG, serdes_type, LAST_CBRC_SERDES_TYPE);


    /* Set number of links based on serdes type */
    if (cbrc_serdes_type_is_40g(serdes_type))
    {
        max_links = 4;
    }
    else
    {
        max_links = 1;
    }

  
    /* Check link(s) for availability */
    count = 0;
    while ((NULL != link_ptr) && (count < max_links) && (PMC_SUCCESS == result))
    {
        if (!link_ptr->available)
        {
            result = CBRC_ERR_LINK_NOT_AVAIL;
        }
        else 
        {
            link_ptr = cbrc_link_get_next(cbrc_handle_ptr, link_ptr);
            count++; 
        }
    } /* end while */

    PMC_RETURN(result);
} /* cbrc_port_is_avail */



/*******************************************************************************
* cbrc_port_is_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks to see if link is provisioned. This check is used before
*   deprovisioning a link.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link_ptr          - Pointer to first link to be part of the logical port 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the link is provisioned.
*   FALSE if the link is not provisioned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 cbrc_port_is_prov(cbrc_handle_t       *cbrc_handle_ptr,
                                cbrc_link_info_t    *link_ptr)
{
    BOOL8   result = TRUE;

    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    if (link_ptr->available) 
    {
        result = FALSE;
    }

    PMC_RETURN(result);
} /* cbrc_port_is_prov */


/*******************************************************************************
* cbrc_port_is_active
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks to see if link is active. This check is used before
*   deprovisioning a link.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link_ptr          - Pointer to first link to be part of the logical port 
*   dir               - rx or tx
*
* OUTPUTS:
*
* RETURNS:
*   TRUE if the link is active.
*   FALSE if the link is not active.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_port_is_active(cbrc_handle_t      *cbrc_handle_ptr,
                                      cbrc_link_info_t   *link_ptr,
                                      cbrc_port_dir_t     dir)
{
    BOOL8   result = TRUE;

    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(dir < LAST_CBRC_PORT_DIR, CBRC_ERR_INVALID_DIR, dir, LAST_CBRC_PORT_DIR);

    switch (dir)
    {
    case CBRC_PORT_DIR_TX:
        if (!link_ptr->tx_active) 
        {
            result = FALSE;
        }
        break;
        
    case CBRC_PORT_DIR_RX:
        if (!link_ptr->rx_active) 
        {
            result = FALSE;
        }
        break;
        
    default:
        PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */
    

    PMC_RETURN(result);
} /* cbrc_port_is_active */






/*******************************************************************************
* cbrc_link_is_head
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks to see if a link_ptr is the head of a logical group for 40G ports.
*   This check is used before deprovisioning a port.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link_ptr          - Pointer to first link to be part of the logical port 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if link is at the head of a logical port.
*   FALSE if link is not at the head of a logical port.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 cbrc_link_is_head(cbrc_handle_t    *cbrc_handle_ptr,
                                cbrc_link_info_t *link_ptr)
{
    BOOL8    result = TRUE;
    
    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    
    if (link_ptr != link_ptr->head) 
    {
        result = FALSE;
    }

    PMC_RETURN(result);
} /* cbrc_link_is_head */





/*******************************************************************************
*  cbrc_block_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CBRC ss to be in low power 
*   - power savings mode or out of low power mode and ready for individual 
*   configuration.
*   
*
* INPUTS:
*   *cbrc_handle_ptr    - pointer to CBRC handle instance to be operated on
*   block_energy_state  - Energy state of CBRC
*                           PMC_BLOCK_LOWPWR_ON_ALTERABLE: Low power standby
*                           PMC_BLOCK_LOWPWR_ON_DEFAULT: Low power mode and reset
*                                                        to default values
*                           PMC_BLOCK_LOWPWR_OFF_ALTERABLE: Out of low power and
*                                                          Operational
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_block_energy_state_set(cbrc_handle_t *cbrc_handle_ptr, 
                                              pmc_block_energy_state_enum block_energy_state)
{
    /* Variable declaration */
    UINT32 reset_val  = 0;
    UINT32 lowpwr_val = 0;
    UINT32 i;

    PMC_ENTRY();    
    
    /* GB need to set the LOWPWR and RST bit for the subsystem.*/
    switch (block_energy_state)
    {
    case PMC_BLOCK_LOWPWR_ON_ALTERABLE:
        reset_val = 0;
        lowpwr_val = 1;
        break;
    case PMC_BLOCK_LOWPWR_ON_DEFAULT:
        reset_val = 1;
        lowpwr_val = 1;
        break;
    case PMC_BLOCK_LOWPWR_OFF_ALTERABLE:
        reset_val = 0;
        lowpwr_val = 0;
        break;
    case PMC_BLOCK_LOWPWR_OFF_DEFAULT:
        reset_val = 1;
        lowpwr_val = 0;
        break;
    default:
        PMC_ASSERT(0, PMC_ERR_INVALID_PARAMETERS, 0, 0);
        break;
    }
    
    PMC_LOG(PMC_LOG_SEV_HIGHEST, CBRC_LOG_CODE_LOG, 0, 0, "reset_val = %u\n", reset_val);  
    PMC_LOG(PMC_LOG_SEV_HIGHEST, CBRC_LOG_CODE_LOG, 0, 0, "lowpwr_val = %u\n", lowpwr_val); 
    
    /* Top level SDH PMG Reset and Power */
    for (i = 0; i <= 1; i++) 
    {
        /*
          cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set(NULL, cbrc_handle_ptr, i, reset_val);
          cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set(NULL, cbrc_handle_ptr, i, reset_val);
          cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set(NULL, cbrc_handle_ptr, i, reset_val);
          cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle_ptr, i, reset_val);

          cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set(NULL, cbrc_handle_ptr, i, lowpwr_val);
          cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set(NULL, cbrc_handle_ptr, i, lowpwr_val);
          cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set(NULL, cbrc_handle_ptr, i, lowpwr_val);
        */
    }


    /* SDH PMG Slices Reset and Power */
    for (i = 0; i <= MAX_CBRC_CBRC_SDH_PMG_SDH_PMG_MTSB; i++)
    {
        /*
          sdh_pmg_local_access_init(cbrc_handle_ptr->sdh_pmg_rx_ptr[i]);
          sdh_pmg_local_access_init(cbrc_handle_ptr->sdh_pmg_tx_ptr[i]);
        */
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* cbrc_block_energy_state_set */     





/*******************************************************************************
* cbrc_link_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets a pointer to the link info structure for the desired link number.
*  The link info is currently stored in an array. This function is abstracts
*  the link info array so that the code can scale if the number of link numbers
*  drastically increases.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Desired link
*
* OUTPUTS:
*   *link_ptr         - Pointer to link info structure
*
* RETURNS:
*   PMC_SUCCESS       - Pointer to link info successfuly returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_link_get(cbrc_handle_t     *cbrc_handle_ptr,
                               UINT32             link,
                               cbrc_link_info_t **link_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    /* Return pointer to link_info structure based by indexing array */
    *link_ptr = &cbrc_handle_ptr->var.links[link];
    
    PMC_RETURN(result);
} /* cbrc_link_get */



/*******************************************************************************
* cbrc_link_get_next
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the following link info element. 
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   *link_ptr         - Pointer to current link info element.
*
* OUTPUTS:
*
* RETURNS:
*   *next_link_ptr    - Pointer to next link info element.
*
* NOTES:
*
*******************************************************************************/
PRIVATE cbrc_link_info_t* cbrc_link_get_next(cbrc_handle_t      *cbrc_handle_ptr,
                                             cbrc_link_info_t   *link_ptr)
{
    cbrc_link_info_t    *next_link_ptr;
    
    PMC_ENTRY();

    /* Check arguments - assert since args have been checked by public fns */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != link_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    /* Check if this is the tail of the list */
    if (link_ptr == &cbrc_handle_ptr->var.links[CBRC_MAX_LINK - 1])
    {
        next_link_ptr = NULL;
    }
    else
    {
        /* Return next link info pointer */
        next_link_ptr = ++link_ptr;
    }

    PMC_RETURN(next_link_ptr);
} /* cbrc_link_get_next */


/*******************************************************************************
* cbrc_client_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets client rate based on client mode.   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   mode              - Client mode
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   client_rate     - The client rate in Hz
*
* NOTES:
*
*******************************************************************************/
PUBLIC DOUBLE cbrc_client_rate_get(cbrc_handle_t    *cbrc_handle_ptr,
                                   cbrc_port_mode_t  mode)
{
    DOUBLE  client_rate = 0;
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);


    switch (mode)
    {
    case CBRC_PORT_MODE_40G_STS768:
    case CBRC_PORT_MODE_40G_STM256:
        client_rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
        break;
        
    case CBRC_PORT_MODE_10G_STS192:
    case CBRC_PORT_MODE_10G_STM64:
        client_rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
        break;
        
    case CBRC_PORT_MODE_10G_FC1200_ODU1F:             
    case CBRC_PORT_MODE_10G_FC1200_ODU2F:
	case CBRC_PORT_MODE_10G_64B_66B_IDLE:
        client_rate = UTIL_GLOBAL_FC_1200_RATE;
        break;
   
    case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:             
        client_rate = UTIL_GLOBAL_FC_1200_TRANS_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_FC800:
	case CBRC_PORT_MODE_10G_8B_10B_IDLE:
    case CBRC_PORT_MODE_10G_8B_10B_ARB:   
        client_rate = UTIL_GLOBAL_FC_800_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_CPRI_5G:             
        client_rate = UTIL_GLOBAL_CPRI_5_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_CPRI_6G:             
        client_rate = UTIL_GLOBAL_CPRI_6_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_CPRI_10G:             
        client_rate = UTIL_GLOBAL_CPRI_7_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_GDPS_5G:             
        client_rate = UTIL_GLOBAL_GDPS_5G_RATE;
        break;
    
    case CBRC_PORT_MODE_10G_GDPS_10G:             
        client_rate = UTIL_GLOBAL_GDPS_10G_RATE;
        break;

    case CBRC_PORT_MODE_10G_10GE:
        client_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        break;
    
    default:
        PMC_ASSERT(0, CBRC_ERR_INVALID_ARG, mode, LAST_CBRC_PORT_MODE);
        break;
    } /* end switch */

    
    PMC_RETURN(client_rate);
} /* cbrc_client_rate_get */

/* TEST FUNCTIONS */

/*******************************************************************************
* cbrc_test_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   "Hello World" CBRC test API for verification testing.
*
*
* INPUTS:
*   *cbrc_handle      - pointer to CBRC handle instance to be operated on
*   pmc_error_value     - 32 bit value to be returned by call to this function
*   write_value         - 32 bit value to be written to a hardcoded register
*                         address within CBRC.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_test_write(cbrc_handle_t *cbrc_handle, 
                                 UINT32 pmc_error_value,
                                 UINT32 write_value)
{
    PMC_ENTRY();

    /* write_value to the hardcoded register in cbrc */
    PMC_LOG_TRACE("In cbrc_test_write\n");
    pmc_sys_reg_write(cbrc_handle->base.sys_handle, 0x0, write_value);
    PMC_LOG_TRACE("Called pmc_sys_reg_write: writing 0x%x\n", write_value);
    
    PMC_RETURN(pmc_error_value);

} /* cbrc_test_write */

/*******************************************************************************
*  cbrc_test_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   "Hello World" CBRC test API for verification testing.
*
*
* INPUTS:
*   *cbrc_handle      - pointer to CBRC handle instance to be operated on
*   pmc_error_value     - 32 bit value to be returned by call to this function
*
* OUTPUTS:
*   read_value          - 32 bit value read from a hardcoded register
*                         address within CBRC.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_test_read(cbrc_handle_t *cbrc_handle, 
                                UINT32 pmc_error_value,
                                UINT32 *read_value)
{
    UINT32  reg_val;
    
    PMC_ENTRY();

    /* read_value from hardcoded register in cbrc */
    PMC_LOG_TRACE("In cbrc_test_read\n");
    *read_value = pmc_sys_reg_read ( cbrc_handle->base.sys_handle, 0x0);
    PMC_LOG_TRACE("Called pmc_sys_reg_read: Output 0x%x\n", *read_value);
    
    reg_val = mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get(NULL, cbrc_handle, 0);
    PMC_LOG_TRACE("Called mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get: 0x%x\n", reg_val);
   
    PMC_RETURN(pmc_error_value);

} /* cbrc_test_read */  

/*******************************************************************************
* cbrc_scbs3_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes SCBS3 for CBRC.
*
* INPUTS:
*   scbs_cfg - Pointer to SCBS3 configuration structure.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_scbs3_init(scbs3_cfg_t* scbs_cfg)
{   
    PMC_ENTRY();
    
    scbs_cfg->num_grp                    = 1;
    scbs_cfg->max_chnl                   = 96;
    scbs_cfg->max_rows                   = 24;
    scbs_cfg->total_rate                 = 120;
    scbs_cfg->total_rate_units           = UTIL_SCHD_1G_BIT_SEC;
    scbs_cfg->toc_sync                   = SCBS3_TOC_SYNC_CBRC;
    scbs_cfg->toc_sync_en                = TRUE;
    scbs_cfg->page_swap_mode             = SCBS3_PAGE_SWAP_BIT;
    scbs_cfg->bonus_mode_enabled         = FALSE;
    scbs_cfg->enable_row_resizing        = FALSE;
    scbs_cfg->cal_entries_alloc_method   = UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB;
    
    PMC_RETURN();
} /* cbrc_scbs3_init */

/*******************************************************************************
* cbrc_db_all_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the CBRC databases.
*
*
* INPUTS:
*   *cbrc_handle_ptr            - pointer to CBRC handle instance
*   *scbs_cfg                   - SCBS onfiguration structure
*    cbrc_handle_init_cfg_ptr   - CBRC configuration structure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_db_all_init(cbrc_handle_t *cbrc_handle_ptr,
                              scbs3_cfg_t *scbs_cfg,
                              cbrc_handle_init_cfg_t cbrc_handle_init_cfg_ptr)
{   
    UINT32 i;
    
    PMC_ENTRY();
    
    for (i = 0; i < CBRC_NUM_SCHD; i++)
    {
        PMC_ASSERT( cbrc_handle_init_cfg_ptr.schd_populate_mode[i] < LAST_CBRC_SCHD_POPULATE, CBRC_ERR_INVALID_ARG, 0 , 0);
        
        if(CBRC_SCHD_POPULATE_LOCAL == cbrc_handle_init_cfg_ptr.schd_populate_mode[i]) 
        {        
            cbrc_handle_ptr->cfg.schd_use_ctxt[i].alloc_mode = UTIL_SCHD_ALLOC_METHOD_CBRC;
            cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_id = LAST_UTIL_GEN_DB;
        }
        else if(CBRC_SCHD_POPULATE_DB == cbrc_handle_init_cfg_ptr.schd_populate_mode[i]) 
        {
            switch(i)
            {
            case 0: /* SCBS3 DB case for CBRC */
                cbrc_handle_ptr->cfg.schd_use_ctxt[0].alloc_mode = UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB;
                cbrc_handle_ptr->cfg.schd_use_ctxt[0].db_id = UTIL_GEN_DB_CBRC_SCBS3;
                break;
            case 1: /* MPMO DB case for CBRC */
                cbrc_handle_ptr->cfg.schd_use_ctxt[1].alloc_mode = UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB;
                cbrc_handle_ptr->cfg.schd_use_ctxt[1].db_id = UTIL_GEN_DB_CBRC_MPMO;
                break;
            default:
                cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_id = LAST_UTIL_GEN_DB;
                cbrc_handle_ptr->cfg.schd_use_ctxt[i].alloc_mode = LAST_UTIL_SCHD_ALLOC_METHOD;
            }
        } else {
            /* unspecified schedular mode case, code error */
            PMC_ASSERT(FALSE, CBRC_ERR_UNSUPPORTED_SCHD_MODE, 0, 0); /* failure if data base attach failed */
        }
    }
    
    
    for (i = 0; i < CBRC_NUM_SCHD; i++) /* so only scbs3 db is init here. MPMO db is taken care in mpmo_handle_init() */
    {
        /* set the cal_entries_alloc_method and db_id as a function of
           schd_populate_mode determined earlier */
        scbs_cfg->cal_entries_alloc_method = cbrc_handle_ptr->cfg.schd_use_ctxt[i].alloc_mode;
        scbs_cfg->db_id = cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_id;
        
        if(CBRC_SCHD_POPULATE_DB == cbrc_handle_init_cfg_ptr.schd_populate_mode[i])
        {
            cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_client_handle = gen_db_attach(&(cbrc_handle_ptr->base),cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_id);
            PMC_ASSERT(cbrc_handle_ptr->cfg.schd_use_ctxt[i].db_client_handle != 0, CBRC_ERR_DB_ATTACH_FAIL, 0, 0); /* failure if data base attach failed */ 
        }
    }
    PMC_RETURN();
} /* cbrc_db_all_init */




/*******************************************************************************
* cbrc_cbr_fsgm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the CBR_FSGM:
*    - Fault insertion enable/disable
*    - Fault propagation
*    - Fault pattern mode 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to activate (0..11)
*   fault_ins_en      - enable or disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_cbr_fsgm_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                   cbrc_port_dir_t           dir,
                                   UINT32                    link,
                                   UINT32                    fault_ins_en)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr;
    cbr_fsgm_handle_t   *cbr_fsgm_handle_ptr;
    UINT32              index;
    UINT32              slice;

    PMC_ENTRY();
    
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        PMC_RETURN(CBRC_ERR_INVALID_LINK); 
    }

    if (dir >= LAST_CBRC_PORT_DIR)
    {
        PMC_RETURN(CBRC_ERR_INVALID_DIR); 
    }

    result = cbrc_link_get(cbrc_handle_ptr, link, &link_ptr);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* Determine FSGM slice */
    slice = link / CBRC_CBR_FSGM_MAX_INDEX;
    index = link % CBRC_CBR_FSGM_MAX_INDEX;

    /* Get CBR_FSGM handle */
    if (CBRC_PORT_DIR_TX == dir) {
        cbr_fsgm_handle_ptr = cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice];
    } else {
        cbr_fsgm_handle_ptr = cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice];
    }
    
    if (PMC_SUCCESS == result) 
    {
        result = cbr_fsgm_cfg(cbr_fsgm_handle_ptr, index, fault_ins_en );
        PMC_LOG_TRACE("handle...value %lu\n", (unsigned long)cbr_fsgm_handle_ptr);
        PMC_LOG_TRACE("[%s:%d] cbr_fsgm_cfg done ", __FUNCTION__, __LINE__);
    }

    PMC_RETURN(result);
} /* cbrc_cbr_fsgm_cfg */


/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the SDH PRBS generator or monitor on a given link.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to activate (0..11)
*   *cfg_ptr          - PRBS configuration structure to configure payload
*                       mode and inversion.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC  PMC_ERROR cbrc_sdh_prbs_set(cbrc_handle_t         *handle,
                                    cbrc_port_dir_t        dir,
                                    UINT8                  link,
                                    sdh_pmg_prbs_cfg_t    *cfg_ptr)
{
    PMC_ERROR result;
    BOOL8     prbs_enable;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cfg_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
     
    prbs_enable = cfg_ptr->enable_prbs;

    switch(dir)
    {
    case CBRC_PORT_DIR_RX:
        result = cbrc_sdh_prbs_mon_cfg(handle, link, cfg_ptr, prbs_enable);
        break;
    case CBRC_PORT_DIR_TX:
        result = cbrc_sdh_prbs_gen_cfg(handle, link, cfg_ptr, prbs_enable);
        break;
    default:
        result = CBRC_ERR_INVALID_DIR;
        break;
    }
     
    PMC_RETURN(result);
}/* cbrc_sdh_prbs_cfg */

/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves SDH PMG PRBS configuration on a given channel.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   dir               - Direction of traffic
*   link              - Post-mld link to activate (0..11)
*   force_reg_rd      - When TRUE configuration is retrieved from SDH PMG register space
*                       When FALSE SWDRV context is used to retrieved configuration
*
* OUTPUTS:
*    *cfg_ptr         - PRBS configuration structure of configured payload
*                       mode, inversion, and enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS       - when function execution is successful otherwise a 
*                       descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC  PMC_ERROR cbrc_sdh_prbs_get(cbrc_handle_t         *handle,
                                    cbrc_port_dir_t        dir,
                                    UINT8                  link,
                                    BOOL8                  force_reg_rd,       
                                    sdh_pmg_prbs_cfg_t    *cfg_ptr)
{
    config_and_status_MON_STS_1_PATH_CFG_buffer_t mon_sts_buf[1];
    config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t gen_sts_buf[1];
    prgm_buffer_t prgm_buf[1];
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cfg_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (CBRC_PORT_DIR_RX == dir)
    {
        result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    }
    else 
    {
        result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    }

    if (PMC_SUCCESS == result)
    {
        switch(dir)
        {
        case CBRC_PORT_DIR_RX:
            /* copy RX context when PRBS is activate or read registers value */
            if (FALSE == force_reg_rd && FALSE != handle->var.prbs_ctxt[link].rx_cfg_active)
            {
                PMC_MEMCPY(cfg_ptr, &handle->var.prbs_ctxt[link].rx_cfg, sizeof(sdh_pmg_prbs_cfg_t));
            }
            else
            {

                cfg_ptr->enable_prbs = FALSE;
                cfg_ptr->inv_cfg  =FALSE;
                cfg_ptr->prbs_type = LAST_SDH_PMG_PRBS_TYPE;
                
                /* Check PRBS reset state */
                if (0 == prbs_mg_field_PRBS_RX_LOW_PWR_get(NULL, handle))
                {
                    /* initialize buffers */
                    config_and_status_MON_STS_1_PATH_CFG_buffer_init(mon_sts_buf);
                    prgm_buffer_init(prgm_buf, handle, link);
                    
                    result = config_and_status_MON_STS_1_PATH_CFG_buffer_retrieve(handle, (UINT32)link, 1, mon_sts_buf);
                    if (PMC_SUCCESS == result)
                    {
                        cfg_ptr->enable_prbs = config_and_status_ifield_MON_ENA_get(mon_sts_buf);
                        cfg_ptr->inv_cfg  = config_and_status_ifield_MON_INV_PRBS_get(mon_sts_buf);
                        cfg_ptr->prbs_type = SDH_PMG_PRBS_TYPE_RX_MONITOR;
                    }
                }
            }
            break;
        case CBRC_PORT_DIR_TX:
            /* copy TX context when PRBS is activate or read registers value */
            if (FALSE == force_reg_rd && FALSE != handle->var.prbs_ctxt[link].tx_cfg_active)
            {
                PMC_MEMCPY(cfg_ptr, &handle->var.prbs_ctxt[link].tx_cfg, sizeof(sdh_pmg_prbs_cfg_t));
            }
            else
            {
                cfg_ptr->enable_prbs = FALSE;
                cfg_ptr->inv_cfg  =FALSE;
                cfg_ptr->prbs_type = LAST_SDH_PMG_PRBS_TYPE;
                
                /* Check PRBS reset state */
                if (0 == prbs_mg_field_PRBS_TX_LOW_PWR_get(NULL, handle))
                {                    
                    /* initialize buffers */
                    config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init(gen_sts_buf);
                    prgm_buffer_init(prgm_buf, handle, link);
                    result = config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_retrieve(handle, (UINT32)link, 1, gen_sts_buf);
                    if (PMC_SUCCESS == result)
                    {
                        cfg_ptr->enable_prbs = config_and_status_ifield_LINKENA0_get(gen_sts_buf);
                        cfg_ptr->inv_cfg  = config_and_status_ifield_GEN_INV_PRBS_get(gen_sts_buf);
                        cfg_ptr->prbs_type = SDH_PMG_PRBS_TYPE_TX_GENERATOR;
                    }
                }
            }
            break;
        default:
            result = CBRC_ERR_INVALID_DIR;
            break;
        }
    }
     
    PMC_RETURN(result);
}/* cbrc_sdh_prbs_cfg */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_cbrc_prgm_prbs_oh_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Inserts specified overhead byte into the PRBS output.
*
* INPUTS:
*   *handle           - pointer to CBRC handle instance
*   link              - Post-mld link to activate (0..11)
*   ins_type          - overhead byte to insert
*   *ins_value_ptr    - 8-bit value to insert into overhead
*   ins_enable        - Enable or disable the byte insertion. If FALSE, then
*                       ins_value can be NULL.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS        - when API execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_oh_ins_set(cbrc_handle_t           *handle,
                                          UINT8                    link,
                                          sdh_pmg_prbs_oh_ins_t    ins_type,
                                          char                    *ins_value_ptr,
                                          BOOL8                     ins_enable)
{
    PMC_ERROR           result = PMC_SUCCESS;
    trmp_buffer_t trmp_buf[1];
    UINT32 reg_val;

    PMC_ENTRY();
 
    /* Check arguments */
    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ins_enable == FALSE || NULL != ins_value_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);
    
    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    

    trmp_buffer_init(trmp_buf, handle, link);

    reg_val = (UINT32)ins_value_ptr[0] & 0xff;
    /* select OH to be updated */
    switch(ins_type)
    {
    case SDH_PMG_PRBS_OH_INS_NATIONAL:
        trmp_field_NATIONALEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_NATIONALV_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_E2:
        trmp_field_E2REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_E2V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_Z2:
        trmp_field_Z2REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_Z2V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_Z1:
        trmp_field_Z1REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_Z1V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_S1:
        trmp_field_S1REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_S1V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_D4D12:
        trmp_field_D4D12REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_D4D12V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_K1K2:
        trmp_field_K1K2REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_K1V_set(trmp_buf, handle, link, ins_value_ptr[0]);
            trmp_field_K2V_set(trmp_buf, handle, link, ins_value_ptr[1]);
        }
        break;
    case SDH_PMG_PRBS_OH_INS_D1D3:
        trmp_field_D1D3REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_D1D3V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_F1:
        trmp_field_F1REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_F1V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_E1:
        trmp_field_E1REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_E1V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_Z0:
        trmp_field_Z0REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_Z0V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    case SDH_PMG_PRBS_OH_INS_J0:
        trmp_field_J0REGEN_set(trmp_buf, handle, link, ins_enable);
        if (TRUE == ins_enable)
        {
            trmp_field_J0V_set(trmp_buf, handle, link, reg_val);            
        }
        break;
    default:
        result = CBRC_ERR_INVALID_PRBS_OH;
        break;
    }
    trmp_buffer_flush(trmp_buf);

    PMC_RETURN(result);
}/*cbrc_sdh_prbs_oh_ins_set*/


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_oh_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Gets overhead byte from the PRBS output.
*
* INPUTS:
*   *handle           - pointer to CBRC handle instance
*   link              - Post-mld link to activate (0..11)
*   ins_type          - overhead byte to insert
*   *ins_value_ptr    - 8-bit value to insert into overhead
*   ins_enable_ptr    - Enable or disable the byte insertion. If FALSE, then
*                       ins_value_ptr can be NULL.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS        - when API execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_oh_ins_get(cbrc_handle_t           *handle,
                                          UINT8                    link,
                                          sdh_pmg_prbs_oh_ins_t    ins_type,
                                          char                    *ins_value_ptr,
                                          BOOL                    *ins_enable_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    trmp_buffer_t trmp_buf[1];
 
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ins_enable_ptr && NULL != ins_value_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);
    
    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    

    trmp_buffer_init(trmp_buf, handle, link);

    /* select OH to be retrived */
    switch(ins_type)
    {
    case SDH_PMG_PRBS_OH_INS_NATIONAL:
        *ins_enable_ptr = trmp_field_NATIONALEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_NATIONALV_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_E2:
        *ins_enable_ptr = trmp_field_E2REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_E2V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_Z2:
        *ins_enable_ptr = trmp_field_Z2REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_Z2V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_Z1:
        *ins_enable_ptr = trmp_field_Z1REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_Z1V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_S1:
        *ins_enable_ptr = trmp_field_S1REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_S1V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_D4D12:
        *ins_enable_ptr = trmp_field_D4D12REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_D4D12V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_K1K2:
        ins_value_ptr[0] = trmp_field_K1V_get(trmp_buf, handle, link);
        ins_value_ptr[1] = trmp_field_K2V_get(trmp_buf, handle, link);
        break;
    case SDH_PMG_PRBS_OH_INS_D1D3:
        *ins_enable_ptr = trmp_field_D1D3REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_D1D3V_get(trmp_buf, handle, link);    
        break;
    case SDH_PMG_PRBS_OH_INS_F1:
        *ins_enable_ptr = trmp_field_F1REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0]  = (char) trmp_field_F1V_get(trmp_buf, handle, link); 
        break;
    case SDH_PMG_PRBS_OH_INS_E1:
        *ins_enable_ptr = trmp_field_E1REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_E1V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_Z0:
        *ins_enable_ptr = trmp_field_Z0REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_Z0V_get(trmp_buf, handle, link);            
        break;
    case SDH_PMG_PRBS_OH_INS_J0:
        *ins_enable_ptr = trmp_field_J0REGEN_get(trmp_buf, handle, link);
        ins_value_ptr[0] = (char) trmp_field_J0V_get(trmp_buf, handle, link);            
        break;
    default:
        result = CBRC_ERR_INVALID_PRBS_OH;
        break;
    }
    trmp_buffer_flush(trmp_buf);

    PMC_RETURN(result);
}/*digi_cbr_sdh_prbs_oh_ins_get*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves the prbs monitor synchroniation state for a given link and a 
*    a given STS-1 path.
*
* INPUTS:
*   handle             - Pointer to subsystem handle
*   link               - Post-mld link to activate (0..11)
*
* OUTPUTS:
*    *status           - PRBS monitor status for given STS-1 path.
*    *err_cnt          - Errors counter value
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_status_get(cbrc_handle_t           *handle,
                                          UINT8                    link,
                                          util_patt_status_t      *status,
                                          UINT32                  *err_cnt)
{
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 reg_value;
    UINT32 i;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != status, CBRC_ERR_NULL_PARAMETERS, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);
    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }


    *status = LAST_UTIL_PATT_STATUS;
    if (PMC_SUCCESS == result)
    {        
        if (FALSE != handle->var.prbs_ctxt[link].rx_cfg_active &&
            TRUE == handle->var.prbs_ctxt[link].rx_cfg.enable_prbs)
        {      
            if (LAST_UTIL_PATT_STATUS == *status)
            {
                for (i = 0; i < 16; i++)
                {
                    /* update status */
                    reg_value = prgm_field_MON1_SYNC_V_get(NULL, handle, i);

                    if (1 == reg_value)
                    {
                        *status = UTIL_PATT_STATUS_LOCKED;
                    }
                    else
                    {
                        *status = UTIL_PATT_STATUS_TRYING_TO_LOCK;
                        break;
                    }
                }
                /* retrieve MON PRBS errors counter */
                result = cbrc_sdh_prbs_err_cnt_get(handle, link, err_cnt);
            }            
        }
        else
        {
            *status = UTIL_PATT_STATUS_DISABLED;
        }
    }

    PMC_RETURN(result);
}/* cbrc_sdh_prbs_mon_status_get */

/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_resync()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Resynchronizes the prbs monitor.
*
* INPUTS:
*   handle             - Pointer to subsystem handle
*   link               - Post-mld link to activate (0..11)
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_resync(cbrc_handle_t         *handle,
                                      UINT8                 link)
{
    config_and_status_MON_STS_1_PATH_CFG_buffer_t mon_sts_buf[1];
    cbrc_link_info_t   *link_ptr;
    UINT32 j;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);

    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* initialize buffers */
    config_and_status_MON_STS_1_PATH_CFG_buffer_init(mon_sts_buf);
    for (i = 1; i <= (PRGM_SLICE_MAX_NUM + 1); i++)
    {
        for (j = 1; j <= CBRC_NUM_STS_1_PATHS_MAX; j++)
        {
            /* emulate rising edge in RESYNC register-bit */
            config_and_status_ifield_RESYNC_set(mon_sts_buf, (UINT32)0);
            config_and_status_MON_STS_1_PATH_CFG_buffer_apply(handle, (UINT32)i, j, mon_sts_buf);
            
            config_and_status_ifield_RESYNC_set(mon_sts_buf, (UINT32)1);
            config_and_status_MON_STS_1_PATH_CFG_buffer_apply(handle, (UINT32)i, j, mon_sts_buf);
            
            config_and_status_ifield_RESYNC_set(mon_sts_buf, (UINT32)0);
            config_and_status_MON_STS_1_PATH_CFG_buffer_apply(handle, (UINT32)i, j, mon_sts_buf);
        } 
    }   

    PMC_RETURN(result);
}/* cbrc_sdh_prbs_resync */
 

/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_err_ins()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Inserts an error into the PRBS stream. The MSB of the next byte will be
*    inverted, inducing a single bit error.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_error_ins(cbrc_handle_t         *handle,
                                         UINT8                  link)
{
    config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t gen_sts_buf;
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);
    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    if (PMC_SUCCESS == result)
    {
        config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init(&gen_sts_buf);
        config_and_status_ifield_FORCE_ERR_set(&gen_sts_buf, 1);
        result = config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_apply(handle, 0, 1, &gen_sts_buf);
    }

    PMC_RETURN(result);
} /* cbrc_cbr_prbs_error_ins */


/*******************************************************************************
* FUNCTION: cbrc_prbs_rx_bringup_down
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enables or disables PRBS RX block. Set up or down reset,
*    low power and rx channel enable.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   *cfg_ptr          - PRBS configuration structure to configure payload
*                       mode and inversion
*   chnl_id           - channel identifier
*   oc768_mode        - channel rate 10G or 40G
*   enable            - TRUE: PRBS RX block is power up 
*                       FALSE: PRBS RX is power down
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_rx_bringup_down(cbrc_handle_t         *handle,
                                            sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                            UINT32                 chnl_id,
                                            BOOL8                  oc768_mode,
                                            BOOL8                  enable)
{
    config_and_status_MON_STS_1_PATH_CFG_buffer_t mon_sts_buf[1];
    config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t mon_prbs_msb_buf[1];
    prgm_buffer_t prgm_buf[1];
    BOOL8 mon_sts12csl;
    BOOL8 mon_sts12c;
    UINT8 mon_msslen;
    UINT8 reg_val;
    UINT32 i; 
    UINT32 j; 
    UINT32 tmp;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(FALSE == enable || NULL != cfg_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);   

    reg_val = TRUE == enable ?  CBRC_ENABLE : CBRC_DISABLE;

    if( TRUE == enable)
    {
        /* update RX PRBS reset, low power, and rx channel enable */
        prbs_mg_field_PRBS_RX_SW_RST_set (NULL, handle, (UINT32)  ~reg_val & 0x0);

        /* update RX PRBS reset, low power, and rx channel enable */
        prbs_mg_field_PRBS_RX_LOW_PWR_set (NULL, handle, (UINT32) ~reg_val & 0x1);
        prbs_mg_field_PRBS_RX_SW_RST_set (NULL, handle, (UINT32)  ~reg_val & 0x1);
         
        cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set(NULL, handle, 0, chnl_id);
        if (TRUE == oc768_mode)
        {
            prbs_mg_field_OC_768_MODE_set (NULL, handle, 0x1);
        }
        else
        {
            prbs_mg_field_OC_768_MODE_set (NULL, handle, 0x0);
        }
    }

    prbs_mg_field_PRBS_MG_RX_CHNL_EN_set(NULL, handle, reg_val);
    prbs_mg_field_PRBS_MG_RX_CHNL_EN_set(NULL, handle, reg_val);
    
    /* disable the firmware queue since PRGM block could not answer to BUSY poll */
    l5sys_firmware_queue_disable(handle);

    if( TRUE == enable)
    {
        config_and_status_MON_STS_1_PATH_CFG_buffer_init(mon_sts_buf);
        
        config_and_status_ifield_MON_INV_PRBS_set(mon_sts_buf,  (UINT32)cfg_ptr->inv_cfg);    
        config_and_status_ifield_MON_ENA_set(mon_sts_buf, TRUE);
        
        for (i = 0; i < (PRGM_SLICE_MAX_NUM + 1); i++)
        {
            mon_sts12c = 0x1;
            mon_msslen = 0x4;
            if ( 0 == i)
            {
                mon_sts12csl = 0x0;
            }
            else
            {
                mon_sts12csl = 0x1;
            }
            
            /* initialize buffers */
            prgm_buffer_init(prgm_buf, handle, i);
            prgm_field_MON_STS12CSL_set(prgm_buf, handle, i, mon_sts12csl);
            prgm_field_MON_STS12C_set(prgm_buf, handle, i, mon_sts12c);
            prgm_field_MON_MSSLEN_set(prgm_buf, handle, i, mon_msslen);
            prgm_buffer_flush(prgm_buf);
            
            for (j = 1; j <= CBRC_NUM_STS_1_PATHS_MAX && PMC_SUCCESS == result; j++)
            {
                if (PMC_SUCCESS == result)
                {
                    result = config_and_status_MON_STS_1_PATH_CFG_buffer_apply(handle, (UINT32)i, j, mon_sts_buf);
                }
            
                if (PMC_SUCCESS == result)
                {
                    config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_init(mon_prbs_msb_buf);   
                    config_and_status_ifield_MON_PRBS_set(mon_prbs_msb_buf, 0x1);
                    result = config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_apply(handle, (UINT32) i, j, mon_prbs_msb_buf);
                
                    if (PMC_SUCCESS == result)
                    {
                        config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_init(mon_prbs_msb_buf);        
                        result = config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_retrieve(handle, (UINT32) i, j, mon_prbs_msb_buf);
                    }
                }                     
            } 
        }
    }

    prbs_mg_field_ASCLK_PULSE_set(NULL, handle, (UINT32) TRUE);   
    prbs_mg_field_ASCLK_PULSE_set(NULL, handle, (UINT32) FALSE);   

    if (TRUE == enable)
    {
        for (i = 0; i < (PRGM_SLICE_MAX_NUM + 1) &&
                 PMC_SUCCESS == result; i++)
        {
            tmp = prgm_reg_MON_SYNC_INT_STAT_read(NULL, handle, i);

            prgm_field_TIP_set(NULL, handle, i, 0);
            
            /* write in TIP and pool until errors counter register is updated */
            /* or number of polls exceeded */
            if (0 != prgm_field_TIP_poll(NULL, handle, i, 0, PMC_POLL_EQUAL,
                                         CBRC_PRGM_MG_TIP_POLL_MAX_CNT,
                                         NULL, CBRC_PRGM_MG_TIP_POLL_DELAY))
            {
                result = CBRC_ERR_PRBS_ERR_CNT_READ_FAIL;
            }
        }
    }

    /* re-enable the firmware equeue */
    l5sys_firmware_queue_disable(handle);

    if( FALSE == enable)
    {
        /* update RX PRBS reset, low power, and rx channel enable */
        prbs_mg_field_PRBS_RX_LOW_PWR_set (NULL, handle, (UINT32) ~reg_val & 0x1);
        prbs_mg_field_PRBS_RX_SW_RST_set (NULL, handle, (UINT32)  ~reg_val & 0x1);
    }

    if (PMC_SUCCESS == result)
    {
        handle->var.prbs_ctxt[chnl_id].rx_cfg.enable_prbs = enable;
    }
    
    PMC_RETURN(result);
}/*cbrc_prbs_rx_bringup_down*/

/*******************************************************************************
* FUNCTION: cbrc_prbs_tx_bringup_down
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enables or disables PRBS TX block. Set up or down reset,
*    low power and tx channel enable.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   *cfg_ptr          - PRBS configuration structure to configure payload
*                       mode and inversion
*   chnl_id           - channel identifier
*   oc768_mode        - channel rate 10G or 40G
*   enable            - TRUE: PRBS TX block is power up 
*                       FALSE: PRBS TX is power down
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when function is successful otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_tx_bringup_down(cbrc_handle_t         *handle,
                                            sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                            UINT32                 chnl_id,
                                            BOOL8                  oc768_mode,
                                            BOOL8                  enable)
{
    UINT32 reg_val;
    prgm_buffer_t prgm_buf[1];
    config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t gen_sts_buf;
    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t gen_prbs_msb_buf[1];
    /* config_and_status_GENERATOR_B1_E1_VAL_buffer_t gen_b1_buf; */
    BOOL8 gen_sts12csl;
    BOOL8 gen_sts12c;
    UINT8 gen_msslen;
    UINT32 j;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
    BOOL pn11_en;
    sdh_pmg_handle_t   *sdh_handle_ptr=NULL;
    UINT32 pn11_index;

    PMC_ENTRY();

    reg_val = TRUE == enable ?  CBRC_ENABLE : CBRC_DISABLE;

    if (TRUE == enable)
    {
        /* update TX PRBS reset, low power, and rx channel enable */
        prbs_mg_field_PRBS_TX_LOW_PWR_set (NULL, handle, ~reg_val & 0x1);
        prbs_mg_field_PRBS_TX_SW_RST_set (NULL, handle,  ~reg_val & 0x1);
    }
       
    prbs_mg_field_PRBS_MG_TX_CHNL_EN_set(NULL, handle, CBRC_DISABLE);
    if (TRUE == enable)
    {
        if (TRUE == oc768_mode)
        {
            prbs_mg_field_OC_768_MODE_set (NULL, handle, 0x1);
        }
        else
        {
            prbs_mg_field_OC_768_MODE_set (NULL, handle, 0x0);
        }
    }

    if (TRUE == enable)
    {
        gen_sts12c = 0x1;
        gen_msslen = 0x4;
            
        for (j = 0; j < (PRGM_SLICE_MAX_NUM + 1); j++)
        {
            if (0 == j)
            {
                gen_sts12csl = 0x0;
            }
            else
            {
                gen_sts12csl = 0x1;
            }
            /* initialize buffers */
            prgm_buffer_init(prgm_buf, handle, j);
                
            prgm_field_GEN_STS12CSL_set(prgm_buf, handle, j, gen_sts12csl);
            prgm_field_GEN_STS12C_set(prgm_buf, handle, j, gen_sts12c);
            prgm_field_GEN_MSSLEN_set(prgm_buf, handle, j, gen_msslen);
            prgm_buffer_flush(prgm_buf);
        }
            
            
        config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init(&gen_sts_buf);
        /* config_and_status_GENERATOR_B1_E1_VAL_buffer_init(&gen_b1_buf); */
        
        config_and_status_ifield_GEN_INV_PRBS_set(&gen_sts_buf, (UINT32)cfg_ptr->inv_cfg);
        config_and_status_ifield_GEN_AMODE_set(&gen_sts_buf, 0x1);
        config_and_status_ifield_LINKENA0_set(&gen_sts_buf, TRUE);
        config_and_status_ifield_S_set(&gen_sts_buf, 0x2);

        for (i = 0; i < (PRGM_SLICE_MAX_NUM + 1); i++)
        {   
            for (j = 1; j <= CBRC_NUM_STS_1_PATHS_MAX && PMC_SUCCESS == result; j++)
            {
                if (PMC_SUCCESS == result)
                {
                    result = config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_apply(handle, i, j, &gen_sts_buf);
                }
                if (PMC_SUCCESS == result)
                {
                    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_init(gen_prbs_msb_buf);        
                    
                    config_and_status_ifield_GEN_PRBS_set(gen_prbs_msb_buf, 0x1);
                    result = config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_apply(handle, (UINT32) i, j, gen_prbs_msb_buf);                        
                }
            }        
            
        }   
    }
    else
    {
        config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init(&gen_sts_buf);
        config_and_status_ifield_LINKENA0_set(&gen_sts_buf, 0);
        for (i = 0; i < (PRGM_SLICE_MAX_NUM + 1); i++)
        {
            for (j = 1; j <= CBRC_NUM_STS_1_PATHS_MAX && PMC_SUCCESS == result; j++)
            {
                result = config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_apply(handle, i, j, &gen_sts_buf);   
            }
        }
    }

    prbs_mg_field_ASCLK_PULSE_set(NULL, handle, (UINT32) TRUE); 
    prbs_mg_field_PRBS_MG_TX_CHNL_EN_set(NULL, handle, reg_val);    

    if (FALSE == enable)
    {
        /* update TX PRBS reset, low power, and rx channel enable */
        prbs_mg_field_PRBS_TX_LOW_PWR_set (NULL, handle, ~reg_val & 0x1);
        prbs_mg_field_PRBS_TX_SW_RST_set (NULL, handle,  ~reg_val & 0x1);   
    }

    if (PMC_SUCCESS == result)
    {
        handle->var.prbs_ctxt[chnl_id].tx_cfg.enable_prbs = enable;
    }

    /*if PN11 enabled, disabled it if PRNS generator is activated */ 
    /* or restore PN11 insertion after PRBS generator deactivation */
    if (PMC_SUCCESS == result)
    { 
        /* Check if datapath is provisioned for this feature */
        result = cbrc_feature_validate(handle, chnl_id, CBRC_FEATURE_MODE_SDH);    
    
        if (PMC_SUCCESS == result) 
        {
            result = cbrc_sdh_pmg_get_handle(handle, CBRC_PORT_DIR_TX, chnl_id, &sdh_handle_ptr, &pn11_index);
        }
         
        handle->var.prbs_ctxt[chnl_id].tx_pn11_lof = LAST_SDH_PMG_PN11_INSERT;
        if (PMC_SUCCESS == result) 
        {            
            if (TRUE == enable)
            {      
                result = sdh_pmg_pn11_ins_get(sdh_handle_ptr, pn11_index, SDH_PMG_PN11_INSERT_LOF, &pn11_en);
                
                if (PMC_SUCCESS == result && TRUE == pn11_en)
                {                
                    result = sdh_pmg_pn11_ins(sdh_handle_ptr, pn11_index, SDH_PMG_PN11_INSERT_LOF, FALSE);
                    
                    
                    if (PMC_SUCCESS == result)
                    {
                        handle->var.prbs_ctxt[chnl_id].tx_pn11_lof = SDH_PMG_PN11_INSERT_LOF;
                    }
                }
            }
            else
            {
                if (SDH_PMG_PN11_INSERT_LOF == handle->var.prbs_ctxt[chnl_id].tx_pn11_lof)
                {  
                    result = sdh_pmg_pn11_ins(sdh_handle_ptr, pn11_index, SDH_PMG_PN11_INSERT_LOF, TRUE);  
                    if (PMC_SUCCESS == result)
                    { 
                        handle->var.prbs_ctxt[chnl_id].tx_pn11_lof = LAST_SDH_PMG_PN11_INSERT;                 
                    }
                }
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }

    PMC_RETURN(result);
}/*cbrc_prbs_tx_bringup_down*/       
        
/*******************************************************************************
* FUNCTION: cbrc_prbs_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Disable RX and TX PRBS of a given link
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   Not used for now, but it could be useful in the future
*******************************************************************************/
PRIVATE void cbrc_prbs_deprov(cbrc_handle_t     *handle,
                              UINT8             link)
{
    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* disable MON and GEN PRBS if there are enabled */            
    if (TRUE == handle->var.prbs_ctxt[link].rx_cfg_active)
    {
        result = cbrc_sdh_prbs_mon_cfg(handle, link, NULL, FALSE);
    }
    if (TRUE == handle->var.prbs_ctxt[link].tx_cfg_active)
    {
        result = cbrc_sdh_prbs_gen_cfg(handle, link, NULL, FALSE);
    }

    PMC_RETURN();
}/*cbrc_prbs_deprov*/

/*******************************************************************************
* FUNCTION: cbrc_prbs_rx_ctxt_push
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Save RX PRBS context.
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   link              - Post-mld link to activate (0..11)
*   cfg_ptr           - pointer to sdh PRBS configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_rx_ctxt_push(cbrc_handle_t      *handle,
                                         UINT8               link,
                                         sdh_pmg_prbs_cfg_t *cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }


    if (FALSE == handle->var.prbs_ctxt[link].rx_cfg_active)
    {
        handle->var.prbs_ctxt[link].rx_cfg_active = TRUE;
    }


    /* save prbs state to on (TRUE) */
    PMC_MEMCPY(&handle->var.prbs_ctxt[link].rx_cfg, cfg_ptr, sizeof(sdh_pmg_prbs_cfg_t));
    
    PMC_RETURN(result);
}/*cbrc_prbs_rx_ctxt_push*/


/*******************************************************************************
* FUNCTION: cbrc_prbs_rx_ctxt_pop
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Pop /free RX PRBS context.
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*   *are_all_prbs_off - TRUE when all RX PRBS are OFF, FALSE otherwise
*
* RETURNS:
*   PMC_SUCCESS       - when function execution is successful otherwise a 
*                       descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_rx_ctxt_pop(cbrc_handle_t     *handle,
                                        UINT8              link,
                                        BOOL8             *are_all_prbs_off)
{
    UINT32 itr;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != are_all_prbs_off, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    if (FALSE != handle->var.prbs_ctxt[link].rx_cfg_active)
    {       

        handle->var.prbs_ctxt[link].rx_cfg_active = FALSE; 
    }
        

    /* find if all prbs are off */
    *are_all_prbs_off = TRUE;
    for (itr = 0; itr < CBRC_MAX_LINK && *are_all_prbs_off == TRUE; itr++)
    {
        if (FALSE != handle->var.prbs_ctxt[itr].rx_cfg_active)
        {
            if (TRUE == handle->var.prbs_ctxt[itr].rx_cfg.enable_prbs)
            {
                *are_all_prbs_off = FALSE;
            }
        }
    }    

    PMC_RETURN(result);
}/*cbrc_prbs_rx_ctxt_pop*/

/*******************************************************************************
* FUNCTION: cbrc_prbs_tx_ctxt_push
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Save TX PRBS context.
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   link              - Post-mld link to activate (0..11)
*   cfg_ptr           - pointer to sdh PRBS configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_tx_ctxt_push(cbrc_handle_t      *handle,
                                         UINT8               link,
                                         sdh_pmg_prbs_cfg_t *cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    

    if (FALSE == handle->var.prbs_ctxt[link].tx_cfg_active)
    {
        handle->var.prbs_ctxt[link].tx_cfg_active = TRUE;        
    }

    /* save prbs state to on (TRUE) */
    PMC_MEMCPY(&handle->var.prbs_ctxt[link].tx_cfg, cfg_ptr, sizeof(sdh_pmg_prbs_cfg_t));
    
    PMC_RETURN(result);
}/*cbrc_prbs_tx_ctxt_push*/


/*******************************************************************************
* FUNCTION: cbrc_prbs_tx_ctxt_pop
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Pop /free TX PRBS context.
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*   *are_all_prbs_off - TRUE when all TX PRBS are OFF, FALSE otherwise
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_tx_ctxt_pop(cbrc_handle_t     *handle,
                                        UINT8              link,
                                        BOOL8             *are_all_prbs_off)
{
    UINT32 itr;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != are_all_prbs_off, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    if (FALSE != handle->var.prbs_ctxt[link].tx_cfg_active)
    {        
        handle->var.prbs_ctxt[link].tx_cfg_active = FALSE;
    }
        

    /* find if all prbs are off */
    *are_all_prbs_off = TRUE;
    for (itr = 0; itr < CBRC_MAX_LINK && *are_all_prbs_off == TRUE; itr++)
    {
        if (FALSE != handle->var.prbs_ctxt[itr].tx_cfg_active)
        {
            if (TRUE == handle->var.prbs_ctxt[itr].tx_cfg.enable_prbs)
            {
                *are_all_prbs_off = FALSE;
            }
        }
    }    

    PMC_RETURN(result);
}/* cbrc_prbs_tx_ctxt_pop*/

/*******************************************************************************
* FUNCTION: cbrc_prbs_prbs_oc_mode_conflict_detect
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validate PRBS generator or monitor activation.
*
* INPUTS:
*   handle            - Pointer to subsystem handle.
*   is_tx_dir         - TRUE to validate PRBS generator 
*                       FALSE to validate PRBS monitor   
*   oc_768_mode       - TRUE when PRBS is validated for a 40G link
*                       FALSE when PRBS is validated for a 10G link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when PRBS can be activated, otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_prbs_enable_validate(cbrc_handle_t     *handle,
                                            BOOL8              is_tx_dir,
                                            BOOL8              oc_768_mode)
{
    UINT32 itr;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* lookup PRBS context and check TX prbs state */
    if (FALSE == is_tx_dir)
    {
        for (itr = 0; itr < CBRC_MAX_LINK && PMC_SUCCESS == rc; itr++)
        {
            if (FALSE != handle->var.prbs_ctxt[itr].rx_cfg_active)
            {
                if (FALSE != handle->var.prbs_ctxt[itr].rx_cfg.enable_prbs)
                {
                    rc = CBRC_ERR_MON_PRBS_ALREADY_SELECTED;
                }
                else if (FALSE != handle->var.prbs_ctxt[itr].tx_cfg_active &&
                         TRUE == handle->var.prbs_ctxt[itr].tx_cfg.enable_prbs &&
                         oc_768_mode != cbrc_serdes_type_is_40g(handle->var.links[itr].serdes))
                {
                    rc = CBRC_ERR_SDH_PRBS_CONFLICT;
                }
            }
        }
    }    
    else
    {
        for (itr = 0; itr < CBRC_MAX_LINK && PMC_SUCCESS == rc; itr++)
        {
            if (FALSE != handle->var.prbs_ctxt[itr].tx_cfg_active)
            {
                if (FALSE != handle->var.prbs_ctxt[itr].tx_cfg.enable_prbs)
                {
                    rc = CBRC_ERR_GEN_PRBS_ALREADY_SELECTED;
                }
                else if (FALSE != handle->var.prbs_ctxt[itr].rx_cfg_active &&
                         TRUE == handle->var.prbs_ctxt[itr].rx_cfg.enable_prbs &&
                         oc_768_mode != cbrc_serdes_type_is_40g(handle->var.links[itr].serdes))
                {
                    rc = CBRC_ERR_SDH_PRBS_CONFLICT;
                }
            }
        }
    }    

    PMC_RETURN(rc);
}/*cbrc_prbs_enable_validate*/

/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_gen_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the SDH PRBS generator on a given channel.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*   *cfg_ptr          - PRBS configuration structure to configure payload
*                       mode and inversion
*   prbs_enable       - Enable or disable the PRBS. If FALSE, then cfg can
*                       be NULL.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sdh_prbs_gen_cfg(cbrc_handle_t         *handle,
                                        UINT8                 link,
                                        sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                        BOOL8                 prbs_enable)
{
    cbrc_link_info_t   *link_ptr;
    BOOL8 are_all_prbs_off;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 nb_links = 1;
    UINT32 i;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(FALSE == prbs_enable || NULL != cfg_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);
    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    /* check that there is no RX PRBS activated with a different OC_768_MODE */
    if (TRUE == prbs_enable)
    {
        result = cbrc_prbs_enable_validate(handle,
                                           TRUE,
                                           cbrc_serdes_type_is_40g(link_ptr->serdes));
        
        if (PMC_SUCCESS == result)
        {
            /* power up and enable TX PRBS block when it's not done yet  */
            result = cbrc_prbs_tx_bringup_down(handle, 
                                               cfg_ptr, 
                                               link, 
                                               cbrc_serdes_type_is_40g(link_ptr->serdes), 
                                               TRUE);
        }
    }
    
    if (PMC_SUCCESS == result)
    {            
        if (TRUE == cbrc_serdes_type_is_40g(link_ptr->serdes))
        {
            nb_links = 4;
        }
        
        for (i = 0; i < nb_links; i++)
        {
            /* set TX multiplexer */
            mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set(NULL, handle, 
                                                      link + i, 
                                                      prbs_enable ? CBRC_ENABLE : CBRC_DISABLE);
        }
        
        if (TRUE == prbs_enable)
        {
            /* save link tx context */
            result = cbrc_prbs_tx_ctxt_push(handle, link, cfg_ptr); 
        }
        else
        {
            result = cbrc_prbs_tx_ctxt_pop(handle, link, &are_all_prbs_off);
            /* power down TX PRBS block when all TX PRBS are disabled */
            if (PMC_SUCCESS == result && TRUE == are_all_prbs_off)
            {
                result = cbrc_prbs_tx_bringup_down(handle, NULL, link, 
                                                   cbrc_serdes_type_is_40g(link_ptr->serdes), 
                                                   FALSE);
            }
        }
    }

    PMC_RETURN(result);
} /* cbrc_cbr_prbs_gen_cfg */
    
/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_mon_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the SDH PRBS monitor on a given channel.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*   *cfg_ptr          - PRBS configuration structure to configure payload
*                       mode and inversion
*   prbs_enable       - Enable or disable the PRBS. If FALSE, then cfg can
*                       be NULL.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sdh_prbs_mon_cfg(cbrc_handle_t         *handle,
                                        UINT8                 link,
                                        sdh_pmg_prbs_cfg_t    *cfg_ptr,
                                        BOOL8                 prbs_enable)
{
    cbrc_link_info_t   *link_ptr;
    BOOL8 are_all_prbs_off;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(FALSE == prbs_enable || NULL != cfg_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);

    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    /* check that there is no TX PRBS activated with a different OC_768_MODE */
    if (TRUE == prbs_enable )
    {
        result = cbrc_prbs_enable_validate(handle,
                                           FALSE,
                                           cbrc_serdes_type_is_40g(link_ptr->serdes));
        if (PMC_SUCCESS == result)
        {
            result = cbrc_prbs_rx_bringup_down(handle, 
                                               cfg_ptr, 
                                               link, 
                                               cbrc_serdes_type_is_40g(link_ptr->serdes), 
                                               prbs_enable);
        }
    }

    if (PMC_SUCCESS == result)
    {              
        if (TRUE == prbs_enable)
        {
            /* save link rx context */
            result = cbrc_prbs_rx_ctxt_push(handle, link, cfg_ptr);
        }
        else
        {
            /* restore current mode AND PRBS_MG_RX_CHNL_EN, PRBS_RX_SW_RST & PRBS_RX_LOW_PWR */
            result = cbrc_prbs_rx_ctxt_pop(handle, link, &are_all_prbs_off);

            /* power down RX PRBS block when all TX PRBS are disabled */
            if (PMC_SUCCESS == result && TRUE == are_all_prbs_off)
            {
                result = cbrc_prbs_rx_bringup_down(handle, 
                                                   cfg_ptr, 
                                                   link, 
                                                   cbrc_serdes_type_is_40g(link_ptr->serdes), 
                                                   FALSE);
            }
        }
    }            

    PMC_RETURN(result);
} /* cbrc_cbr_prbs_mon_cfg */
    
/*******************************************************************************
* cbrc_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable a CBRC MLD Loopback.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - link
*   loopback          - Type of loopback - see cbrc_loopback_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_loopback_cfg(cbrc_handle_t            *cbrc_handle,
                                   UINT32                   link,
                                   cbrc_loopback_t          loopback)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 prev_loopback, i, sfis_index, max_index = 1;
    cbrc_link_info_t   *link_ptr;
    UINT32 link_list[CBRC_MAX_LINK];

    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, 0, 0);


    prev_loopback = cbrc_handle->var.loopback[link];
    /* make sure the user isn't activating a loopback when a previous loopback is already active */
    if (loopback != CBRC_DISABLE_LOOPBACK)
    {
        PMC_ASSERT(prev_loopback == CBRC_DISABLE_LOOPBACK,CBRC_ERR_LOOPBACK,0,0);
    }

    /* Retrieve pointer to requested link element */
    result = cbrc_link_get(cbrc_handle, link, &link_ptr);

   
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(cbrc_handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }
    /* 40g -> 4 links, else 1 */
    if (cbrc_serdes_type_is_40g(link_ptr->serdes))
    {
        max_index = 4;
    }
    sfis_index = (link == 0 ? 0 : 1);

    /* build the list of links to configure the loopback on */
    PMC_MEMSET(link_list, 0, sizeof(link_list));
    switch (link_ptr->serdes)
    {
    case CBRC_SERDES_TYPE_10G_XFI:
        link_list[0] = link;
        max_index = 1;
        break;
    
    case CBRC_SERDES_TYPE_40G_SFIS:
    case CBRC_SERDES_TYPE_40G_SFI51:
    case CBRC_SERDES_TYPE_40G_STL:
        for (i = 0; i < max_index; i++)
        {
            link_list[i] = link + i;
        }
        break;

    default:
        break;
    } /* end switch */

    
    /*  Configure the loopback */
    for(i=0;i < max_index && result == PMC_SUCCESS;i++)
    {
        switch(loopback) 
        {
        case CBRC_FACILITY_LOOPBACK:
            cbrc_fc_dlvr_field_LPBK_MUX_SEL_set(NULL,cbrc_handle,link_list[i],CBRC_ENABLE);
            break;

        case CBRC_DIAG_LOOPBACK:
            mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set(NULL, cbrc_handle, link_list[i], CBRC_ENABLE);
            break;

        case CBRC_DISABLE_LOOPBACK:
            /* Disable loopbacks */
            mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set(NULL, cbrc_handle, link_list[i], CBRC_DISABLE);
            cbrc_fc_dlvr_field_LPBK_MUX_SEL_set(NULL,cbrc_handle,link_list[i],CBRC_DISABLE);
            break;

        default:
            PMC_ASSERT(FALSE,CBRC_ERR_LOOPBACK,0,0);
        } 
        cbrc_handle->var.loopback[link] = loopback;
    }
    PMC_RETURN(result);
} /* cbrc_loopback_cfg */

/*******************************************************************************
* cbrc_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the status of the CBRC loopback.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - link

* OUTPUTS:
*   loopback_ptr      - loopback status
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_loopback_status_get(cbrc_handle_t            *cbrc_handle,
                                          UINT32                   link,
                                          cbrc_loopback_t          *loopback_ptr)
{
    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, 0, 0);

    *loopback_ptr = cbrc_handle->var.loopback[link];
    PMC_RETURN(PMC_SUCCESS);
} /* cbrc_loopback_status_get */



/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_gen_lfsr_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves the PRBS monitor LFSR state.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*    *status          - read LFSR stages state. The state ?all zeros? 
*                       could happen if a problem occurs.
*                         
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_gen_lfsr_state_get(cbrc_handle_t            *handle,
                                                  UINT8                     link,
                                                  UINT32                   *status)
{
    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t gen_prbs_msb_buf[1];
    config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t gen_prbs_lsb_buf[1];
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 reg_value;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != status, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_TX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_init(gen_prbs_msb_buf);
    config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_init(gen_prbs_lsb_buf);
    
    result = config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_retrieve(handle, 0, 1, gen_prbs_msb_buf);
    
    if (PMC_SUCCESS == result)
    {
        
        result = config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_retrieve(handle, 0, 1, gen_prbs_lsb_buf);
        if (PMC_SUCCESS == result) 
        {
            /* (0x00000012 bits 15:0) bits 7:22 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR */
            reg_value = gen_prbs_msb_buf[0].base.data[0];
            *status = (((reg_value & CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_OFF) & 0xffff ) << 7;

            /* (0x00000014 bits 6:0) bits 0:6 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR */
            reg_value = gen_prbs_lsb_buf[0].base.data[0];
            *status |= ((reg_value & CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_OFF);
        }
    }

    PMC_RETURN(result);
}/* cbrc_sdh_prbs_gen_lfsr_state_get */


/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_mon_lfsr_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves the PRBS monitor LFSR state.
*
* INPUTS:
*   handle            - Pointer to subsystem handle
*   link              - Post-mld link to activate (0..11)
*
* OUTPUTS:
*    *status          - read LFSR stages state. The state ?all zeros? 
*                       could happen if a problem occurs.
*                         
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sdh_prbs_mon_lfsr_state_get(cbrc_handle_t            *handle,
                                                  UINT8                     link,
                                                  UINT32                   *status)
{
    config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t mon_prbs_msb_buf[1];
    config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t mon_prbs_lsb_buf[1];
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 reg_value;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != status, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_init(mon_prbs_msb_buf);
    config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_init(mon_prbs_lsb_buf);
    
    result = config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_retrieve(handle, 0, 1, mon_prbs_msb_buf);
    
    if (PMC_SUCCESS == result)
    {        
        result = config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_retrieve(handle, 0, 1, mon_prbs_lsb_buf);
        if (PMC_SUCCESS == result) 
        {
            /* (0x00000012 bits 15:0) bits 7:22 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR */
            reg_value = mon_prbs_msb_buf[0].base.data[0];
            *status = (((reg_value & CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK) >> CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_OFF) & 0xffff ) << 7;
            
            /* (0x00000014 bits 6:0) bits 0:6 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR */
            reg_value = mon_prbs_lsb_buf[0].base.data[0];
            *status |= ((reg_value & CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK) >> CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_OFF);
        }
    }

    PMC_RETURN(result);
}/* cbrc_sdh_prbs_mon_lfsr_state_get */


/*******************************************************************************
* FUNCTION: cbrc_get_cbr_fsgm_handle()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves cbr_fsgm handle based on link and direction.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  **cbr_fsgm_handle_pptr  - CBR FSGM handle
*   *index_ptr             - index within CBR FSGM
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_get_cbr_fsgm_handle(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32               link,
                                          cbrc_port_dir_t      dir,
                                          cbr_fsgm_handle_t  **cbr_fsgm_handle_pptr,
                                          UINT32              *index_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         slice;

    PMC_ENTRY();

    *index_ptr = link % CBR_FSGM_MAX_INDEX;
    slice  = link / CBR_FSGM_MAX_INDEX;

    /* Get CBR FSGM handle */
    if (CBRC_PORT_DIR_TX == dir) {
        *cbr_fsgm_handle_pptr = cbrc_handle_ptr->cbr_fsgm_tx_ptr[slice];
    } else {
        *cbr_fsgm_handle_pptr = cbrc_handle_ptr->cbr_fsgm_rx_ptr[slice];
    }


    PMC_RETURN(result);
} /* cbrc_get_cbr_fsgm_handle */

/*******************************************************************************
* FUNCTION: cbrc_get_fc800_pmg_handle()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves fc800_pmg handle based on link and direction.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  **fc800_pmg_handle_pptr - FC800_PMG handle
*   *index_ptr             - index within FC800_PMG
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_get_fc800_pmg_handle(cbrc_handle_t       *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           fc800_pmg_handle_t **fc800_pmg_handle_pptr,
                                           UINT32              *index_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();


    /* Get FC800_PMG handle */
    if (CBRC_PORT_DIR_TX == dir) {
        *fc800_pmg_handle_pptr = cbrc_handle_ptr->fc800_pmg_tx_ptr;
    } else {
        *fc800_pmg_handle_pptr = cbrc_handle_ptr->fc800_pmg_rx_ptr;
    }

    
    *index_ptr = link;

    PMC_RETURN(result);
} /* cbrc_get_cbr_fsgm_handle */


/*******************************************************************************
* FUNCTION: cbrc_get_fc1200_pmon_handle()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  **fc1200_handle_pptr  - FC1200 PMON handle
*   *index_ptr           - index within FC1200 PMON
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_get_fc1200_pmon_handle(cbrc_handle_t           *cbrc_handle_ptr,
                                             UINT32                   link,
                                             cbrc_port_dir_t          dir,
                                             fc1200_pmon_handle_t   **fc1200_handle_pptr,
                                             UINT32                  *index_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();


    /* Get FC1200 PMON handle */
    if (CBRC_PORT_DIR_TX == dir) {
        *fc1200_handle_pptr = cbrc_handle_ptr->fc1200_pmon_tx_ptr;
    } else {
        *fc1200_handle_pptr = cbrc_handle_ptr->fc1200_pmon_rx_ptr;
    }

    *index_ptr = link;

    PMC_RETURN(result);
} /* cbrc_get_fc1200_pmon_handle */




/*******************************************************************************
* FUNCTION: cbrc_fsgm_pattern_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   pattern           - Fault pattern to be inserted 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_pattern_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                       UINT32               link,
                                       cbrc_port_dir_t      dir,
                                       cbr_fsgm_pattern_t   pattern)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }


    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_pattern_cfg(cbr_fsgm_handle, index, pattern);   
    } 

    PMC_RETURN(result);
} /* cbrc_fsgm_pattern_cfg */





/*******************************************************************************
* FUNCTION: cbrc_fsgm_pattern_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *pattern_ptr        - Configured fault pattern to be inserted 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_pattern_cfg_get(cbrc_handle_t       *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           cbr_fsgm_pattern_t  *pattern_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_pattern_cfg_get(cbr_fsgm_handle, index, pattern_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fsgm_pattern_cfg_get */




/*******************************************************************************
* FUNCTION: cbrc_fsgm_user_fault_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets User Fault Pattern value to be inserted.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   user_pattern      - User fault pattern to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_user_fault_pattern_set(cbrc_handle_t                 *cbrc_handle_ptr,
                                                  UINT32                         link,
                                                  cbrc_port_dir_t                dir,
                                                  cbr_fsgm_user_fault_pattern_t  user_pattern)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }


    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_user_fault_pattern_set(cbr_fsgm_handle, index, user_pattern);   
    } 

    PMC_RETURN(result);
} /* cbrc_fsgm_user_fault_pattern_set */





/*******************************************************************************
* FUNCTION: cbrc_fsgm_user_fault_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured User Fault Pattern value to be inserted.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*   user_pattern_ptr  - Configured user fault pattern
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_user_fault_pattern_get(cbrc_handle_t                  *cbrc_handle_ptr,
                                                  UINT32                          link,
                                                  cbrc_port_dir_t                 dir,
                                                  cbr_fsgm_user_fault_pattern_t  *user_pattern_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_user_fault_pattern_get(cbr_fsgm_handle, index, user_pattern_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fsgm_user_fault_pattern_get */



/*******************************************************************************
* FUNCTION: cbrc_fsgm_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces fault pattern into output. Fault pattern is configured with 
*   cbrc_fsgm_pattern_cfg();
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enable or disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_fault_force(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link,
                                       cbrc_port_dir_t   dir,
                                       BOOL8              enable)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_fault_force(cbr_fsgm_handle, index, enable);   
    } 


    PMC_RETURN(result);
} /* cbrc_fsgm_fault_force */





/*******************************************************************************
* FUNCTION: cbrc_fsgm_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if fault pattern is being forced into the output. 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_fault_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL             *enable_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_fault_force_get(cbr_fsgm_handle, index, enable_ptr);   
    } 


    PMC_RETURN(result);
} /* cbrc_fsgm_fault_force_get */



/*******************************************************************************
* FUNCTION: cbrc_fsgm_pn11_mon_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables PN11 monitoring. Valid for 10G and 40G modes.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enable PN11 monitoring
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_pn11_mon_enable(cbrc_handle_t       *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           BOOL8                 enable)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_pn11_mon_enable(cbr_fsgm_handle, index, enable);   
    } 


    PMC_RETURN(result);
} /* cbrc_fsgm_pn11_mon_enable */


/*******************************************************************************
* FUNCTION: cbrc_fsgm_pn11_mon_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 monitoring is enabled or disabled. 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_pn11_mon_enable_get(cbrc_handle_t   *cbrc_handle_ptr,
                                               UINT32               link,
                                               cbrc_port_dir_t      dir,
                                               BOOL                *enable_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_pn11_mon_enable_get(cbr_fsgm_handle, index, enable_ptr);   
    } 


    PMC_RETURN(result);
} /* cbrc_fsgm_pn11_mon_enable */



/*******************************************************************************
* FUNCTION: cbrc_fsgm_insert_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how patterns are inserted into the output stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*  *cfg_ptr           - pointer to pattern insertion configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_insert_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                      UINT32                    link,
                                      cbrc_port_dir_t           dir,
                                      cbr_fsgm_pattern_cfg_t   *cfg_ptr)

{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_insert_cfg(cbr_fsgm_handle, index, cfg_ptr);   
    } 



    PMC_RETURN(result);
} /* cbrc_fsgm_insert_cfg */




/*******************************************************************************
* FUNCTION: cbrc_fsgm_insert_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves config for how patterns are inserted into the output stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *cfg_ptr           - Configured pattern insertion configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fsgm_insert_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                          UINT32                    link,
                                          cbrc_port_dir_t           dir,
                                          cbr_fsgm_pattern_cfg_t   *cfg_ptr)

{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_insert_cfg_get(cbr_fsgm_handle, index, cfg_ptr);   
    } 



    PMC_RETURN(result);
} /* cbrc_fsgm_insert_cfg_get */




/*******************************************************************************
* FUNCTION: cbrc_patt_ins_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the generation of the selected fault pattern.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   force             - Enable the insertion of the fault pattern
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_patt_ins_fault_force(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL8              force)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_patt_ins_fault_force(cbr_fsgm_handle, index, force);   
    } 

    PMC_RETURN(result);
} /* cbrc_patt_ins_fault_force */



/*******************************************************************************
* FUNCTION: cbrc_patt_ins_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the generation of the selected fault pattern is forced.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *force_ptr         - Force generation is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_patt_ins_fault_force_get(cbrc_handle_t        *cbrc_handle_ptr,
                                               UINT32                link,
                                               cbrc_port_dir_t       dir,
                                               BOOL                 *force_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_patt_ins_fault_force_get(cbr_fsgm_handle, index, force_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_patt_ins_fault_force_get */

/*******************************************************************************
* FUNCTION: cbrc_patt_ins_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enables the insertion when needed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_patt_ins_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link,
                                      cbrc_port_dir_t   dir,
                                      BOOL8              enable)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_patt_ins_enable(cbr_fsgm_handle, index, enable);   
    } 

    PMC_RETURN(result);
} /* cbrc_patt_ins_enable */




/*******************************************************************************
* FUNCTION: cbrc_patt_ins_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves  if the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel is enabled or disabled.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_patt_ins_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32            link,
                                          cbrc_port_dir_t   dir,
                                          BOOL             *enable_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    cbr_fsgm_handle_t   *cbr_fsgm_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle_ptr, link, dir,
                                          &cbr_fsgm_handle,
                                          &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = cbr_fsgm_patt_ins_enable_get(cbr_fsgm_handle, index, enable_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_patt_ins_enable_get */


/*******************************************************************************
* FUNCTION: cbrc_fc800_user_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the user defined pattern to be inserted into the transmit path.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   user_pattern      - 32-bit user pattern to be inserted
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_user_pattern_set(cbrc_handle_t    *cbrc_handle_ptr,
                                             UINT32            link,
                                             cbrc_port_dir_t   dir,
                                             UINT32            user_pattern)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_user_pattern_set(fc800_pmg_handle, index, user_pattern);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_user_pattern_set */


/*******************************************************************************
* FUNCTION: cbrc_fc800_user_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the user defined pattern to be inserted into the transmit path. 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*   user_pattern_ptr     - configured 32-bit user pattern to be inserted
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_user_pattern_get(cbrc_handle_t    *cbrc_handle_ptr,
                                             UINT32            link,
                                             cbrc_port_dir_t   dir,
                                             UINT32           *user_pattern_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_user_pattern_get(fc800_pmg_handle, index, user_pattern_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_user_pattern_get */



/*******************************************************************************
* FUNCTION: cbrc_fc800_pattern_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the pattern insertion during DLOLB or fc800_pmg_pattern_force() 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   pattern_ins_cfg   - Source of pattern to insert (user or NOS/PN11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_pattern_ins_set(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_pattern_ins_set(fc800_pmg_handle, index, pattern_ins_cfg);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_pattern_ins_set */


/*******************************************************************************
* FUNCTION: cbrc_fc800_pattern_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the configured the pattern insertion
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*   pattern_ins_cfg_ptr - Source of pattern to insert (user or NOS/PN11)
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_pattern_ins_get(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_pattern_ins_get(fc800_pmg_handle, index, pattern_ins_cfg_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_pattern_ins_get */




/*******************************************************************************
* FUNCTION: cbrc_fc800_pattern_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the configured pattern into the datastream.
*
*   The pattern source (user pattern or NOS/PN11) can be configured with 
*   cbrc_fc800_pattern_ins_set().
*
*   The user pattern can be configured with cbrc_fc800_user_pattern_set().
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   force             - TRUE to force the pattern. 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_pattern_force(cbrc_handle_t          *cbrc_handle_ptr,
                                          UINT32                  link,
                                          cbrc_port_dir_t         dir,
                                          BOOL8                    force)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_pattern_force(fc800_pmg_handle, index, force);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_pattern_force*/


/*******************************************************************************
* FUNCTION: cbrc_fc800_pattern_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the configured pattern is forced into the datastream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*   force_ptr            - TRUE if pattern is forced into datastream.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_pattern_force_get(cbrc_handle_t         *cbrc_handle_ptr,
                                              UINT32                 link,
                                              cbrc_port_dir_t        dir,
                                              BOOL                  *force_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_pattern_force_get(fc800_pmg_handle, index, force_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_pattern_force_get */


/*******************************************************************************
* FUNCTION: cbrc_fc800_dlolb_gen_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures what defects contribute to DLOLB generation.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   dlolb_gen_cfg_ptr - DLOLB generation configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_dlolb_gen_set(cbrc_handle_t          *cbrc_handle_ptr,
                                          UINT32                  link,
                                          cbrc_port_dir_t         dir,
                                          fc800_pmg_dlolb_gen_t  *dlolb_gen_cfg_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_dlolb_gen_set(fc800_pmg_handle, index, dlolb_gen_cfg_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_dlolb_gen_set */


/*******************************************************************************
* FUNCTION: cbrc_fc800_dlolb_gen_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves what defects contribute to DLOLB generation.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*   dlolb_gen_cfg_ptr   - DLOLB generation configuration
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc800_dlolb_gen_get(cbrc_handle_t         *cbrc_handle_ptr,
                                          UINT32                 link,
                                          cbrc_port_dir_t        dir,
                                          fc800_pmg_dlolb_gen_t *dlolb_gen_cfg_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;    
    fc800_pmg_handle_t  *fc800_pmg_handle;
    UINT32               index;

    PMC_ENTRY();


    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_CBR);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc800_pmg_handle(cbrc_handle_ptr, link, dir,
                                           &fc800_pmg_handle,
                                           &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_dlolb_gen_get(fc800_pmg_handle, index, dlolb_gen_cfg_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc800_dlolb_gen_get */



/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_dlolb_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures FC1200 PMON DLOLB insertion options.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*  *cfg_ptr           - pointer to configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_cfg(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc1200_pmon_dlolb_cfg_t      *cfg_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_dlolb_cfg(fc1200_handle, index, cfg_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_dlolb_cfg */






/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_dlolb_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FC1200 PMON DLOLB insertion options.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*    *cfg_ptr         - pointer to configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_cfg_get(cbrc_handle_t                 *cbrc_handle_ptr,
                                                UINT32                         link,
                                                cbrc_port_dir_t                dir,
                                                fc1200_pmon_dlolb_cfg_t       *cfg_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_dlolb_cfg_get(fc1200_handle, index, cfg_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_dlolb_cfg_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_dlolb_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Forces DLOLB.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_force(cbrc_handle_t    *cbrc_handle_ptr,
                                              UINT32            link,
                                              cbrc_port_dir_t   dir,
                                              BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_dlolb_force(fc1200_handle, index, enable);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_dlolb_force */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_dlolb_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves if DLOLB is forced.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_force_get(cbrc_handle_t      *cbrc_handle_ptr,
                                                  UINT32              link,
                                                  cbrc_port_dir_t     dir,
                                                  BOOL               *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_dlolb_force_get(fc1200_handle, index, enable_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_dlolb_force_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_invalid_block_mode_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how blocks with invalid block type encodings are handled when
*   encountered during processing.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   mode              - configures behaviour when an invalid block is received
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_invalid_block_mode_set(
    cbrc_handle_t                    *cbrc_handle_ptr,
    UINT32                            link,
    cbrc_port_dir_t                   dir,
    fc1200_pmon_invalid_block_mode_t  mode) 
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_invalid_block_mode_set(fc1200_handle, index, mode);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_invalid_block_mode_set */



/*******************************************************************************
* FUNCTION: cbrc_fc1200_pmon_invalid_block_mode_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves how blocks with invalid block type encodings are handled when
*   encountered during processing.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*  *mode_ptr          - behaviour when an invalid block is received
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_pmon_invalid_block_mode_get(
    cbrc_handle_t                    *cbrc_handle_ptr,
    UINT32                            link,
    cbrc_port_dir_t                   dir,
    fc1200_pmon_invalid_block_mode_t *mode_ptr) 
{
    PMC_ERROR               result = PMC_SUCCESS;    
    fc1200_pmon_handle_t   *fc1200_handle;
    UINT32                  index;


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_fc1200_pmon_handle(cbrc_handle_ptr, link, dir,
                                             &fc1200_handle,
                                             &index); 
    }

    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_invalid_block_mode_get(fc1200_handle, index, mode_ptr);   
    } 

    PMC_RETURN(result);
} /* cbrc_fc1200_pmon_invalid_block_mode_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_prbs31_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables the PRBS31 generator and monitor. 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_prbs31_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (CBRC_PORT_DIR_RX == dir)
    {
        fc1200_pcs_rx_field_PRBS31_ENA_set(NULL, cbrc_handle_ptr, link, enable);   
    }
    else
    {
        fc1200_pcs_tx_field_PRBS31_ENA_set(NULL, cbrc_handle_ptr, link, enable);   
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_prbs31_enable */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_prbs31_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the the PRBS31 generator and monitor is enabled or disabled.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_prbs31_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                               UINT32            link,
                                               cbrc_port_dir_t   dir,
                                               BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (CBRC_PORT_DIR_RX == dir)
    {
        *enable_ptr = fc1200_pcs_rx_field_PRBS31_ENA_get(NULL, cbrc_handle_ptr, link);   
    }
    else
    {
        *enable_ptr = fc1200_pcs_tx_field_PRBS31_ENA_get(NULL, cbrc_handle_ptr, link);   
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_prbs31_enable_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_lfos_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts LFOS bits into rx or tx stream. LFOS bits can be configured with
*   digi_cbr_fc1200_lfos_set().
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_lfos_force(cbrc_handle_t    *cbrc_handle_ptr,
                                        UINT32            link,
                                        cbrc_port_dir_t   dir,
                                        BOOL8             enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (CBRC_PORT_DIR_RX == dir)
    {
        fc_tttm_field_FORCE_LF_set(NULL, cbrc_handle_ptr, link, enable);   
    }
    else
    {
        fc_tttd_field_FORCE_LF_set(NULL, cbrc_handle_ptr, link, enable);   
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_lfos_force */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_lfos_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if LFOS bits are forced into rx or tx stream. 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_lfos_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                            UINT32            link,
                                            cbrc_port_dir_t   dir,
                                            BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }


    if (CBRC_PORT_DIR_RX == dir)
    {
        *enable_ptr = fc_tttm_field_FORCE_LF_get(NULL, cbrc_handle_ptr, link);   
    }
    else
    {
        *enable_ptr = fc_tttd_field_FORCE_LF_get(NULL, cbrc_handle_ptr, link);   
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_lfos_force_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_lfos_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Local Fault Ordered Sets (LFOS) bits that can be inserted
*   into the rx or tx stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*  *lfos_ptr          - pointer to LFOS bits to set
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_lfos_set(cbrc_handle_t       *cbrc_handle_ptr,
                                      UINT32               link,
                                      cbrc_port_dir_t      dir,
                                      cbrc_fc1200_lfos_t  *lfos_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_ptr, CBRC_ERR_INVALID_ARG, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (CBRC_PORT_DIR_RX == dir)
    {
        fc_tttm_lfield_LFOS_set(NULL, cbrc_handle_ptr, link, lfos_ptr->lfos);
    }
    else
    {
        fc_tttd_lfield_LFOS_set(NULL, cbrc_handle_ptr, link, lfos_ptr->lfos);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_lfos_set */



/*******************************************************************************
* FUNCTION: cbrc_fc1200_lfos_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Local Fault Ordered Sets (LFOS) bits that can be inserted
*   into the rx or tx stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   dir               - direction of traffic
*
* OUTPUTS:
*    *lfos_ptr         - pointer to configured LFOS bits
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_lfos_get(cbrc_handle_t         *cbrc_handle_ptr,
                                      UINT32                 link,
                                      cbrc_port_dir_t        dir,
                                      cbrc_fc1200_lfos_t    *lfos_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_ptr, CBRC_ERR_INVALID_ARG, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, dir, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (CBRC_PORT_DIR_RX == dir)
    {
        fc_tttm_lfield_LFOS_get(NULL, cbrc_handle_ptr, link, lfos_ptr->lfos);
    }
    else
    {
        fc_tttd_lfield_LFOS_get(NULL, cbrc_handle_ptr, link, lfos_ptr->lfos);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_lfos_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_lfos_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS insertion during error conditions in the FC1200 RX stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*  *lfos_cfg_ptr      - pointer to LFOS config structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_lfos_cfg(cbrc_handle_t             *cbrc_handle_ptr,
                                         UINT32                     link,
                                         cbrc_fc1200_rx_lfos_cfg_t *lfos_cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT32        mask;

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_cfg_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        mask = lfos_cfg_ptr->mask;

        if (CBRC_FC1200_RX_LFOS_CFG_MASK_LINK_FAIL_ENABLE & mask)
        {
            fc_tttm_field_LFOS_LINK_FAIL_set(NULL, cbrc_handle_ptr, link, lfos_cfg_ptr->link_fail_enable);
        }

        if (CBRC_FC1200_RX_LFOS_CFG_MASK_SYNC_LOST_ENABLE & mask)
        {
            fc_tttm_field_LFOS_SYNC_LOST_set(NULL, cbrc_handle_ptr, link, lfos_cfg_ptr->sync_lost_enable);
        }

        if (CBRC_FC1200_RX_LFOS_CFG_MASK_HIGH_BER_ENABLE  & mask)
        {
            fc_tttm_field_LFOS_HIBER_set(NULL, cbrc_handle_ptr, link, lfos_cfg_ptr->high_ber_enable);
        }
    } /* end if */

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_lfos_cfg */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_lfos_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves LFOS insertion config for error conditions in the FC1200 RX stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*    *lfos_cfg_ptr     - pointer to LFOS config structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_lfos_cfg_get(cbrc_handle_t                 *cbrc_handle_ptr,
                                             UINT32                         link,
                                             cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_cfg_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        lfos_cfg_ptr->mask = CBRC_FC1200_RX_LFOS_CFG_MASK_LINK_FAIL_ENABLE |
            CBRC_FC1200_RX_LFOS_CFG_MASK_SYNC_LOST_ENABLE |
            CBRC_FC1200_RX_LFOS_CFG_MASK_HIGH_BER_ENABLE;

        lfos_cfg_ptr->link_fail_enable = fc_tttm_field_LFOS_LINK_FAIL_get(NULL, cbrc_handle_ptr, link);
        lfos_cfg_ptr->sync_lost_enable = fc_tttm_field_LFOS_SYNC_LOST_get(NULL, cbrc_handle_ptr, link);
        lfos_cfg_ptr->high_ber_enable  = fc_tttm_field_LFOS_HIBER_get(NULL, cbrc_handle_ptr, link);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_lfos_cfg_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_tx_lfos_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS insertion during error conditions in the FC1200 TX stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*  *lfos_cfg_ptr      - pointer to LFOS config structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_tx_lfos_cfg(cbrc_handle_t              *cbrc_handle_ptr,
                                         UINT32                      link,
                                         cbrc_fc1200_tx_lfos_cfg_t  *lfos_cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT32        mask;

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_cfg_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        mask = lfos_cfg_ptr->mask;

        if (CBRC_FC1200_TX_LFOS_CFG_MASK_LOS_ENABLE & mask)
        {
            fc1200_pcs_tx_field_LOS_LF_ENA_set(NULL, cbrc_handle_ptr, link, lfos_cfg_ptr->los_enable);
        }

        if (CBRC_FC1200_TX_LFOS_CFG_MASK_EXT_ENABLE & mask)
        {
            fc1200_pcs_tx_field_LOS_LF_ENA_set(NULL, cbrc_handle_ptr, link, lfos_cfg_ptr->ext_enable);
        }
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_tx_lfos_cfg */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_tx_lfos_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves LFOS insertion config for error conditions in the FC1200 RX stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *lfos_cfg_ptr      - pointer to LFOS config structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_tx_lfos_cfg_get(cbrc_handle_t              *cbrc_handle_ptr,
                                             UINT32                      link,
                                             cbrc_fc1200_tx_lfos_cfg_t  *lfos_cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lfos_cfg_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        lfos_cfg_ptr->mask = CBRC_FC1200_TX_LFOS_CFG_MASK_LOS_ENABLE |
            CBRC_FC1200_TX_LFOS_CFG_MASK_EXT_ENABLE;


        lfos_cfg_ptr->los_enable = fc1200_pcs_tx_field_LOS_LF_ENA_get(NULL, cbrc_handle_ptr, link);
        lfos_cfg_ptr->ext_enable = fc1200_pcs_tx_field_LOS_LF_ENA_get(NULL, cbrc_handle_ptr, link);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_tx_lfos_cfg_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_restore_invalid_idle_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls how a 66b block with block type 0x1E, but an
*   unrecognized payload, should be handled when received.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   enable            - Enable: The invalid idle block is restored to a valid
*                               Idle/Idle block.
*                       Disable: The invalid idle block is treated as an
*                                errored block. 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_restore_invalid_idle_enable(
    cbrc_handle_t    *cbrc_handle_ptr,
    UINT32            link,
    BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set(NULL, cbrc_handle_ptr, link, enable);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_restore_invalid_idle_enable */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_restore_invalid_idle_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration for how a 66b block with block type 0x1E, but an
*   unrecognized payload, should be handled when received.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *enable_ptr        - Enable: The invalid idle block is restored to a valid
*                               Idle/Idle block.
*                       Disable: The invalid idle block is treated as an
*                                errored block. 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_restore_invalid_idle_enable_get(
    cbrc_handle_t    *cbrc_handle_ptr,
    UINT32            link,
    BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        *enable_ptr = fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_rx_restore_invalid_idle_enable_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_descrambler_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the FC1200 RX line descrambler.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_descrambler_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                                   UINT32            link,
                                                   BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc1200_pcs_rx_field_DESCRAMBLER_ENA_set(NULL, cbrc_handle_ptr, link, enable);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_descrambler_enable */

/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_descrambler_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves if the FC1200 RX line descrambler is enabled/disabled.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_descrambler_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                                       UINT32            link,
                                                       BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        *enable_ptr = fc1200_pcs_rx_field_DESCRAMBLER_ENA_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_rx_descrambler_enable_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_ecb_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts ECB bits into rx stream. ECB bits can be configured with
*   digi_cbr_fc1200_rx_ecb_set().
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_force(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32            link,
                                          BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc_tttm_field_FORCE_ERR_set(NULL, cbrc_handle_ptr, link, enable);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_ecb_force */






/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_ecb_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if ECB bit insertion into rx stream is enabled/disabled.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *enable_ptr        - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                              UINT32            link,
                                              BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        *enable_ptr = fc_tttm_field_FORCE_ERR_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);

} /* cbrc_fc1200_rx_ecb_force_get */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_ecb_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Error Control Block (ECB) bits 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*  *ecb_ptr           - pointer to ECB bits to set
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_set(cbrc_handle_t      *cbrc_handle_ptr,
                                        UINT32              link,
                                        cbrc_fc1200_ecb_t  *ecb_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ecb_ptr, CBRC_ERR_INVALID_ARG, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }


    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc_tttm_lfield_ECB_set(NULL, cbrc_handle_ptr, link, ecb_ptr->ecb);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_ecb_set */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_ecb_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the Error Control Block (ECB) bits 
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *ecb_ptr           - pointer to ECB bits to set
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_get(cbrc_handle_t      *cbrc_handle_ptr,
                                        UINT32              link,
                                        cbrc_fc1200_ecb_t  *ecb_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ecb_ptr, CBRC_ERR_INVALID_ARG, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    
    if (PMC_SUCCESS == result)
    {
        fc_tttm_lfield_ECB_get(NULL, cbrc_handle_ptr, link, ecb_ptr->ecb);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_ecb_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_gfp_payload_scramble_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables GFP payload scrambling.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   enable            - 1: The GFP payload is scambled with the x43 + 1
*                          self synchronous scrambler.
*                       0: Scrambling is disabled.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_payload_scramble_enable(cbrc_handle_t  *cbrc_handle_ptr,
                                                            UINT32          link,
                                                            BOOL8            enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc_tttm_field_DIS_SCRAM_set(NULL, cbrc_handle_ptr, link, !enable);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_rx_gfp_payload_sramble_enable */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_gfp_payload_scramble_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if GFP payload scrambling is enabled/disabled.
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *enable_ptr        - 1: The GFP payload is scambled with the x43 + 1
*                          self synchronous scrambler.
*                       0: Scrambling is disabled.
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_payload_scramble_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                                                UINT32            link,
                                                                BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        *enable_ptr = !fc_tttm_field_DIS_SCRAM_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_rx_gfp_payload_scramble_enable_get */







/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_gfp_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GFP core header and payload information bytes
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*  *gfp_ptr           - pointer to GFP bytes for configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_set(cbrc_handle_t         *cbrc_handle_ptr,
                                        UINT32                 link,
                                        cbrc_fc1200_rx_gfp_t  *gfp_ptr)
{

    PMC_ERROR     result = PMC_SUCCESS;    
    UINT32        mask;

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != gfp_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_RX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
     
    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        mask = gfp_ptr->mask;

        if (CBRC_FC1200_RX_GFP_MASK_PLI & mask)
        {
            fc_tttm_field_PLI_set(NULL, cbrc_handle_ptr, link, gfp_ptr->pli);
        }

        if (CBRC_FC1200_RX_GFP_MASK_CHEC & mask)
        {
            fc_tttm_field_CHEC_set(NULL, cbrc_handle_ptr, link, gfp_ptr->chec);
        }

        if (CBRC_FC1200_RX_GFP_MASK_PTI & mask)
        {
            fc_tttm_field_PTI_set(NULL, cbrc_handle_ptr, link, gfp_ptr->pti);
        }

        if (CBRC_FC1200_RX_GFP_MASK_PFI & mask)
        {
            fc_tttm_field_PFI_set(NULL, cbrc_handle_ptr, link, gfp_ptr->pfi);
        }

        if (CBRC_FC1200_RX_GFP_MASK_EXI & mask)
        {
            fc_tttm_field_EXI_set(NULL, cbrc_handle_ptr, link, gfp_ptr->exi);
        }

        if (CBRC_FC1200_RX_GFP_MASK_UPI & mask)
        {
            fc_tttm_field_UPI_set(NULL, cbrc_handle_ptr, link, gfp_ptr->upi);
        }

        if (CBRC_FC1200_RX_GFP_MASK_THEC & mask)
        {
            fc_tttm_field_THEC_set(NULL, cbrc_handle_ptr, link, gfp_ptr->thec);
        }

        if (CBRC_FC1200_RX_GFP_MASK_RES1 & mask)
        {
            fc_tttm_field_GFP_RES1_set(NULL, cbrc_handle_ptr, link, gfp_ptr->res1);
        }

        if (CBRC_FC1200_RX_GFP_MASK_RES2 & mask)
        {
            fc_tttm_field_GFP_RES2_set(NULL, cbrc_handle_ptr, link, gfp_ptr->res2);
        }

        if (CBRC_FC1200_RX_GFP_MASK_RES3 & mask)
        {
            fc_tttm_field_GFP_RES3_set(NULL, cbrc_handle_ptr, link, gfp_ptr->res3);
        }
        
        if (CBRC_FC1200_RX_GFP_MASK_RES4 & mask)
        {
            fc_tttm_field_GFP_RES4_set(NULL, cbrc_handle_ptr, link, gfp_ptr->res4);
        }
    }



    PMC_RETURN(result);

} /* cbrc_fc1200_rx_gfp_set */


/*******************************************************************************
* FUNCTION: cbrc_fc1200_rx_gfp_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured GFP core header and payload information bytes
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*  *gfp_ptr           - configured GFP bytes
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_get(cbrc_handle_t         *cbrc_handle_ptr,
                                        UINT32                 link,
                                        cbrc_fc1200_rx_gfp_t  *gfp_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != gfp_ptr, CBRC_ERR_INVALID_ARG, 0, 0);
    
    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }
    
    if (PMC_SUCCESS == result)
    {
        gfp_ptr->mask = CBRC_FC1200_RX_GFP_MASK_PLI  |
            CBRC_FC1200_RX_GFP_MASK_CHEC |
            CBRC_FC1200_RX_GFP_MASK_PTI  |
            CBRC_FC1200_RX_GFP_MASK_PFI  |
            CBRC_FC1200_RX_GFP_MASK_EXI  |
            CBRC_FC1200_RX_GFP_MASK_UPI  |
            CBRC_FC1200_RX_GFP_MASK_THEC |
            CBRC_FC1200_RX_GFP_MASK_RES1 |
            CBRC_FC1200_RX_GFP_MASK_RES2 |
            CBRC_FC1200_RX_GFP_MASK_RES3 |
            CBRC_FC1200_RX_GFP_MASK_RES4;

        
        gfp_ptr->pli  = fc_tttm_field_PLI_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->chec = fc_tttm_field_CHEC_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->pti  = fc_tttm_field_PTI_get(NULL, cbrc_handle_ptr, link); 
        gfp_ptr->pfi  = fc_tttm_field_PFI_get(NULL, cbrc_handle_ptr, link); 
        gfp_ptr->exi  = fc_tttm_field_EXI_get(NULL, cbrc_handle_ptr, link); 
        gfp_ptr->upi  = fc_tttm_field_UPI_get(NULL, cbrc_handle_ptr, link); 
        gfp_ptr->thec = fc_tttm_field_THEC_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->res1 = fc_tttm_field_GFP_RES1_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->res2 = fc_tttm_field_GFP_RES2_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->res3 = fc_tttm_field_GFP_RES3_get(NULL, cbrc_handle_ptr, link);
        gfp_ptr->res4 = fc_tttm_field_GFP_RES4_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_rx_gfp_get */




/*******************************************************************************
* FUNCTION: cbrc_fc1200_tx_corrupt_blk_conv_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables converting corrupt blocks to error blocks in the FC1200 
*   TX stream.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   enable            - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_tx_corrupt_blk_conv_enable(
    cbrc_handle_t    *cbrc_handle_ptr,
    UINT32            link,
    BOOL8              enable)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set(NULL, cbrc_handle_ptr, link, enable);
    }

    PMC_RETURN(result);
} /* cbrc_fc1200_tx_corrupt_blk_conv_enable */





/*******************************************************************************
* FUNCTION: cbrc_fc1200_tx_corrupt_blk_conv_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if converting corrupt blocks to error blocks in the FC1200 
*   TX stream is enabled or disabled.
*   
*
* INPUTS:
*   cbrc_handle_ptr   - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_fc1200_tx_corrupt_blk_conv_enable_get(
    cbrc_handle_t    *cbrc_handle_ptr,
    UINT32            link,
    BOOL             *enable_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;    


    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle_ptr, CBRC_ERR_NULL_HANDLE, 0, 0);

    result = cbrc_link_and_dir_validate(cbrc_handle_ptr, CBRC_PORT_DIR_TX, link);
    
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Check if datapath is provisioned for this feature */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_feature_validate(cbrc_handle_ptr, link, CBRC_FEATURE_MODE_FC1200_GFP);    
    }

    if (PMC_SUCCESS == result)
    {
        *enable_ptr = fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get(NULL, cbrc_handle_ptr, link);
    }


    PMC_RETURN(result);
} /* cbrc_fc1200_tx_corrupt_blk_conv_enable_get */


/*******************************************************************************
* FUNCTION: cbrc_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   cbrc_handle      - Pointer to subsystem handle
*   link             - Post-mld link (0..11)
*   non_null_ptr     - Pointer to check that shouldn't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_int_validate(cbrc_handle_t    *cbrc_handle,
                                   UINT32            link,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    
    if (link >= CBRC_MAX_LINK)
    {
        result = CBRC_ERR_INVALID_LINK;
    }

    if (NULL == non_null_ptr)
    {
        result = CBRC_ERR_NULL_PARAMETERS;
    }


    PMC_RETURN(result);
} /* cbrc_int_validate */



/*******************************************************************************
* FUNCTION: cbrc_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_int_chnl_enable(cbrc_handle_t    *cbrc_handle,
                                      UINT32            link,
                                      cbrc_int_chnl_t  *int_table_ptr,
                                      cbrc_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable)
{
    PMC_ERROR               result          = PMC_SUCCESS;
    UINT32                  link1           = link;
    UINT32                  link2           = link;
    cbrc_link_info_t       *link_ptr        = NULL;
    sdh_pmg_handle_t       *sdh_handle;
    cbr_fsgm_handle_t      *cbr_fsgm_handle;
    UINT32                  index;
    UINT32                  i;
    BOOL8                   has_stl256 = FALSE;
    UINT32                  stl256_idx;
    BOOL8                   has_sfis = FALSE;
    UINT32                  sfis_idx;
    BOOL8                   has_sfi51 = FALSE;
    UINT32                  sfi51_idx;

    PMC_ENTRY();

    result = cbrc_int_validate(cbrc_handle, link, int_en_table_ptr);
    
    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle, link, &link_ptr);
    } 

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for STL256, SFIS, SFI5.1 for link */
        result = cbrc_int_params_get(cbrc_handle,
                                     cbrc_handle->var.links[link].serdes,
                                     link,
                                     &has_stl256,
                                     &stl256_idx,
                                     &has_sfis,
                                     &sfis_idx,
                                     &has_sfi51,
                                     &sfi51_idx);
    }


#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT    UTIL_GLOBAL_INT_ENABLE
    
#ifdef CBRC_INT_RANGE
#undef CBRC_INT_RANGE
#endif
#define CBRC_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE
   
#ifdef CBRC_INT_FIELD
#undef CBRC_INT_FIELD
#endif
#define CBRC_INT_FIELD    UTIL_GLOBAL_INT_ENABLE_FIELD12
   
#ifdef CBRC_INT_FIELD_ARRAY
#undef CBRC_INT_FIELD_ARRAY
#endif
#define CBRC_INT_FIELD_ARRAY    UTIL_GLOBAL_INT_FIELD12_ARRAY_ENABLE
   
#ifdef CBRC_INT_IND
#undef CBRC_INT_IND
#endif
#define CBRC_INT_IND     UTIL_GLOBAL_INT_NOOP
   

    CBRC_INT_TABLE_DEFINE();

    CBRC_INT_TABLE_DEFINE_FW();
   
    /* SDH PMG */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_TX, link, &sdh_handle, &index);

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_enable(sdh_handle,
                                             index,
                                             &int_table_ptr->sdh_pmg_tx,
                                             &int_en_table_ptr->sdh_pmg_tx,
                                             enable);
        }
       
        if (PMC_SUCCESS == result)
        {
            result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_RX, link, &sdh_handle, &index);
        }

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_enable(sdh_handle,
                                             index,
                                             &int_table_ptr->sdh_pmg_rx,
                                             &int_en_table_ptr->sdh_pmg_rx,
                                             enable);
        }
    }

    
    /* FC800 PMG */
    if (PMC_SUCCESS == result)
    {
        result = fc800_pmg_int_chnl_enable(cbrc_handle->fc800_pmg_rx_ptr,
                                           link,
                                           &int_table_ptr->fc800_pmg_rx,
                                           &int_en_table_ptr->fc800_pmg_rx,
                                           enable);
        if (PMC_SUCCESS == result)
        {
            result = fc800_pmg_int_chnl_enable(cbrc_handle->fc800_pmg_tx_ptr,
                                               link,
                                               &int_table_ptr->fc800_pmg_tx,
                                               &int_en_table_ptr->fc800_pmg_tx,
                                               enable);
        }
    }



    /* CBR FSGM */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_RX,
                                          &cbr_fsgm_handle,
                                          &index); 

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_enable(cbr_fsgm_handle,
                                              index,
                                              &int_table_ptr->cbr_fsgm_rx,
                                              &int_en_table_ptr->cbr_fsgm_rx,
                                              enable);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_TX,
                                              &cbr_fsgm_handle,
                                              &index); 
        }

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_enable(cbr_fsgm_handle,
                                              index,
                                              &int_table_ptr->cbr_fsgm_tx,
                                              &int_en_table_ptr->cbr_fsgm_tx,
                                              enable);
        }
    }


    /* FC1200 PMON */
    if (PMC_SUCCESS == result)
    {
        result = fc1200_pmon_int_chnl_enable(cbrc_handle->fc1200_pmon_rx_ptr,
                                             link,
                                             &int_table_ptr->fc1200_pmon_rx,
                                             &int_en_table_ptr->fc1200_pmon_rx,
                                             enable);
        if (PMC_SUCCESS == result)
        {
            result = fc1200_pmon_int_chnl_enable(cbrc_handle->fc1200_pmon_tx_ptr,
                                                 link,
                                                 &int_table_ptr->fc1200_pmon_tx,
                                                 &int_en_table_ptr->fc1200_pmon_tx,
                                                 enable);
        }
    }

    /* MPMA/MPMO */
    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_enable(cbrc_handle->mpmo_ptr,
                                      link,
                                      &int_table_ptr->mpmo,
                                      &int_en_table_ptr->mpmo,
                                      enable);
    }
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_enable(cbrc_handle->mpma_ptr,
                                      link,
                                      &int_table_ptr->mpma,
                                      &int_en_table_ptr->mpma,
                                      enable);
    }


    /* SERDES */
    if (PMC_SUCCESS == result && TRUE == has_stl256)
    {
        result = cbrc_stl256_int_chnl_enable(cbrc_handle, 
                                             stl256_idx,
                                             &int_table_ptr->stl256,
                                             &int_en_table_ptr->stl256,
                                             enable);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = cbrc_sfis_int_chnl_enable(cbrc_handle, 
                                           sfis_idx,
                                           &int_table_ptr->sfis,
                                           &int_en_table_ptr->sfis,
                                           enable);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = cbrc_sfi51_int_chnl_enable(cbrc_handle, 
                                            sfi51_idx,
                                            &int_table_ptr->sfi51,
                                            &int_en_table_ptr->sfi51,
                                            enable);
    }

    int_en_table_ptr->stl256_valid = has_stl256;
    int_en_table_ptr->sfis_valid = has_sfis;
    int_en_table_ptr->sfi51_valid = has_sfi51;

    PMC_RETURN(result);
} /* cbrc_int_chnl_enable */




/*******************************************************************************
* FUNCTION: cbrc_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_int_chnl_clear(cbrc_handle_t    *cbrc_handle,
                                     UINT32            link,
                                     cbrc_int_chnl_t  *int_table_ptr,
                                     cbrc_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR               result          = PMC_SUCCESS;
    UINT32                  link1           = link;
    UINT32                  link2           = link;
    cbrc_link_info_t       *link_ptr        = NULL;
    sdh_pmg_handle_t       *sdh_handle    = NULL;
    cbr_fsgm_handle_t      *cbr_fsgm_handle;
    UINT32                  index = 0;
    UINT32                  i;
    BOOL                    clear_sdh_pmg, clear_fc1200, clear_cbr_fsgm, clear_fc800;
    BOOL8                   has_stl256;
    UINT32                  stl256_idx;
    BOOL8                   has_sfis;
    UINT32                  sfis_idx;
    BOOL8                   has_sfi51;
    UINT32                  sfi51_idx;
    shpi_indirect_IND_INT_STAT_buffer_t shpi_ind_int_stat;
    BOOL            plopi, paisi, plopci, paisci;

    PMC_ENTRY();

    result = cbrc_int_validate(cbrc_handle, link, int_en_table_ptr);
    

    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle, link, &link_ptr);
    } 

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for STL256, SFIS, SFI5.1 for link */
        result = cbrc_int_params_get(cbrc_handle,
                                     cbrc_handle->var.links[link].serdes,
                                     link,
                                     &has_stl256,
                                     &stl256_idx,
                                     &has_sfis,
                                     &sfis_idx,
                                     &has_sfi51,
                                     &sfi51_idx);
    }

    
#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CLEAR
    
#ifdef CBRC_INT_RANGE
#undef CBRC_INT_RANGE
#endif
#define CBRC_INT_RANGE UTIL_GLOBAL_INT_CLEAR_RANGE
   
#ifdef CBRC_INT_FIELD
#undef CBRC_INT_FIELD
#endif
#define CBRC_INT_FIELD    UTIL_GLOBAL_INT_CLEAR_FIELD12
   
#ifdef CBRC_INT_FIELD_ARRAY
#undef CBRC_INT_FIELD_ARRAY
#endif
#define CBRC_INT_FIELD_ARRAY    UTIL_GLOBAL_INT_FIELD12_ARRAY_CLEAR
   
#ifdef CBRC_INT_IND
#undef CBRC_INT_IND
#endif
#define CBRC_INT_IND     UTIL_GLOBAL_INT_NOOP


    CBRC_INT_TABLE_DEFINE();

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    CBRC_INT_TABLE_DEFINE_FW();
#endif
   
    if ((NULL == int_table_ptr) && (NULL != link_ptr))
    {
        clear_fc800     = cbrc_port_is_fc800(link_ptr->mode);
        clear_sdh_pmg   = cbrc_port_is_sonet(link_ptr->mode);
        clear_cbr_fsgm  = !cbrc_port_mode_is_fc1200_gfp(link_ptr->mode);
        clear_fc1200    = cbrc_port_mode_is_fc1200_gfp(link_ptr->mode);
    }
    else if (NULL != int_table_ptr)
    {
        clear_fc800     = int_table_ptr->fc800_pmg_valid;
        clear_sdh_pmg   = int_table_ptr->sdh_pmg_valid;
        clear_cbr_fsgm  = int_table_ptr->cbr_fsgm_valid;
        clear_fc1200    = int_table_ptr->fc1200_valid;
    }
    else
    {
      result = CBRC_ERR_NULL_PARAMETERS;
    }



    /* FC800 PMG */
    if ((PMC_SUCCESS == result) &&
        (TRUE == clear_fc800))
    {
        result = fc800_pmg_int_chnl_clear(cbrc_handle->fc800_pmg_rx_ptr,
                                          link,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fc800_pmg_rx),
                                          &int_en_table_ptr->fc800_pmg_rx);
        
        if (PMC_SUCCESS == result)
        {
            result = fc800_pmg_int_chnl_clear(cbrc_handle->fc800_pmg_tx_ptr,
                                              link,
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fc800_pmg_tx),
                                              &int_en_table_ptr->fc800_pmg_tx);
        }
    }
    


    /* SDH PMG */
    if ((PMC_SUCCESS == result) &&
        (TRUE == clear_sdh_pmg))
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_TX, link, &sdh_handle, &index);

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_clear(sdh_handle,
                                            index,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sdh_pmg_tx),
                                            &int_en_table_ptr->sdh_pmg_tx);
        }
       
        if (PMC_SUCCESS == result)
        {
            result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_RX, link, &sdh_handle, &index);
        }

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_clear(sdh_handle,
                                            index,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sdh_pmg_rx),
                                            &int_en_table_ptr->sdh_pmg_rx);
        }

        if (PMC_SUCCESS == result)
        {
            /* SHPI Indirects */
            UINT32 shpi_index = sdh_pmg_sber_index_get(sdh_handle, index);
            if (NULL == int_table_ptr)
            {
                plopi = TRUE;
                paisi = TRUE;
                plopci = TRUE;
                paisci = TRUE;
            }
            else
            {
                plopi = int_table_ptr->shpi_plopi;
                paisi = int_table_ptr->shpi_paisi;
                plopci = int_table_ptr->shpi_plopci;
                paisci = int_table_ptr->shpi_paisci;
            }

            if ((TRUE == plopi) || (TRUE == paisi) || (TRUE == plopci) || (TRUE == paisci))
            {
                shpi_indirect_IND_INT_STAT_buffer_init(&shpi_ind_int_stat);
                shpi_ind_int_stat.base.data[0] |= (plopi) << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF;
                shpi_ind_int_stat.base.data[0] |= (paisi) << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF;
                shpi_ind_int_stat.base.data[0] |= (plopci) << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF;
                shpi_ind_int_stat.base.data[0] |= (paisci) << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF;

                shpi_indirect_IND_INT_STAT_buffer_apply(cbrc_handle,
                                                        shpi_index,
                                                        &shpi_ind_int_stat);
            }
        }

    }

    


    /* CBR FSGM */
    if ((PMC_SUCCESS == result) &&
        (TRUE == clear_cbr_fsgm))
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_RX,
                                          &cbr_fsgm_handle,
                                          &index); 

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_clear(cbr_fsgm_handle,
                                             index,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cbr_fsgm_rx),
                                             &int_en_table_ptr->cbr_fsgm_rx);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_TX,
                                              &cbr_fsgm_handle,
                                              &index); 
        }

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_clear(cbr_fsgm_handle,
                                             index,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cbr_fsgm_tx),
                                             &int_en_table_ptr->cbr_fsgm_tx);
        }
    }


    /* FC1200 PMON */
    if ((PMC_SUCCESS == result) &&
        (TRUE == clear_fc1200))
    {
        result = fc1200_pmon_int_chnl_clear(cbrc_handle->fc1200_pmon_rx_ptr,
                                            link,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fc1200_pmon_rx),
                                            &int_en_table_ptr->fc1200_pmon_rx);
        if (PMC_SUCCESS == result)
        {
            result = fc1200_pmon_int_chnl_clear(cbrc_handle->fc1200_pmon_tx_ptr,
                                                link,
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fc1200_pmon_tx),
                                                &int_en_table_ptr->fc1200_pmon_tx);
        }
    }
    /* MPMA/MPMO */
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_clear(cbrc_handle->mpma_ptr,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpma),
                                     &int_en_table_ptr->mpma);
    }
    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_clear(cbrc_handle->mpmo_ptr,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpmo),
                                     &int_en_table_ptr->mpmo);
    }

    /* SERDES */
    if (PMC_SUCCESS == result && TRUE == has_stl256)
    {
        result = cbrc_stl256_int_chnl_clear(cbrc_handle, 
                                            stl256_idx,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, stl256),
                                            &int_en_table_ptr->stl256);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = cbrc_sfis_int_chnl_clear(cbrc_handle, 
                                          sfis_idx,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sfis),
                                          &int_en_table_ptr->sfis);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = cbrc_sfi51_int_chnl_clear(cbrc_handle, 
                                           sfi51_idx,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sfi51),
                                           &int_en_table_ptr->sfi51);
    }

    PMC_RETURN(result);
} /* cbrc_int_chnl_clear */



/*******************************************************************************
* FUNCTION: cbrc_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_int_chnl_retrieve(cbrc_handle_t    *cbrc_handle,
                                        UINT32            link,
                                        cbrc_int_chnl_t  *filt_table_ptr,
                                        cbrc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR               result          = PMC_SUCCESS;
    UINT32                  link1           = link;
    UINT32                  link2           = link;
    cbrc_link_info_t       *link_ptr        = NULL;
    sdh_pmg_handle_t       *sdh_handle    = NULL;
    cbr_fsgm_handle_t      *cbr_fsgm_handle;
    UINT32                  index     = 0;
    UINT32                  i;
    BOOL8                   has_stl256;
    UINT32                  stl256_idx;
    BOOL8                   has_sfis;
    UINT32                  sfis_idx;
    BOOL8                   has_sfi51;
    UINT32                  sfi51_idx;
    shpi_indirect_IND_INT_STAT_buffer_t shpi_ind_int_stat;
    shpi_indirect_IND_INT_VAL_buffer_t  shpi_ind_int_val;

    PMC_ENTRY();

    result = cbrc_int_validate(cbrc_handle, link, int_table_ptr);


    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle, link, &link_ptr);
    }

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for STL256, SFIS, SFI5.1 for link */
        result = cbrc_int_params_get(cbrc_handle,
                                     link_ptr->serdes,
                                     link,
                                     &has_stl256,
                                     &stl256_idx,
                                     &has_sfis,
                                     &sfis_idx,
                                     &has_sfi51,
                                     &sfi51_idx);
    }


    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(cbrc_handle, cbr_dint, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(cbrc_handle, pkt_dint, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(cbrc_handle, pkt_int, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, fc1200_pcs_rx, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, fc1200_pcs_tx, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, fc_tttd, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, fc_tttm, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, prgm, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(cbrc_handle, mldprbs_pcbi, UTIL_GLOBAL_INT_OPTIMIZE);

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_RETRIEVE
    
#ifdef CBRC_INT_RANGE
#undef CBRC_INT_RANGE
#endif
#define CBRC_INT_RANGE UTIL_GLOBAL_INT_RETRIEVE_RANGE
   
#ifdef CBRC_INT_FIELD
#undef CBRC_INT_FIELD
#endif
#define CBRC_INT_FIELD    UTIL_GLOBAL_INT_RETRIEVE_FIELD12
   
#ifdef CBRC_INT_FIELD_ARRAY
#undef CBRC_INT_FIELD_ARRAY
#endif
#define CBRC_INT_FIELD_ARRAY    UTIL_GLOBAL_INT_FIELD12_ARRAY_RETRIEVE
   
#ifdef CBRC_INT_IND
#undef CBRC_INT_IND
#endif
#define CBRC_INT_IND     UTIL_GLOBAL_INT_NOOP


    CBRC_INT_TABLE_DEFINE();

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    CBRC_INT_TABLE_DEFINE_FW();
#endif
   
#ifdef CBRC_STATUS
#undef CBRC_STATUS
#endif
#define CBRC_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    
#ifdef CBRC_STATUS_RANGE
#undef CBRC_STATUS_RANGE
#endif
#define CBRC_STATUS_RANGE UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
#ifdef CBRC_STATUS_FIELD
#undef CBRC_STATUS_FIELD
#endif
#define CBRC_STATUS_FIELD    UTIL_GLOBAL_STATUS_RETRIEVE_FIELD12
   
#ifdef CBRC_STATUS_FIELD_ARRAY
#undef CBRC_STATUS_FIELD_ARRAY
#endif
#define CBRC_STATUS_FIELD_ARRAY    UTIL_GLOBAL_STATUS_FIELD12_ARRAY_RETRIEVE
   
#ifdef CBRC_STATUS_IND
#undef CBRC_STATUS_IND
#endif
#define CBRC_STATUS_IND     UTIL_GLOBAL_INT_NOOP


    CBRC_STATUS_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, cbr_dint);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, pkt_dint);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, pkt_int);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, fc1200_pcs_rx);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, fc1200_pcs_tx);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, fc_tttd);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, fc_tttm);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, prgm);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, mldprbs_pcbi);
    
    
    
    /* FC800 PMG */
    if ((PMC_SUCCESS == result) &&
        (cbrc_port_is_fc800(link_ptr->mode)
#ifdef CBRC_SONET_LOS_V_WORKAROUND
        || cbrc_port_is_sonet(link_ptr->mode)
#endif
        ))
    {
        int_table_ptr->fc800_pmg_valid = TRUE;

        result = fc800_pmg_int_chnl_retrieve(cbrc_handle->fc800_pmg_rx_ptr,
                                             link,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fc800_pmg_rx),
                                             &int_table_ptr->fc800_pmg_rx);
        
        if (PMC_SUCCESS == result)
        {
            result = fc800_pmg_int_chnl_retrieve(cbrc_handle->fc800_pmg_tx_ptr,
                                                 link,
                                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fc800_pmg_tx),
                                                 &int_table_ptr->fc800_pmg_tx);
        }
    }
    


    /* SDH PMG */
    if ((PMC_SUCCESS == result) &&
        (cbrc_port_is_sonet(link_ptr->mode)))
    {
        if (cbrc_handle->var.links[link].tx_active) {
            result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_TX, link, &sdh_handle, &index);

            if (PMC_SUCCESS == result) 
            {
                result = sdh_pmg_int_chnl_retrieve(sdh_handle,
                                                   index,
                                                   UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sdh_pmg_tx),
                                                   &int_table_ptr->sdh_pmg_tx);
                if (PMC_SUCCESS != result) PMC_RETURN(result);
            }
            if (mpmo_ch_enable_get(cbrc_handle->mpmo_ptr, link)) {
                int_table_ptr->sdh_pmg_valid_tx = TRUE;
            }
            int_table_ptr->sdh_pmg_valid = TRUE;
        }
       
        if (cbrc_handle->var.links[link].rx_active) {
            result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_RX, link, &sdh_handle, &index);

            if (PMC_SUCCESS == result) 
            {
                result = sdh_pmg_int_chnl_retrieve(sdh_handle,
                                                   index,
                                                   UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sdh_pmg_rx),
                                                   &int_table_ptr->sdh_pmg_rx);
                if (PMC_SUCCESS != result) PMC_RETURN(result);
#ifdef CBRC_SONET_LOS_V_WORKAROUND
                int_table_ptr->sdh_pmg_rx.los_i = int_table_ptr->fc800_pmg_rx.rx_asd_i;
                int_table_ptr->sdh_pmg_rx.los_v = int_table_ptr->fc800_pmg_rx.rx_asd_v;
#endif
            }
            if (mpma_ch_enable_get(cbrc_handle->mpma_ptr, link)) {
                int_table_ptr->sdh_pmg_valid_rx = TRUE;
            }
            int_table_ptr->sdh_pmg_valid = TRUE;
            {
                /* SHPI Indirects */
                UINT32 shpi_index = sdh_pmg_sber_index_get(sdh_handle, index);    
                shpi_indirect_IND_INT_STAT_buffer_init(&shpi_ind_int_stat);
    
                shpi_indirect_IND_INT_STAT_buffer_retrieve(cbrc_handle,
                                                           shpi_index,
                                                           &shpi_ind_int_stat);
   
                int_table_ptr->shpi_paisci = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF;
                int_table_ptr->shpi_plopci = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF;
                int_table_ptr->shpi_paisi = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF;
                int_table_ptr->shpi_plopi = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF;


                shpi_indirect_IND_INT_VAL_buffer_init(&shpi_ind_int_val);
    
                shpi_indirect_IND_INT_VAL_buffer_retrieve(cbrc_handle,
                                                          shpi_index,
                                                          &shpi_ind_int_val);
   
                int_table_ptr->shpi_paiscv = (shpi_ind_int_val.base.data[0] & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_OFF;
                int_table_ptr->shpi_plopcv = (shpi_ind_int_val.base.data[0] & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_OFF;
                int_table_ptr->shpi_paisv = (shpi_ind_int_val.base.data[0] & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_OFF;
                int_table_ptr->shpi_plopv = (shpi_ind_int_val.base.data[0] & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_MSK) 
                    >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_OFF;

            }
        }
    }

    


    /* CBR FSGM */
    if ((PMC_SUCCESS == result) &&
        (!cbrc_port_mode_is_fc1200_gfp(link_ptr->mode)))
    {
        int_table_ptr->cbr_fsgm_valid = TRUE;

        result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_RX,
                                          &cbr_fsgm_handle,
                                          &index); 

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_retrieve(cbr_fsgm_handle,
                                                index,
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cbr_fsgm_rx),
                                                &int_table_ptr->cbr_fsgm_rx);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_TX,
                                              &cbr_fsgm_handle,
                                              &index); 
        }

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_retrieve(cbr_fsgm_handle,
                                                index,
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cbr_fsgm_tx),
                                                &int_table_ptr->cbr_fsgm_tx);
        }
    }


    /* FC1200 PMON */
    if ((PMC_SUCCESS == result) &&
        (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode)))
    {
        int_table_ptr->fc1200_valid = TRUE;

        result = fc1200_pmon_int_chnl_retrieve(cbrc_handle->fc1200_pmon_rx_ptr,
                                               link,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fc1200_pmon_rx),
                                               &int_table_ptr->fc1200_pmon_rx);
        if (PMC_SUCCESS == result)
        {
            result = fc1200_pmon_int_chnl_retrieve(cbrc_handle->fc1200_pmon_tx_ptr,
                                                   link,
                                                   UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fc1200_pmon_tx),
                                                   &int_table_ptr->fc1200_pmon_tx);
        }
    }

    /* MPMA/MPMO */
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_retrieve(cbrc_handle->mpma_ptr,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpma),
                                        &int_table_ptr->mpma);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_retrieve(cbrc_handle->mpmo_ptr,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpmo),
                                        &int_table_ptr->mpmo);
    }

    /* SERDES */
    if (PMC_SUCCESS == result && TRUE == has_stl256)
    {
        result = cbrc_stl256_int_chnl_retrieve(cbrc_handle, 
                                               stl256_idx,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, stl256),
                                               &int_table_ptr->stl256);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = cbrc_sfis_int_chnl_retrieve(cbrc_handle, 
                                             sfis_idx,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sfis),
                                             &int_table_ptr->sfis);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = cbrc_sfi51_int_chnl_retrieve(cbrc_handle, 
                                              sfi51_idx,
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sfi51),
                                              &int_table_ptr->sfi51);
    }

    if (PMC_SUCCESS == result)
    {
        int_table_ptr->stl256_valid = has_stl256;
        int_table_ptr->sfis_valid = has_sfis;
        int_table_ptr->sfi51_valid = has_sfi51;
    }

    PMC_RETURN(result);
} /* cbrc_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: cbrc_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Link
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_int_chnl_enabled_check(cbrc_handle_t     *cbrc_handle,
                                             UINT32             link,
                                             cbrc_int_chnl_t   *int_en_table_ptr,
                                             BOOL              *int_found_ptr)
{
    PMC_ERROR               result          = PMC_SUCCESS;
    UINT32                  link1           = link;
    UINT32                  link2           = link;
    cbrc_link_info_t       *link_ptr        = NULL;
    sdh_pmg_handle_t       *sdh_handle    = NULL;
    cbr_fsgm_handle_t      *cbr_fsgm_handle;
    UINT32                  index     = 0;
    UINT32                  i;
    BOOL8                   int_found8 = FALSE;
    BOOL8                   has_stl256;
    UINT32                  stl256_idx;
    BOOL8                   has_sfis;
    UINT32                  sfis_idx;
    BOOL8                   has_sfi51;
    UINT32                  sfi51_idx;
    shpi_indirect_IND_INT_STAT_buffer_t shpi_ind_int_stat;

    PMC_ENTRY();

    result = cbrc_int_validate(cbrc_handle, link, int_found_ptr);



    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle, link, &link_ptr);
    } 

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for STL256, SFIS, SFI5.1 for link */
        result = cbrc_int_params_get(cbrc_handle,
                                     cbrc_handle->var.links[link].serdes,
                                     link,
                                     &has_stl256,
                                     &stl256_idx,
                                     &has_sfis,
                                     &sfis_idx,
                                     &has_sfi51,
                                     &sfi51_idx);
    }


#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CHECK
    
#ifdef CBRC_INT_RANGE
#undef CBRC_INT_RANGE
#endif
#define CBRC_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
   
#ifdef CBRC_INT_FIELD
#undef CBRC_INT_FIELD
#endif
#define CBRC_INT_FIELD    UTIL_GLOBAL_INT_CHECK_FIELD12
   
#ifdef CBRC_INT_FIELD_ARRAY
#undef CBRC_INT_FIELD_ARRAY
#endif
#define CBRC_INT_FIELD_ARRAY    UTIL_GLOBAL_INT_FIELD12_ARRAY_CHECK
   
#ifdef CBRC_INT_IND
#undef CBRC_INT_IND
#endif
#define CBRC_INT_IND     UTIL_GLOBAL_INT_NOOP


    CBRC_INT_TABLE_DEFINE();

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    CBRC_INT_TABLE_DEFINE_FW();
#endif
   
    /* FC800 PMG */
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr) &&
        (cbrc_port_is_fc800(link_ptr->mode)))
    {
        result = fc800_pmg_int_chnl_enabled_check(cbrc_handle->fc800_pmg_rx_ptr,
                                                  link,
                                                  &int_en_table_ptr->fc800_pmg_rx,
                                                  int_found_ptr);
        
        if ((PMC_SUCCESS == result) &&
            (FALSE == *int_found_ptr))
        {
            result = fc800_pmg_int_chnl_enabled_check(cbrc_handle->fc800_pmg_tx_ptr,
                                                      link,
                                                      &int_en_table_ptr->fc800_pmg_tx,
                                                      int_found_ptr);
        }
    }
    


    /* SDH PMG */
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr) &&
        (cbrc_port_is_sonet(link_ptr->mode)))
    {
        result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_TX, link, &sdh_handle, &index);

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_enabled_check(sdh_handle,
                                                    index,
                                                    &int_en_table_ptr->sdh_pmg_tx,
                                                    int_found_ptr);
        }
       
        if (PMC_SUCCESS == result)
        {
            result = cbrc_sdh_pmg_get_handle(cbrc_handle, CBRC_PORT_DIR_RX, link, &sdh_handle, &index);
        }

        if (PMC_SUCCESS == result) 
        {
            result = sdh_pmg_int_chnl_enabled_check(sdh_handle,
                                                    index,
                                                    &int_en_table_ptr->sdh_pmg_rx,
                                                    int_found_ptr);
        }
        if (PMC_SUCCESS == result)
        {
            /* SHPI Indirects */
            UINT32 shpi_index = sdh_pmg_sber_index_get(sdh_handle, index);    
            if (FALSE == *int_found_ptr)
            {
                shpi_indirect_IND_INT_STAT_buffer_init(&shpi_ind_int_stat);

                shpi_indirect_IND_INT_STAT_buffer_retrieve(cbrc_handle,
                                                           shpi_index,
                                                           &shpi_ind_int_stat);

                if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->shpi_paisci))
                {
                    *int_found_ptr = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK) 
                        >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF;
                }
                if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->shpi_plopci))
                {
                    *int_found_ptr = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK) 
                        >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF;
                }
                if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->shpi_paisi))
                {
                    *int_found_ptr = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK) 
                        >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF;
                }
                if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->shpi_plopi))
                {
                    *int_found_ptr = (shpi_ind_int_stat.base.data[0] & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK) 
                        >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF;
                }
            }
        }

    }


    /* CBR FSGM */
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr) &&
        (!cbrc_port_mode_is_fc1200_gfp(link_ptr->mode)))
    {
        result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_RX,
                                          &cbr_fsgm_handle,
                                          &index); 

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_enabled_check(cbr_fsgm_handle,
                                                     index,
                                                     &int_en_table_ptr->cbr_fsgm_rx,
                                                     int_found_ptr);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = cbrc_get_cbr_fsgm_handle(cbrc_handle, link, CBRC_PORT_DIR_TX,
                                              &cbr_fsgm_handle,
                                              &index); 
        }

        if (PMC_SUCCESS == result)
        {
            result = cbr_fsgm_int_chnl_enabled_check(cbr_fsgm_handle,
                                                     index,
                                                     &int_en_table_ptr->cbr_fsgm_tx,
                                                     int_found_ptr);
        }
    }


    /* FC1200 PMON */
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr) &&
        (cbrc_port_mode_is_fc1200_gfp(link_ptr->mode)))
    {
        result = fc1200_pmon_int_chnl_enabled_check(cbrc_handle->fc1200_pmon_rx_ptr,
                                                    link,
                                                    &int_en_table_ptr->fc1200_pmon_rx,
                                                    int_found_ptr);

        if (PMC_SUCCESS == result)
        {
            result = fc1200_pmon_int_chnl_enabled_check(cbrc_handle->fc1200_pmon_tx_ptr,
                                                        link,
                                                        &int_en_table_ptr->fc1200_pmon_tx,
                                                        int_found_ptr);
        }
    }

    /* MPMA/MPMO */
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = mpma_int_chnl_enabled_check(cbrc_handle->mpma_ptr,
                                             link,
                                             &int_en_table_ptr->mpma,
                                             &int_found8);
        *int_found_ptr = (BOOL) int_found8;
    }
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = mpmo_int_chnl_enabled_check(cbrc_handle->mpmo_ptr,
                                             link,
                                             &int_en_table_ptr->mpmo,
                                             &int_found8   );
        *int_found_ptr = (BOOL) int_found8;
    }

    /* SERDES */
    if ((PMC_SUCCESS == result && TRUE == has_stl256) &&
        (FALSE == *int_found_ptr))
    {
        result = cbrc_stl256_int_chnl_enabled_check(cbrc_handle, 
                                                    stl256_idx,
                                                    &int_en_table_ptr->stl256,
                                                    int_found_ptr);
    }

    if ((PMC_SUCCESS == result && TRUE == has_sfis) &&
        (FALSE == *int_found_ptr))
    {
        result = cbrc_sfis_int_chnl_enabled_check(cbrc_handle, 
                                                  sfis_idx,
                                                  &int_en_table_ptr->sfis,
                                                  int_found_ptr);
    }

    if ((PMC_SUCCESS == result && TRUE == has_sfi51) &&
        (FALSE == *int_found_ptr))
    {
        result = cbrc_sfi51_int_chnl_enabled_check(cbrc_handle, 
                                                   sfi51_idx,
                                                   &int_en_table_ptr->sfi51,
                                                   int_found_ptr);
    }

    PMC_RETURN(result);
} /* cbrc_int_chnl_enabled_check */


/*******************************************************************************
* cbrc_int_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to retrieve which interrupt structures
*   are associated with the current link.
* INPUTS:
*   cbrc_handle            - Pointer to subsystem handle
*
*   serdes_type            - Enumerated Serdes type:
*                            CBRC_SERDES_TYPE_NONE = 0,
*                            CBRC_SERDES_TYPE_10G_XFI,
*                            CBRC_SERDES_TYPE_40G_STL,
*                            CBRC_SERDES_TYPE_40G_SFIS,
*                            CBRC_SERDES_TYPE_40G_SFI51,
*
*   link                   - Link [0..11]
*
* OUTPUTS:
*   has_stl256             - TRUE: STL256 interrupts are valid at index
*                            stl256_idx and should be retrieved
*
*   stl256_idx             - Index for the STL256 interrupts if
*                            has_stl256=TRUE [0-3]
*
*   has_sfis               - TRUE: SFIS interrupts are valid for index
*                            sfis_idx and should be retrieved
*
*   sfis_idx               - Index for the SFIS interrupts if
*                            has_sfis=TRUE [0-1]
*
*   has_sfi51              - TRUE: SFI51 interrupts are valid for index
*                            sfi51_idx and should be retrieved
*
*   sfi51_idx              - Index for the SFI51 interrupts if
*                            has_sfi51=TRUE [0-1]
*
* RETURNS:
*   PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_int_params_get(cbrc_handle_t        *cbrc_handle,
                                      cbrc_serdes_type_t    serdes_type,
                                      UINT32                link,
                                      BOOL8                *has_stl256,
                                      UINT32               *stl256_idx,
                                      BOOL8                *has_sfis,
                                      UINT32               *sfis_idx,
                                      BOOL8                *has_sfi51,
                                      UINT32               *sfi51_idx)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_type < LAST_CBRC_SERDES_TYPE, CBRC_ERR_INVALID_ARG, serdes_type, LAST_CBRC_SERDES_TYPE);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, link, CBRC_MAX_LINK);

    /* Initialize returned values */
    *has_stl256 = FALSE;
    *stl256_idx = 0;
    *has_sfis = FALSE;
    *sfis_idx = 0;
    *has_sfi51 = FALSE;
    *sfi51_idx = 0;

    switch (serdes_type)
    {
    case CBRC_SERDES_TYPE_10G_XFI:
        break;

    case CBRC_SERDES_TYPE_40G_STL:
        *has_stl256 = TRUE;

        if (0 == link)
        {
            *stl256_idx = 0;
        }
        else if (4 == link)
        {
            *stl256_idx = 1;
        }
        else if (8 == link)
        {
            *stl256_idx = 2;
        }
        else
        {
            result = CBRC_ERR_INVALID_LINK;
        }
        break;

    case CBRC_SERDES_TYPE_40G_SFIS:
        *has_sfis = TRUE;
        if (0 == link)
        {
            *sfis_idx = 0;
        }
        else
        {
            *sfis_idx = 1;
        }
        break;

    case CBRC_SERDES_TYPE_40G_SFI51:
        *has_sfi51 = TRUE;
        if (0 == link)
        {
            *sfi51_idx = 0;
        }
        else
        {
            *sfi51_idx = 1;
        }
        break;

    default:
        break;
    } /* end switch */
    

    PMC_RETURN(result);
}/* cbrc_int_params_get */


/*******************************************************************************
* FUNCTION: cbrc_stl256_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   stl256_link       - STL 254 link [0-2]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_stl256_int_chnl_enable(cbrc_handle_t          *cbrc_handle,
                                              UINT32                  stl256_link,
                                              cbrc_stl256_int_chnl_t *int_table_ptr,
                                              cbrc_stl256_int_chnl_t *int_en_table_ptr,
                                              BOOL                    enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (stl256_link >= 3)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = stl256_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_ENABLE
        
    CBRC_STL256_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_stl256_int_chnl_enable */



/*******************************************************************************
* FUNCTION: cbrc_sfis_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfis_link         - SFIS link [0-1]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfis_int_chnl_enable(cbrc_handle_t        *cbrc_handle,
                                            UINT32                sfis_link,
                                            cbrc_sfis_int_chnl_t *int_table_ptr,
                                            cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                            BOOL                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (sfis_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfis_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_ENABLE
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_ENABLE
        
    CBRC_SFIS_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfis_int_chnl_enable */


/*******************************************************************************
* FUNCTION: cbrc_sfi51_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfi51_link        - SFI 5.1 link [0-1]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_enable(cbrc_handle_t         *cbrc_handle,
                                             UINT32                 sfi51_link,
                                             cbrc_sfi51_int_chnl_t *int_table_ptr,
                                             cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                             BOOL                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfi51_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_ENABLE
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_ENABLE
        
    CBRC_SFI51_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfi51_int_chnl_enable */



/*******************************************************************************
* FUNCTION: cbrc_stl256_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   stl256_link       - STL 254 link [0-2]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_stl256_int_chnl_clear(cbrc_handle_t          *cbrc_handle,
                                             UINT32                  stl256_link,
                                             cbrc_stl256_int_chnl_t *int_table_ptr,
                                             cbrc_stl256_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (stl256_link >= 3)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = stl256_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CLEAR
        
    CBRC_STL256_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_stl256_int_chnl_clear */



/*******************************************************************************
* FUNCTION: cbrc_sfis_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfis_link         - SFIS link [0-1]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfis_int_chnl_clear(cbrc_handle_t        *cbrc_handle,
                                           UINT32                sfis_link,
                                           cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                           cbrc_sfis_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (sfis_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfis_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CLEAR
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_CLEAR
        
    CBRC_SFIS_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfis_int_chnl_clear */



/*******************************************************************************
* FUNCTION: cbrc_sfi51_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfi51_link        - SFI 5.1 link [0-1]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_clear(cbrc_handle_t         *cbrc_handle,
                                            UINT32                 sfi51_link,
                                            cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                            cbrc_sfi51_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfi51_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CLEAR
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_CLEAR
        
    CBRC_SFI51_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfi51_int_chnl_clear */



/*******************************************************************************
* FUNCTION: cbrc_stl256_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   stl256_link       - STL 254 link [0-2]
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_stl256_int_chnl_retrieve(cbrc_handle_t           *cbrc_handle,
                                                UINT32                   stl256_link,
                                                cbrc_stl256_int_chnl_t  *filt_table_ptr,
                                                cbrc_stl256_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               link;
    PMC_ENTRY();

    if (stl256_link >= 3)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = stl256_link;

    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, stl256,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_RETRIEVE
        
    CBRC_STL256_INT_TABLE_DEFINE();


    /* Status */
    
#ifdef CBRC_STATUS
#undef CBRC_STATUS
#endif
#define CBRC_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
    CBRC_STL256_STATUS_TABLE_DEFINE();
   
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, stl256);
    
    PMC_RETURN(result);
} /* cbrc_stl256_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: cbrc_sfis_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfis_link         - SFIS link [0-1]
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfis_int_chnl_retrieve(cbrc_handle_t         *cbrc_handle,
                                              UINT32                 sfis_link,
                                              cbrc_sfis_int_chnl_t  *filt_table_ptr,
                                              cbrc_sfis_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               link;
    PMC_ENTRY();

    if (sfis_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfis_link;

    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, cbrc_sfis_rx,  UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, cbrc_sfis_tx,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_RETRIEVE
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_RETRIEVE
        
    CBRC_SFIS_INT_TABLE_DEFINE();


    /* Status */
    
#ifdef CBRC_STATUS
#undef CBRC_STATUS
#endif
#define CBRC_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
#ifdef CBRC_STATUS_ARRAY
#undef CBRC_STATUS_ARRAY
#endif
#define CBRC_STATUS_ARRAY UTIL_GLOBAL_STATUS_RETRIEVE
        
    CBRC_SFIS_STATUS_TABLE_DEFINE();
   
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, cbrc_sfis_rx);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, cbrc_sfis_tx);
    
    PMC_RETURN(result);
} /* cbrc_sfis_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: cbrc_sfi51_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfi51_link        - SFI 5.1 link [0-1]
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_retrieve(cbrc_handle_t          *cbrc_handle,
                                               UINT32                  sfi51_link,
                                               cbrc_sfi51_int_chnl_t  *filt_table_ptr,
                                               cbrc_sfi51_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               link;
    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfi51_link;

    UTIL_GLOBAL_INT_BUFFER_INIT(cbrc_handle, cbrc_sfi51,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_RETRIEVE
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_RETRIEVE
        
    CBRC_SFI51_INT_TABLE_DEFINE();


    /* Status */
    
#ifdef CBRC_STATUS
#undef CBRC_STATUS
#endif
#define CBRC_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
#ifdef CBRC_STATUS_ARRAY
#undef CBRC_STATUS_ARRAY
#endif
#define CBRC_STATUS_ARRAY UTIL_GLOBAL_STATUS_RETRIEVE
        
    CBRC_SFI51_STATUS_TABLE_DEFINE();
   
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbrc_handle, cbrc_sfi51);
    
    PMC_RETURN(result);
} /* cbrc_sfi51_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: cbrc_stl256_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   stl256_link       - STL 254 link [0-2]
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_stl256_int_chnl_enabled_check(cbrc_handle_t          *cbrc_handle,
                                                     UINT32                  stl256_link,
                                                     cbrc_stl256_int_chnl_t *int_en_table_ptr,
                                                     BOOL                   *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;

    PMC_ENTRY();

    if (stl256_link >= 3)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = stl256_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CHECK
        
    CBRC_STL256_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_stl256_int_chnl_enabled_check */



/*******************************************************************************
* FUNCTION: cbrc_sfis_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfis_link         - SFIS link [0-1]
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfis_int_chnl_enabled_check(cbrc_handle_t        *cbrc_handle,
                                                   UINT32                sfis_link,
                                                   cbrc_sfis_int_chnl_t *int_en_table_ptr,
                                                   BOOL                 *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;

    PMC_ENTRY();

    if (sfis_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfis_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CHECK
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_CHECK
        
    CBRC_SFIS_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfis_int_chnl_enabled_check */



/*******************************************************************************
* FUNCTION: cbrc_sfi51_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   sfi51_link        - SFI 5.1 link [0-1]
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfi51_int_chnl_enabled_check(cbrc_handle_t        *cbrc_handle,
                                                    UINT32                sfi51_link,
                                                    cbrc_sfi51_int_chnl_t *int_en_table_ptr,
                                                    BOOL                  *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(CBRC_ERR_LINK_NOT_AVAIL);
    }

    link = sfi51_link;

#ifdef CBRC_INT
#undef CBRC_INT
#endif
#define CBRC_INT UTIL_GLOBAL_INT_CHECK
        
#ifdef CBRC_INT_ARRAY
#undef CBRC_INT_ARRAY
#endif
#define CBRC_INT_ARRAY UTIL_GLOBAL_INT_CHECK
        
    CBRC_SFI51_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cbrc_sfi51_int_chnl_enabled_check */



/*******************************************************************************
* FUNCTION: cbrc_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   cbrc_handle     - Pointer to cbrc handle
*   enable          - Enables/disables interrupt and status support.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_int_init(cbrc_handle_t    *cbrc_handle,
                                BOOL8             enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    prbs_mg_field_SHPI_U_INT_EN_set(NULL, cbrc_handle, 0xF * enable);
    prbs_mg_field_PRGM_INT_EN_set(NULL, cbrc_handle, 0xF * enable);

    for (i = 0; i < LAST_CBRC_PORT_DIR; i++)
    {

        /* cbrc_sdhpmg_irq[1] and [2], eventually towards PCIe and device */
        cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set(NULL, cbrc_handle, i, enable);
        cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set(NULL, cbrc_handle, i, enable);
        cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set(NULL, cbrc_handle, i, enable);

        cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set(NULL, cbrc_handle, i, enable);
        cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set(NULL, cbrc_handle, i, enable);
        cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set(NULL, cbrc_handle, i, enable);
    } 


    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_write(NULL, cbrc_handle, 0xFFFFFFFF * enable);
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_write(NULL, cbrc_handle, 0xFFFFFFFF * enable);

    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_write(NULL, cbrc_handle, 0xFFFFFFFF * enable); 
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_write(NULL, cbrc_handle, 0xFFFFFFFF * enable); 
    
    cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG1_write(NULL, cbrc_handle, 0xFFFFFFFF * enable); 
    cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG2_write(NULL, cbrc_handle, 0xFFFFFFFF * enable); 

    prbs_mg_reg_MTSB_INT_EN_write(NULL, cbrc_handle, 0xFFFFFFFF * enable); 

    PMC_RETURN(result);
} /* cbrc_int_init */





/*******************************************************************************
* FUNCTION: cbrc_sdh_prbs_mon_err_cnt_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves the prbs_monitor errors counter value.
*
* INPUTS:
*   handle             - Pointer to subsystem handle
*   link               - Post-mld link to activate (0..11)
*
* OUTPUTS:
*    *err_cnt          - Errors counter value.
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sdh_prbs_err_cnt_get(cbrc_handle_t         *handle,
                                            UINT8                  link,
                                            UINT32                *err_cnt)
{
    config_and_status_MON_ERR_CNT_buffer_t err_cnt_buf[1];
    cbrc_link_info_t   *link_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 num_poll_fails;
    UINT32 i;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != err_cnt, CBRC_ERR_NULL_PARAMETERS, 0, 0);

    result = cbrc_link_and_dir_validate(handle, CBRC_PORT_DIR_RX, link);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = cbrc_link_get(handle, link, &link_ptr);

    
    /* Check if requested link is provisioned */
    if (PMC_SUCCESS == result)
    {
        if (!cbrc_port_is_prov(handle, link_ptr))
        {
            result = CBRC_ERR_PORT_NOT_PROV;
        }
    }

    *err_cnt = 0;
    if (PMC_SUCCESS == result)
    {
        /* Trigger ERR CNT register read:
           A write in this register will trigger the transfer of the
           error counters to holding registers where they can be read. 
        */
        for (i = 0; i < 16; i ++)
        {
            prgm_field_TIP_set(NULL, handle, i, 0);

            /* disable the firmwar equeue since we need to look at the poll return status */
            l5sys_firmware_queue_disable(handle);
            
            /* write in TIP and pool until errors counter register is updated */
            /* or number of polls exceeded */
            if (0 != prgm_field_TIP_poll(NULL, handle, i, 0, PMC_POLL_EQUAL,
                                         CBRC_PRGM_MG_TIP_POLL_MAX_CNT,
                                         &num_poll_fails, CBRC_PRGM_MG_TIP_POLL_DELAY)
                || num_poll_fails >= CBRC_PRGM_MG_TIP_POLL_MAX_CNT)
            {
                result = CBRC_ERR_PRBS_ERR_CNT_READ_FAIL;
            }
            else
            {
                result = PMC_SUCCESS;
            }
            /* re-enable the firmware queue */
            l5sys_firmware_queue_enable(handle);
            
            if (PMC_SUCCESS == result) 
            {
                config_and_status_MON_ERR_CNT_buffer_init(err_cnt_buf);
                
                /* retrieve err count register value */
                result = config_and_status_MON_ERR_CNT_buffer_retrieve(handle, (UINT32)i, 1, err_cnt_buf);
                
                if (PMC_SUCCESS == result)
                {
                    *err_cnt = config_and_status_ifield_ERR_CNT_get(err_cnt_buf);
                }     
                if (*err_cnt != 0)
                {
                    break;
                }
            }
        }
    }

    PMC_RETURN(result);
}/* cbrc_sdh_prbs_mon_err_cnt_get */


/*******************************************************************************
* FUNCTION: cbrc_feature_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Helper function to check if a the link is provisioned for the correct
*   datapath corresponding to the feature.
*
* INPUTS:
*   cbrc_handle     - Pointer to cbrc handle
*   link            - First post-MLD link to be part of the logical port (0..11)
*   mode            - mode required by feature to check if configured on this link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_feature_validate(cbrc_handle_t       *cbrc_handle,
                                       UINT32               link,
                                       cbrc_feature_mode_t  mode)
{
    PMC_ERROR           result = PMC_SUCCESS;
    cbrc_link_info_t   *link_ptr = NULL;
    cbrc_port_mode_t    prov_mode;

    PMC_ENTRY();
    
    PMC_ASSERT(mode < LAST_CBRC_FEATURE_MODE, CBRC_ERR_CODE_ASSERT, mode, LAST_CBRC_FEATURE_MODE);

    /* Retieve pointer to requested link element */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_link_get(cbrc_handle, link, &link_ptr);
    }


    /* Check if port is provisioned */
    if ((PMC_SUCCESS == result) &&
        (!cbrc_port_is_prov(cbrc_handle, link_ptr)))
    {
        result = CBRC_ERR_PORT_NOT_PROV;
    }


    /* Get provisioned mode */
    prov_mode = link_ptr->mode;


    /* Check against desired feature */ 
    switch (mode)
    {
    case CBRC_FEATURE_MODE_SDH:
        if ((CBRC_PORT_MODE_40G_STS768 != prov_mode) &&
            (CBRC_PORT_MODE_40G_STM256 != prov_mode) &&
            (CBRC_PORT_MODE_10G_STS192 != prov_mode) &&
            (CBRC_PORT_MODE_10G_STM64  != prov_mode))
        {
            result = CBRC_ERR_FEATURE_REQ_SDH;
        }
        break;

    case CBRC_FEATURE_MODE_CBR:
        if (CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E == prov_mode)
        {
            result = CBRC_ERR_FEATURE_REQ_CBR;
        }
        break;

    case CBRC_FEATURE_MODE_CPRI:
        if ((CBRC_PORT_MODE_10G_CPRI_5G  != prov_mode) &&
            (CBRC_PORT_MODE_10G_CPRI_6G  != prov_mode) &&
            (CBRC_PORT_MODE_10G_CPRI_10G != prov_mode))
        {
            result = CBRC_ERR_FEATURE_REQ_CPRI;
        }
        break;

    case CBRC_FEATURE_MODE_GDPS:
        if ((CBRC_PORT_MODE_10G_GDPS_5G  != prov_mode) &&
            (CBRC_PORT_MODE_10G_GDPS_10G != prov_mode))
        {
            result = CBRC_ERR_FEATURE_REQ_GDPS;
        }
        break;

    case CBRC_FEATURE_MODE_FC800:
        if (CBRC_PORT_MODE_10G_FC800 != prov_mode)
        {
            result = CBRC_ERR_FEATURE_REQ_FC800;
        }
        break;

    case CBRC_FEATURE_MODE_FC1200:
        if ((CBRC_PORT_MODE_10G_FC1200_ODU1F != prov_mode) &&
            (CBRC_PORT_MODE_10G_FC1200_ODU2F != prov_mode))
        {
            result = CBRC_ERR_FEATURE_REQ_FC1200;
        }
        break;

    case CBRC_FEATURE_MODE_FC1200_GFP:
        if (CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E != prov_mode)
        {
            result = CBRC_ERR_FEATURE_REQ_FC1200_GFP;
        }
        break;
        
    default:
        break;
    } /* end switch */
    

    PMC_RETURN(result);
} /* cbrc_feature_validate */


/*******************************************************************************
* cbrc_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a cbrc_recover_var_t context and energy state from register space.
*
*  This function is used by cbrc_handle_restart_init. It also recovers 
*  volatile information from cbrc_var_t context.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*  *cbrc_handle            - pointer to CBRC handle instance to be operated on
*   top_energy_state_reg      - energy state from top-level
*   
*
* OUTPUTS:
*   *cbrc_var_reg          - A partial cbrc_var_t context + additional
*                              information to facilite audit phase
*   *cbrc_energy_state_reg - CBRC energy states 
*
* RETURNS:
*  PMC_SUCCESS: 
*   recovery is a success 
*  CBRC_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT: 
*   Communal ressource energy states are incoherent. top will have
*    to shutdown the subsystem
*  CBRC_ERR_CONTEXT_REG_INCOHERENT:
*   Register conetxt is incoherent. top will have
*    to shutdown the subsystem
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_ctxt_reg_recover(cbrc_handle_t        *cbrc_handle, 
                                        pmc_energy_state_t    top_energy_state_reg,         
                                        cbrc_recover_var_t   *cbrc_var_reg,              
                                        cbrc_energy_state_t  *cbrc_energy_state_reg)
{
    UINT32                       itr;
    UINT32                       jtr;
    UINT32                       incr;
    PMC_ERROR                    rc = PMC_SUCCESS;
    PMC_ERROR                    loop_rc = PMC_SUCCESS;
    
    cbrc_serdes_type_t serdes_type[CBRC_MAX_LINK];
    fc800_pmg_mode_t   ilvr_fc800_mode;
    fc800_pmg_mode_t   dlvr_fc800_mode; 
    sdh_pmg_sts_mode_t rx_sdh_pmg_mode; 
    sdh_pmg_sts_mode_t tx_sdh_pmg_mode;
    sdh_pmg_handle_t      *rx_sdh_pmg_handle_ptr;
    sdh_pmg_handle_t      *tx_sdh_pmg_handle_ptr;
    UINT32 sdh_pmg_index;
    UINT32 sdh_pmg_slice;

    PMC_ENTRY();

    /* 
       create a register context 
    */
    /* start from a clean context */
    cbrc_var_init(NULL, &cbrc_var_reg->var, FALSE); 

    /* this information is coming from top-level register */
    cbrc_var_reg->var.cbrc_start_state = (top_energy_state_reg == PMC_ENERGY_STATE_RESET);

    /* 
       IF start_state is TRUE, the susbystem is completly in power down
       enet_register_var is completed (anyway, digi-layer should not call use with PMC_ENERGY_STATE_RESET)
    */
    if (FALSE == cbrc_var_reg->var.cbrc_start_state)
    {
        /* depending on enabled sub blocks, determine which datapaths are operational :*/
        /* fill up 
           cbrc_link_info_t    links[CBRC_MAX_LINK];
           struct cbrc_link_info_  *head;    
           BOOL                     available;
           BOOL                     tx_active;
           BOOL                     rx_active;           
           cbrc_serdes_type_t       serdes;
           cbrc_port_mode_t         mode;
           UINT32              cbrc_sdh_link_mask; 
           cbrc_prbs_ctxt_t    **prbs_ctxt;
           BOOL8 rx_cfg_active;
           sdh_pmg_prbs_cfg_t rx_cfg;
           BOOL8 tx_cfg_active;
           sdh_pmg_prbs_cfg_t tx_cfg;    
           sdh_pmg_pn11_insert_t tx_pn11_lof;           
           cbrc_loopback_t     loopback[CBRC_MAX_LINK];
        */


        PMC_MEMSET((void *)cbrc_var_reg->mode_fuzzy, 0x0, CBRC_MAX_LINK * sizeof(BOOL8));

        /* determine serdes type */
        cbrc_serdes_type_get(cbrc_handle, (cbrc_serdes_type_t *)&serdes_type[0]);

        for (itr = 0; itr < CBRC_MAX_LINK; itr += incr)
        {
            loop_rc = PMC_SUCCESS;
            incr = 1;
            sdh_pmg_index = itr % CBRC_SDH_PMG_MAX_INDEX;
            sdh_pmg_slice = itr / CBRC_SDH_PMG_MAX_INDEX;
            rx_sdh_pmg_handle_ptr = cbrc_handle->sdh_pmg_rx_ptr[sdh_pmg_slice];
            tx_sdh_pmg_handle_ptr = cbrc_handle->sdh_pmg_tx_ptr[sdh_pmg_slice];
            cbrc_var_reg->is_40g_dp[itr] = FALSE;

            /* determine link mode */
            if (CBRC_SERDES_TYPE_10G_XFI == serdes_type[itr])
            {
                /* possible modes: 
                   case CBRC_PORT_MODE_10G_FC800:
                   case CBRC_PORT_MODE_10G_CPRI_5G:
                   case CBRC_PORT_MODE_10G_CPRI_6G:
                   case CBRC_PORT_MODE_10G_CPRI_10G:
                   case CBRC_PORT_MODE_10G_GDPS_5G:
                   case CBRC_PORT_MODE_10G_GDPS_10G:
                   case CBRC_PORT_MODE_10G_STS192:
                   case CBRC_PORT_MODE_10G_STM64:
                   case CBRC_PORT_MODE_10G_STS192_PRBS:
                   case CBRC_PORT_MODE_10G_STM64_PRBS:
                   case CBRC_PORT_MODE_10G_FC1200_ODU1F:
                   case CBRC_PORT_MODE_10G_FC1200_ODU2F:
                   case CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E:*/
                if (CBRC_ACTIVE == fc1200_pcs_rx_field_SW_RESET_get(NULL, cbrc_handle, itr))
                {
                    if (CBRC_FC_TTTM_OSIR_DENOM == fc_tttm_field_OSIR_DENOM_get(NULL, cbrc_handle, itr))
                    {
                        cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E;
                    }
                    else
                    {
                        /* fuzzy: mode can be CBRC_PORT_MODE_10G_FC1200_ODU1F or CBRC_PORT_MODE_10G_FC1200_ODU2F*/
                        cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_FC1200_ODU2F;
                        cbrc_var_reg->mode_fuzzy[itr] = TRUE;
                    }
                }
                else
                {
                    /* possible SDH */
                    if (TRUE == sdh_pmg_prov_status_get(rx_sdh_pmg_handle_ptr, sdh_pmg_index) ||
                        TRUE == sdh_pmg_prov_status_get(tx_sdh_pmg_handle_ptr, sdh_pmg_index))
                    {
                        (void)sdh_pmg_cfg_mode_get(rx_sdh_pmg_handle_ptr, sdh_pmg_index, &rx_sdh_pmg_mode);
                        (void)sdh_pmg_cfg_mode_get(tx_sdh_pmg_handle_ptr, sdh_pmg_index, &tx_sdh_pmg_mode);

                        if (rx_sdh_pmg_mode != tx_sdh_pmg_mode)
                        {
                            if (PMC_SUCCESS == loop_rc)
                            {
                                loop_rc = CBRC_ERR_CONTEXT_REG_INCOHERENT;
                            }
                        }
                       
                        switch(rx_sdh_pmg_mode)
                        {
                        case SDH_PMG_STS_MODE_10G_STM64:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_STM64;
                            incr = CBRC_SDH_PMG_MAX_INDEX;
                            break;
                        case SDH_PMG_STS_MODE_10G_STS192:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_STS192;
                            break;
                        default:
                            break;
                        }
                        cbrc_var_reg->mode_fuzzy[itr] = TRUE; /* it could be a CBRC_PORT_MODE_10G_STS192_PRBS or CBRC_PORT_MODE_10G_STM64_PRBS */
                    }

                    /* possible FC800 */
                    else if (CBRC_ENABLE == cbrc_fc_ilvr_field_FC800PMG_CH_EN_get(NULL, cbrc_handle, itr))
                    {
                        fc800_pmg_mode_get(cbrc_handle->fc800_pmg_rx_ptr, itr, &ilvr_fc800_mode);
                        fc800_pmg_mode_get(cbrc_handle->fc800_pmg_tx_ptr, itr, &dlvr_fc800_mode);
                        if (ilvr_fc800_mode != dlvr_fc800_mode)
                        {
                            if (PMC_SUCCESS == loop_rc)
                            {
                                loop_rc = CBRC_ERR_CONTEXT_REG_INCOHERENT;
                            }
                        }
                        switch(ilvr_fc800_mode)
                        {
                        case FC800_PMG_MODE_10G_FC800:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_FC800;
                            break;
                        case FC800_PMG_MODE_10G_CPRI_5G:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_CPRI_5G;
                            break;
                        case FC800_PMG_MODE_10G_CPRI_6G:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_CPRI_6G;
                            break;
                        case FC800_PMG_MODE_10G_CPRI_10G:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_CPRI_10G;
                            break;
                        case FC800_PMG_MODE_10G_GDPS_5G:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_GDPS_5G;
                            break;
                        case FC800_PMG_MODE_10G_GDPS_10G:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_GDPS_10G;
                            break;
         case FC800_PMG_MODE_10G_8B_10B_IDLE:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_8B_10B_IDLE;
                            break;
         case FC800_PMG_MODE_10G_8B_10B_ARB:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_8B_10B_ARB;
                            break;              
         case FC800_PMG_MODE_10G_64B_66B_IDLE:
                            cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_10G_64B_66B_IDLE;
                            break;
                        default:
                            if (TRUE == fc800_pmg_status_get(cbrc_handle->fc800_pmg_rx_ptr, itr))
                            {                                
                                cbrc_var_reg->mode_fuzzy[itr] = TRUE;
                            }
                            break;
                        }
                    }
                }                
            }
            else 
            {
                /* possible modes:
                   case CBRC_PORT_MODE_40G_STS768:
                   case CBRC_PORT_MODE_40G_STM256:
                */
                
                /* avoid duplicated code */
                /* possible SDH */
                if (TRUE == sdh_pmg_prov_status_get(rx_sdh_pmg_handle_ptr, sdh_pmg_index) ||
                    TRUE == sdh_pmg_prov_status_get(tx_sdh_pmg_handle_ptr, sdh_pmg_index))
                {
                    (void)sdh_pmg_cfg_mode_get(rx_sdh_pmg_handle_ptr, sdh_pmg_index, &rx_sdh_pmg_mode);
                    (void)sdh_pmg_cfg_mode_get(tx_sdh_pmg_handle_ptr, sdh_pmg_index, &tx_sdh_pmg_mode);
                    
                    if (rx_sdh_pmg_mode != tx_sdh_pmg_mode)
                    {
                        if (PMC_SUCCESS == loop_rc)
                        {
                            loop_rc = CBRC_ERR_CONTEXT_REG_INCOHERENT;
                        }
                    }
                    
                    switch(rx_sdh_pmg_mode)
                    {
                    case SDH_PMG_STS_MODE_40G_STS768:
                        cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_40G_STS768;
                        incr = CBRC_SDH_PMG_MAX_INDEX;
                        break;
                    case SDH_PMG_STS_MODE_40G_STM256:
                        cbrc_var_reg->var.links[itr].mode = CBRC_PORT_MODE_40G_STM256;
                        break;
                    default:
                        break;
                    }
                    cbrc_var_reg->mode_fuzzy[itr] = TRUE; /* it could be a CBRC_PORT_MODE_10G_STS768 or CBRC_PORT_MODE_10G_STM256 */
                    if (LAST_CBRC_PORT_MODE != cbrc_var_reg->var.links[itr].mode)
                    {
                        cbrc_var_reg->is_40g_dp[itr] = TRUE;
                    }
                }
            }
            if (PMC_SUCCESS == loop_rc && CBRC_PORT_MODE_NONE != cbrc_var_reg->var.links[itr].mode)
            {
                cbrc_var_reg->var.links[itr].available = FALSE;
                cbrc_var_reg->var.links[itr].serdes = serdes_type[itr];
                /* determine TX and RX slice activation state get channel enable */
                if (CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E != cbrc_var_reg->var.links[itr].mode)
                {
                    cbrc_var_reg->var.links[itr].rx_active = cbr_fsgm_patt_ins_reset_status_get(cbrc_handle->cbr_fsgm_rx_ptr[itr / CBRC_CBR_FSGM_MAX_INDEX], itr % CBRC_CBR_FSGM_MAX_INDEX) ? FALSE : TRUE;
                    cbrc_var_reg->var.links[itr].tx_active = cbr_fsgm_patt_ins_reset_status_get(cbrc_handle->cbr_fsgm_tx_ptr[itr / CBRC_CBR_FSGM_MAX_INDEX], itr % CBRC_CBR_FSGM_MAX_INDEX) ? FALSE : TRUE;
                }                
                else 
                {
                    if (CBRC_ENABLE == cbrc_fc_ilvr_field_FCPTE_CH_EN_get(NULL, cbrc_handle, itr) &&
                        CBRC_DISABLE == cbr_int_field_CHN_EN_get(NULL, cbrc_handle, itr))
                    {
                        cbrc_var_reg->var.links[itr].rx_active = TRUE;
                    }
                    if (CBRC_ENABLE == cbrc_fc_dlvr_field_CBR_CH_EN_get(NULL, cbrc_handle) &&
                        CBRC_ENABLE == cbrc_fc_dlvr_field_FCPTD_CH_EN_get(NULL, cbrc_handle, itr) && 
                        CBRC_ENABLE == cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get(NULL, cbrc_handle, itr))
                    {
                        cbrc_var_reg->var.links[itr].tx_active = TRUE;
                    }
                }

                /* update cbrc_sdh_link_mask */
                if (cbrc_port_is_sonet(cbrc_var_reg->var.links[itr].mode))
                {
                    if (TRUE == cbrc_var_reg->var.links[itr].rx_active)
                    {
                        CBRC_SDH_LINK_MASK_SET(cbrc_var_reg->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_RX, sdh_pmg_slice, sdh_pmg_index);
                    }
                    if (TRUE == cbrc_var_reg->var.links[itr].tx_active)
                    {
                        CBRC_SDH_LINK_MASK_SET(cbrc_var_reg->var.cbrc_sdh_link_mask, CBRC_PORT_DIR_TX, sdh_pmg_slice, sdh_pmg_index);
                    }
                }

                /* retrieve prbs status */
                (void)cbrc_sdh_prbs_get(cbrc_handle, CBRC_PORT_DIR_RX, itr, TRUE, &cbrc_var_reg->var.prbs_ctxt[itr].rx_cfg);
                (void)cbrc_sdh_prbs_get(cbrc_handle, CBRC_PORT_DIR_TX, itr, TRUE, &cbrc_var_reg->var.prbs_ctxt[itr].tx_cfg);

                /* retrieve loopback mode */
                cbrc_var_reg->var.loopback[itr] = cbrc_loopback_mode_get(cbrc_handle, itr, cbrc_var_reg->is_40g_dp[itr]);

                if (TRUE == cbrc_var_reg->is_40g_dp[itr])
                {
                    incr = LINK_MASK_SLICE_OFFSET;
                    for (jtr = itr + 1; jtr < (itr + LINK_MASK_SLICE_OFFSET); jtr += 1)
                    {
                        PMC_ASSERT(jtr < CBRC_MAX_LINK, CBRC_ERR_INVALID_ARG, jtr, CBRC_MAX_LINK);
                        PMC_MEMCPY((void *)&cbrc_var_reg->var.links[jtr], (void *)&cbrc_var_reg->var.links[itr], sizeof(cbrc_link_info_t));
                        PMC_MEMCPY((void *)&cbrc_var_reg->var.prbs_ctxt[jtr], (void *)&cbrc_var_reg->var.prbs_ctxt[itr], sizeof(cbrc_prbs_ctxt_t));
                        cbrc_var_reg->var.loopback[jtr] = cbrc_var_reg->var.loopback[itr]; 
                        cbrc_var_reg->is_40g_dp[jtr] = TRUE;
                        cbrc_var_reg->mode_fuzzy[jtr] = cbrc_var_reg->mode_fuzzy[itr];
                    }
                    
                }
            }

            if (PMC_SUCCESS == rc)
            {
                rc = loop_rc;
            }
        }
        
        /* retrieve oduksc scbs3 calendar entries */
        scbs3_page_get(cbrc_handle->pkt_int_scbs_ptr, 
                       SCBS3_ACTIVE_MEM, 
                       &cbrc_var_reg->scbs3_page);
        
        /* check any inchoherencies such as FC800 and FC1200 activated in the same time */
    }
    
    PMC_RETURN(rc);
} /* cbrc_ctxt_reg_recover */

/*******************************************************************************
* cbrc_loopback_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve cbrc link loopback mode from devie register space.
*   
*
* INPUTS:
*   cbrc_handle       - Pointer to subsystem handle
*   link              - link
*   is_40g_dp         - TRUE if link is head of a 40G link,
*                       otherwise FALSE
*
* OUTPUTS:
*
* RETURNS:
*   cbrc_loopback_t - loopback mode configured for the given link.
*
* NOTES:
*
*******************************************************************************/
PRIVATE cbrc_loopback_t cbrc_loopback_mode_get(cbrc_handle_t            *cbrc_handle,
                                               UINT32                   link,
                                               BOOL8                    is_40g_dp)
{
    UINT32 max_index = TRUE == is_40g_dp ? LINK_MASK_SLICE_OFFSET : 1;
    UINT32 itr;
    cbrc_loopback_t loop_lpbk_mode = CBRC_DISABLE_LOOPBACK;
    cbrc_loopback_t lpbk_mode = CBRC_DISABLE_LOOPBACK;

    PMC_ENTRY();
    
    /* Check arguments */
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < CBRC_MAX_LINK, CBRC_ERR_INVALID_LINK, 0, 0);

    for (itr = link; itr < (link + max_index); itr++)
    {
        /* assume loopback is disabled until it is confirmed */
        lpbk_mode = CBRC_DISABLE_LOOPBACK;

        if (TRUE == cbrc_fc_dlvr_field_LPBK_MUX_SEL_get(NULL,cbrc_handle,itr))
        {
            lpbk_mode = CBRC_FACILITY_LOOPBACK;
        }

        if (TRUE == mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get(NULL, cbrc_handle, itr))
        {
            if (lpbk_mode == CBRC_DISABLE_LOOPBACK)
            {              
                lpbk_mode = CBRC_DIAG_LOOPBACK;  
            }
            else
            {
                /* Deinterleaver and MLD loopback cannot be activated in the same time */
                loop_lpbk_mode = CBRC_DISABLE_LOOPBACK;
                break;
            }
        }

        if (itr == link)
        {
            loop_lpbk_mode = lpbk_mode;
        }
        else
        {
            /* discard loopback mode if there is any incoherency in lanes mux configuration */
            if (loop_lpbk_mode != lpbk_mode)
            {  
                loop_lpbk_mode = CBRC_DISABLE_LOOPBACK;
                break;
            }         
        }
    }

    PMC_RETURN(loop_lpbk_mode);
} /* cbrc_loopback_mode_get */

/*******************************************************************************
* cbrc_serdes_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve serdes type from device register space.
*
* INPUTS:
*   *cbrc_handle        - pointer to CBRC handle instance to be operated on
*   
* OUTPUTS:
*   *serdes_type_ptr   - serdes type associated to each link.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*
*******************************************************************************/
PRIVATE void cbrc_serdes_type_get(cbrc_handle_t      *cbrc_handle,
                                  cbrc_serdes_type_t *serdes_type_ptr)
{
    UINT32 itr;
    UINT32 jtr;
    UINT32 link_itr;
    UINT32 incr;
    UINT32 slice;
    BOOL8  discard_link;
    UINT8  rx_mux_sel = 0;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbrc_handle, CBRC_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_type_ptr, CBRC_ERR_NULL_PARAMETERS, 0, 0);

    for (itr = 0; itr < CBRC_MAX_LINK; itr += incr)
    {
        incr = 1;
        serdes_type_ptr[itr] = CBRC_SERDES_TYPE_NONE;
        /* check that mld prbs clock is enabled */
        if (0 == (itr % LINK_MASK_SLICE_OFFSET))
        {
            slice = itr / LINK_MASK_SLICE_OFFSET;
            /* check if link associated to STL interface */
            if (0 == ((1 << slice) & mldprbs_pcbi_field_STL256_SYNC_RST_get(NULL, cbrc_handle)) &&
                0 != ((1 << slice) & mldprbs_pcbi_field_STL256_CLK_EN_get(NULL, cbrc_handle)))
            {
                for (jtr = itr; jtr < (itr + LINK_MASK_SLICE_OFFSET); jtr += 1)
                {
                    if (CBRC_RESET != mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get(NULL, cbrc_handle, jtr))
                    {
                        break;
                    }
                }
                if ((itr+LINK_MASK_SLICE_OFFSET) == jtr)
                {
                    serdes_type_ptr[itr] = CBRC_SERDES_TYPE_40G_STL;
                    rx_mux_sel = CBRC_MLD_RX_STL;
                }
            }
            /* check if link associated to SFIS interface */
            else if (0 != ((1 << slice) & mldprbs_pcbi_field_SFIS_CLK_EN_get(NULL, cbrc_handle)) &&
                     CBRC_ACTIVE == cbrc_sfis_rx_field_RX_SW_RST_get(NULL, cbrc_handle, slice) &&
                     CBRC_ACTIVE ==  cbrc_sfis_tx_field_TX_SW_RST_get(NULL, cbrc_handle, slice))
            {
                serdes_type_ptr[itr] = CBRC_SERDES_TYPE_40G_SFIS;
                rx_mux_sel = CBRC_MLD_RX_SFIS;
            }
            /* check if link associated to SFI51 interface */
            else if (0 != ((1 << slice) & mldprbs_pcbi_field_SFI51_CLK_EN_get(NULL, cbrc_handle)))
            {
                serdes_type_ptr[itr] = CBRC_SERDES_TYPE_40G_SFI51;
                rx_mux_sel = CBRC_MLD_RX_SFI51;
            }            
        }

        /* no 40G link was found, try to check if a 10G link */
        if (CBRC_SERDES_TYPE_NONE == serdes_type_ptr[itr])
        {
            if (CBRC_MLD_RX_XFI == mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get(NULL, cbrc_handle, itr))
            {              
                serdes_type_ptr[itr] = CBRC_SERDES_TYPE_10G_XFI;  
            }    
        }
        else if(0 == (itr % LINK_MASK_SLICE_OFFSET))
        {    
            discard_link = FALSE;
                
            if ((CBRC_SERDES_TYPE_40G_STL != serdes_type_ptr[itr] &&
                 CBRC_SERDES_TYPE_40G_SFIS != serdes_type_ptr[itr] && 
                 CBRC_SERDES_TYPE_40G_SFI51 != serdes_type_ptr[itr]) &&
                1 > (itr / LINK_MASK_SLICE_OFFSET))

            {
                discard_link = TRUE;
            }
            else
            {
                /* check that MLD is well configured for all lanes associated to 40G datapath */
                for (link_itr = itr; link_itr < (itr + LINK_MASK_SLICE_OFFSET); link_itr += 1)
                {
                    if ((UINT32)rx_mux_sel != mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get(NULL, cbrc_handle, link_itr))
                    {
                        /* discard all lanes if there is any incoherency */
                        discard_link = TRUE;
                        break;
                    }
                }
            }

            /* an incoherency was found in 40G link, disacrd current lane */
            if (TRUE == discard_link)
            {
                serdes_type_ptr[itr] = CBRC_SERDES_TYPE_NONE;
            }
            else
            {
                incr = LINK_MASK_SLICE_OFFSET;
                for (link_itr = itr; link_itr < (itr + incr); link_itr += 1)
                {
                    serdes_type_ptr[link_itr] = serdes_type_ptr[itr];
                }                
            }
        }
    }

    PMC_RETURN();
} /* cbrc_serdes_type_get */

/*******************************************************************************
* cbrc_db_link_entry_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function audit SCBS3 calendar of a specified link.
*
* INPUTS:
*   *cbrc_handle_ptr     - pointer to CBRC handle instance
*    cbrc_db_id          - CBRC database id
*   *scbs3_page_ptr      - SCBS3 page to audit
*    link                - link identifier
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfull otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_db_link_entry_audit(cbrc_handle_t     *cbrc_handle_ptr,
                                           cbrc_db_id_t       cbrc_db_id,
                                           scbs3_page_t      *scbs3_page_ptr,
                                           UINT32             link)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               num_of_recs = 0;
    mapotn_db_key        cbrc_db_key;
    void                *key;
    mapotn_db_query_res  *q_result = NULL;
    UINT32               itr;
    UINT32               cal_itr;
    UINT32               db_handle;
    UINT32               cal_entries[96];
    UINT32               num_cal_entries = 0;
    util_gen_db_id_t     db_id;
            
    PMC_ENTRY();
    
    
    cbrc_db_key.calendar_entry = DO_NOT_USE_KEY;
    cbrc_db_key.channel_num = link;
    key = (void *)&cbrc_db_key;
    db_id = cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id;
    db_handle = cbrc_handle_ptr->cfg.schd_use_ctxt[cbrc_db_id].db_id;

    /* check that there are some calendar entries associated to link */
    num_cal_entries = gen_db_query_count(&(cbrc_handle_ptr->base),
                                         db_id,
                                         db_handle,
                                         (void *)key);
    
    if (num_cal_entries == 0)
    {
        result = CBRC_ERR_CONTEXT_CLEANUP;
    }
                  
    num_cal_entries = 0;
    /* query all callendar entries belonging to link*/
    for (itr = 0; itr < 96 && PMC_SUCCESS == result; itr++)
    {
        /* query rd_channel_id data for the iteration */
        cbrc_db_key.calendar_entry = itr;
        cbrc_db_key.channel_num = DO_NOT_USE_KEY;

        num_of_recs = gen_db_query(&(cbrc_handle_ptr->base),
                                   db_id,
                                   db_handle,
                                   (void *)key,
                                   (void **)&q_result);
        if (q_result->num_of_recs > 0)
        {
            if (q_result->p_recs[0]->channel_num == link)
            {
                cal_entries[num_cal_entries] = itr;
                num_cal_entries += 1;
            }
        }
        else
        {
            result = CBRC_ERR_CONTEXT_CLEANUP;
        }
    }


    /* compare register based calendar slot against database entries */
    for (cal_itr = 0; cal_itr < num_cal_entries && PMC_SUCCESS == result; cal_itr++)
    {
        /* link calendar entries are have a offset depending on userbits value */
        /* See util_schd.c in util_schd_group_dist_gen_db() */
        if (link != (scbs3_page_ptr->calendar[cal_entries[cal_itr]] & ~(cbrc_handle_ptr->pkt_int_scbs_ptr->util_schd_handle->var.schd_ctxt->chnl_ctxt[link].scbs_user_bits << 4)))
        {
            PMC_LOG_TRACE("%d - cal_entries[%d]= %d\n", num_cal_entries, cal_itr, cal_entries[cal_itr]);
            PMC_LOG_TRACE("in %s, compare link(%d) == calendar(%d)\n", __FUNCTION__, link, scbs3_page_ptr->calendar[cal_entries[cal_itr]]);
            result = CBRC_ERR_CONTEXT_CLEANUP;
        }
    }

    PMC_RETURN(result);

} /* cbrc_db_link_entry_audit */


/*******************************************************************************
* cbrc_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Audit memory context with recovered register context. This function 
*   determines if a cleanup is required
*   
*   A clean context which represents the intersection between both context is also 
*   generated.
*
* INPUTS:
*   *cbrc_handle        - pointer to CBRC handle instance to be operated on
*   *cbrc_var_reg       - A partial cbrc_var_t context + additional
*                           information to facilite audit phase
*   *cbrc_energy_state_reg - CBRC energy states 
*
* OUTPUTS:
*   *cbrc_var_clean     - A clean context which represents the intersection
*                           between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS:
*    Memory & register context matches
*   CBRC_ERR_CONTEXT_CLEANUP:
*    A cleanup is required.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_ctxt_audit(cbrc_handle_t        *cbrc_handle,
                                  cbrc_recover_var_t   *cbrc_var_reg,
                                  cbrc_energy_state_t  *cbrc_energy_state_reg,
                                  cbrc_var_t           *cbrc_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;  
    PMC_ERROR loop_rc = PMC_SUCCESS;  
    UINT32 itr;
    BOOL8 discard;

    PMC_ENTRY();
    
    /* init the new context  */
    cbrc_var_init(NULL, cbrc_var_clean, FALSE);

    /* audit energy state */
    if (cbrc_var_reg->var.cbrc_start_state != cbrc_start_state_test(cbrc_handle))
    {       
        cbrc_var_clean->cbrc_start_state = TRUE;
        rc = CBRC_ERR_CONTEXT_CLEANUP;        
        PMC_LOG_TRACE("step 0 rc = %x %d %d\n", rc, cbrc_var_reg->var.cbrc_start_state, cbrc_start_state_test(cbrc_handle));
    } 
    else if (cbrc_var_reg->var.cbrc_start_state == FALSE) 
    {
        /* compare 
           cbrc_link_info_t    links[CBRC_MAX_LINK];
           struct cbrc_link_info_  *head;    
           BOOL                     available;
           BOOL                     tx_active;
           BOOL                     rx_active;           
           cbrc_serdes_type_t       serdes;
           cbrc_port_mode_t         mode;
           UINT32              cbrc_sdh_link_mask; 
           cbrc_prbs_ctxt_t    **prbs_ctxt;
           BOOL8 rx_cfg_active;
           sdh_pmg_prbs_cfg_t rx_cfg;
           BOOL8 tx_cfg_active;
           sdh_pmg_prbs_cfg_t tx_cfg;    
           sdh_pmg_pn11_insert_t tx_pn11_lof;           
           cbrc_loopback_t     loopback[CBRC_MAX_LINK];
        */
        for (itr = 0; itr < CBRC_MAX_LINK; itr += 1)
        {
            loop_rc =  PMC_SUCCESS;
            discard = FALSE;

            if (cbrc_var_reg->var.links[itr].available != cbrc_handle->var.links[itr].available)
            {
                PMC_LOG_TRACE("%s failed: step 01 - avalaible incorrect for link %d -> (reg)%d (ctx)%d\n", 
                              __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.links[itr].available, (UINT32)cbrc_handle->var.links[itr].available);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && cbrc_var_reg->var.links[itr].tx_active != cbrc_handle->var.links[itr].tx_active)
            {
                PMC_LOG_TRACE("%s failed: step 02 -  tx_active incorrect for link %d -> (reg)%d (ctx)%d\n", 
                              __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.links[itr].tx_active, (UINT32)cbrc_handle->var.links[itr].tx_active);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && cbrc_var_reg->var.links[itr].rx_active != cbrc_handle->var.links[itr].rx_active)
            {
                PMC_LOG_TRACE("%s failed: step 03 -  rx_active incorrect for link %d -> (reg)%d (ctx)%d\n", 
                              __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.links[itr].rx_active, (UINT32)cbrc_handle->var.links[itr].rx_active);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && cbrc_var_reg->var.links[itr].serdes != cbrc_handle->var.links[itr].serdes)
            {
                PMC_LOG_TRACE("%s failed: step 04 - serdes_type incorrect for link %d -> (reg)%d (ctx)%d\n", 
                              __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.links[itr].serdes, (UINT32)cbrc_handle->var.links[itr].serdes);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && cbrc_var_reg->var.links[itr].mode != cbrc_handle->var.links[itr].mode)
            {
                if (TRUE == cbrc_var_reg->mode_fuzzy[itr])
                {
                    if (((CBRC_PORT_MODE_10G_STS192 == cbrc_var_reg->var.links[itr].mode &&
                          CBRC_PORT_MODE_10G_STM64 == cbrc_handle->var.links[itr].mode) ||
                         (CBRC_PORT_MODE_10G_STS192 == cbrc_var_reg->var.links[itr].mode &&
                          CBRC_PORT_MODE_10G_10GE == cbrc_handle->var.links[itr].mode) || 
                         (CBRC_PORT_MODE_10G_STS192 == cbrc_var_reg->var.links[itr].mode &&
                          CBRC_PORT_MODE_10G_STS192_PRBS == cbrc_handle->var.links[itr].mode) ||
                         (CBRC_PORT_MODE_10G_STS192 == cbrc_var_reg->var.links[itr].mode &&
                          CBRC_PORT_MODE_10G_STM64_PRBS == cbrc_handle->var.links[itr].mode) ||
                         (CBRC_PORT_MODE_40G_STS768 == cbrc_var_reg->var.links[itr].mode &&
                          CBRC_PORT_MODE_40G_STM256 == cbrc_handle->var.links[itr].mode)))
                    {
                    }
                    else if (((CBRC_PORT_MODE_10G_FC1200_ODU2F ==  cbrc_var_reg->var.links[itr].mode &&
                               CBRC_PORT_MODE_10G_FC1200_ODU1F == cbrc_handle->var.links[itr].mode)))
                    {                        
                    }
                    else
                    {
                        discard = TRUE;  
                    }
                }
                else
                {
                    discard = TRUE;
                }
                
                if (TRUE == discard)
                {
                    PMC_LOG_TRACE("%s failed: step 05 - mode incorrect for link %d -> (reg)%d (ctx)%d\n", __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.links[itr].mode, (UINT32)cbrc_handle->var.links[itr].mode);
                    loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
                }
            }

            if (PMC_SUCCESS == loop_rc && TRUE == cbrc_handle->var.prbs_ctxt[itr].rx_cfg_active &&
                (cbrc_var_reg->var.prbs_ctxt[itr].rx_cfg.prbs_type != cbrc_handle->var.prbs_ctxt[itr].rx_cfg.prbs_type ||
                 cbrc_var_reg->var.prbs_ctxt[itr].rx_cfg.inv_cfg != cbrc_handle->var.prbs_ctxt[itr].rx_cfg.inv_cfg ||
                 cbrc_var_reg->var.prbs_ctxt[itr].rx_cfg.enable_prbs != cbrc_handle->var.prbs_ctxt[itr].rx_cfg.enable_prbs))
            {
                PMC_LOG_TRACE("%s failed: step 06 - prbs rx_cfg incorrect for link %d \n", __FUNCTION__, itr);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && TRUE == cbrc_handle->var.prbs_ctxt[itr].tx_cfg_active &&
                (cbrc_var_reg->var.prbs_ctxt[itr].tx_cfg.prbs_type != cbrc_handle->var.prbs_ctxt[itr].tx_cfg.prbs_type ||
                 cbrc_var_reg->var.prbs_ctxt[itr].tx_cfg.inv_cfg != cbrc_handle->var.prbs_ctxt[itr].tx_cfg.inv_cfg ||
                 cbrc_var_reg->var.prbs_ctxt[itr].tx_cfg.enable_prbs != cbrc_handle->var.prbs_ctxt[itr].tx_cfg.enable_prbs))
            {
                PMC_LOG_TRACE("%s failed: step 07 - prbs tx_cfg incorrect for link %d \n", __FUNCTION__, itr);
                loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
            }

            if (PMC_SUCCESS == loop_rc && cbrc_var_reg->var.loopback[itr] != cbrc_handle->var.loopback[itr])
            {
                if (FALSE == cbrc_var_reg->is_40g_dp[itr] ||
                    0 == (itr % LINK_MASK_SLICE_OFFSET))
                {
                    PMC_LOG_TRACE("%s failed: step 08 - loopback incorrect for link %d -> (reg)%d (ctx)%d\n", __FUNCTION__, itr, (UINT32)cbrc_var_reg->var.loopback[itr], (UINT32)cbrc_handle->var.loopback[itr]);
                    loop_rc = CBRC_ERR_CONTEXT_CLEANUP;
                }
            }

            /* check that link is registered in DB */
            if (PMC_SUCCESS == loop_rc && 
                FALSE == cbrc_var_reg->var.links[itr].available)
            {
                if (FALSE == cbrc_var_reg->is_40g_dp[itr] || 
                    (0 == (itr % LINK_MASK_SLICE_OFFSET)))
                {
                    loop_rc = cbrc_db_link_entry_audit(cbrc_handle,
                                                       CBRC_DB_ID_PKT_INT_MPMA,
                                                       &cbrc_var_reg->scbs3_page,
                                                       itr);
                }
            }

            /* copy link properties in var clean */
            if (PMC_SUCCESS == loop_rc)
            {
                PMC_MEMCPY((void *)&cbrc_var_clean->links[itr], (void *)&cbrc_handle->var.links[itr], sizeof(cbrc_link_info_t));
                PMC_MEMCPY((void *)&cbrc_var_clean->prbs_ctxt[itr], (void *)&cbrc_handle->var.prbs_ctxt[itr], sizeof(cbrc_prbs_ctxt_t));
                cbrc_var_clean->loopback[itr] = cbrc_handle->var.loopback[itr];              
                if (TRUE == cbrc_port_is_sonet(cbrc_var_clean->links[itr].mode) &&
                    FALSE == cbrc_var_clean->links[itr].available)
                {
                    /*
                     * For a 40G cage only want to do this calculation once
                     * There are 4 ports but only the first one is
                     * actually part of the calculation. For 10G each one
                     * contributes to the calculation.
                     */
                    if (((cbrc_var_clean->links[itr].mode == CBRC_PORT_MODE_40G_STS768 ||
                          cbrc_var_clean->links[itr].mode == CBRC_PORT_MODE_40G_STM256) &&
                         0 == itr % CBRC_SDH_PMG_MAX_INDEX) ||
                        (cbrc_var_clean->links[itr].mode == CBRC_PORT_MODE_10G_STS192 ||
                         cbrc_var_clean->links[itr].mode == CBRC_PORT_MODE_10G_STM64 ||
                         cbrc_var_clean->links[itr].mode == CBRC_PORT_MODE_10G_10GE))
                    {
                        if (TRUE == cbrc_var_clean->links[itr].tx_active)
                        {
                            CBRC_SDH_LINK_MASK_SET(cbrc_var_clean->cbrc_sdh_link_mask, CBRC_PORT_DIR_TX, itr / CBRC_SDH_PMG_MAX_INDEX, itr % CBRC_SDH_PMG_MAX_INDEX);
                        }
                        
                        if (TRUE == cbrc_var_clean->links[itr].rx_active)
                        {
                            CBRC_SDH_LINK_MASK_SET(cbrc_var_clean->cbrc_sdh_link_mask, CBRC_PORT_DIR_RX, itr / CBRC_SDH_PMG_MAX_INDEX, itr % CBRC_SDH_PMG_MAX_INDEX);
                        }
                    } 
                }
            }
            
            if (PMC_SUCCESS == rc)
            {
                rc = loop_rc;
            }
        }
    }

    PMC_RETURN(rc);
} /* cbrc_ctxt_audit */

/*******************************************************************************
*  cbrc_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure CBRC and its sub-block.
*
* INPUTS:
*   *cbrc_handle           - handle to the Cbrc instance to be operated on containing 
*                            variable configuraton retrieved from saved context
*   *cbrc_var_reg          - CBRC variable configuration recovered from device registers
*   *cbrc_energy_state_reg - CBRC energy state
*   *cbrc_var_clean        - new CBRC variable configuration that matches device state.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS if operation is successfully executed, 
*   otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_ctxt_cleanup(cbrc_handle_t         *cbrc_handle,
                                    cbrc_recover_var_t    *cbrc_var_reg,
                                    cbrc_energy_state_t   *cbrc_energy_state_reg,
                                    cbrc_var_t            *cbrc_var_clean)
{
    PMC_ERROR          rc = PMC_SUCCESS;
    UINT32             num_link_to_clean = 0;
    UINT32             link_to_clean[CBRC_MAX_LINK];
    UINT32             link_itr;

    PMC_ENTRY();

    PMC_LOG_TRACE("about to clean up cbrc\n");

    /* Perform register cleanup and sub-blocks memory context cleanup */
    if (cbrc_start_state_test(cbrc_handle) == FALSE)
    {
        for (link_itr = 0; link_itr < CBRC_MAX_LINK;  link_itr += 1)
        {
            if (TRUE == cbrc_var_clean->links[link_itr].available ||
                (FALSE ==  cbrc_var_clean->links[link_itr].tx_active && 
                 FALSE == cbrc_var_clean->links[link_itr].rx_active))
            {                
                PMC_LOG_TRACE("CBRC link %d needs to be cleanup\n", link_itr);
                link_to_clean[num_link_to_clean] = link_itr;
                num_link_to_clean += 1;
                cbrc_var_reg->is_40g_dp[link_itr] = FALSE;
            }
        }        
        
        /* the old cbrc var context is incoherent, we use the new one */
        PMC_MEMCPY(&cbrc_handle->var, cbrc_var_clean, sizeof(cbrc_var_t));  
        
        /* update var->links[].head */
        for (link_itr = 0; link_itr < CBRC_MAX_LINK; link_itr += 1)
        {
            if (TRUE == cbrc_var_reg->is_40g_dp[link_itr])
            {
                cbrc_handle->var.links[link_itr].head = &cbrc_handle->var.links[LINK_MASK_SLICE_OFFSET * (link_itr/LINK_MASK_SLICE_OFFSET)];
            }
        }
        
        
        /* clean MPMA & MPMO */
        mpma_ch_clean(cbrc_handle->mpma_ptr, link_to_clean, num_link_to_clean);
        mpmo_ch_clean(cbrc_handle->mpmo_ptr, link_to_clean, num_link_to_clean);
        
        /* clean up MLD */
        cbrc_mld_prbs_cleanup(cbrc_handle, num_link_to_clean, link_to_clean); 
        
        /* clean up ILVR */    
        cbrc_fc_ilvr_cleanup(cbrc_handle, num_link_to_clean, link_to_clean);
        
        /* clean up DLVR */
        cbrc_fc_dlvr_cleanup(cbrc_handle, num_link_to_clean, link_to_clean);
        
        /* clean up  SDH */
        for (link_itr = 0; link_itr < CBRC_SDH_PMG_MAX_SLICE; link_itr += 1)
        {
            /*perhaps we will need to create a cbrc_sdh_pmg_cleanup*/
            sdh_pmg_cleanup(cbrc_handle->sdh_pmg_rx_ptr[link_itr], link_itr * LINK_MASK_SLICE_OFFSET, num_link_to_clean, link_to_clean);
            sdh_pmg_cleanup(cbrc_handle->sdh_pmg_tx_ptr[link_itr], link_itr * LINK_MASK_SLICE_OFFSET, num_link_to_clean, link_to_clean);
            /* Put slice into low power if no links are still provisioned */
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle, CBRC_PORT_DIR_RX, link_itr))
            {
                cbrc_sdh_pmg_reset(cbrc_handle, CBRC_PORT_DIR_RX, link_itr*LINK_MASK_SLICE_OFFSET, CBRC_RESET);
            }
            if (!IS_CBRC_SDH_SLICE_PROV(cbrc_handle, CBRC_PORT_DIR_TX, link_itr))
            {
                cbrc_sdh_pmg_reset(cbrc_handle, CBRC_PORT_DIR_TX, link_itr*LINK_MASK_SLICE_OFFSET, CBRC_RESET);
            }
        }
        
        /* Put macro into reset if no links are still provisioned */
        if (!IS_CBRC_SDH_PROV(cbrc_handle))
        {
            cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle, CBRC_PORT_DIR_RX, CBRC_RESET);
            cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set(NULL, cbrc_handle, CBRC_PORT_DIR_TX, CBRC_RESET);
        }
        
        PMC_LOG_TRACE("number of cleaned streams: %d\n", num_link_to_clean);
    }
    else
    {
        cbrc_var_init(cbrc_handle, &cbrc_handle->var, FALSE);
    }

    PMC_RETURN(rc);
    
} /* cbrc_ctxt_cleanup */

/*******************************************************************************
* cbrc_sfis_odd_parity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures SFIS bits of odd parity.
*
* INPUTS:
*   cbrc_handle       - pointer to cbrc handle instance.
*   link              - link to configure.
*   dir               - direction concerned by the configuration: RX, TX or both.
*   inv               - when TRUE all bits of odd parity are inverted
*                       when FALSE all bits of odd parity are processed normally
*
* OUTPUTS:
*   None.  
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sfis_odd_parity_set(cbrc_handle_t            *cbrc_handle,
                                          UINT8                     link,
                                          util_global_direction_t   dir,
                                          BOOL8                     inv)
{
    BOOL8 up_rx_odd = FALSE;
    BOOL8 up_tx_odd = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice;
    const UINT32 reg_val = (TRUE ==  inv ? 1 : 0);
   
    PMC_ENTRY();

    /* Determine which interface is concerned by the configuration: RX or TX, or both */
    switch(dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        up_rx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_TX:
        up_tx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        up_rx_odd = TRUE;
        up_tx_odd = TRUE;
        break;
    default:
        rc = CBRC_ERR_INVALID_ARG;
        break;
    }

    /* retrieve SFIS slice id */
    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sfis_slice_get(cbrc_handle, link, &slice);
    }

    /* Configure registers */
    if (PMC_SUCCESS == rc)
    {        
        if (TRUE == up_rx_odd)
        {
            cbrc_sfis_rx_field_RX_SW_RST_set(NULL,cbrc_handle, slice, 1);
            cbrc_sfis_rx_field_RX_ODD_INV_set(NULL,cbrc_handle, slice, reg_val);
            cbrc_sfis_rx_field_RX_SW_RST_set(NULL,cbrc_handle, slice, 0);
        }
        
        if (TRUE == up_tx_odd)
        {
            cbrc_sfis_tx_field_TX_SW_RST_set(NULL,cbrc_handle, slice, 1);
            cbrc_sfis_tx_field_TX_ODD_INV_set(NULL,cbrc_handle, slice, reg_val);
            cbrc_sfis_tx_field_TX_SW_RST_set(NULL,cbrc_handle, slice, 0);
        }
    }

    PMC_RETURN(rc);
}/* cbrc_sfis_odd_parity_set */

/*******************************************************************************
* cbrc_sfis_odd_parity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns status of SFIS bits of odd parity.
*
* INPUTS:
*   cbrc_handle       - pointer to cbrc handle instance.
*   link              - link to configure.
*   dir               - direction concerned by the configuration: RX, TX only.
*
* OUTPUTS:
*   *inv_ptr          - when TRUE all bits of odd parity are inverted
*                       when FALSE all bits of odd parity are processed normally
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbrc_sfis_odd_parity_get(cbrc_handle_t           *cbrc_handle,
                                          UINT8                    link,
                                          util_global_direction_t  dir,
                                          BOOL8                   *inv_ptr)
{
    BOOL8 rd_rx_odd = FALSE;
    BOOL8 rd_tx_odd = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice;
    UINT32 reg_val = 0;
   
    PMC_ENTRY();
    
    /* Determine which interface is concerned by the configuration: RX or TX, or both */
    switch(dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        rd_rx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_TX:
        rd_tx_odd = TRUE;
        break;
    default:
        rc = CBRC_ERR_INVALID_ARG;
        break;
    }

    /* retrieve SFIS slice id */
    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sfis_slice_get(cbrc_handle, link, &slice);
    }

    /* read registers */
    if (PMC_SUCCESS == rc)
    {        
        if (TRUE == rd_rx_odd)
        {
            reg_val = cbrc_sfis_rx_field_RX_ODD_INV_get(NULL,cbrc_handle, slice);
        }
        
        if (TRUE == rd_tx_odd)
        {
            reg_val = cbrc_sfis_tx_field_TX_ODD_INV_get(NULL,cbrc_handle, slice);
        }
    } 

    /* update *inv_ptr according to register value */
    if (PMC_SUCCESS == rc)
    {  
        *inv_ptr = (1 == reg_val) ? TRUE : FALSE;
    }
        
    PMC_RETURN(rc);
}/* cbrc_sfis_odd_parity_get */

/*******************************************************************************
* cbrc_sfis_slice_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns SFIS slice identifier associated to dsi stream.
*
* INPUTS:
*   cbrc_handle       - pointer to cbrc handle instance.
*   link              - link to configure.
*
* OUTPUTS:
*   *slice_id_ptr     - SFIS slice id associated to given  dsi stream
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cbrc_sfis_slice_get(cbrc_handle_t  *cbrc_handle,
                                      UINT32          link,
                                      UINT8          *slice_id_ptr)
{
    PMC_ERROR rc = CBRC_ERR_INVALID_ARG;
    cbrc_link_info_t   *link_ptr;

    PMC_ENTRY();
     
    rc = cbrc_link_get(cbrc_handle, link, &link_ptr);

    if (PMC_SUCCESS == rc)
    {
        if (cbrc_serdes_type_is_40g(link_ptr->serdes) &&
            CBRC_SERDES_TYPE_40G_SFIS == link_ptr->serdes)
        {        
            if (0 == link)
            {
                *slice_id_ptr = 0;
            }
            else
            {
                *slice_id_ptr = 1;
            }
            rc = PMC_SUCCESS;
        }           
    }

    PMC_RETURN(rc);
}/* cbrc_sfis_slice_get */

/*******************************************************************************
* cbrc_sfis_odd_parity_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function resets SFIS bits of odd parity.
*
* INPUTS:
*   cbrc_handle          - pointer to cbrc handle instance.
*   slice                - SFIS slice to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void cbrc_sfis_odd_parity_reset(cbrc_handle_t    *cbrc_handle,
                                        UINT8             slice)
{
    PMC_ENTRY();

    cbrc_sfis_rx_field_RX_ODD_INV_set(NULL,cbrc_handle, slice, 0);
    cbrc_sfis_tx_field_TX_ODD_INV_set(NULL,cbrc_handle, slice, 0);
    
    PMC_RETURN();
}

/*
** End of file
*/
