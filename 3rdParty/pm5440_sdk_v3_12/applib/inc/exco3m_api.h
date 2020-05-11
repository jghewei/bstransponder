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

#ifndef _EXCO3M_API_H
#define _EXCO3M_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*
** Enumerated Types
*/



/*
** Constants
*/
#define EXCO3M_N_LANES 4

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct exco3m_handle_struct_t exco3m_handle_t; /*!< Declare exco3m handle */

/*******************************************************************************
* STRUCTURE: exco3m_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EXCO3M level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   tx_los_i;
    BOOL8   tx_lf_os_det_i;
    BOOL8   tx_rf_os_det_i;
    BOOL8   tx_idle_test_err_det_i;
    BOOL8   tx_skew_range_viol_i;
    BOOL8   tx_pcs_inv_blk_det_i;
    BOOL8   tx_pcs_err_blk_det_i;
    BOOL8   tx_pcs_hber_i;
    BOOL8   tx_pcs_lobl_i[EXCO3M_N_LANES];
    BOOL8   tx_pcs_ber_det_i[EXCO3M_N_LANES];
    BOOL8   tx_pcs_bip_err_det_i[EXCO3M_N_LANES];
    BOOL8   tx_loam_i[EXCO3M_N_LANES];
    BOOL8   tx_mrk_inv_det_i[EXCO3M_N_LANES];
    BOOL8   rx_tsf_i;
    BOOL8   rx_lf_os_det_i;
    BOOL8   rx_rf_os_det_i;
    BOOL8   rx_idle_test_err_det_i;
    BOOL8   rx_pcs_err_blk_det_i;
    BOOL8   rx_1027b_lobl_i;
    BOOL8   rx_1027b_hber_i;
    BOOL8   rx_1027b_ber_i;
    BOOL8   rx_skew_range_viol_i;
    BOOL8   rx_513b_pos_err_det_i;
    BOOL8   rx_well_pkt_err_i;
    BOOL8   rx_lane_algn_mrk_err_i;
    BOOL8   rx_1027b_par_err_i;
    BOOL8   rx_otn_bip_err_det_i[EXCO3M_N_LANES];
    BOOL8   rx_loam_i[EXCO3M_N_LANES];
    BOOL8   rx_pcs_mrk_inv_det_i[EXCO3M_N_LANES];
       
    BOOL8   tx_los_v;
    BOOL8   tx_pcs_hber_v;
    BOOL8   tx_pcs_lobl_v[EXCO3M_N_LANES];
    BOOL8   tx_loam_v[EXCO3M_N_LANES];
    BOOL8   rx_tsf_v;
    BOOL8   rx_1027b_lobl_v;
    BOOL8   rx_1027b_hber_v;
    BOOL8   rx_loam_v[EXCO3M_N_LANES];
} exco3m_int_chnl_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
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
#define EXCO3M_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXCO3M_LOG_CODE_DEBUG,    "EXCO3M: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXCO3M_ERR_CODE_ASSERT,   "EXCO3M: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXCO3M_LOG_CODE_LOG,      "EXCO3M: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXCO3M_ERR_INVALID_PARAMETERS,      "EXCO3M: Err invalid parameters" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum EXCO3M_LOG_ERR_INDEX
{
    EXCO3M_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_EXCO3M << 16),
    EXCO3M_LOG_ERR_TABLE
    EXCO3M_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char EXCO3M_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char EXCO3M_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _EXCO3M_API_H */

/*
** end of file
*/
