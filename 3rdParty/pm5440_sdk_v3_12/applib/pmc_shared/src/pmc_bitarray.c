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
*     This file implements operators for bit arrays of variable size.
*     It uses a UINT32 as the base bit array size, and then operates on
*     on multiples of the bit array automatically so that the user does not
*     need to manage it.
*     
*     Examples:
*     - Create a bit array of 72 bits
*       pmc_bitarray_t  odu_pid_array[PMC_BIT_ARRAY_SIZE(72)];
*
*     - Set the 50th bit of the array
*       result = pmc_bitarray_set_bit(odu_pid_array, 50);
*
*     - OR together two arrays so that bits 1 and 10 are set.
*       pmc_bitarray_t  odu_pid_array1[PMC_BIT_ARRAY_SIZE(72)];
*       pmc_bitarray_t  odu_pid_array2[PMC_BIT_ARRAY_SIZE(72)];
*
*       result = pmc_bitarray_set_bit(odu_pid_array1, 1);   // output array
*       result = pmc_bitarray_set_bit(odu_pid_array2, 10);
*
*       result = pmc_bitarray_or(odu_pid_array1, odu_pid_array2);
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_log.h"
#include "pmc_bitarray.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

#define PMC_BITARRAY_BIT_LOC(bit_loc)      (bit_loc % 32)
#define PMC_BITARRAY_PRINT_ELEM_LEN        9  /* Characters per array element */
#define PMC_BITARRAY_PRINT_TAIL_LEN        1  /* Null termination */

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/

/*******************************************************************************
* FUNCTION: pmc_bitarray_loc_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a bit location in a bitarray, return which array element it is in,
*   where in the array element the bit is, and a bit array element with that
*   single bit set.
*
* INPUTS:
*   bit_location    - Location in a bit array
*
* OUTPUTS:
*   array_loc       - The bit array element that the requested bit is in
*   array_bit_loc   - The bit within the element that the requested bit is in
*   array_bit_set   - An array element with the array_bit_loc set
*
* RETURNS:
*   None 
*
* NOTES:
*
*******************************************************************************/
PRIVATE void pmc_bitarray_loc_get(UINT32            bit_location,
                                  UINT32           *array_loc,
                                  UINT32           *array_bit_loc,
                                  pmc_bitarray_t   *array_bit_set)
{
    PMC_ENTRY();

    /* Find which UINT32 array element the bit is located in */
    *array_loc = PMC_BITARRAY_SIZE(bit_location) - 1;

    /* Find where in the UINT32 the bit is located */
    *array_bit_loc = PMC_BITARRAY_BIT_LOC(bit_location);

    /* Return a bitarray element with the bit location set */
    *array_bit_set = 0;
    *array_bit_set |= (1 << *array_bit_loc);

    PMC_RETURN();
} /* pmc_bitarray_loc_get */

/*
** Public Functions
*/

/*******************************************************************************
* FUNCTION: pmc_bitarray_set_bit()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the specified bit in the bit array.
*
* INPUTS:
*   bit_array           - Bit array
*   bit_location        - Location of bit to set (0 index based)
*
* OUTPUTS:
*
* RETURNS:
*   None 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_set_bit(pmc_bitarray_t    bit_array[],
                                 UINT32            bit_location)
{
    UINT32          array_loc;
    UINT32          array_bit_loc;
    pmc_bitarray_t  array_bit_set;

    PMC_ENTRY();

    /* Find which UINT32 array element the bit is in and the bit location */
    pmc_bitarray_loc_get(bit_location, &array_loc, &array_bit_loc, &array_bit_set);
    bit_array[array_loc] |= array_bit_set;

    PMC_RETURN();
} /* pmc_bitarray_set_bit */

/*******************************************************************************
* FUNCTION: pmc_bitarray_clr_bit()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears the specified bit in the bit array.
*   
* INPUTS:
*   bit_array           - Bit array
*   bit_location        - Location of bit to clear (0 index based)
*
* OUTPUTS:
*
* RETURNS:
*   None 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_clr_bit(pmc_bitarray_t    bit_array[],
                                      UINT32            bit_location)
{
    UINT32          array_loc;
    UINT32          array_bit_loc;
    pmc_bitarray_t  array_bit_set;

    PMC_ENTRY();

    /* Find which UINT32 array element the bit is in and the bit location */
    pmc_bitarray_loc_get(bit_location, &array_loc, &array_bit_loc, &array_bit_set);
    bit_array[array_loc] &= ~array_bit_set;

    PMC_RETURN();
} /* pmc_bitarray_clr_bit */

/*******************************************************************************
* FUNCTION: pmc_bitarray_is_bit_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns if the bit at the specified location is set
*
* INPUTS:
*   bit_array           - Bit array
*   bit_location        - Location of bit to check (0 index based)
*
* OUTPUTS:
*
* RETURNS:
*   TRUE  - Bit is set
*   FALSE - Bit is not set
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 pmc_bitarray_is_bit_set(pmc_bitarray_t    bit_array[],
                                     UINT32            bit_location)
{
    UINT32          array_loc;
    UINT32          array_bit_loc;
    pmc_bitarray_t  array_bit_set;

    PMC_ENTRY();

    /* Find which UINT32 array element the bit is in and the bit location */
    pmc_bitarray_loc_get(bit_location, &array_loc, &array_bit_loc, &array_bit_set);

    if (bit_array[array_loc] & array_bit_set)
    {
        PMC_RETURN(TRUE);
    }

    PMC_RETURN(FALSE);
} /* pmc_bitarray_is_bit_set */

/*******************************************************************************
* FUNCTION: pmc_bitarray_and()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Logical ANDs together two bit arrays. 
*   output_array &= input_array
*
* INPUTS:
*   output_array    - Output bit array
*   input_array     - Input bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_and(pmc_bitarray_t    output_array[],
                             pmc_bitarray_t    input_array[],
                             UINT32            size_in_bits)
{
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        output_array[i] &= input_array[i]; 
    } 

    PMC_RETURN();
} /* pmc_bitarray_and */

/*******************************************************************************
* FUNCTION: pmc_bitarray_or()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Logical ORs together two bit arrays. 
*   output_array |= input_array
*   
*
* INPUTS:
*   output_array    - Output bit array
*   input_array     - Input bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   None 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_or(pmc_bitarray_t    output_array[],
                            pmc_bitarray_t    input_array[],
                            UINT32            size_in_bits)
{
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        output_array[i] |= input_array[i]; 
    }

    PMC_RETURN();
} /* pmc_bitarray_or */

/*******************************************************************************
* FUNCTION: pmc_bitarray_zero()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets entire bit array to zero
*
* INPUTS:
*   bit_array       - Bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   None 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_zero(pmc_bitarray_t    bit_array[],
                              UINT32            size_in_bits)
{
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        bit_array[i] = 0;
    } 

    PMC_RETURN();
} /* pmc_bitarray_zero */

/*******************************************************************************
* FUNCTION: pmc_bitarray_is_all_zeroes()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns if the entire bit array is zeroes  
*
* INPUTS:
*   bit_array       - Bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   TRUE    - Bit array is all zeroes
*   FALSE   - Bit array is not all zeroes
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 pmc_bitarray_is_all_zeroes(pmc_bitarray_t    bit_array[],
                                        UINT32            size_in_bits)
{
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        if (bit_array[i])
        {
            /* A non-zero bit was found */
            PMC_RETURN(FALSE);
        }
    }

    PMC_RETURN(TRUE);
} /* pmc_bitarray_is_all_zeroes */

/*******************************************************************************
* FUNCTION: pmc_bitarray_is_equal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns if two bit arrays are equal
*
* INPUTS:
*   bit_array1       - Bit array
*   bit_array2       - Bit array
*   size_in_bits     - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   TRUE    - Bit arrays are equal
*   FALSE   - Bit arrays are not equal
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 pmc_bitarray_is_equal(pmc_bitarray_t    bit_array1[],
                                   pmc_bitarray_t    bit_array2[],
                                   UINT32            size_in_bits)
{
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        if (bit_array1[i] != bit_array2[i])
        {
            /* Bit array elements were not equal*/
            PMC_RETURN(FALSE);
        }
    }

    PMC_RETURN(TRUE);
} /* pmc_bitarray_is_equal */

/*******************************************************************************
* FUNCTION: pmc_bitarray_print()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints bitarray.
* 
* INPUTS:
*   bit_array       - Bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
* 
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_bitarray_print(pmc_bitarray_t      bit_array[],
                               UINT32              size_in_bits)
{
    INT32       i;
    char       *str, *cur_str;
    UINT32      str_size;

    PMC_ENTRY();
    
    str_size = PMC_BITARRAY_PRINT_ELEM_LEN * PMC_BITARRAY_SIZE(size_in_bits) +
               PMC_BITARRAY_PRINT_TAIL_LEN;
     
    str = (char*) PMC_CALLOC(str_size);
    cur_str = str;

    for (i = PMC_BITARRAY_SIZE(size_in_bits) - 1; (i >= 0) && (str_size > 0); i--)
    {
        sprintf(cur_str, "%08x ", bit_array[i]);
        cur_str += PMC_BITARRAY_PRINT_ELEM_LEN;
        str_size -= PMC_BITARRAY_PRINT_ELEM_LEN;
    }

    PMC_LOG_TRACE("Bitarray: 0x%s\n", str);
    
    PMC_FREE(&str);

    PMC_RETURN();
} /* pmc_bitarray_print */

/*******************************************************************************
* FUNCTION: pmc_bitarray_get_array
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function that returns the bitarray as a string. Allocates memory used to
*  store the bit array that must be deallocated by the caller.
* 
*
* INPUTS:
*   bit_array       - Bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   String holding the bit array.
*
* NOTES:
*
*******************************************************************************/
PUBLIC char * pmc_bitarray_get_array(pmc_bitarray_t      bit_array[],
                                     UINT32              size_in_bits)
{
    INT32       i;
    char        *cur_str = NULL;
    UINT32      str_size;
    char *print_str_ptr = NULL;

    PMC_ENTRY();

    str_size = PMC_BITARRAY_PRINT_ELEM_LEN * PMC_BITARRAY_SIZE(size_in_bits) +
               PMC_BITARRAY_PRINT_TAIL_LEN;

    print_str_ptr = (char*) PMC_CALLOC(str_size);
    cur_str = print_str_ptr;

    for (i = PMC_BITARRAY_SIZE(size_in_bits) - 1; (i >= 0) && (str_size > 0); i--)
    {
        sprintf(cur_str, "%08x ", bit_array[i]);
        cur_str += PMC_BITARRAY_PRINT_ELEM_LEN;
        str_size -= PMC_BITARRAY_PRINT_ELEM_LEN;
    }

    PMC_RETURN(print_str_ptr);
} /* pmc_bitarray_get_array */



/*******************************************************************************
* FUNCTION: pmc_bitarray_pop_count_elem()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the population count on a 32bit bitmask.
*
* INPUTS:
*   i           - Value to perform population count on.
*
* OUTPUTS:
*
* RETURNS:
*   UINT32      - Number of 1's in the 32bit value
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 pmc_bitarray_pop_count_elem(UINT32   i)
{

    PMC_ENTRY();
    
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    i = (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    
    PMC_RETURN(i);
} /* pmc_bitarray_pop_count_elem */

/*******************************************************************************
* FUNCTION: pmc_bitarray_pop_count()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the population count of a bit array.
*
* INPUTS:
*   bit_array       - Bit array
*   size_in_bits    - Size of bitarray in bits
*
* OUTPUTS:
*
* RETURNS:
*   UINT32  - Number of 1s in the array.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 pmc_bitarray_pop_count(pmc_bitarray_t      bit_array[],
                                     UINT32              size_in_bits)
{
    UINT32          i;
    UINT32          count  = 0;

    PMC_ENTRY();

    
    for (i = 0; i < PMC_BITARRAY_SIZE(size_in_bits); i++)
    {
        count += pmc_bitarray_pop_count_elem(bit_array[i]);
    }


    PMC_RETURN(count);
} /* pmc_bitarray_pop_count */

/*
** End of file
*/
