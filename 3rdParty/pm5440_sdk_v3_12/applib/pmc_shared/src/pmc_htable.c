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
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_log.h"
#include "pmc_htable.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/

PRIVATE UINT32 pmc_htable_hash(UINT32 key, UINT32 max_size);

/*******************************************************************************
* FUNCTION: pmc_htable_init
* ______________________________________________________________________________
*
* DESCRIPTION: Initilaize the hash tablle
*
* INPUTS:
*  htable_ptr - A pointer to a hash table descriptor
*  max_size    - Max numbeer of entries (SHALL be a power of 2)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR pmc_htable_init(pmc_htable_t * htable_ptr, UINT32 max_size)
{
    PMC_ERROR result = PMC_SUCCESS;
    htable_ptr->max_size = max_size;
    htable_ptr->table_ptr = (pmc_htable_entry_t * )PMC_CALLOC(max_size * sizeof(pmc_htable_entry_t));
    PMC_ASSERT(htable_ptr->table_ptr != NULL, PMC_ERR_FAIL,0,0);
    PMC_MEMSET(htable_ptr->table_ptr, 0, htable_ptr->max_size * sizeof(pmc_htable_entry_t));
    return(result);
} 

/*******************************************************************************
* FUNCTION: pmc_htable_uninit
* ______________________________________________________________________________
*
* DESCRIPTION: UniInitilaize the hash table
*
* INPUTS:
*  htable_ptr - A pointer to a hash table descriptor
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_htable_uninit(pmc_htable_t * htable_ptr)
{
    if (NULL != htable_ptr->table_ptr)
        PMC_FREE(&htable_ptr->table_ptr);
} 

/*******************************************************************************
* FUNCTION: pmc_htable_clear
* ______________________________________________________________________________
*
* DESCRIPTION: Clear hatble entries
*
* INPUTS:
*  htable_ptr - A pointer to a hash table descriptor
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR pmc_htable_clear(pmc_htable_t * htable_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_MEMSET(htable_ptr->table_ptr, 0, htable_ptr->max_size * sizeof(pmc_htable_entry_t));
    return(result);
} 

/*******************************************************************************
* FUNCTION: pmc_htable_add()
* ______________________________________________________________________________
*
* DESCRIPTION: Add a new item in the hash table 
*
* INPUTS:
*  htable_ptr - A pointer to a hash table descriptor
*  key        - how the new item will be indexed 
*  value      - the new value
*
* OUTPUTS: 
*
* RETURNS:
*   TRUE: item has been sucessfully added 
*   FALSE: add failed
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL pmc_htable_add(pmc_htable_t *htable_ptr, UINT32 key, UINT32 value)
{
    UINT32 pos,pos_search;
    UINT32 table_size = htable_ptr->max_size;
    pmc_htable_entry_t *table = htable_ptr->table_ptr;

    pos = pmc_htable_hash(key,table_size);
    
    pos_search = pos;
    while(table[pos_search].key != key && table[pos_search].key != 0)
    {
        pos_search = (pos_search +1) % table_size;
        if (pos_search == pos)
        {
            PMC_ASSERT(FALSE, PMC_ERR_FAIL,pos_search,pos);
            #ifdef HTABLE_DEBUG
            PMC_PRINT("add key table full\n");
            #endif
            return(FALSE); /* table full */
        }
    }
    if(table[pos_search].key == key)
    {
        #ifdef HTABLE_DEBUG
        PMC_PRINT("add key %d already present update value = %d\n",key,value);
        #endif
        table[pos_search].value = value;
    }
    else    
    {
        PMC_ASSERT(table[pos_search].key == 0, PMC_ERR_FAIL,pos_search,pos);
        table[pos_search].key= key;
        table[pos_search].value= value;
        #ifdef HTABLE_DEBUG
        PMC_PRINT("add key %d new value = %d\n",key,value);
        #endif
    }
    
    return(TRUE);
} 

/*******************************************************************************
* FUNCTION: pmc_htable_lookup()
* ______________________________________________________________________________
*
* DESCRIPTION: Get the contain of the table based on the key
*
* INPUTS:
*  htable_ptr - A pointer to a hash table descriptor
*  key        - the key
*
* OUTPUTS:
*  *value     - the retrieved value
*
* RETURNS:
*   TRUE: lookup was successfull
*   FALSE: lookup failed
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL pmc_htable_lookup(pmc_htable_t *htable_ptr, UINT32 key, UINT32 *value)
{
    UINT32 pos,pos_search;
    UINT32 table_size = htable_ptr->max_size;
    pmc_htable_entry_t *table = htable_ptr->table_ptr;

    pos = pmc_htable_hash(key,table_size);
    
    pos_search = pos;
    while(table[pos_search].key != key && table[pos_search].key != 0)
    {
        pos_search = (pos_search +1) % table_size;
        PMC_ASSERT(pos_search != pos, PMC_ERR_FAIL,pos_search,pos); /* table full */
    }
    
    /* not found */
    if (table[pos_search].key == 0)
    {
        #ifdef HTABLE_DEBUG
        PMC_PRINT("lookup - key %d not found\n", key);
        #endif
        *value = 0;
        return(FALSE);
    }
    PMC_ASSERT(table[pos_search].key == key, PMC_ERR_FAIL,pos_search,key);
    
    *value = table[pos_search].value;
    #ifdef HTABLE_DEBUG
    PMC_PRINT("lookup - key %d found value = %d\n",key,*value);
    #endif

    return(TRUE);
} 


/*******************************************************************************
* FUNCTION: pmc_htable_hash()
* ______________________________________________________________________________
*
* DESCRIPTION: A generic hash function. The efficiency of a has function 
*              depends on the application. 
*
* INPUTS:
* key - the key
* max_size - max number of entries
*
* OUTPUTS:
*  
* RETURNS:
*   UINT32 - the index in the table
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 pmc_htable_hash(UINT32 key, UINT32 max_size)
{
#define PMC_HTABLE_MAGIC_NO 357913941
    UINT32 h = key;

    h *= PMC_HTABLE_MAGIC_NO;
    h ^= h << 24;
    h += ~PMC_HTABLE_MAGIC_NO;
    h ^= h >> 31;
    h ^= h << 31;
    
    h &= (max_size -1);
    #ifdef HTABLE_DEBUG
    PMC_PRINT("key = %d, hash = %d max size = %d\n",key, h, max_size);
    #endif
    return(h);


} 

