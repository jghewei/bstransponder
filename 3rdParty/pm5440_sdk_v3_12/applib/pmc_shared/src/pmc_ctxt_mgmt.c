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
/*
** Include Files
*/
#include <stdio.h>
#include "pmc_sys.h"
#include "pmc_log.h"
#include "pmc_ctxt_mgmt.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
/* Set PMC_CTXT_WORD_SIZE to the size of the largest type we use */
#define PMC_CTXT_WORD_SIZE sizeof(UINT64)

/*
** Local Structures and Unions
*/

/*
** Private Data
*/

/*
** Forward References
*/
PRIVATE void pmc_ctxt_check_ctxt(pmc_ctxt_mgmt_header_t *ctxt_header);

/*
** Public Functions
*/
/******************************************************************************
* FUNCTION: pmc_ctxt_mem_create
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Create the contiguous digi context memory block. This function does
*      the only system calloc call.
*
* INPUTS:
*      sys_handle - handle to the system/device (one per device)
*      is_restart - true/false whether a restart case. If true context is
*      restored via pmc_mem_ctxt_restore()
*
* OUTPUTS:
*      none
*
* RETURNS:
*      The pointer to the context memory block.
*
* NOTES:
******************************************************************************/
PUBLIC void *pmc_ctxt_mem_create(pmc_sys_handle_t *sys_handle,
                                 BOOL8 is_restart)
{

    UINT32              num_buf   = 1;
    UINT16              mem_type  = PMC_MEM_POOL_TYPE_CONTEXT;
    pmc_mem_align_enum  alignment = PMC_MEM_POOL_ALIGN_CTXT;
    pmc_ctxt_mgmt_ctxt_t *ctxt = NULL;
    pmc_mem_pool_handle mem_pool = NULL;

    PMC_ENTRY();

    mem_pool = pmc_mem_pool_create(sys_handle,sizeof(pmc_ctxt_mgmt_ctxt_t),
                                   num_buf,mem_type,alignment);
    ctxt = (pmc_ctxt_mgmt_ctxt_t *)pmc_mem_pool_alloc(mem_pool, sizeof(pmc_ctxt_mgmt_ctxt_t));
    PMC_ASSERT(ctxt != NULL, PMC_SHARED_ERR_MALLOC_FAILED, 0, 0);

    /*
     * If this is a restart then restore the context from where ever it
     * was stored. Then set the mem_pool in the context to ensure that it 
     * is not overwritten when the restore occurs.
     */
    if (is_restart)
    {
        pmc_mem_ctxt_restore((void *)ctxt, sizeof(pmc_ctxt_mgmt_ctxt_t), sys_handle);
    }
    else
    {
        /* setup header fields */
        ctxt->header.ptr_reg  = (UINT8 **)&ctxt->ptr_reg;
        ctxt->header.digi_hdl = (UINT8 *)&ctxt->data;
    }

    ctxt->header.mem_pool = mem_pool;

#ifdef PMC_PRINT_CALLOC_TO_FILE
    /* If first calloc, open file with 'wb' so it will overwrite
     * file if previously created, otherwise just append to file. */
    ctxt->header.f = fopen("calloc.txt","wb");
    fclose(ctxt->header.f);
#endif

    PMC_LOG_TRACE("%s\n%s%p\n%s%p\n%s%p\n",
                  "Context pointers at create:",
                  "\tctxt                  ", ctxt,
                  "\tctxt->header.ptr_reg  ", ctxt->header.ptr_reg,
                  "\tctxt->header.digi_hdl ", ctxt->header.digi_hdl);

    PMC_RETURN((void *)ctxt);
} /* pmc_ctxt_mem_create() */

/******************************************************************************
* FUNCTION: pmc_ctxt_mem_init
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Initialize the context, including setting the header values to initial
*      defaults and setting the context data to 0.
*
* INPUTS:
*      ctxt_handle - handle to the context to be initialized (one per device)
*
* OUTPUTS:
*      none
*
* RETURNS:
*      none
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_mem_init(void *ctxt_handle)
{
    pmc_ctxt_mgmt_ctxt_t *ctxt;

    PMC_ENTRY();
    ctxt = (pmc_ctxt_mgmt_ctxt_t *)ctxt_handle;

    ctxt->header.next_calloc_ptr = ctxt->header.digi_hdl;
    ctxt->header.total_size = sizeof(pmc_ctxt_mgmt_ctxt_t);
    ctxt->header.ctxt_calloc_cntr = 0;
    ctxt->header.ctxt_ptr_registry = ctxt->header.ptr_reg;

    /* Set the context data to 0 to emulate what would happen with PMC_CALLOC
     * We don't have to memset the pointer registry because we don't use values
     * in the registry that haven't been set first.
     */
    PMC_MEMSET(&ctxt->data, 0, sizeof(ctxt->data));
    PMC_RETURN();

} /* pmc_ctxt_mem_init() */

/******************************************************************************
* FUNCTION: pmc_ctxt_mem_destroy
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Free the context memory block
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      none
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_mem_destroy(void *ctxt_header)
{
    /*No error checking, pmc digi ctxt ptr set will log errors.*/
    pmc_ctxt_mgmt_header_t  *ctxt;
    UINT32 size_of_ctxt;

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    pmc_ctxt_check_ctxt(ctxt);
    PMC_LOG_TRACE("%s\n%s%p\n%s%p\n%s%p\n",
                  "Context pointers at destroy:",
                  "\tctxt           ", ctxt,
                  "\tctxt->ptr_reg  ", ctxt->ptr_reg,
                  "\tctxt->digi_hdl ", ctxt->digi_hdl);

    size_of_ctxt = sizeof(pmc_ctxt_mgmt_header_t) 
        + (sizeof(UINT8*) * PMC_CTXT_NUM_REG_PTRS)
        + (ctxt->next_calloc_ptr - ctxt->digi_hdl);
    PMC_LOG_TRACE("Context size at destroy: %d\n", size_of_ctxt);
    pmc_mem_pool_free(ctxt->mem_pool, (void **)&ctxt);
} /* pmc_ctxt_mem_destroy() */

/******************************************************************************
* FUNCTION: pmc_ctxt_calloc_function
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Handles all the ss/blk PMC_CTXT_CALLOC calls. Pointers returned are
*      word aligned. Will assert if there is not enough space in the context
*      memory block for the new block.
*
* INPUTS:
*      size        - the size to reserve in the context memory block
*      ctxt_header - pointer to the context header
*      func_name   - the name of the calling function
*      line_number - line in the file where allocation occurred
*
* OUTPUTS:
*      none
*
* RETURNS:
*      Pointer to the ss/blks block of memory its handle.
*
* NOTES:
******************************************************************************/
PUBLIC void *pmc_ctxt_calloc_function(UINT32 size, void *ctxt_header, const char* func_name, UINT32 line_number)
{
    void *ptr = NULL;
    pmc_ctxt_mgmt_header_t  *ctxt;
    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    pmc_ctxt_check_ctxt(ctxt);

    /* word align pointers */
    if (size % PMC_CTXT_WORD_SIZE != 0)
        size += (PMC_CTXT_WORD_SIZE - (size % PMC_CTXT_WORD_SIZE));

    ptr = (void *)ctxt->next_calloc_ptr;
    /* pointers are char * so pointer arithmetic would be correct */
    if((ctxt->next_calloc_ptr + size) <
            (ctxt->digi_hdl + PMC_CTXT_DIGI_HANDLE_SIZE))
    {
        ctxt->next_calloc_ptr = ctxt->next_calloc_ptr + size;
    } else
    {
        PMC_ASSERT( FALSE, PMC_SHARED_ERR_CTXT_CALLOC_FAILED, 0, 0 );
    }

    ctxt->ctxt_calloc_cntr++;
#ifdef PMC_PRINT_CALLOC_TO_FILE
    ctxt->f = fopen("calloc.txt","ab");
    fprintf(ctxt->f,"%d, %p, %p, %p, %d,\t%d,\t%s:%u\n", /* fprintf() allowed in porting function */
            ctxt->ctxt_calloc_cntr,
            ctxt->ptr_reg,
            ctxt->digi_hdl,
            ctxt->next_calloc_ptr,
            size,
            ctxt->total_size,
            func_name,
            line_number);
    fclose(ctxt->f);
#endif

    return(ptr);
} /* pmc_ctxt_calloc_function() */

/******************************************************************************
* FUNCTION: pmc_ctxt_free_function
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Free a given context pointer.
*
* INPUTS:
*      ptr         - a double pointer to the block to be freed
*      ctxt_header - pointer to the context header
*      func_name   - the name of the calling function
*
* OUTPUTS:
*      none
*
* RETURNS:
*      none
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_free_function(void **ptr, void *ctxt_header, const char* func_name)
{
    pmc_ctxt_mgmt_header_t  *ctxt;
    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    pmc_ctxt_check_ctxt(ctxt);

    if (ptr == NULL || *ptr == NULL)
    {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0);
    }
    else
    {
        ctxt->ctxt_calloc_cntr--;
        if (ctxt->ctxt_calloc_cntr == 0)
        {
            PMC_LOG_TRACE("calling pmc free on ctxt, should be digi handle = %p\n", ctxt->digi_hdl);
            PMC_LOG_TRACE("freeing %p\n",*ptr);
        }
        *ptr = NULL;
    }
} /* pmc_ctxt_free_function() */

/******************************************************************************
* FUNCTION: pmc_ctxt_digi_handle_get
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Calculate the location of the digi handle given a pointer to the
*      context.
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      pointer to the digi handle
*
* NOTES:
******************************************************************************/
PUBLIC void *pmc_ctxt_mgmt_digi_handle_get(void *ctxt_header)
{
    void *rv = NULL;
    pmc_ctxt_mgmt_header_t  *ctxt;
    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    pmc_ctxt_check_ctxt(ctxt);

    PMC_ENTRY();
    rv = (void *)(ctxt->digi_hdl);
    PMC_RETURN(rv);
} /* pmc_ctxt_mgmt_digi_handle_get() */

/******************************************************************************
* FUNCTION: pmc_ctxt_ptr_register
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Register the location of a pointer value that will need to be updated
*      when the context is reloaded.
*
* INPUTS:
*      ptr         - pointer to the pointer to be updated.
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      none
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_ptr_register(void* ptr, void* ctxt_header)
{
    pmc_ctxt_mgmt_header_t  *ctxt;

    PMC_ENTRY();
    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    pmc_ctxt_check_ctxt(ctxt);
    if (NULL != ptr) {
        /* Check that we haven't already registered this pointer. */
        if (NULL == pmc_ctxt_look_for_registered_ptr(ptr, ctxt_header))
        {
            PMC_ASSERT(ptr >= (void *)ctxt->digi_hdl && ptr < (void *)ctxt->next_calloc_ptr, PMC_SHARED_ERR_CTXT_PTR_OO_BOUNDS, 0, 0);
            *ctxt->ctxt_ptr_registry = (UINT8 *)ptr;
            ctxt->ctxt_ptr_registry++;
            PMC_ASSERT((void *)ctxt->ctxt_ptr_registry < (void *)ctxt->digi_hdl,PMC_SHARED_ERR_CTXT_REGISTER_FAILED,0,0);
            if (NULL != *((void **)ptr))
            {
                PMC_ASSERT(*((void **)ptr) >= (void *)ctxt && *((void **)ptr) < (void *)ctxt->next_calloc_ptr, PMC_SHARED_ERR_CTXT_PTR_OO_BOUNDS, 0, 0);
            }
        }
    };
    PMC_RETURN();
};


/******************************************************************************
* FUNCTION: pmc_ctxt_look_for_registered_ptr
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Search the pointer registry for the given pointer.
*
* INPUTS:
*      ptr - pointer to be searched for.
*      ctxt_header - pointer to the context header
*
* OUTPUTS(void *):
*      none
*
* RETURNS:
*      Pointer to the location in the registry that contains the given pointer.
*
* NOTES:
******************************************************************************/
PUBLIC void *pmc_ctxt_look_for_registered_ptr(void* ptr, void* ctxt_header)
{
    UINT8** index;
    void   *rv;
    pmc_ctxt_mgmt_header_t  *ctxt;
    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;

    PMC_ENTRY();

    rv = NULL;
    for (index = (UINT8 **)ctxt->ptr_reg; index < ctxt->ctxt_ptr_registry; index++)
    {
        if ((UINT8 *)ptr == *index) {
            rv = (void *)index;
            break;
        };
    };
    PMC_RETURN(rv);
};

/******************************************************************************
* FUNCTION: pmc_ctxt_log_unregistered_pointers
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Looks at every unregistered word in the context.  If the word contains
*      a value that looks like a pointer to something in the context, it is
*      logged.
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      nothing
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_log_unregistered_pointers(void* ctxt_header)
{
    PMC_ENTRY();

#ifdef USE_PMC_CTXT_CALLOC
    /* Commented out for now because Valgrind has problems with looking at
     * uninitialized pointers
     *
    UINT8** search_ptr;
    UINT8*** search_ptr_ptr;
    pmc_ctxt_mgmt_header_t  *ctxt;

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;
    search_ptr = NULL;

    PMC_ASSERT(NULL != ctxt, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0 );
    PMC_ASSERT(NULL != ctxt->digi_hdl, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0 );
    PMC_ASSERT(NULL != ctxt->next_calloc_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0 );
    pmc_ctxt_check_ctxt(ctxt);
    for (search_ptr = (UINT8 **)ctxt->digi_hdl; search_ptr < (UINT8 **)ctxt->next_calloc_ptr; search_ptr++)
    {

        if ((*search_ptr >= (UINT8 *)ctxt) 
            && 
            (*search_ptr < ctxt->next_calloc_ptr) 
            &&
            (NULL == pmc_ctxt_look_for_registered_ptr((void *)search_ptr, ctxt_header)))
        {

            PMC_LOG_TRACE("WARNING: unregistered %p at: %p\n", *search_ptr, search_ptr);

        }
    }

    for (search_ptr_ptr = (UINT8 ***)ctxt->ptr_reg; search_ptr_ptr < (UINT8***)ctxt->ctxt_ptr_registry; search_ptr_ptr++ )
    {
        if (((**search_ptr_ptr < (UINT8 *)ctxt)
            ||
            (**search_ptr_ptr > ctxt->next_calloc_ptr))
            &&
            (**search_ptr_ptr != NULL))
        {
            PMC_LOG_TRACE("WARNING: registered pointer to outside the context %p at: %p\n", **search_ptr_ptr, *search_ptr_ptr);
        }
    }
    */
#endif

    PMC_RETURN();
};

/******************************************************************************
* FUNCTION: pmc_ctxt_update_pointers
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Determines the offset between the pointer passed in and where the
*      pointer used to point to by looking at the header fields.
*      Then, the function updates the context header, the pointer registry and
*      all the pointers the registry points to.
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      nothing
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_update_pointers(void* ctxt_header)
{
    PMC_PTRDIFF             offset;
    pmc_ctxt_mgmt_header_t  *ctxt;
    UINT8                   ***index;

    PMC_ENTRY();

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;

    offset = (PMC_PTRDIFF)(ctxt) - ((PMC_PTRDIFF)(ctxt->ptr_reg) - sizeof(pmc_ctxt_mgmt_header_t));

    /* OK, now update the header */
    ctxt->ptr_reg           = (UINT8**)((PMC_PTRDIFF)(ctxt->ptr_reg) + offset);
    ctxt->digi_hdl          = (UINT8* )((PMC_PTRDIFF)(ctxt->digi_hdl) + offset);
    ctxt->next_calloc_ptr   = (UINT8* )((PMC_PTRDIFF)(ctxt->next_calloc_ptr) + offset);
    ctxt->ctxt_ptr_registry = (UINT8**)((PMC_PTRDIFF)(ctxt->ctxt_ptr_registry) + offset);
    /* Note that the file pointer doesn't need to be reset, since it gets
     * opened/closed every time it's used.
     */

    /* Now the pointer registry and everything it points to */
    for (index = (UINT8 ***)ctxt->ptr_reg; index < (UINT8***)ctxt->ctxt_ptr_registry; index++)
    {
        *index = (UINT8** )((PMC_PTRDIFF)(*index) + offset);

        /* and what it points to */
        if (NULL != **index)
        {
            PMC_ASSERT((((PMC_PTRDIFF)**index + offset )>= (PMC_PTRDIFF)(ctxt)) && (((PMC_PTRDIFF)**index + offset) < (PMC_PTRDIFF)(ctxt->next_calloc_ptr)), PMC_SHARED_ERR_CTXT_PTR_OO_BOUNDS, 0, 0);
            **index = (UINT8*)((PMC_PTRDIFF)(**index) + offset);
        }
    }

    PMC_RETURN();
}

/******************************************************************************
* FUNCTION: pmc_ctxt_adjust_context
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Adds or removes some space in the context at the specified point.
*
* INPUTS:
*      ctxt_header - pointer to the context header
*      shift_point - pointer to where to add or subtract memory.
*      offset      - amount to shift.
*
* OUTPUTS:
*      none
*
* RETURNS:
*      nothing
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_adjust_context(void* ctxt_header, UINT8 *shift_point, PMC_PTRDIFF offset)
{
    pmc_ctxt_mgmt_header_t  *ctxt;
    UINT8                   ***index;
    UINT32 size_after_shift_point;

    PMC_ENTRY();

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;

/* if the pointer is past the shift point, then add the offset to the pointer */
#define SHIFT_IF_NEEDED(ptr) if ((UINT8 *)(ptr) >= shift_point) { *(UINT8**)(&ptr) = (UINT8*)((PMC_PTRDIFF)(ptr) + offset); }

    /* update the header */
    SHIFT_IF_NEEDED(ctxt->ptr_reg);
    SHIFT_IF_NEEDED(ctxt->digi_hdl);
    SHIFT_IF_NEEDED(ctxt->next_calloc_ptr);
    SHIFT_IF_NEEDED(ctxt->ctxt_ptr_registry);

    /*
     * as a trivial example assume the following values:
     *
     *  ctxt        is 0x1000,
     *  shift_point is 0x1010,
     *  offset      is 0x4, and
     *  total_size  is 0x30
     *
     * and assume that ctxt starts as:
     *  [00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f]
     *
     * size_after_shift_point is 0x30 - (0x1000 - 0x1010) - 0x4 = 0x30 - 0x10 - 0x4 = 0x1c
     *
     * so in this example, the memcpy would be memcpy(0x1010,0x1014,0x1c);
     *
     * and after the memcpy we would have the following ctxt:
     *  [00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 00 00 00 00 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b]
     *
     * the loop adds 0x4 to all pointers that were greater than or equal to 0x1010
     */

    /* we need to do a memcpy from shift_point to shift_point + offset */
    size_after_shift_point = ctxt->total_size - ((PMC_PTRDIFF)(ctxt_header) - (PMC_PTRDIFF)(shift_point)) - offset;

    /* shift memory to create new space,... note: source and destination are overlapping */
    memcpy(shift_point,shift_point+offset,size_after_shift_point);

    /* fill new space with zeros */
    if (offset > 0) {
        memset(shift_point,0,offset);
    }
    
    /* Now shift the pointer registry and everything it points to */
    for (index = (UINT8 ***)ctxt->ptr_reg; index < (UINT8***)ctxt->ctxt_ptr_registry; index++)
    {
        SHIFT_IF_NEEDED(*index);
        
        /* and what it points to */
        if (NULL != **index)
        {
            SHIFT_IF_NEEDED(**index);
            PMC_ASSERT((((PMC_PTRDIFF)**index )>= (PMC_PTRDIFF)(ctxt)) && (((PMC_PTRDIFF)**index) < (PMC_PTRDIFF)(ctxt->next_calloc_ptr)), PMC_SHARED_ERR_CTXT_PTR_OO_BOUNDS, 0, 0);
        }
    }

    PMC_RETURN();
} /* of pmc_ctxt_adjust_context */


/******************************************************************************
* FUNCTION: pmc_ctxt_size_get
* ____________________________________________________________________________
*
* DESCRIPTION:
*   Returns the size of the context.
*
* INPUTS:
*   ctxt_header        - A pointer to a context/context header.
*
* OUTPUTS:
*      none
*
* RETURNS:
*      Context size
*
* NOTES:
******************************************************************************/
PUBLIC UINT32 pmc_ctxt_size_get(void *ctxt_header)
{
    pmc_ctxt_mgmt_header_t  *ctxt;
    UINT32 ctxt_size;

    PMC_ENTRY();

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;

    ctxt_size = ctxt->total_size;

    PMC_RETURN(ctxt_size);
}


/******************************************************************************
* FUNCTION: pmc_ctxt_set_sw_version
* ____________________________________________________________________________
*
* DESCRIPTION:
*   Sets the sw_version in the context header.
*
* INPUTS:
*   ctxt_header        - A pointer to a context/context header.
*   sw_version         - The SW version.
*
* OUTPUTS:
*      none
*
* RETURNS:
*      Context size
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_ctxt_set_sw_version(void *ctxt_header, UINT32 sw_version)
{
    pmc_ctxt_mgmt_header_t  *ctxt;

    PMC_ENTRY();

    ctxt = (pmc_ctxt_mgmt_header_t *)ctxt_header;

    ctxt->sw_version = sw_version;

    PMC_RETURN();
}

/******************************************************************************
* FUNCTION: pmc_ctxt_check_sanity
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Does a more thorough check of the context to make sure it makes sense. 
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      PMC_SUCCESS if the context is good, not PMC_SUCCESS if the context is 
*      bad.
*
* NOTES:
******************************************************************************/
PUBLIC PMC_ERROR pmc_ctxt_check_sanity(void *ctxt_header)
{
    pmc_ctxt_mgmt_ctxt_t* ctxt;
    ctxt = (pmc_ctxt_mgmt_ctxt_t *) ctxt_header;
    UINT8*** index;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* First, let's check that the header makes sense */
    if (ctxt->header.total_size  != sizeof(pmc_ctxt_mgmt_ctxt_t))
    {
        ret_val = PMC_SHARED_ERR_CTXT_INVALID_HEADER_TOTAL_SIZE;
    }

    if (ctxt->header.ptr_reg  != (UINT8 **)&ctxt->ptr_reg)
    {
        ret_val = PMC_SHARED_ERR_CTXT_INVALID_HEADER_PTR_REG;
    }

    if (ctxt->header.digi_hdl != (UINT8 *)&ctxt->data)
    {
        ret_val = PMC_SHARED_ERR_CTXT_INVALID_HEADER_DIGI_HDL;
    }

    if ((PMC_PTRDIFF)(ctxt->header.next_calloc_ptr) > (PMC_PTRDIFF)(ctxt->data) + (PMC_PTRDIFF)sizeof(pmc_ctxt_mgmt_ctxt_t)
        ||
        (PMC_PTRDIFF)(ctxt->header.next_calloc_ptr) < (PMC_PTRDIFF)(ctxt->data))
    {
        ret_val = PMC_SHARED_ERR_CTXT_INVALID_HEADER_CALLOC_PTR;
    }

    if ((PMC_PTRDIFF)(ctxt->header.ctxt_ptr_registry) > (PMC_PTRDIFF)(ctxt->data)
        ||
        (PMC_PTRDIFF)(ctxt->header.ctxt_ptr_registry) < (PMC_PTRDIFF)(ctxt->header.ptr_reg))
    {
        ret_val = PMC_SHARED_ERR_CTXT_INVALID_HEADER_CTXT_PTR_REG;
    }

    /* Check the pointer registery for pointers to things outside the context */
    for (index = (UINT8 ***)ctxt->ptr_reg; index < (UINT8***)ctxt->header.ctxt_ptr_registry; index++)
    {
        if (NULL != *index)
        {
            /* It is possible for the registered pointer to be NULL.  In a
             * few cases, the pointer is registered but not filled in */
            if (NULL != **index)
            {
                if ((PMC_PTRDIFF)**index < (PMC_PTRDIFF)(ctxt) 
                    ||
                    (PMC_PTRDIFF)**index > (PMC_PTRDIFF)(ctxt->header.next_calloc_ptr))
                {
                    ret_val = PMC_SHARED_ERR_CTXT_INVALID_REGISTERED_PTR;
                }
            }
        }
        else
        {
            /* This is bad - the pointer in the table is NULL */
            ret_val = PMC_SHARED_ERR_CTXT_INVALID_REGISTERED_PTR;
        }
    }

    PMC_RETURN(ret_val);
};


/* End of file */


/*
** Private Functions
*/

/******************************************************************************
* FUNCTION: pmc_ctxt_check_ctxt
* ____________________________________________________________________________
*
* DESCRIPTION:
*      Does a superficial check of the context pointers to make sure they sort
*      of make sense.
*
* INPUTS:
*      ctxt_header - pointer to the context header
*
* OUTPUTS:
*      none
*
* RETURNS:
*      nothing
*
* NOTES:
******************************************************************************/
PRIVATE void pmc_ctxt_check_ctxt(pmc_ctxt_mgmt_header_t *ctxt_header)
{
    pmc_ctxt_mgmt_ctxt_t* ctxt;
    ctxt = (pmc_ctxt_mgmt_ctxt_t *) ctxt_header;
    PMC_ENTRY();

    PMC_ASSERT(ctxt_header->ptr_reg  == (UINT8 **)&ctxt->ptr_reg,
        PMC_SHARED_ERR_INVALID_POINTER, 0, 0);
    PMC_ASSERT(ctxt_header->digi_hdl == (UINT8 *)&ctxt->data,
        PMC_SHARED_ERR_INVALID_POINTER, 0, 0);
    PMC_RETURN();
};


/* End of file */

