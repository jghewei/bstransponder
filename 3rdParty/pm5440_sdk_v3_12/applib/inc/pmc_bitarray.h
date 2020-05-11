/*******************************************************************************
*  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*  DESCRIPTION:
*     This header file provides the types, macros, inline functions, and
*     prototypes for the bit array operations used throughout the software.
*
*  NOTES:
*     None.
*
*******************************************************************************/
#ifndef _PMC_BITARRAY_H
#define _PMC_BITARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_types.h"
#include "pmc_err.h"


typedef UINT32  pmc_bitarray_t; /*!< Declare the size of the bitarray type */

/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/

/* Converts bit array size in bits to number of bit array base structures */
#define PMC_BITARRAY_SIZE(size_in_bits)     ((size_in_bits / 32) + 1)

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC void pmc_bitarray_set_bit(pmc_bitarray_t    bit_array[],
                                 UINT32            bit_location);

PUBLIC void pmc_bitarray_clr_bit(pmc_bitarray_t    bit_array[],
                                 UINT32            bit_location);

PUBLIC BOOL8 pmc_bitarray_is_bit_set(pmc_bitarray_t    bit_array[],
                                     UINT32            bit_location);

PUBLIC void pmc_bitarray_zero(pmc_bitarray_t    bit_array[],
                              UINT32            size_in_bits);

PUBLIC void pmc_bitarray_and(pmc_bitarray_t    output_array[],
                             pmc_bitarray_t    input_array[],
                             UINT32            size_in_bits);

PUBLIC void pmc_bitarray_or(pmc_bitarray_t    output_array[],
                            pmc_bitarray_t    input_array[],
                            UINT32            size_in_bits);

PUBLIC BOOL8 pmc_bitarray_is_all_zeroes(pmc_bitarray_t    bit_array[],
                                        UINT32            size_in_bits);

PUBLIC BOOL8 pmc_bitarray_is_equal(pmc_bitarray_t    bit_array1[],
                                   pmc_bitarray_t    bit_array2[],
                                   UINT32            size_in_bits);

PUBLIC void pmc_bitarray_print(pmc_bitarray_t      bit_array[],
                               UINT32              size_in_bits);

PUBLIC char * pmc_bitarray_get_array(pmc_bitarray_t      bit_array[],
                                     UINT32              size_in_bits);

PUBLIC UINT32 pmc_bitarray_pop_count(pmc_bitarray_t      bit_array[],
                                     UINT32              size_in_bits);

#ifdef __cplusplus
}
#endif

#endif /* _PMC_BITARRAY_H */

/*
** end of file
*/
