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

#ifndef _MP_MGEN_API_H
#define _MP_MGEN_API_H

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

typedef struct mp_mgen_handle_struct_t mp_mgen_handle_t;  /*!< Opaque reference to module handle see mp_mgen_handle_struct_t*/

/*******************************************************************************
* STRUCTURE: mp_mgen_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for initial configuration of MP_MGEN.
*   
* ELEMENTS:
*   
*   
*******************************************************************************/
typedef struct 
{
    /* To add static MP_MGEN config context */
    UINT32 trib_slot_num;
} mp_mgen_cfg_t;

/*******************************************************************************
* STRUCTURE: mp_mgen_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for dynamically changing configuration of MP_MGEN.
*   
*   
* ELEMENTS:
*
*   
*******************************************************************************/
typedef struct 
{
    /* To add dynamic MP_MGEN config context */
    
    
} mp_mgen_var_t;

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
#define MP_MGEN_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MP_MGEN_LOG_CODE_DEBUG,               "MP_MGEN: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MP_MGEN_ERR_CODE_ASSERT,              "MP_MGEN: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MP_MGEN_LOG_CODE_LOG,                 "MP_MGEN: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MP_MGEN_ERR_INVALID_PARAMETERS,       "MP_MGEN: Invalid parameters" )
    

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum MP_MGEN_LOG_ERR_INDEX
{
    MP_MGEN_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_MP_MGEN << 16),
    MP_MGEN_LOG_ERR_TABLE
    MP_MGEN_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char MP_MGEN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char MP_MGEN_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _MP_MGEN_API_H */

/*
** end of file
*/
