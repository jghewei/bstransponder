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
*     This file contains the function prototypes for the the CBRC subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _CBRC_API_H
#define _CBRC_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "cbr_fsgm_api.h"
#include "sdh_pmg_api.h"
#include "fc800_pmg_api.h"
#include "fc1200_pmon_api.h"
#include "mpmo_api.h"
#include "mpma_api.h"

/*
** Constants
*/
#ifndef DOXYGEN_PUBLIC_ONLY 
#define     CBRC_MAX_CHANNELS       12 
#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: cbrc_port_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the direction of traffic: receive and transmit.
*   This is used with cbrc_port_activate() and cbrc_port_deactivate().
*
* ELEMENTS:
*    CBRC_PORT_DIR_TX - Egress direction to be activated
*    CBRC_PORT_DIR_RX - Ingress direction to be activated
*    LAST_CBRC_PORT_DIR - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    CBRC_PORT_DIR_RX = 0,
    CBRC_PORT_DIR_TX,

    LAST_CBRC_PORT_DIR,
} cbrc_port_dir_t;




/*******************************************************************************
* ENUM: cbrc_fc1200_rx_lfos_cfg_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in cbrc_fc1200_rx_lfos_cfg_t
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_FC1200_RX_LFOS_CFG_MASK_LINK_FAIL_ENABLE = 0x1,
    CBRC_FC1200_RX_LFOS_CFG_MASK_SYNC_LOST_ENABLE = 0x2,
    CBRC_FC1200_RX_LFOS_CFG_MASK_HIGH_BER_ENABLE  = 0x4,
} cbrc_fc1200_rx_lfos_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: cbrc_fc1200_rx_lfos_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring FC1200 RX LFOS insertion options.
*
* ELEMENTS:
*   mask              - cbrc_fc1200_rx_lfos_cfg_mask_t bitmask to indicate config
*                       to set
*
*   link_fail_enable  - 1: Loss of signal causes LFOS to be asserted.
*                       0: Loss of signal has no effect on LFOS.
*
*   sync_lost_enable  - 1: Loss of sync causes LFOS to be asserted.
*                       0: Loss of sync has no effect on LFOS.
*  
*   high_ber_enable  - 1: High BER causes LFOS to be asserted.
*                      0: High BER has no effect on LFOS.
*
*
*******************************************************************************/
typedef struct {
    UINT32      mask;
    BOOL        link_fail_enable; 
    BOOL        sync_lost_enable; 
    BOOL        high_ber_enable; 
} cbrc_fc1200_rx_lfos_cfg_t;



/*******************************************************************************
* ENUM: cbrc_fc1200_tx_lfos_cfg_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in cbrc_fc1200_tx_lfos_cfg_t
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_FC1200_TX_LFOS_CFG_MASK_LOS_ENABLE = 0x1,
    CBRC_FC1200_TX_LFOS_CFG_MASK_EXT_ENABLE = 0x2,
} cbrc_fc1200_tx_lfos_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: cbrc_fc1200_tx_lfos_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring FC1200 TX LFOS insertion options.
*
* ELEMENTS:
*   mask              - cbrc_fc1200_tx_lfos_cfg_mask_t bitmask to indicate config
*                       to set
*
*   los_enable        - 1: System side LOS causes LFOS to be asserted.
*                       0: System side LOS has no effect on LFOS.
*  
*   ext_enable        - 1: LFOS insertion via external control is enabled
*                       0: LFOS insertion via external control is disabled
*
*
*******************************************************************************/
typedef struct {
    UINT32      mask;
    BOOL        los_enable; 
    BOOL        ext_enable; 
} cbrc_fc1200_tx_lfos_cfg_t;



/*******************************************************************************
* STRUCTURE: cbrc_fc1200_lfos_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for seting Local Fault Ordered Sets bits 
*
* ELEMENTS:
*   lfos - 64 bits to set for LFOS insertion
*
*
*******************************************************************************/
typedef struct {
    UINT32      lfos[2];
} cbrc_fc1200_lfos_t;



/*******************************************************************************
* STRUCTURE: cbrc_fc1200_ecb_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for seting Error Control Block bits 
*
* ELEMENTS:
*   ecb  - 64 bits to set for ECB insertion
*
*
*******************************************************************************/
typedef struct {
    UINT32      ecb[2];
} cbrc_fc1200_ecb_t;



/*******************************************************************************
* ENUM: cbrc_fc1200_rx_gfp_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in cbrc_fc1200_rx_gfp_t
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_FC1200_RX_GFP_MASK_PLI  = 0x0001,
    CBRC_FC1200_RX_GFP_MASK_CHEC = 0x0002,
    CBRC_FC1200_RX_GFP_MASK_PTI  = 0x0004,
    CBRC_FC1200_RX_GFP_MASK_PFI  = 0x0008,
    CBRC_FC1200_RX_GFP_MASK_EXI  = 0x0010,
    CBRC_FC1200_RX_GFP_MASK_UPI  = 0x0020,
    CBRC_FC1200_RX_GFP_MASK_THEC = 0x0040,
    CBRC_FC1200_RX_GFP_MASK_RES1 = 0x0080,
    CBRC_FC1200_RX_GFP_MASK_RES2 = 0x0100,
    CBRC_FC1200_RX_GFP_MASK_RES3 = 0x0200,
    CBRC_FC1200_RX_GFP_MASK_RES4 = 0x0400,
} cbrc_fc1200_rx_gfp_mask_t;


/*******************************************************************************
* STRUCTURE: cbrc_fc1200_rx_gfp_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring FC1200 RX GFP core header and payload
*   information bytes
*
* ELEMENTS:
*   mask  - cbrc_fc1200_rx_gfp_mask_t bitmask to indicate config
*           to set
*
*   pli   - 16 bits. The PLI bit field contains the value inserted into the PLI
*           field of the GFP core header. The default (0x225C) corresponds to a
*           8796 octet GFP payload.
*
*   chec  - 16 bits. The CHEC bit field contains the value inserted into the
*           cHEC field of the GFP core header. The default (0xFBFD) is the
*           correct CRC-16 for the default PLI value.
*
*   pti   - 3 bits. The PTI bit field contains the value inserted into the PTI
*           field of the GFP payload header. The default (0x0) corresponds to
*           "User data" as per Table 6-1 of ITU-T G.7041.
*
*   pfi   - 1 bit. The PFI bit field contains the value inserted into the PFI
*           field of the GFP payload header. The default (0x0) indicates the
*           presence of the GFP FCS field.
*
*   exi   - 4 bits. The EXI bit field contains the value inserted into the EXI
*           field of the GFP payload header. The default (0x0) indicates there
*           is no GFP extension header.
*
*   upi   - 8 bits. The UPI bit field contains the value inserted into the UPI
*           field of the GFP payload header. The default (0x15) is consistent
*           with Table 6-3 of ITU-T G.7041 (i.e. Transparent transcoded FC-1200).
*
*   thec  - 16 bits. The THEC bit field contains the value inserted into the
*           tHEC field of the GFP payload header. The default (0x41E7) is the
*           correct CRC-16 for the default payload header.
*
*   res1  - 32 bits. The RES1 bit field contains the value inserted into the
*           first four octets of the GFP payload information field. RES1[31:24]
*           is inserted into the first octet and RES1[7:0] is inserted into the
*           fourth octet.
*
*   res2  - 32 bits. The RES2 bit field contains the value inserted into the
*           fifth through the eighth octets of the GFP payload information field.
*           RES2[31:24] is inserted into the fifth octet and RES2[7:0] is
*           inserted into the eighth octet.
*
*   res3  - 32 bits. The RES3 bit field contains the value inserted into the 9th
*           through the 12th octets of the GFP payload information field.
*           RES3[31:24] is inserted into the 9th octet and RES3[7:0] is inserted
*           into the 12th octet.
*
*   res4  - 32 bits. The RES4 bit field contains the value inserted into the
*           13th through the 16th octets of the GFP payload information field.
*           RES4[31:24] is inserted into the 13th octet and RES4[7:0] is inserted
*           into the 16th octet.
*
*******************************************************************************/
typedef struct {
    UINT32   mask;

    UINT16   pli;
    UINT16   chec;
    UINT8    pti;
    UINT8    pfi;
    UINT8    exi;
    UINT8    upi;
    UINT16   thec;

    UINT32   res1;
    UINT32   res2;
    UINT32   res3;
    UINT32   res4;
} cbrc_fc1200_rx_gfp_t;

/*******************************************************************************
* STRUCTURE: cbrc_stl256_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CBRC STL256 level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {

    /* Interrupts */
    BOOL8  dais_i;
    BOOL8  oof_i;
    BOOL8  lane_fifo_ovr_i;
    BOOL8  lane0_oof_stl_i;
    BOOL8  lane1_oof_stl_i;
    BOOL8  lane2_oof_stl_i;
    BOOL8  lane3_oof_stl_i;
    BOOL8  lane0_dlof_stl_i;
    BOOL8  lane1_dlof_stl_i;
    BOOL8  lane2_dlof_stl_i;
    BOOL8  lane3_dlof_stl_i;
    BOOL8  dstl_ais_i;
    BOOL8  dlol_i;
    BOOL8  atsf_i;
    BOOL8  lane0_dlos_i;
    BOOL8  lane1_dlos_i;
    BOOL8  lane2_dlos_i;
    BOOL8  lane3_dlos_i;
    BOOL8  lane0_oor_stl_i;
    BOOL8  lane1_oor_stl_i;
    BOOL8  lane2_oor_stl_i;
    BOOL8  lane3_oor_stl_i;
    BOOL8  markers_not_unique_i;
    BOOL8  lane_id_not_consistent_i;
    BOOL8  excessive_skew_i;
    
    /* Status */
    BOOL8  dais_v;
    BOOL8  oof_v;
    BOOL8  lane0_oof_stl_v;
    BOOL8  lane1_oof_stl_v;
    BOOL8  lane2_oof_stl_v;
    BOOL8  lane3_oof_stl_v;
    BOOL8  lane0_dlof_stl_v;
    BOOL8  lane1_dlof_stl_v;
    BOOL8  lane2_dlof_stl_v;
    BOOL8  lane3_dlof_stl_v;
    BOOL8  dstl_ais_v;
    BOOL8  dlol_v;
    BOOL8  atsf_v;
    BOOL8  lane0_dlos_v;
    BOOL8  lane1_dlos_v;
    BOOL8  lane2_dlos_v;
    BOOL8  lane3_dlos_v;
    BOOL8  lane0_oor_stl_v;
    BOOL8  lane1_oor_stl_v;
    BOOL8  lane2_oor_stl_v;
    BOOL8  lane3_oor_stl_v;
    BOOL8  markers_not_unique_v;
    BOOL8  lane_id_not_consistent_v;
    BOOL8  excessive_skew_v;

} cbrc_stl256_int_chnl_t;


/*******************************************************************************
* STRUCTURE: cbrc_sfis_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CBRC SFIS level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {

    /* Interrupts */
    BOOL8   rxlos_dsc_i;
    BOOL8   rxoof_i;
    BOOL8   rxudr_i;
    BOOL8   rxudr_dsc_i;
    BOOL8   rxovr0_i;
    BOOL8   rxovr1_i;
    BOOL8   rxovr2_i;
    BOOL8   rxovr3_i;
    BOOL8   rxovr4_i;
    BOOL8   rxovr5_i;
    BOOL8   rxovr6_i;
    BOOL8   rxovr7_i;
    BOOL8   rxovr8_i;
    BOOL8   rxovr9_i;
    BOOL8   rxovr_dsc_i;
    BOOL8   rx_biterr_dsc_i;
    BOOL8   txovr_i;
    BOOL8   txudr_i;
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 rxlos_i          :10;
    UINT32 rxooa_i          :10;
    UINT32 rx_biterr_i      :10;
    UINT32 unused_rx_i_bits :2;     /*!< explicitily pad to 32 bit alignment */
#else
    UINT32 unused_rx_i_bits :2;     /*!< explicitily pad to 32 bit alignment */
    UINT32 rxlos_i          :10;
    UINT32 rxooa_i          :10;
    UINT32 rx_biterr_i      :10;
#endif
    
    /* Status */
    BOOL8   rxlos_dsc_v;
    BOOL8   rxoof_v;
    BOOL8   rxudr_v;
    BOOL8   rxudr_dsc_v;
    BOOL8   rxovr0_v;
    BOOL8   rxovr1_v;
    BOOL8   rxovr2_v;
    BOOL8   rxovr3_v;
    BOOL8   rxovr4_v;
    BOOL8   rxovr5_v;
    BOOL8   rxovr6_v;
    BOOL8   rxovr7_v;
    BOOL8   rxovr8_v;
    BOOL8   rxovr9_v;
    BOOL8   rxovr_dsc_v;
    BOOL8   rx_biterr_dsc_v;
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 rxlos_v          :10;
    UINT32 rxooa_v          :10;
    UINT32 rx_biterr_v      :10;
    UINT32 unused_rx_v_bits :2;     /*!< explicitly pad to 32 bit alignment */
#else
    UINT32 unused_rx_v_bits :2;     /*!< explicitly pad to 32 bit alignment */
    UINT32 rxlos_v          :10;
    UINT32 rxooa_v          :10;
    UINT32 rx_biterr_v      :10;
#endif

} cbrc_sfis_int_chnl_t;


/*******************************************************************************
* STRUCTURE: cbrc_sfi51_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CBRC SFI 5.1 level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {

    /* Interrupts */
    BOOL8   rx_deskew_los_i;
    BOOL8   rx_deskew_oof_i;
    BOOL8   rx_deskew_lof_i;
    BOOL8   rx_deskew_fifo_unfl_i;
    BOOL8   rx_deskew_fifo_ovfl_i;
    UINT32  rx_deskew_ooa_i  :16;
    UINT32  rx_chn_bit_err_i :16;
    BOOL8   tx_fifo_unfl_i;
    BOOL8   tx_fifo_ovfl_i;
    
    /* Status */
    BOOL8   rx_deskew_los_v;
    BOOL8   rx_deskew_oof_v;
    BOOL8   rx_deskew_lof_v;
    BOOL8   rx_deskew_fifo_unfl_v;
    BOOL8   rx_deskew_fifo_ovfl_v;
    UINT32  rx_deskew_ooa_v  :16;
    UINT32  rx_chn_bit_err_v :16;
    BOOL8   tx_fifo_unfl_v;
    BOOL8   tx_fifo_ovfl_v;

} cbrc_sfi51_int_chnl_t;


/*******************************************************************************
* STRUCTURE: cbrc_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CBRC level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8                   cbr_dint_fifo_uf_int_i;
    BOOL8                   cbr_dint_fifo_ovf_int_i;
    BOOL8                   cbr_dint_stag_buf_ovf_int_i;
    BOOL8                   pkt_dint_cfc_fifo_ovr_i;
    BOOL8                   pkt_dint_cfc_fifo_udr_i;
    BOOL8                   pkt_int_cfc_fifo_ovr_i;
    BOOL8                   pkt_int_cfc_fifo_udr_i;
   
    BOOL8                   cbr_dint_fifo_uf_int_v;
    BOOL8                   cbr_dint_fifo_ovf_int_v;
    BOOL8                   cbr_dint_stag_buf_ovf_int_v;
    BOOL8                   pkt_dint_cfc_fifo_ovr_v;
    BOOL8                   pkt_dint_cfc_fifo_udr_v;
    BOOL8                   pkt_int_cfc_fifo_ovr_v;
    BOOL8                   pkt_int_cfc_fifo_udr_v;



    /* SDH PMG */
    BOOL8                   sdh_pmg_valid;
    BOOL8                   sdh_pmg_valid_rx;
    BOOL8                   sdh_pmg_valid_tx;
    sdh_pmg_int_chnl_t      sdh_pmg_rx;
    sdh_pmg_int_chnl_t      sdh_pmg_tx;


    /* FC800 PMG */
    BOOL8                   fc800_pmg_valid;
    fc800_pmg_int_chnl_t    fc800_pmg_rx;    
    fc800_pmg_int_chnl_t    fc800_pmg_tx;    


    /* CBR FSGM */
    BOOL8                   cbr_fsgm_valid;
    cbr_fsgm_int_chnl_t     cbr_fsgm_rx;    
    cbr_fsgm_int_chnl_t     cbr_fsgm_tx;    
    

    /* FC1200 (PMON, PCS, TTTD, TTTM) */
    BOOL8                   fc1200_valid;
    fc1200_pmon_int_chnl_t  fc1200_pmon_rx;    
    fc1200_pmon_int_chnl_t  fc1200_pmon_tx;    

    BOOL8                   fc1200_pcs_rx_los_i;
    BOOL8                   fc1200_pcs_rx_loss_sync_i;
    BOOL8                   fc1200_pcs_rx_high_ber_i;
    BOOL8                   fc1200_pcs_rx_lf_i;
    BOOL8                   fc1200_pcs_rx_rf_i;
    BOOL8                   fc1200_pcs_tx_local_fault_i;
    BOOL8                   fc1200_pcs_tx_buf_underrun_i;
    BOOL8                   fc_tttd_crc24err_i;
    BOOL8                   fc_tttd_underrun_i;
    BOOL8                   fc_tttd_invalblkhdr_i;
    BOOL8                   fc_tttm_link_fail_i;
    BOOL8                   fc_tttm_hiber_i;
    BOOL8                   fc_tttm_losync_i;
    BOOL8                   fc_tttm_badblk_i;
    BOOL8                   fc_tttm_fovr_i;
    BOOL8                   fc_tttm_dpi_i;
    BOOL8                   tx_xfi_fifo_unfl_int_i;
    BOOL8                   tx_xfi_fifo_ovfl_int_i;
    BOOL8                   prgm_mon_err_i[16];
    BOOL8                   prgm_mon_sync_i[16];
    BOOL8                   shpi_paisci;
    BOOL8                   shpi_plopci;
    BOOL8                   shpi_paisi;
    BOOL8                   shpi_plopi;

    BOOL8                   fc1200_pcs_rx_los_v;
    BOOL8                   fc1200_pcs_rx_loss_sync_v;
    BOOL8                   fc1200_pcs_rx_high_ber_v;
    BOOL8                   fc1200_pcs_rx_lf_v;
    BOOL8                   fc1200_pcs_rx_rf_v;
    BOOL8                   fc1200_pcs_tx_local_fault_v;
    BOOL8                   fc1200_pcs_tx_buf_underrun_v;
    BOOL8                   fc_tttm_link_fail_v;
    BOOL8                   fc_tttm_hiber_v;
    BOOL8                   fc_tttm_losync_v;
    BOOL8                   prgm_mon_sync_v[16];
    BOOL8                   shpi_paiscv;
    BOOL8                   shpi_plopcv;
    BOOL8                   shpi_paisv;
    BOOL8                   shpi_plopv;

    /* MPMA/MPMO */
    mpma_int_chnl_t         mpma;
    mpmo_int_chnl_t         mpmo; 

    /* STL */
    BOOL8                  stl256_valid;
    cbrc_stl256_int_chnl_t stl256;

    /* SFIS */
    BOOL8                  sfis_valid;
    cbrc_sfis_int_chnl_t   sfis;

    /* SFI 5.1 */
    BOOL8                  sfi51_valid;
    cbrc_sfi51_int_chnl_t  sfi51;

} cbrc_int_chnl_t;


/*
** Macro Definitions
*/
/*
** Structures and Unions
*/
#ifndef DOXYGEN_PUBLIC_ONLY 
typedef struct cbrc_handle_struct_t cbrc_handle_t; /*!< Opaque reference to module handle */ 


#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*
** Log Messaging
*/

/* 
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums 
 */
#define CBRC_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_LOG_CODE_DEBUG,                  "CBRC: Log debug" )                             \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_CODE_ASSERT,                 "CBRC: Err assert" )                            \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_LOG_CODE_LOG,                    "CBRC: Log logging code" )                      \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_NULL_HANDLE,                 "CBRC: Null handle" )                           \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_NULL_PARAMETERS,             "CBRC: Null parameters" )                           \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_PRBS_ERR_CNT_READ_FAIL,      "CBRC: Cannot read PRBS error counter register" )                           \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_NOT_AVAIL,              "CBRC: Link unavailable" )                      \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_LINK,                "CBRC: Invalid link" )                          \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_STS1_PATH,           "CBRC: Invalid STS-1 path" )                          \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_ARG,                 "CBRC: Invalid argument" )                      \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_DB_ATTACH_FAIL,              "CBRC: DB Attachment Operation Failed" )        \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_DB_ENTRY_NOT_FOUND,          "CBRC: DB entry not found" )                  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_UNSUPPORTED_SCHD_MODE,       "CBRC: Invalid Scheduler Mode" )            \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_UNSUPPORTED_SCHD_DB_MODE,    "CPB: Scheduler Mode Does not Support Database" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_NOT_HEAD,               "CBRC: Provided link is not head of group" )    \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_PORT_NOT_PROV,               "CBRC: Port not provisioned" )                  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_DIR,                 "CBRC: Invalid direction specified" )           \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_DIR_ALREADY_ACTIVE,     "CBRC: Port dir already active" )           \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_DIR_NOT_ACTIVE,         "CBRC: Port dir not active" )                   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_NO_SW_CONTENT,               "CBRC: No SW content" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_PRBS_OH,             "CBRC: PRBS overhead type invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INVALID_PRBS_MODE,           "CBRC: PRBS mode invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_INCORRECT_PRBS_TYPE,         "CBRC: Incorrect PRBS type" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_PRBS_STILL_ENABLED,     "CBRC: Link PRBS is enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_MON_PRBS_ALREADY_SELECTED,   "CBRC: Only one link can be monitored" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_GEN_PRBS_ALREADY_SELECTED,   "CBRC: PRBS generator already enabled for one link" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_SDH_PRBS_CONFLICT,           "CBRC: PRBS generator and monitor cannot be activated for channels having different OC modes" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LINK_PRBS_NOT_ENABLED,       "CBRC: Link PRBS is not enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_SDH,             "CBRC: Feature requires link to be cfg for SDH" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_CBR,             "CBRC: Feature requires link to be cfg for FC800, GDPS, CPRI, FC1200 ODU1F/2F, or SDH" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_CPRI,            "CBRC: Feature requires link to be cfg for CPRI" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_GDPS,            "CBRC: Feature requires link to be cfg for GDPS" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_FC800,           "CBRC: Feature requires link to be cfg for FC800" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_FC1200,          "CBRC: Feature requires link to be cfg for FC1200 ODU1F/2F" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_FEATURE_REQ_FC1200_GFP,      "CBRC: Feature requires link to be cfg for FC1200 ODU2e GFP" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_LOOPBACK,                    "CBRC: Loopback Already Enabled" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_CONTEXT_INCOHERENT,                   "CBRC: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_CONTEXT_REG_INCOHERENT,               "CBRC: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_CONTEXT_CLEANUP,                      "CBRC: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBRC_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT, "CBRC: Communal ressource energy states incoherent" ) \
    

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum CBRC_LOG_ERR_INDEX
{
    CBRC_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_CBRC << 16),
    CBRC_LOG_ERR_TABLE
    CBRC_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char CBRC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char CBRC_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */
#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _CBRC_API_H */

/*
** end of file
*/
