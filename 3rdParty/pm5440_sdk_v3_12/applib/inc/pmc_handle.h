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
*   DESCRIPTION:
*     base handle for tsb's and mtsb's.
*
* // all handles put a pmc_handle_t as their first field:
* typedef struct mapotn_handle_t;
* {
*    pmc_handle_t base;
*    ...
* }
* 
* for the following examples suppose you have a tsb blocks as in the following:
*
* 0x00000000 digi
* 0x00200000 digi:mapotn
* 0x00200100 digi:mapotn:rgfp
*
* # define BASE_ADDR_MAPOTN 0x00200000
* # define BASE_ADDR_RGFP   0x00200100
*
* Then the top handle would be initialized with:
*
* // Top level mtsb goes:
* pmc_handle_init(NULL,digi,sys_handle,PMC_MID_DIGI,"DIGI",0);
* 
* // mapotn would use:
* pmc_handle_init(digi,mapotn,sys_handle,PMC_MID_DIGI_MAPOTN,"MAPOTN",BASE_ADDR_MAPOTN);
* 
* // rgfp would use:
* pmc_handle_init(mapotn,rgfp,sys_handle,PMC_MID_DIGI_RGFP,"RGFP",BASE_ADDR_RGFP);
* 
* // to log a list of all initialized handles
* pmc_handle_all_handles_print( digi );
* 
* // to find an direct child you
* mapotn = pmc_handle_by_name_find( digi, "MAPOTN" )
* rgfp = pmc_handle_by_name_find( rgfp, "RGFP" )
* 
* // to find a child by path
* mapotn = pmc_handle_by_name_find( digi, BASE_ADDR_MAPOTN )
* 
* // to find a child by address
* rgfp = pmc_handle_by_name_find( digi, BASE_ADDR_RGFP )
* 
* NOTES:
*
******************************************************************************/

#ifndef _PMC_HANDLE_H
#define _PMC_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*---- Include Files -------------------------------------------------------*/
#include "pmc_types.h"

/*---- Public Constant and Type Declarations ------------------------------*/

/** maximum length of a tsb_name */
#define PMC_HANDLE_MAX_TSB_NAME_LEN 30

/** maximum length of a path */
#define PMC_HANDLE_MAX_TSB_PATH_LEN 200

/** this macro is used to convert 2 legacy variable in a pointer */
#ifdef PMC_M64 
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_PTR(A,B)  (pmc_mutex_suspended_ctxt_t*) (((UINT64) A)<<32 | (UINT64) B)
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_A(C) (UINT32) (((UINT64) C) >> 32)
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_B(C) (UINT32) (((UINT64) C) & 0xFFFFFFFF)
#else 
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_PTR(A,B)  (pmc_mutex_suspended_ctxt_t*) B
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_A(C) 0
#define PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_B(C) (UINT32) C
#endif 

/*******************************************************************************
* STRUCTURE: pmc_handle_common_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Contains information that is unique to a DIGI instance.
*
* ELEMENTS:
*       hostmsg_handle       - pointer to the hostmsg handle
*       util_gen_db          - pointer to the DB metadata
*       log_buffer           - pointer to the circular log buffer
*       sw_version           - the software version
*       build_device_id      - the digi device ID the SW was compiled with
*       device_id            - the digi device ID as read from the device
*       device_revision_code - the digi device revision as read from the device
*
*******************************************************************************/
typedef struct pmc_handle_common_info_t 
{
    void*           hostmsg_handle;
    void*           util_gen_db;
    void*           log_buffer;
    UINT32          sw_version;
    UINT16          build_device_id;
    UINT16          device_id;
    UINT8           device_revision_code;
} pmc_handle_common_info_t;

/*******************************************************************************
* STRUCTURE: pmc_mutex_suspended_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure capture the context of a suspended thread
*   
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct pmc_mutex_suspended_ctxt_t 
{
    UINT32 thread_id; /*!< level of recursion currently at (lock/unlock only occurs at base level) */
    UINT32 recursive_level; /*!< id of the locking thread to check if recursive in current thread */
    struct pmc_mutex_suspended_ctxt_t *next;  /*!< pointer to the next pmc_mutext_suspended_ctxt_t */
} pmc_mutex_suspended_ctxt_t;

/*******************************************************************************
* STRUCTURE: pmc_mutex_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Mutex lock status structure
*   
* ELEMENTS:
*   user_ctxt - the context handle of the user code to manage the context
*   keep_lock - flag to indicate whether to keep the lock upon exit to all for bridging between functions
*   recursive_level - level of recursion currently at (lock/unlock only occurs at base level
*   current_thread_id - id of the locking thread to check if recursive in current thread
*******************************************************************************/
typedef struct pmc_mutex_ctxt_t 
{
    void *user_ctxt;
    BOOL8 keep_lock;
    UINT32 recursive_level;
    UINT32 current_thread_id;
    UINT32 suspended_recursive_level;
    UINT32 suspended_thread_id;    
} pmc_mutex_ctxt_t;

/*******************************************************************************
* STRUCTURE: pmc_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base information that is stored in every handle to allow it to be traced and
*   operated upon without having to know the full type of the handle.
*   
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct pmc_handle_t 
{
    pmc_sys_handle_t *sys_handle; /*!< opaque type of the customer's system access variables (like mmap handle) */
    PMC_VAR_CONST UINT32 base_address;     /*!< base address of this tsb (relative to the device, not the mtsb) */
    char tsb_name[PMC_HANDLE_MAX_TSB_NAME_LEN];          /*!< name used for debugging and by_name/by_path handle lookup */
    UINT32 tsb_mid;          /*!< module id used for logging and by_address lookup and for parameter validation */
    void*  ctxt;  /*!< Pointer to the context **/
    struct pmc_handle_common_info_t *common_info; /*!< Pointer to a struct holding information unique to a DIGI instance */
    pmc_mutex_ctxt_t    mutex_ctxt; /*!< mutex ctxt */
    struct pmc_handle_t *parent_handle;     /*!< handle of the parent (NULL for top level) */
    struct pmc_handle_t *next_sibling;  /*!< private variable used for traversing the handle tree */
    struct pmc_handle_t *first_child;   /*!< private variable used for traversing the handle tree */
    struct pmc_handle_t *last_child;    /*!< private variable used for building the handle tree */
} pmc_handle_t;

/*
 * parent and child should be of type pmc_handle_t.
 */
void pmc_handle_init( void *parent_handle, void *child_handle, pmc_sys_handle_t *sys_handle, UINT32 tsb_mid, const char *tsb_name, UINT32 base_address );

/**
 * displays debug messages of all the child handles and base addresses
 */
void pmc_handle_all_handles_print( /* pmc_handle_t */ void *pmc_handle );

/**
 * finds a direct child of a handle
 *
 * e.g. pmc_handle_by_name_find( digi_handle, "MAPOTN1" );
 */
pmc_handle_t *pmc_handle_by_name_find( /* pmc_handle_t */ void *pmc_handle, const char *name );

/**
 * finds a child of a handle based on a ":" separated path list
 *
 * e.g. pmc_handle_by_path_find( digi_handle, "MAPOTN1:RGFP2" );
 */
pmc_handle_t *pmc_handle_by_path_find( /* pmc_handle_t */ void *pmc_handle, char *path );

/**
 * finds a child at a particular absoluate address and with the specified module id.
 *
 * if you pass in a module id of 0 then just the base address must match.
 *
 * e.g. pmc_handle_by_address_find( digi_handle, PMC_MID_DIGI_MAPOTN, BASE_ADDR_MAPOTN_1_RGFP_1 )
 */
pmc_handle_t *pmc_handle_by_address_find( /* pmc_handle_t */ void *pmc_handle, UINT32 mid, UINT32 base_address );


/**
 * test the pointer is not NULL and tsb_mid to match the input MID
 *
  *
 * e.g. pmc_handle_by_address_find( digi_handle, PMC_MID_DIGI_MAPOTN )
 */

/* operator to get the immediate parent module for a module */
pmc_handle_t * pmc_handle_parent_get( void *_h );

/* operator to get the ultimate parent module for a module */
pmc_handle_t * pmc_handle_ultimate_parent_get( void *_h );

void pmc_handle_fix_sys_handle_ptrs(pmc_handle_t *pmc_handle);

#define pmc_handle_mid_verify(pmc_handle, mid) do \
{ \
    PMC_ASSERT((pmc_handle) != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, (mid), 0); \
    PMC_ASSERT((mid) == (pmc_handle)->base.tsb_mid, PMC_SHARED_ERR_CODE_MID_MISMATCH, (mid), (pmc_handle)->base.tsb_mid);  \
} while (0) /* pmc_handle_mid_verify */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_PMC_HANDLE_H*/


/* End of File */
