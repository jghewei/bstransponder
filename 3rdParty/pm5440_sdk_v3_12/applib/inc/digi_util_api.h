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
*     This file contains the function prototypes for DIGI utilities
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_UTIL_API_H
#define _DIGI_UTIL_API_H

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
* ENUM: digi_util_int_print_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to configure how to print interrupt/status
*
* ELEMENTS:
*   DIGI_UTIL_INT_PRINT_ALL     - Prints all fields in interrupt table
*   DIGI_UTIL_INT_PRINT_ACTIVE  - Prints only the active fields in the table
*   DIGI_UTIL_INT_PRINT_NONE    - Does not print anything. Used by the interrupt
*                                 printing functions to only return if an
*                                 interrupt has been found in the table.
*******************************************************************************/
typedef enum digi_util_int_print_t
{
    DIGI_UTIL_INT_PRINT_ALL = 0,
    DIGI_UTIL_INT_PRINT_ACTIVE,
    DIGI_UTIL_INT_PRINT_NONE,

    LAST_DIGI_UTIL_INT_PRINT,
} digi_util_int_print_t;


/*
** Global variables
*/


/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: digi_util_int_format_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Format for printing interrupts
*
* ELEMENTS:
*   See typedef
*******************************************************************************/
typedef struct digi_util_int_format_t
{
    UINT32      field_width;
    UINT32      value_width;
    UINT32      num_columns;
} digi_util_int_format_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR digi_util_int_odtu_dmx_chnl_print(digi_handle_t            *digi_handle,
                                                   odtu_dmx_int_chnl_t      *int_table_ptr,
                                                   digi_util_int_print_t     print_type,
                                                   digi_util_int_format_t   *format_ptr,
                                                   BOOL                     *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_odu_rfrm_chnl_print(digi_handle_t            *digi_handle,
                                                   odu_rfrm_int_chnl_t      *int_table_ptr,
                                                   digi_util_int_print_t     print_type,
                                                   digi_util_int_format_t   *format_ptr,
                                                   BOOL                     *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_mapotn_chnl_print(digi_handle_t            *digi_handle,
                                                 mapotn_int_chnl_t        *int_table_ptr,
                                                 digi_util_int_print_t     print_type,
                                                 digi_util_int_format_t   *format_ptr,
                                                 BOOL                     *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_otn_print(digi_handle_t         *digi_handle,
                                         digi_otn_int_t        *int_table_ptr,
                                         digi_util_int_print_t  print_type,
                                         BOOL                  *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_mapper_print(digi_handle_t         *digi_handle,
                                            digi_mapper_int_t     *int_table_ptr,
                                            digi_util_int_print_t  print_type,
                                            BOOL                  *int_found_ptr);

PUBLIC PMC_ERROR digi_util_cal_gfp_core_header(UINT16           pli,
                                               UINT16           *cHEC_ptr,
                                               UINT32           *core_header_ptr);
PUBLIC PMC_ERROR digi_util_cal_gfp_payload(UINT16       type,
                                           UINT16       *tHEC_ptr,
                                           UINT32       *payload_ptr);

#ifdef __cplusplus
}
#endif


#endif /* _DIGI_UTIL_API_H */

/*
** end of file
*/
