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

#ifndef _ETIME_H
#define _ETIME_H

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
#include "etime_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: etime_data_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet line rates.  The
*   element terms have a syntax:
*      ETIME_LINK_<rate>
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_LINK_10G = 0x0,
    ETIME_LINK_40G = 0x1,
    ETIME_LINK_100G = 0x2
} etime_data_rate_t;

/*******************************************************************************
* ENUM: etime_cfc_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for  CFC Timestamp mode.
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_CFC_48B_MODE  = 0x0,
    ETIME_CFC_47B_MODE  = 0x1
} etime_cfc_mode_t;

/*******************************************************************************
* ENUM: etime_refl_drop_err_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Reflection Packet Drop / Error Configuration
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_REFL_ERR = 0x0,
    ETIME_REFL_DROP = 0x1
} etime_refl_drop_err_t;

/*******************************************************************************
* ENUM: etime_refl_dst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Reflection Packet Destination 
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_REFL_DST_CPU = 0x0,
    ETIME_REFL_DST_MGMT = 0x1
} etime_refl_dst_t;


/*******************************************************************************
* ENUM: etime_ptp_freq_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for PTP Counter Frequency
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_PTP_CNTR_125MHZ = 0x0,
    ETIME_PTP_CNTR_155MHZ = 0x1
} etime_ptp_freq_t;


/*******************************************************************************
* ENUM: etime_line_ref_freq_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for LINE_REF Counter Frequency
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_LINE_REF_125MHZ = 0x0,
    ETIME_LINE_REF_155MHZ = 0x1
} etime_line_ref_freq_t;


/*******************************************************************************
* ENUM: etime_ts_page_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for TS_PAGE Counter Value 
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    ETIME_TS_PAGE_A = 0x0,
    ETIME_TS_PAGE_D = 0x1,
    ETIME_TS_PAGE_G = 0x2,
    ETIME_TS_PAGE_B = 0x3,
    ETIME_TS_PAGE_E = 0x4,
    ETIME_TS_PAGE_H = 0x5,
    ETIME_TS_PAGE_C = 0x6,
    ETIME_TS_PAGE_F = 0x7
} etime_ts_page_t;


/*******************************************************************************
* ENUM: etime_intr_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Interrupt Status type
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
  ETIME_NO_INTR        = 0x0,
  ETIME_GP             = 0x1,
  ETIME_SB_ECC_ERR     = 0x2,
  ETIME_MB_ECC_ERR     = 0x3,
  ETIME_EXT_RAM_OVR    = 0x4,
  ETIME_PKT_RAM_OVR    = 0x5,
  ETIME_PKT_RAM_DIS_WR = 0x6,
  ETIME_PKT_RAM_DIS_RD = 0x7,
  ETIME_TS_RAM_DIS_WR  = 0x8,
  ETIME_TS_RAM_DIS_RD  = 0x9,
  ETIME_EXT_DESYNC     = 0xA
} etime_intr_status_t;


/*******************************************************************************
* ENUM: etime_intr_value_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Interrupt Value type 
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
  ETIME_NO_VALUE  = 0x0,
  ETIME_INTR_RISE = 0x1,
  ETIME_INTR_FALL = 0x2
} etime_intr_value_t;


/*******************************************************************************
* ENUM: etime_irig_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for IRIG mode
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
  ETIME_IRIG_TOD  = 0x0,
  ETIME_IRIG_B00X = 0x1
} etime_irig_mode_t;

/*
** Constants
*/
#ifndef ETIME_NUM_LINKS 
#define ETIME_NUM_LINKS                                  12
#endif

/* Valid links */
#define ETIME_40G_LINK_0    0
#define ETIME_40G_LINK_4    4
#define ETIME_40G_LINK_8    8
#define ETIME_100G_LINK_0   0

/* Timestamper to Ingress line latency */
#define ETIME_TOD_ING_DLY_LANE0      0x0000
#define ETIME_TOD_ING_DLY_LANE1_40G  ETIME_TOD_ING_DLY_LANE0 + 6
#define ETIME_TOD_ING_DLY_LANE2_40G  ETIME_TOD_ING_DLY_LANE0 + 13
#define ETIME_TOD_ING_DLY_LANE1_100G ETIME_TOD_ING_DLY_LANE0 + 13
#define ETIME_TOD_ING_DLY_LANE2_100G ETIME_TOD_ING_DLY_LANE0 + 26
#define ETIME_CFC_ING_DLY_LANE0      0x0000
#define ETIME_CFC_ING_DLY_LANE1_40G  ETIME_CFC_ING_DLY_LANE0 + 6
#define ETIME_CFC_ING_DLY_LANE2_40G  ETIME_CFC_ING_DLY_LANE0 + 13
#define ETIME_CFC_ING_DLY_LANE1_100G ETIME_CFC_ING_DLY_LANE0 + 13
#define ETIME_CFC_ING_DLY_LANE2_100G ETIME_CFC_ING_DLY_LANE0 + 26
#define ETIME_TOD_EGR_DLY_LANE0      0x0000
#define ETIME_TOD_EGR_DLY_LANE1_40G  ETIME_TOD_EGR_DLY_LANE0 + 6
#define ETIME_TOD_EGR_DLY_LANE2_40G  ETIME_TOD_EGR_DLY_LANE0 + 13
#define ETIME_TOD_EGR_DLY_LANE1_100G ETIME_TOD_EGR_DLY_LANE0 + 13
#define ETIME_TOD_EGR_DLY_LANE2_100G ETIME_TOD_EGR_DLY_LANE0 + 26
#define ETIME_CFC_EGR_DLY_LANE0      0x0000
#define ETIME_CFC_EGR_DLY_LANE1_40G  ETIME_CFC_EGR_DLY_LANE0 + 6
#define ETIME_CFC_EGR_DLY_LANE2_40G  ETIME_CFC_EGR_DLY_LANE0 + 13
#define ETIME_CFC_EGR_DLY_LANE1_100G ETIME_CFC_EGR_DLY_LANE0 + 13
#define ETIME_CFC_EGR_DLY_LANE2_100G ETIME_CFC_EGR_DLY_LANE0 + 26

/* Reflection DPI scheduler constants */
#define ETIME_REFL_DPI_INACTIVE      0x15
#define ETIME_REFL_DFLT_OFFSET       0x1
#define ETIME_REFL_DFLT_LST_TS       0xB
#define ETIME_REFL_DFLT_10G_DLY      0x3FF
#define ETIME_REFL_DFLT_40G_DLY      0x3FE
#define ETIME_REFL_DFLT_100G_DLY     0x3FD

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: etime_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {
/*    
    UINT32 num_etime_chans;
*/    
} etime_cfg_t;


/*******************************************************************************
* STRUCTURE: etime_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.

*******************************************************************************/
typedef struct {
/*    
    BOOL8 etime_chan_en[ETIME_NUM_CHANNELS];
*/     
} etime_var_t;

/*******************************************************************************
* STRUCTURE: etime_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the etime context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*******************************************************************************/
typedef struct etime_handle_struct_t {
    pmc_handle_t base;
    
    etime_cfg_t cfg;
    etime_var_t var;
    
} etime_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC etime_handle_t *etime_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void etime_ctxt_destroy(etime_handle_t *etime_handle);
PUBLIC void etime_handle_init(etime_handle_t *hndl);
PUBLIC PMC_ERROR etime_handle_restart_init(etime_handle_t *hndl,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                           pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR etime_init(etime_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL8 etime_start_state_test(etime_handle_t *etime_handle);
PUBLIC UINT32 etime_base_address_get(etime_handle_t *etime_handle);

PUBLIC PMC_ERROR etime_refl_lnk_rst(etime_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR etime_rx_ts_cfg(etime_handle_t *hndl, UINT32 link, etime_data_rate_t data_rate, etime_cfc_mode_t cfc_mode, UINT32 link_latency);
PUBLIC PMC_ERROR etime_tx_ts_cfg(etime_handle_t *hndl, UINT32 link, etime_data_rate_t data_rate, etime_cfc_mode_t cfc_mode, UINT32 link_latency);
PUBLIC PMC_ERROR etime_refl_cfg(etime_handle_t *hndl, UINT32 link, etime_data_rate_t data_rate, etime_refl_drop_err_t drop_err, etime_refl_dst_t refl_dst, BOOL8 use_page_swap);
PUBLIC PMC_ERROR etime_rx_ts_ena(etime_handle_t *hndl, BOOL8 enable);
PUBLIC PMC_ERROR etime_tx_ts_ena(etime_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC PMC_ERROR etime_refl_ena(etime_handle_t *hndl, BOOL8 enable);
PUBLIC PMC_ERROR etime_ptp_cntr_cfg(etime_handle_t *hndl, UINT32 offset_quanta, etime_ptp_freq_t ptp_freq, BOOL8 enable);
PUBLIC PMC_ERROR etime_line_ref_cntr_cfg(etime_handle_t *hndl, UINT32 sampling_intvl, etime_line_ref_freq_t line_ref_freq, BOOL8 enable);
PUBLIC PMC_ERROR etime_tod_offset_upd(etime_handle_t *hndl, etime_ts_page_t ts_page, 
                                                            UINT32 offset_sec_hi, UINT32 offset_sec_lo, UINT32 offset_nsec);
PUBLIC PMC_ERROR etime_tod_upd_time_cfg(etime_handle_t *hndl, UINT32 upd_time_sec_hi, UINT32 upd_time_sec_lo, UINT32 upd_time_nsec);
PUBLIC PMC_ERROR etime_cfc_offset_upd(etime_handle_t *hndl, UINT32 offset_nsec_hi, UINT32 offset_nsec_lo);


/* Counters */
PUBLIC PMC_ERROR etime_frc_ptp_get(etime_handle_t *hndl, UINT32 *sec_cntr_ptr, UINT32 *nsec_cntr_ptr);
PUBLIC PMC_ERROR etime_refl_stats_cnt_get    (etime_handle_t    *hndl, 
                                              UINT32            link,
                                              etime_data_rate_t data_rate, 
                                              UINT32            *count_val_ptr);


/* IRIG */
PUBLIC void etime_irigi_config_get(etime_handle_t    *hndl,
                                   etime_irig_mode_t *mode,
                                   BOOL8              *is_enabled);

PUBLIC PMC_ERROR etime_irigi_config_set(etime_handle_t    *hndl,
                                        etime_irig_mode_t mode,
                                        BOOL8              enable);
PUBLIC PMC_ERROR etime_irigi_data_get  (etime_handle_t    *hndl,
                                        UINT32            *data4_ptr,
                                        UINT32            *data3_ptr,
                                        UINT32            *data2_ptr,
                                        UINT32            *data1_ptr);

PUBLIC void etime_irigo_config_get(etime_handle_t    *hndl,
                                   etime_irig_mode_t *mode,
                                   BOOL8              *is_enabled);

PUBLIC PMC_ERROR etime_irigo_config_set(etime_handle_t    *hndl,
                                        etime_irig_mode_t mode,
                                        BOOL8             enable);
PUBLIC PMC_ERROR etime_irigo_data_set  (etime_handle_t    *hndl,
                                        UINT32            data4,
                                        UINT32            data3,
                                        UINT32            data2,
                                        UINT32            data1);
PUBLIC void etime_block_non_default_cfg(etime_handle_t *hndl);
PUBLIC void etime_energy_state_reg_set(etime_handle_t *hndl,
                                       pmc_energy_state_t energy_state);

PUBLIC void etime_activated_link_get(etime_handle_t   *hndl,
                                     BOOL8           *rx_actv,
                                     BOOL8           *tx_actv_link);


#ifdef __cplusplus
}
#endif

#endif /* _ETIME_H */

/*
** end of file
*/
