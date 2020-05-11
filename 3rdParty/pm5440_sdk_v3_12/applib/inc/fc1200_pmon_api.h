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

#ifndef _FC1200_PMON_API_H
#define _FC1200_PMON_API_H

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

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct fc1200_pmon_handle_struct_t fc1200_pmon_handle_t; /*!< Opaque reference to module handle */ 



/*******************************************************************************
* STRUCTURE: fc1200_pmon_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   FC1200_PMON level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8 high_ber_int_i;
    BOOL8 loss_sync_int_i;
    BOOL8 los_int_i;

    BOOL8 high_ber_int_v;
    BOOL8 loss_sync_int_v;
    BOOL8 los_int_v;
} fc1200_pmon_int_chnl_t;

/*******************************************************************************
* ENUM: fc1200_pmon_dlolb_cfg_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in fc1200_pmon_dlolb_cfg_t
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    FC1200_PMON_DLOLB_CFG_MASK_LOS_ENABLE       = 0x1,
    FC1200_PMON_DLOLB_CFG_MASK_LOSS_SYNC_ENABLE = 0x2,
    FC1200_PMON_DLOLB_CFG_MASK_HIGH_BER_ENABLE  = 0x4,
} fc1200_pmon_dlolb_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: fc1200_pmon_dlolb_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring FC1200 RX PMON options.
*
* ELEMENTS:
*   mask                 - fc1200_pmon_dlolb_cfg_mask_t bitmask to indicate config
*                          to set
*
*
*   los_enable           - 1: Loss of signal causes DLOLB to be asserted.
*                          0: Loss of signal has no effect on DLOLB.
*
*   loss_sync_enable     - 1: Loss of sync causes DLOLB to be asserted.
*                          0: Loss of sync has no effect on DLOLB.
*  
*   high_ber_enable      - 1: High BER causes DLOLB to be asserted.
*                          0: High BER has no effect on DLOLB.
*
*
*******************************************************************************/
typedef struct {
    UINT32  mask;
    BOOL    los_enable; 
    BOOL    loss_sync_enable; 
    BOOL    high_ber_enable; 
} fc1200_pmon_dlolb_cfg_t;


/*******************************************************************************
* ENUM: fc1200_pmon_invalid_block_mode_t    
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to configure how blocks with invalid block type encodings are
*   handled when encountered during processing.
*  
*
* ELEMENTS:
*   FC1200_PMON_INVALID_BLOCK_MODE_IGNORED    - Blocks with an invalid block
*                                               type are ignored (not processed).
*   FC1200_PMON_INVALID_BLOCK_MODE_CONV_IDLE  - Blocks with an invalid block type
*                                               are converted to idle blocks
*                                               before processing.
*   FC1200_PMON_INVALID_BLOCK_MODE_CONV_ERROR - Blocks with an invalid block type
*                                               are converted to error blocks
*                                               before processing.
*   CBRC_FC1200_PMON_INVALID_BLOCK_MODE       - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    FC1200_PMON_INVALID_BLOCK_MODE_IGNORED    = 0x0,
    FC1200_PMON_INVALID_BLOCK_MODE_CONV_IDLE  = 0x1,
    FC1200_PMON_INVALID_BLOCK_MODE_CONV_ERROR = 0x2,

    LAST_FC1200_PMON_INVALID_BLOCK_MODE,
} fc1200_pmon_invalid_block_mode_t;




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
#define FC1200_PMON_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC1200_PMON_LOG_CODE_DEBUG,    "FC1200_PMON: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC1200_PMON_ERR_CODE_ASSERT,   "FC1200_PMON: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC1200_PMON_ERR_NULL_HANDLE,   "FC1200_PMON: Null handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC1200_PMON_ERR_INVALID_ARG,   "FC1200_PMON: Invalid argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC1200_PMON_LOG_CODE_LOG,      "FC1200_PMON: Log logging code" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum FC1200_PMON_LOG_ERR_INDEX
{
    FC1200_PMON_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_FC1200_PMON << 16),
    FC1200_PMON_LOG_ERR_TABLE
    FC1200_PMON_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char FC1200_PMON_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char FC1200_PMON_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _FC1200_PMON_API_H */

/*
** end of file
*/
