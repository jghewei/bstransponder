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

#ifndef _STS_SW_API_H
#define _STS_SW_API_H

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
* STRUCTURE: sts_sw_odtu_cnt_tot_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store ODTU Count total mappting for
*   LO containers.
*
* ELEMENTS:
*
*   
*******************************************************************************/
/* 
typedef enum
{
    ODTU_CNT_TOT_ODU0_TO_ODU4   = 317,
    ODTU_CNT_TOT_ODU0_TS_ODU0   = 318,    
    ODTU_CNT_TOT_ODU1_TS_ODU0   = 318,
    ODTU_CNT_TOT_ODU1_TS_ODU1   = 635,
    ODTU_CNT_TOT_ODU2-TS_ODU0   = 2539,
    ODTU_CNT_TOT_ODU2_TS_ODU1   = 1270,
    ODTU_CNT_TOT_ODU1E_TS_ODU0  = 2539,
    ODTU_CNT_TOT_ODU1F_TS_ODU0  = 2539,
    ODTU_CNT_TOT_ODU2E_TS_ODU0  = 2856,
    ODTU_CNT_TOT_ODU2F_TS_ODU0  = 2539,
    ODTU_CNT_TOT_
         
} sts_sw_odtu_cnt_tot_enum;
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


/*
** Global variables
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
#define STS_SW_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_LOG_CODE_DEBUG,                           "STS_SW: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_ERR_CODE_ASSERT,                          "STS_SW: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_LOG_CODE_LOG,                             "STS_SW: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_LOG_HOSTMSG,                              "STS_SW: Host message log" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_CHNL_STATE_NOT_START,                     "STS_SW: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_CHNL_STATE_NOT_EQUIPPED,                  "STS_SW: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_CHNL_STATE_NOT_OPERATIONAL,               "STS_SW: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_ERR_INVALID_PARAMETERS,                   "STS_SW: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_ERR_INVALID_PTR,                          "STS_SW: STS_SW handle is not valid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(STS_SW_TS_PORT_UNAVAIL,                          "STS_SW: Invalid TS port" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum STS_SW_LOG_ERR_INDEX
{
    STS_SW_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_STS_SW << 16),
    STS_SW_LOG_ERR_TABLE
    STS_SW_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char STS_SW_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char STS_SW_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _STS_SW_API_H */

/*
** end of file
*/
