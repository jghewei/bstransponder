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

#ifndef _ETRANS_H
#define _ETRANS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "etrans_api.h"
#include "exco3m.h"
#include "exco3m_api.h"
#include "exco4m.h"
#include "exco4m_api.h"
#include "gsup43_c73.h"
#include "gsup43_c73_api.h"

/*
** Enumerated Types
*/

/* Status type */
/*******************************************************************************
* ENUM: etrans_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for etrans fifo status
*
* ELEMENTS:
*   ETRANS_RX_100G_FIFO_OVF  - 100G FIFO overflow
*   ETRANS_RX_100G_FIFO_OVF  - 40G FIFO overflow
*   ETRANS_RX_100G_FIFO_OVF  - 10G FIFO overflow
*
*******************************************************************************/
typedef enum
{
    ETRANS_RX_100G_FIFO_OVF,
    ETRANS_RX_40G_FIFO_OVF,
    ETRANS_RX_10G_FIFO_OVF
    /* Add interrupt names as needed */

} etrans_status_t;

/*******************************************************************************
* ENUM: etrans_data_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet line rates.  The
*   element terms have a syntax:
*      ETRANS_LINK_<rate>
*
* ELEMENTS:
*   ETRANS_LINK_10G  - Ethernet link is 10G
*   ETRANS_LINK_40G  - Ethernet link is 40G
*   ETRANS_LINK_100G - Ethernet link is 100G
*
*******************************************************************************/
typedef enum 
{
    ETRANS_LINK_10G  = 0x0,
    ETRANS_LINK_40G  = 0x1,
    ETRANS_LINK_100G = 0x2,
    LAST_ETRANS_DATA_RATE
} etrans_data_rate_t;


/*******************************************************************************
* ENUM: etrans_datapath_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for etrans datapath
*
* ELEMENTS:
*    ETRANS_GFP_GSUP43_C62_G709_100GE_MAC  - 100G
*    ETRANS_GFP_GSUP43_C62_G709_40GE_MAC - 40G 
*    ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC - 10G
*    ETRANS_GFP_GSUP43_C73_10GE_MAC - GSUP 10G
*    ETRANS_GMP_G709_100GE_MAC - 100G CBR
*    ETRANS_GMP_G709_40GE_MAC - 40G CBR
*    ETRANS_BMP_GSUP43_C71_10GE_MAC - 10G CBR
*    ETRANS_UNUSED - Unused 100G
*
*******************************************************************************/
typedef enum
{
    ETRANS_GFP_GSUP43_C62_G709_100GE_MAC,
    ETRANS_GFP_GSUP43_C62_G709_40GE_MAC,
    ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC,
    ETRANS_GFP_GSUP43_C73_10GE_MAC,
    ETRANS_GMP_G709_100GE_MAC,
    ETRANS_GMP_G709_40GE_MAC,
    ETRANS_BMP_GSUP43_C71_10GE_MAC,
    ETRANS_UNUSED
} etrans_datapath_t;

/*
** Constants
*/
#define ETRANS_NUM_GSUP43_C73 12
#define ETRANS_NUM_EXCO3M 3

/* #define BASE_ADDR_ETRANS_GSUP43_C73    0x00006000 */
/* #define ETRANS_GSUP43_C73_INST_OFFSET  0x0400 */

/*
** Macro Definitions
*/
#define ETRANS_10G_INT_TABLE_DEFINE() \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, RX_10G_FIFO_OVF , rx_10g_fifo_ovf_i      ); \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, RX_10G_FIFO_UDR , rx_10g_fifo_udr_i      ); \
    ETRANS_INT_FIELD(etrans_handle, etrans,   TX_LINK,   10G_FIFO_OVF    , tx_link_10g_fifo_ovf_i ); \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, TX_10G_FIFO_UDR , tx_10g_fifo_udr_i      ); \

#define ETRANS_10G_STATUS_TABLE_DEFINE() \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, RX_10G_FIFO_OVF_V , rx_10g_fifo_ovf_v      ); \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, RX_10G_FIFO_UDR_V , rx_10g_fifo_udr_v      ); \
    ETRANS_STATUS_FIELD(etrans_handle, etrans,   TX_LINK,   10G_FIFO_OVF_V    , tx_link_10g_fifo_ovf_v ); \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, TX_10G_FIFO_UDR_V , tx_10g_fifo_udr_v      ); \

#define ETRANS_40G_INT_TABLE_DEFINE() \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, RX_40G_FIFO_OVF, rx_40g_fifo_ovf_i ); \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, RX_40G_FIFO_UDR, rx_40g_fifo_udr_i ); \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, TX_40G_FIFO_OVF, tx_40g_fifo_ovf_i ); \
    ETRANS_INT_RANGE(etrans_handle, etrans, field_range, TX_40G_FIFO_UDR, tx_40g_fifo_udr_i ); \

#define ETRANS_40G_STATUS_TABLE_DEFINE() \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, RX_40G_FIFO_OVF_V, rx_40g_fifo_ovf_v ); \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, RX_40G_FIFO_UDR_V, rx_40g_fifo_udr_v ); \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, TX_40G_FIFO_OVF_V, tx_40g_fifo_ovf_v ); \
    ETRANS_STATUS_RANGE(etrans_handle, etrans, field_range, TX_40G_FIFO_UDR_V, tx_40g_fifo_udr_v ); \

#define ETRANS_100G_INT_TABLE_DEFINE() \
    ETRANS_INT_SINGLE(etrans_handle, etrans, RX_100G_FIFO_OVF , rx_100g_fifo_ovf_i ); \
    ETRANS_INT_SINGLE(etrans_handle, etrans, RX_100G_FIFO_UDR , rx_100g_fifo_udr_i ); \
    ETRANS_INT_SINGLE(etrans_handle, etrans, TX_100G_FIFO_OVF , tx_100g_fifo_ovf_i ); \
    ETRANS_INT_SINGLE(etrans_handle, etrans, TX_100G_FIFO_UDR , tx_100g_fifo_udr_i ); \

#define ETRANS_100G_STATUS_TABLE_DEFINE() \
    ETRANS_STATUS_SINGLE(etrans_handle, etrans, RX_100G_FIFO_OVF_V , rx_100g_fifo_ovf_v ); \
    ETRANS_STATUS_SINGLE(etrans_handle, etrans, RX_100G_FIFO_UDR_V , rx_100g_fifo_udr_v ); \
    ETRANS_STATUS_SINGLE(etrans_handle, etrans, TX_100G_FIFO_OVF_V , tx_100g_fifo_ovf_v ); \
    ETRANS_STATUS_SINGLE(etrans_handle, etrans, TX_100G_FIFO_UDR_V , tx_100g_fifo_udr_v ); \

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: etrans_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {

} etrans_cfg_t;

/*******************************************************************************
* STRUCTURE: etrans_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*  data_rate           - data rate 
*  datapath            - datapath
*  loopback            - loopback
*
*******************************************************************************/
typedef struct {
    etrans_data_rate_t data_rate[ETRANS_NUM_LINKS];
    etrans_datapath_t  datapath[ETRANS_NUM_LINKS];
    BOOL8 loopback[ETRANS_NUM_LINKS];
} etrans_var_t;

/*******************************************************************************
* STRUCTURE: etrans_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the etrans context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   exco3m_handle - EXCO3M handle
*   exco4m_handle - EXCO4M handle
*   gsup43_c73_handle - GSUP handle 
*******************************************************************************/
typedef struct etrans_handle_struct_t {
    pmc_handle_t base;

    etrans_cfg_t cfg;
    etrans_var_t var;

    exco3m_handle_t      *exco3m_handle[ETRANS_NUM_EXCO3M];
    exco4m_handle_t      *exco4m_handle;

    gsup43_c73_handle_t  *gsup43_c73_handle[ETRANS_NUM_GSUP43_C73];

} etrans_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC etrans_handle_t *etrans_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void   etrans_ctxt_destroy    (etrans_handle_t *etrans_handle);
PUBLIC void   etrans_handle_init     (etrans_handle_t *etrans_handle);
PUBLIC PMC_ERROR etrans_handle_restart_init(etrans_handle_t *etrans_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR etrans_init(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath, pmc_energy_state_request_t energy_state);
PUBLIC BOOL8 etrans_start_state_test(etrans_handle_t *hndl);
PUBLIC UINT32 etrans_base_address_get(etrans_handle_t *etrans_handle);

PUBLIC UINT32 etrans_intr_enable_set(etrans_handle_t *hndl, BOOL8 enable);
PUBLIC UINT32 etrans_intr_status_get(etrans_handle_t *hndl, etrans_status_t status_type);

PUBLIC UINT32 etrans_ingress_link_to_chan_map_get(etrans_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR etrans_sw_reset(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath);
PUBLIC PMC_ERROR etrans_lowpwr  (etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath, BOOL8 value);
PUBLIC void etrans_block_non_default_cfg(etrans_handle_t *hndl);
PUBLIC void etrans_var_default_init(etrans_handle_t *hndl);
PUBLIC void etrans_energy_state_reg_set ( etrans_handle_t *hndl, etrans_data_rate_t data_rate, pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR etrans_loopback_cfg(etrans_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC PMC_ERROR etrans_loopback_status_get(etrans_handle_t *hndl,
                                            UINT32 link,
                                            BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR etrans_ingress_link_to_chan_map_cfg(etrans_handle_t *hndl, UINT32 chnl, UINT32 link);
PUBLIC PMC_ERROR etrans_egress_chan_to_link_map_cfg (etrans_handle_t *hndl, UINT32 link, UINT32 chnl);

PUBLIC PMC_ERROR etrans_rx_cfg      (etrans_handle_t *hndl, UINT32 link, UINT32 chan, etrans_datapath_t datapath, BOOL8 enable);
PUBLIC PMC_ERROR etrans_tx_cfg       (etrans_handle_t *hndl, UINT32 link, UINT32 chan, etrans_datapath_t datapath, BOOL8 enable, BOOL8 use_page_swap, BOOL8 pmon_only);
PUBLIC PMC_ERROR etrans_rx_activate  (etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath);
PUBLIC PMC_ERROR etrans_rx_deactivate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath);
PUBLIC PMC_ERROR etrans_tx_activate  (etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath);
PUBLIC PMC_ERROR etrans_tx_deactivate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath);

PUBLIC PMC_ERROR etrans_egress_scheduler_cfg(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath, BOOL8 enable, BOOL8 use_page_swap);

PUBLIC UINT32 etrans_lenfifo_ovr_intr_enable_set_10g(etrans_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC BOOL8 etrans_lock_status_get(etrans_handle_t    *hndl, 
                                    UINT32              link, 
                                    etrans_datapath_t   datapath);
PUBLIC void etrans_activated_link_get(etrans_handle_t *etrans_handle,
                                      BOOL8           *rx_actv_link,
                                      BOOL8           *tx_actv_link);

PUBLIC void etrans_rx_link_rate_get(etrans_handle_t       *hndl,
                                    etrans_datapath_t     *datapath,
                                    etrans_data_rate_t    *rate);

PUBLIC void etrans_tx_link_rate_get(etrans_handle_t       *hndl,
                                    etrans_datapath_t     *datapath,
                                    etrans_data_rate_t    *rate);


PUBLIC void etrans_cleanup(etrans_handle_t *hndl,
                           UINT32           num_links, 
                           UINT32          *links);
PUBLIC PMC_ERROR etrans_int_chnl_enable(etrans_handle_t    *etrans_handle,
                                        UINT32              link,
                                        etrans_int_chnl_t  *int_table_ptr,
                                        etrans_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable);
PUBLIC PMC_ERROR etrans_int_chnl_clear(etrans_handle_t    *etrans_handle,
                                       UINT32              link,
                                       etrans_int_chnl_t  *int_table_ptr,
                                       etrans_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR etrans_int_chnl_retrieve(etrans_handle_t    *etrans_handle,
                                          UINT32              link,
                                          etrans_int_chnl_t  *filt_table_ptr,
                                          etrans_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR etrans_int_chnl_enabled_check(etrans_handle_t     *etrans_handle,
                                               UINT32               link,
                                               etrans_int_chnl_t   *int_en_table_ptr,
                                               BOOL8                *int_found_ptr);

PUBLIC PMC_ERROR etrans_tx_upi_set(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t    datapath,
                                   UINT8                pkt_upi_val,
                                   UINT8                os_upi_val,
                                   BOOL8                enable_drop);

PUBLIC PMC_ERROR etrans_tx_upi_get(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t   datapath,
                                   UINT8               *pkt_upi_val_ptr,
                                   UINT8               *os_upi_val_ptr,
                                   BOOL8               *enable_drop_ptr);
PUBLIC PMC_ERROR etrans_rx_upi_set(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t    datapath,
                                   UINT8                pkt_upi_val,
                                   UINT8                os_upi_val);

PUBLIC PMC_ERROR etrans_rx_upi_get(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t   datapath,
                                   UINT8               *pkt_upi_val_ptr,
                                   UINT8               *os_upi_val_ptr);

PUBLIC PMC_ERROR etrans_tx_lane_info_get(etrans_handle_t *hndl, 
                                         UINT32 link, 
                                         etrans_datapath_t etrans_path,  
                                         UINT8 txLaneId[20],
                                         UINT8 txLaneSkewFifoAddr[20]);

#ifdef __cplusplus
}
#endif

#endif /* _ETRANS_H */

/*
** end of file
*/
