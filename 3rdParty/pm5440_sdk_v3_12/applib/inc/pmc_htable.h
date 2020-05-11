/******************************************************************************
*   COPYRIGHT (C) 2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*      This file provides the interfaces and public types for the hash table.
* 
*   NOTES:
*
********************************************************************************/

#ifndef PMC_HTABLE_H
#define PMC_HTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pmc_types.h"
#include "pmc_err.h"

/*******************************************************************************
* STRUCTURE: pmc_htable_entry_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure is used to create a key-value pair for the hash table
*
* ELEMENTS:
*   key - indexing to use to find the value
*   value - the value associated
*******************************************************************************/
typedef struct pmc_htable_entry_t
{
    UINT32 key;
    UINT32 value;    
} pmc_htable_entry_t;

/*******************************************************************************
* STRUCTURE: pmc_htable_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure is used to create and use a hash table
*
* ELEMENTS:
*   max_size - the maximum size of the hash table, must be a power of 2
*   table_ptr - pointer to the first entry in the table
*******************************************************************************/
typedef struct pmc_htable_t
{
    UINT32 max_size;
    pmc_htable_entry_t *table_ptr;
} pmc_htable_t;

PUBLIC PMC_ERROR pmc_htable_init(pmc_htable_t * htable_ptr, UINT32 max_size);
PUBLIC void pmc_htable_uninit(pmc_htable_t * htable_ptr);
PUBLIC BOOL pmc_htable_lookup(pmc_htable_t *htable_ptr, UINT32 key, UINT32 *value);
PUBLIC BOOL pmc_htable_add(pmc_htable_t *htable_ptr, UINT32 key, UINT32 value);
PUBLIC PMC_ERROR pmc_htable_clear(pmc_htable_t * htable_ptr);

#ifdef __cplusplus
}
#endif

#endif /* PMC_HTABLE_H */

/*
** end of file
*/

