/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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

#ifndef _PMC_ERR_H
#define _PMC_ERR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "pmc_types.h"
#include "pmc_mid.h"

typedef UINT32 PMC_ERROR;  /*!< Global error type */

/* Error codes start at this base for each module ID */
#define PMC_ERR_BASE   0x0000

#define PMC_SUCCESS 0
#define PMC_ERR_INVALID_PARAMETERS            PMC_CREATE_UNIQUE_ID(PMC_MID_GLOBAL, 0x0002)  /* used by *_io_inline.h */
#define PMC_ERR_FAIL                          PMC_CREATE_UNIQUE_ID(PMC_MID_GLOBAL, 0x0001)  /* used by mapotn_rgfpf.c */
/* PMC Shared error codes */
/* 
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums 
 */
#define PMC_SHARED_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_LOG_CODE_DEBUG,          "PMC_SHARED: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_LOG_CODE_LEGACY,         "PMC_SHARED: LEGACY LOG code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CODE_ASSERT,         "PMC_SHARED: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_LOG_CODE_LOG,            "PMC_SHARED: Log logging code" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CODE_NULL_PTR,       "PMC_SHARED: Err Null Ptr" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CODE_POLL_TIMEOUT,   "PMC_SHARED: Poll Timeout" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CODE_MID_MISMATCH,   "PMC_SHARED: MID Mismatch" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CODE_TOO_MANY_LOG_MODULE, "PMC_SHARED: Too many log modules registered" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_FREE_NULL_PTR,       "PMC_SHARED: Freeing a NULL pointer\n" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_INVALID_PARAM,       "PMC_SHARED: Invalid parameter" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_NO_MEMORY,           "PMC_SHARED: Not enough free memory" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_NOT_REGISTERED,      "PMC_SHARED: Device update not registered." )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_MALLOC_FAILED,       "PMC_PLATFORM_LAYER: Memory allocation failed" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SW_SIM_ERR_POLL_TIMEOUT,        "PMC_SW_SIM: Err BUSY bit poll of indirect reg-VCAT LCAS mode" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_DEVICE_NOT_INIT,     "PMC_SHARED: Could not connect or intialize the specified device" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_MMAP_FAILED,         "PMC_SHARED: Could not mmap device" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_MEMSET_FAILED,       "PMC_PLATFORM_LAYER: PMC_MEMSET failed" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_MEMCPY_FAILED,       "PMC_PLATFORM_LAYER: PMC_MEMCPY failed" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_MEMCMP_FAILED,       "PMC_PLATFORM_LAYER: PMC_MEMCMP failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_STRLEN_FAILED,       "PMC_PLATFORM_LAYER: PMC_STRLEN failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_STRCAT_FAILED,       "PMC_PLATFORM_LAYER: PMC_STRCAT failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_STRCPY_FAILED,       "PMC_PLATFORM_LAYER: PMC_STRCPY failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_DEVOPEN_FAILED,      "PMC_PLATFORM_LAYER: Device open failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_FILE_OPEN_FAILED,    "PMC_SHARED: File open failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_DEVIOCTL_FAILED,     "PMC_PLATFORM_LAYER: Device ioctl failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_CALLOC_FAILED,  "PMC_CTXT_MGMT: PMC_CTXT_CALLOC failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_INVALID_POINTER,     "PMC_CTXT_MGMT: unknown pointer, not assigned by PMC_CTXT_CALLOC" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_INVALID_COUNTER,     "PMC_CTXT_MGMT: invalid value ctxt mgmt counter" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_REGISTER_FAILED, "PMC_CTXT_MGMT: ran out of room in the pointer registry" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_PTR_OO_BOUNDS,  "PMC_CTXT_MGMT: tried to register a non-context pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INCOHERENT,     "PMC_CTXT_MGMT: incoherent context" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_HEADER_TOTAL_SIZE,"PMC_CTXT_MGMT: header->total_size is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_HEADER_PTR_REG,   "PMC_CTXT_MGMT: header->ptr_reg is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_HEADER_DIGI_HDL,  "PMC_CTXT_MGMT: header->digi_hdl is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_HEADER_CALLOC_PTR,"PMC_CTXT_MGMT: header->next_calloc_ptr is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_HEADER_CTXT_PTR_REG,   "PMC_CTXT_MGMT: header->ctxt_ptr_registry is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CTXT_INVALID_REGISTERED_PTR,   "PMC_CTXT_MGMT: context pointer registry is corrupt." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_REG_INCOHERENT_ENERGY_STATE,  "PMC_RESTART: incoherent energy state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_POLL_FAILED,  "PMC_FW_QUEUE: poll failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_CRASH_EMULATION,  "PMC_SHARED: emulated crash event" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_LOG_QUEUE,  "PMC_FW_QUEUE" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_LOG_TRACE, "" )            \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_BIT_MASK_EXTRACT_FAIL, "PMC_SHARED: fail to extract the bit_mask" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_PCIE_FAILURE, "PMC_SHARED: pcie failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(PMC_SHARED_ERR_QUEUE_STALL, "PMC_SHARED: Message QUEUE STALL" ) \

    /* Trace entry - output is from trace statement and not expected to be returned as error */
    
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,

/*******************************************************************************
* ENUM: PMC_SHARED_LOG_ERR_INDEX
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Define the global error table
*   
* ELEMENTS:
*   See elements for description
*
*******************************************************************************/
enum PMC_SHARED_LOG_ERR_INDEX
{
    PMC_SHARED_LOG_ERR_TABLE_BASE = (PMC_MID_GLOBAL << 16),
    PMC_SHARED_LOG_ERR_TABLE
    PMC_SHARED_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PMC_ERR_H */

/* 
** end of file 
*/

