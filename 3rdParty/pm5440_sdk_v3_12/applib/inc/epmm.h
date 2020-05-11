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

#ifndef _EPMM_H
#define _EPMM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_opsa.h"
#include "epmm_api.h"
#include "mpma.h"
#include "mpma_api.h"
#include "mpmo.h"
#include "mpmo_api.h"
#include "exco3m.h"
#include "exco3m_api.h"
#include "exco4m.h"
#include "exco4m_api.h"
#include "gsup43_c73.h"
#include "gsup43_c73_api.h"


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: epmm_data_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet line rates.  The
*   element terms have a syntax:
*      ENET_LINK_<rate>
*
* ELEMENTS:
*   ENET_LINK_10G  - Ethernet link is 10G
*   ENET_LINK_40G  - Ethernet link is 40G
*   ENET_LINK_100G - Ethernet link is 100G
*
*******************************************************************************/
typedef enum 
{
    EPMM_LINK_10G = 0x0,
    EPMM_LINK_40G = 0x1,
    EPMM_LINK_100G = 0x2,
    LAST_EPMM_DATA_RATE
} epmm_data_rate_t;


/*******************************************************************************
* ENUM: epmm_link_direction_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for link direction.
*
* ELEMENTS:
*   ENET_LINK_DIR_INGRESS  - Ingress datapath
*   ENET_LINK_DIR_EGRESS   - Egress datapath
*
*******************************************************************************/
typedef enum 
{
    EPMM_LINK_DIR_INGRESS = 1,
    EPMM_LINK_DIR_EGRESS  = 2,
    LAST_EPMM_LINK_DIR
} epmm_link_direction_t;

/*******************************************************************************
* ENUM: epmm_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for epmm fifo status
*
* ELEMENTS:
*   EPMM_ING_10G_OP_GSUP_FIFO_OVERFLOW  - FIFO overflow
*   EPMM_ING_10G_OP_GSUP_FIFO_UNDERRUN  - FIFO underflow
*
*******************************************************************************/
typedef enum
{
    EPMM_ING_10G_OP_GSUP_FIFO_OVERFLOW,
    EPMM_ING_10G_OP_GSUP_FIFO_UNDERRUN
    /* Add interrupt names as needed */
} epmm_status_t;

/*******************************************************************************
* ENUM: epmm_datapath_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for epmm datapath
*
* ELEMENTS:
*    EPMM_10GE_GSUP43_73                -10G Gsup
*    EPMM_10GE_GSUP43_73_HI_BER_MON     -10G Gsup used as EMAC 10G HI_BER monitor
*    EPMM_10GE_CBR                      - 10G CBR
*    EPMM_10GE_CBR_GSUP43_73_HI_BER_MON - 10G CBR and GSUP43_73 used as 10G HI_BER monitor
*    EPMM_UNUSED_10G                    - Unused 10G
*    EPMM_UNUSED_40G                    - Unused 40G
*    EPMM_UNUSED_100G                   - Unused 100G
*    EPMM_DATAPATH_UNUSED               - Sentinal
*    EPMM_40GE                          - 40G 
*    EPMM_100GE                         - 100G
*    
*******************************************************************************/
typedef enum
{
    EPMM_10GE_GSUP43_73                = 0x0,
    EPMM_10GE_GSUP43_73_HI_BER_MON     = 0x1,
    EPMM_10GE_CBR                      = 0x2,
    EPMM_10GE_CBR_GSUP43_73_HI_BER_MON = 0x3,

    EPMM_UNUSED_10G,
    EPMM_UNUSED_40G,
    EPMM_UNUSED_100G,
    EPMM_DATAPATH_UNUSED,
    EPMM_40GE,
    EPMM_100GE
} epmm_datapath_t;

/*
** Constants
*/
#define EPMM_NUM_LINK       12
#define EPMM_NUM_GSUP43_C73 EPMM_NUM_LINK
#define EPMM_NUM_EXCO3M 3

#define EPMM_MPMO_TOC 0x00


#define EPMM_NUM_DB_ENTRIES_MAX                                   96
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: epmm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {
/*
    UINT32 num_epmm_chans;
*/
} epmm_cfg_t;

/*******************************************************************************
* STRUCTURE: epmm_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*  cfg_pp_n            - packet period (numerator)
*  cfg_pp_d            - packet period (denominator)
*  cfg_pp_int          - packet period (integer)
*  mpmo_cfg_mode       - MPMO packet mode
*  data_rate           - data rate 
*  datapath            - datapath
*
*******************************************************************************/
typedef struct {
    UINT32 cfg_pp_n;
    UINT32 cfg_pp_d;
    UINT32 cfg_pp_int;
    BOOL8 mpmo_cfg_mode;
    epmm_data_rate_t data_rate[EPMM_NUM_LINK];
    epmm_datapath_t  datapath[EPMM_NUM_LINK];
} epmm_var_t;

/*******************************************************************************
* STRUCTURE: epmm_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the epmm context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   mpma_handle - MPMA handle 
*   mpmo_handle - MPMO handle
*   exco3m_handle - EXCO3M handle
*   exco4m_handle - EXCO4M handle
*   gsup43_c73_handle - GSUP handle 
*   util_schd_handle_mpma - util scheduler for MPMA 
*   util_schd_handle_mpmo - util scheduler for MPMO
*   mpma_db_alloc_mode - MPMA alloc method
*   mpmo_db_handle - MPMO db handle  
*   mpma_db_handle - MPMA db handle 
*******************************************************************************/
typedef struct epmm_handle_struct_t {
    pmc_handle_t base;

    epmm_cfg_t cfg;
    epmm_var_t var;

    mpma_handle_t        *mpma_handle;
    mpmo_handle_t        *mpmo_handle;
    exco3m_handle_t      *exco3m_handle[EPMM_NUM_EXCO3M];
    exco4m_handle_t      *exco4m_handle;
   
    gsup43_c73_handle_t  *gsup43_c73_handle[EPMM_NUM_GSUP43_C73];

    util_schd_handle_t    *util_schd_handle_mpma;
    util_schd_handle_t    *util_schd_handle_mpmo;

    util_schd_alloc_method_t mpma_db_alloc_mode;

    UINT32                 mpmo_db_handle;
    UINT32                 mpma_db_handle;
 
} epmm_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC epmm_handle_t *epmm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void epmm_ctxt_destroy      (epmm_handle_t *epmm_handle);
PUBLIC void epmm_handle_init       (epmm_handle_t *epmm_handle, BOOL8 mpmo_cfg_mode);
PUBLIC void epmm_alloc_init(epmm_handle_t *epmm_handle);
PUBLIC PMC_ERROR epmm_handle_restart_init (epmm_handle_t *epmm_handle, util_global_restart_init_cfg_t *restart_init_cfg_ptr, pmc_energy_state_t top_energy_state);
PUBLIC PMC_ERROR epmm_init(epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath, pmc_energy_state_request_t energy_state);
PUBLIC BOOL8 epmm_start_state_test(epmm_handle_t *hndl);
PUBLIC UINT32 epmm_base_address_get(epmm_handle_t *epmm_handle);
/*PUBLIC PMC_ERROR epmm_intr_enable_set(epmm_handle_t *hndl, UINT32 link, epmm_status_t status_type, BOOL8 enable);*/
PUBLIC PMC_ERROR epmm_intr_status_get(epmm_handle_t *hndl, epmm_status_t status_type);
PUBLIC PMC_ERROR epmm_sw_reset       (epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath);
PUBLIC PMC_ERROR epmm_lowpwr         (epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath, BOOL8 value);
PUBLIC PMC_ERROR epmm_mpmo_ckctl_cfg(epmm_handle_t *hndl,
                                     UINT32 ckctl_port, 
                                     UINT32 chnl,
                                     BOOL8 enable);
PUBLIC PMC_ERROR epmm_mpmo_data_get(epmm_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr);        
PUBLIC void epmm_var_default_init(epmm_handle_t *hndl);
PUBLIC void epmm_energy_state_reg_set ( epmm_handle_t *hndl, epmm_data_rate_t data_rate, pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR epmm_tx_lane_info_get(epmm_handle_t *hndl, 
                                       UINT32 link, 
                                       epmm_datapath_t epmm_path,  
                                       UINT8 txLaneId[20],
                                       UINT8 txLaneSkewFifoAddr[20]);
                                       
PUBLIC PMC_ERROR epmm_lf_os_cfg    (epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t epmm_path,                                     
                                    epmm_link_direction_t   dir,
                                    BOOL8 enable);

PUBLIC PMC_ERROR epmm_rf_os_cfg    (epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t epmm_path, 
                                    epmm_link_direction_t   dir,
                                    BOOL8 enable);
PUBLIC PMC_ERROR epmm_lf_os_cfg_get    (epmm_handle_t *hndl,
                                    UINT32 link,
                                    epmm_datapath_t epmm_path,
                                    epmm_link_direction_t   dir,
                                    BOOL8 *enable);

PUBLIC PMC_ERROR epmm_rf_os_cfg_get    (epmm_handle_t *hndl,
                                    UINT32 link,
                                    epmm_datapath_t epmm_path,
                                    epmm_link_direction_t   dir,
                                    BOOL8 *enable);

PUBLIC void epmm_lf_is_force_enable(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    BOOL8 *force_enable);


PUBLIC void epmm_lf_set_force_enable(epmm_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 enable);
PUBLIC PMC_ERROR epmm_lf_os_auto_insert_cfg(epmm_handle_t *hndl, 
                                            UINT32 link, 
                                            epmm_datapath_t epmm_path,  
                                            epmm_link_direction_t   dir,
                                            BOOL8 enable);
PUBLIC PMC_ERROR epmm_lf_os_auto_insert_cfg_get(epmm_handle_t *hndl,
                                            UINT32 link,
                                            epmm_datapath_t epmm_path,
                                            epmm_link_direction_t   dir,
                                            BOOL8 *enable);
PUBLIC PMC_ERROR epmm_rf_os_auto_insert_cfg(epmm_handle_t *hndl, 
                                            UINT32 link, 
                                            epmm_datapath_t epmm_path, 
                                            epmm_link_direction_t   dir,
                                            BOOL8 enable);
PUBLIC PMC_ERROR epmm_ingress_cfg(epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath, BOOL8 enable);
PUBLIC PMC_ERROR epmm_egress_cfg (epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath, BOOL8 enable);
PUBLIC PMC_ERROR epmm_mpma_data_get(epmm_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr,
                                    DOUBLE *epsilon_ptr);
PUBLIC PMC_ERROR epmm_mpmo_ch_prov(epmm_handle_t *hndl, UINT32 link, 
                                   DOUBLE client_bitrate, 
                                   UINT32 pkt_size, UINT32 pkt_per_int, 
                                   UINT32 pkt_per_n, UINT32 pkt_per_d,
                                   util_opsa_cn_frame_period_t cn_frame_period, 
                                   BOOL8 golden_cn_en);
PUBLIC PMC_ERROR epmm_mpmo_ch_deprov(epmm_handle_t *hndl, UINT32 link);                                   
PUBLIC PMC_ERROR epmm_rx_activate  (epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath);
PUBLIC PMC_ERROR epmm_rx_deactivate(epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath);
PUBLIC PMC_ERROR epmm_tx_activate  (epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath);
PUBLIC PMC_ERROR epmm_tx_deactivate(epmm_handle_t *hndl, UINT32 link, epmm_datapath_t datapath);

PUBLIC void epmm_activated_link_get(epmm_handle_t   *epmm_handle,
                                    BOOL8           *rx_actv_link,
                                    BOOL8           *tx_actv_link);

PUBLIC void epmm_rx_link_rate_get(epmm_handle_t       *hndl,
                                  epmm_datapath_t     *datapath,
                                  epmm_data_rate_t    *rate);

PUBLIC void epmm_tx_link_rate_get(epmm_handle_t       *hndl,
                                  epmm_datapath_t     *datapath,
                                  epmm_data_rate_t    *rate);

PUBLIC void epmm_mpmo_cleanup(epmm_handle_t  *hndl,
                              UINT32          num_links, 
                              UINT32         *links);

PUBLIC void epmm_cleanup(epmm_handle_t  *hndl,
                              UINT32          num_links, 
                              UINT32         *links);

PUBLIC PMC_ERROR epmm_int_chnl_enable(epmm_handle_t    *epmm_handle,
                                      UINT32            link,
                                      epmm_int_chnl_t  *int_table_ptr,
                                      epmm_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable);
PUBLIC PMC_ERROR epmm_int_chnl_clear(epmm_handle_t    *epmm_handle,
                                     UINT32            link,
                                     epmm_int_chnl_t  *int_table_ptr,
                                     epmm_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR epmm_int_chnl_retrieve(epmm_handle_t    *epmm_handle,
                                        UINT32            link,
                                        epmm_int_chnl_t  *filt_table_ptr,
                                        epmm_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR epmm_int_chnl_enabled_check(epmm_handle_t     *epmm_handle,
                                             UINT32             link,
                                             epmm_int_chnl_t   *int_en_table_ptr,
                                             BOOL8              *int_found_ptr);

PUBLIC PMC_ERROR epmm_tx_upi_set(epmm_handle_t   *hndl, 
                                 UINT32           link,
                                 epmm_datapath_t  datapath,                                 
                                 UINT8            pkt_upi_val,
                                 UINT8            os_upi_val,
                                 BOOL8            enable_drop);

PUBLIC PMC_ERROR epmm_tx_upi_get(epmm_handle_t    *hndl, 
                                 UINT32            link,
                                 epmm_datapath_t   datapath,                                 
                                 UINT8            *pkt_upi_val_ptr,
                                 UINT8            *os_upi_val_ptr,
                                 BOOL8            *enable_drop_ptr);

PUBLIC PMC_ERROR epmm_rx_upi_set(epmm_handle_t   *hndl, 
                                 UINT32           link,
                                 epmm_datapath_t  datapath,                                 
                                 UINT8            pkt_upi_val,
                                 UINT8            os_upi_val);

PUBLIC PMC_ERROR epmm_rx_upi_get(epmm_handle_t    *hndl, 
                                 UINT32            link,
                                 epmm_datapath_t   datapath,                                 
                                 UINT8            *pkt_upi_val_ptr,
                                 UINT8            *os_upi_val_ptr);

PUBLIC PMC_ERROR epmm_los_force_rf_action_set(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 BOOL8           enable);

PUBLIC UINT32 epmm_ing_reset_get(epmm_handle_t *hndl,
                                 UINT32 link,
                                 epmm_data_rate_t epmm_rate);

#ifdef __cplusplus
}
#endif

#endif /* _EPMM_H */

/*
** end of file
*/
