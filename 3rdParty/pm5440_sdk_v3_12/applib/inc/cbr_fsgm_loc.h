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
#ifndef _CBR_FSGM_LOC_H
#define _CBR_FSGM_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "cbr_fsgm.h"    
#include "util_global.h"    


#include "cbr_fsgm_io.h"
#include "fc_cbr_patt_ins_io.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define CBR_FSGM_MAX_INDEX  4
/*
** Macro Definitions
*/
#define UTIL_GLOBAL_INT_FIELD4_ENABLE(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        switch(link)                                                            \
        {                                                                       \
            case 0:  tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 1:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 2:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 3:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            default: break;                                                     \
        }                                                                       \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_FIELD4_CLEAR(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        switch(link)                                                            \
        {                                                                       \
            case 0:  tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 1:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 2:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 3:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_FIELD4_RETRIEVE(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        switch(link)                                                            \
        {                                                                       \
            case 0:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 1:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 2:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 3:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_FIELD4_RETRIEVE(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        switch(link)                                                            \
        {                                                                       \
            case 0:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 1:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 2:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 3:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_FIELD4_CHECK(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        switch(link)                                                            \
        {                                                                       \
            case 0:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 1:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 2:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 3:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(NULL, handle); break;  \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)


/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: cbr_fsgm_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for inital values.
*
* ELEMENTS:
*   None.
*
*******************************************************************************/
typedef struct {
    
} cbr_fsgm_cfg_t;



/*******************************************************************************
* STRUCTURE: cbr_fsgm_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*   mode  - Configured mode for the link (e.g. OC-192, CPRI, etc).
*
*******************************************************************************/
typedef struct {
    cbr_fsgm_mode_t                 mode[CBR_FSGM_MAX_INDEX];
    cbr_fsgm_user_fault_pattern_t   user_pattern[CBR_FSGM_MAX_INDEX];
} cbr_fsgm_var_t;



/*******************************************************************************
* STRUCTURE: cbr_fsgm_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining CBR FSGM handle information.
*
* ELEMENTS: 
*   base                - Handle base information
*   cfg                 - Static config structure
*   var                 - Variable config structure
*
*******************************************************************************/    
struct cbr_fsgm_handle_struct_t {
    pmc_handle_t base;
    
    cbr_fsgm_cfg_t cfg;
    cbr_fsgm_var_t var;
       
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
BOOL cbr_fsgm_mode_is_40g(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                          UINT32                index);
BOOL cbr_fsgm_mode_is_oc192(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                            UINT32                index);

#ifdef __cplusplus
}
#endif

#endif /* _CBR_FSGM_LOC_H */

/* 
** end of file 
*/

