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
#ifndef _GSUP43_C73_LOC_H
#define _GSUP43_C73_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "stdlib.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "gsup43_c73.h"
#include "gsup43_c73_io.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/
#define GSUP43_INT_CLEAR_SINGLE(handle, tsb_name, tsb_field, struct_field)      \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_IC_set_to_clear(NULL, handle, TRUE);      \
    }                                                                           \
} while(0)

#define GSUP43_INT_RETRIEVE_SINGLE(handle, tsb_name, tsb_field, struct_field)   \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_IC_get(tsb_name##_buffer_ptr, handle); \
    }                                                                           \
} while(0)

#define GSUP43_INT_CHECK_SINGLE(handle, tsb_name, tsb_field, struct_field)      \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        *int_found_ptr = tsb_name##_field_##tsb_field##_IC_get(NULL, handle);    \
    }                                                                           \
} while(0)


/*
** Structures and Unions
*/

/*
** Global variables
*/

/*
** Function Prototypes
*/
                                                                                                                                       
#ifdef __cplusplus
}
#endif

#endif /* _GSUP43_C73_LOC_H */

/* 
** end of file 
*/

