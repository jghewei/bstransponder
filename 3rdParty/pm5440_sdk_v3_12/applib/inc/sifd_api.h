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
*     This file contains function prototypes, enumerations, and structures
*     for the SIFD subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _SIFD_API_H
#define _SIFD_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: sifd_ilkn_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum, defining the direction of data travel.
*
*
* ELEMENTS:
*  SIFD_ILKN_TX_DIRECTION         - Interlaken Tx Direction
*  SIFD_ILKN_RX_DIRECTION         - Interlaken Rx Direction
*
*******************************************************************************/
typedef enum sifd_ilkn_dir_t
{
    SIFD_ILKN_TX_DIRECTION           = 0,
    SIFD_ILKN_RX_DIRECTION           = 1
} sifd_ilkn_dir_t;

/*******************************************************************************
* ENUM: sifd_ilkn_test_control_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum, defining test pattern control.
*
* ELEMENTS:
*   SIFD_ILKN_TEST_NORMAL_OPERATION      - generate no pattern
*   SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN  - generate programmable pattern
*   SIFD_ILKN_TEST_PRBS                  - generate PRBS pattern
*   SIFD_ILKN_TEST_PRBS_INV              - generate inverted PRBS pattern
*   SIFD_ILKN_TEST_UNDEFINED             - unused, for internal error checking
*
*******************************************************************************/
typedef enum sifd_ilkn_test_control_state_t
{
    SIFD_ILKN_TEST_NORMAL_OPERATION           = 0,
    SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN,
    SIFD_ILKN_TEST_PRBS,
    SIFD_ILKN_TEST_PRBS_INV,
    SIFD_ILKN_TEST_UNDEFINED
} sifd_ilkn_test_control_state_t;

/*******************************************************************************
* ENUM: sifd_patt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors mode
*
* ELEMENTS:
*   SIFD_PATT_MODE_PRBS_7   - PRBS mode 7
*   SIFD_PATT_MODE_PRBS_11  - PRBS mode 11
*   SIFD_PATT_MODE_PRBS_15  - PRBS mode 15
*   SIFD_PATT_MODE_PRBS_23  - PRBS mode 23
*   SIFD_PATT_MODE_PRBS_31  - PRBS mode 31
*   SIFD_PATT_MODE_PRBS_9   - PRBS mode 9
*   SIFD_PATT_MODE_PRBS_29  - PRBS mode 29
*
*******************************************************************************/
typedef enum sifd_patt_mode_t
{
    SIFD_PATT_MODE_PRBS_7   = 0x000000c1,
    SIFD_PATT_MODE_PRBS_11  = 0x00000a01,
    SIFD_PATT_MODE_PRBS_15  = 0x0000c001,
    SIFD_PATT_MODE_PRBS_23  = 0x00840001,
    SIFD_PATT_MODE_PRBS_31  = 0x90000001,
    SIFD_PATT_MODE_PRBS_9   = 0x00000221,
    SIFD_PATT_MODE_PRBS_29  = 0x28000001,
    LAST_SIFD_PATT_MODE
} sifd_patt_mode_t;

/*******************************************************************************
* ENUM: sifd_int_dest_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying SIFD interrupts destination.
*
* ELEMENTS:
*   SIFD_INT_DEST_CPU_MIPS_1   - interrupt output 0 for MIPS CPU internal usage.
*   SIFD_INT_DEST_PCI          - interrupt output 1 for PCIe MSI/MSIx interrupts.
*   SIFD_INT_DEST_DEV_PIN      - interrupt output 2 for device pin.
*   SIFD_INT_DEST_CPU_MIPS_2   - interrupt output 3 for MIPS CPU internal usage.
*
*******************************************************************************/
typedef enum sifd_int_dest_t
{
   SIFD_INT_DEST_CPU_MIPS_1 = 0x0,
   SIFD_INT_DEST_PCI = 0x1,
   SIFD_INT_DEST_DEV_PIN = 0x2,
   SIFD_INT_DEST_CPU_MIPS_2 = 0x3,
   LAST_SIFD_INT_DEST
} sifd_int_dest_t;


/*
** Constants
*/
#define SIFD_ILKN_NULL_FLOW_CONTROL         255
#define SIFD_ILKN_LINK_FLOW_CONTROL         254
#define SIFD_ILKN_LINK_FLOW_CONTROL_2B      0xFFFE0000
#define SIFD_ILKN_NULL_FLOW_CONTROL_2B      0xFFFF0000


/* Number of SIFD interfaces */
#define SIFD_NUM_ILKN_DIR_MAX 2

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/




/*******************************************************************************
* STRUCTURE: sifd_int_link_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SIFD level link table for interrupt and status.
*
*   Note that rx_sl and sif_any are only valid when rx_wrd_sync_lane and
*   rx_crc32_err are both FALSE.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct sifd_int_link_t{

    BOOL8   fifo_over_i;
    BOOL8   fifo_under_i;
    BOOL8   oob_fc_err_i;
    BOOL8   tx_status_msg_change_i;
    BOOL8   tx_fc_cnt_fill_i;
    BOOL8   tx_chan_cnt_fill_i;
    BOOL8   ctlbit_err_i;
    BOOL8   mf_err_i;
    BOOL8   badctl_err_i;
    BOOL8   scram_err_i;
    BOOL8   crc32_err_i;
    BOOL8   cdr_err_i;
    BOOL8   wrd_sync_change_i;
    BOOL8   align_change_i;
    BOOL8   align_err_i;
    BOOL8   burst_err_i;
    BOOL8   bmin_err_i;
    BOOL8   bmax_err_i;
    BOOL8   crc24_err_i;
    BOOL8   ib_fc_err_i;
    BOOL8   rx_status_msg_change_i;
    BOOL8   badctl_cnt_fill_i;
    BOOL8   crc32_cnt_fill_i;
    BOOL8   cdr_cnt_fill_i;
    BOOL8   wrd_sync_cnt_fill_i;
    BOOL8   align_fail_cnt_fill_i;
    BOOL8   align_cnt_fill_i;
    BOOL8   burst_cnt_fill_i;
    BOOL8   bmin_cnt_fill_i;
    BOOL8   bmax_cnt_fill_i;
    BOOL8   crc24_cnt_fill_i;
    BOOL8   rx_fc_cnt_fill_i;
    BOOL8   rx_chan_cnt_fill_i;

    BOOL8   cpb_tx_buf_un_int_i0;
    BOOL8   cpb_tx_buf_align_int_i0;
    BOOL8   oduk_tx_buf_un_int_i0;
    BOOL8   oduk_tx_buf_align_int_i0;
    BOOL8   rx_buf_un_int_i0;
    BOOL8   rx_buf_align_int_i0;

    /* BOOL8 type indicate the whole 24 bits,
     * while enable/clear, TRUE/FALSE will set to all 24bits,
     * and while retrieve, the result is the "or" operation of all 24bits
     */
    BOOL8   ser_rx_los_int_i0;
    BOOL8   tx_sob_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_burst_len_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_burst_drop_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_pkt_ch_change_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_eob_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_sop_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];
    BOOL8   tx_eop_err_int_i0[SIFD_NUM_ILKN_DIR_MAX];

    BOOL8   rx_sob_err_int_i0;
    BOOL8   rx_burst_len_err_int_i0;
    BOOL8   rx_burst_drop_err_int_i0;
    BOOL8   rx_pkt_ch_change_err_int_i0;
    BOOL8   rx_eob_err_int_i0;
    BOOL8   rx_sop_err_int_i0;
    BOOL8   rx_eop_err_int_i0;
    BOOL8   rx_il_err_all_int_i0;
    BOOL8   rx_il_align_int_i0;

    /* BOOL8 type indicate all the 2 bits,
     * while enable/clear, TRUE/FALSE will set to the 2 bits,
     * and while retrieve, the result is the "or" operation of the 2 bits
     */
    BOOL8   rx_il_dat_err_int_i0;
    BOOL8   rx_oduk_h2b_cam_err_int_i0;
    BOOL8   rx_oduk_h2b_cam_mmatch_err_int_i0;
    BOOL8   rx_oduk_h2b_cmpr_err_int_i0;
    BOOL8   rx_oduk_h4b_cmpr_err_int_i0;

    /* Status */
    UINT32  rx_mf_err;
    UINT32  rx_wrd_sync_lane;
    UINT32  rx_wrd_sync_cnt_fill_lane;
    UINT32  rx_crc32_err;
    UINT32  rc_crc32_cnt_fill_lane;
    UINT32  rx_badctl_err;
    UINT32  rx_scram_err;
    UINT32  rx_cdr_err;
    UINT32  rx_crd_cnt_fill_lane;
    UINT32  rx_wrd_sync_chg;
    BOOL8   rx_align_good;
    BOOL8   rx_align_err;

    /* tx_stat_message_rxd */
    BOOL8   current;
    BOOL8   valid;
    BOOL8   sync;
    BOOL8   sif;
    UINT32  tx_sl;

    /* stat_message_rxd */
    BOOL8   rx_status_msg_valid_all;
    BOOL8   rx_status_msg_valid_any;
    BOOL8   rx_status_msg_sync;
    BOOL8   sif_all;
    UINT32  rx_sl;
    BOOL8   sif_any;

} sifd_int_link_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/


/*
** Log Messaging
*/

typedef struct sifd_handle_struct_t sifd_handle_t;  /*!< Opaque reference to module handle */

/*
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums
 */
#define SIFD_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_CODE_DEBUG,             "SIFD: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_ERR_CODE_ASSERT,            "SIFD: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_UNKNOWN,            "SIFD: Err unknown error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_NULL_HANDLE,        "SIFD: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_INVALID_PARAMETERS, "SIFD: Err invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_PRBS_MODE_NOT_FOUND, "SIFD: PRBS mode not incorrect" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_PRBS_STILL_ENABLED, "SIFD: PRBS is still enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_TEST_PATT_STILL_ENABLED, "SIFD: Programble test pattern is still enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_INVALID_PRECONFIG,  "SIFD: Err invalid preconfig" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_CODE_LOG,               "SIFD: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_ERR_DB_ENTRY_NOT_FOUND,    "SIFD: Err database entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_INVALID_STATE,      "SIFD: Log invalid state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_ERR_NO_SW_CONTENT, "SIFD: Temporary return code during development" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_ERR_SIF_SL_NOT_VALID, "SIFD: SIF and SL are not valid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_LOOPBACK_AMBIGUOUS, "SIFD: Loopback state is ambiguous" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_CONTEXT_INCOHERENT, "SIFD: Context and register space are incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_TEST_STATE_TRANSITION, "SIFD: Test state must be set to back normal before a new test state is started" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_LOOPBACK,  "SIFD: Loopback already active" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_RP_TSCC_MISMATCH,  "SIFD: RP/TSCC mismatch in HWRCOH" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SIFD_LOG_ERR_2B_HEADER_UPDATE_INVALID,  "SIFD: 2B header can only be updated with a value where at least 1 bit (0 or 1) remains static" )

/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum SIFD_LOG_ERR_INDEX
{
    SIFD_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_SIFD << 16),
    SIFD_LOG_ERR_TABLE
    SIFD_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char SIFD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char SIFD_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _SIFD_API_H */

/*
** end of file
*/
