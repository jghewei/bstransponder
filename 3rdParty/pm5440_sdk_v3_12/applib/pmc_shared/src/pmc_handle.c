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
*     This file provide initialization and handle output operations.
*
*   NOTES:
*
******************************************************************************/

/*---- Include Files -------------------------------------------------------*/
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include <string.h>
#include <stdio.h>

/*---- Public Constant and Type Declarations ------------------------------*/


/*******************************************************************************
* pmc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This fucntion initializes the pmc_handle based on the parent handle and
*   passed in information. This handle acts much like the base class for all
*   handles in the system and allows for device wide system access info
*   to be used (e.g. logging info).
*
* INPUTS:
*   _parent - parent handle (for upward reference)
*   _child - child references
*   sys_handle - system handle for the device (common to all handles)
*   tsb_mid - unique module id for this TSB within the product
*   tsb_name - string name of the TSB for debug/logging
*   base_address - offset of this TSB from the base address of the device
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_handle_init( void *_parent, void *_child, pmc_sys_handle_t *sys_handle, UINT32 tsb_mid, const char *tsb_name, UINT32 base_address )
{
    pmc_handle_t *parent = (pmc_handle_t*)_parent; /* cast back to a concrete class */
    pmc_handle_t *child = (pmc_handle_t*)_child; /* cast back to a concrete class */
    *(UINT32 *)&(child->base_address) = base_address;
    PMC_ASSERT(PMC_STRLEN(tsb_name) < PMC_HANDLE_MAX_TSB_NAME_LEN, PMC_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_STRCPY(child->tsb_name, tsb_name);
    child->tsb_mid  = tsb_mid;
    child->sys_handle = sys_handle;
    child->parent_handle = (pmc_handle_t*)_parent;
    if (NULL != parent)
    {
        /* If this isn't the top, we need to copy some pointers from the parent
         */
        child->ctxt = parent->ctxt;
        child->common_info = parent->common_info;
    }
    else
    {
        /* This is the top, and we need to allocate the common_info struct */
        child->common_info = (pmc_handle_common_info_t*)PMC_CTXT_CALLOC(sizeof(pmc_handle_common_info_t), child);
        PMC_CTXT_REGISTER_PTR(&child->common_info->util_gen_db, child);
        PMC_CTXT_REGISTER_PTR(&child->common_info->hostmsg_handle, child);
        PMC_CTXT_REGISTER_PTR(&child->common_info->log_buffer, child);
    }
    child->next_sibling = NULL;
    child->first_child = NULL;
    child->last_child = NULL;
    /* We can't use parent here for the pmc_handle pointer because parent is
     * NULL if this is the top of the tree.  The top of the tree is a special
     * case, and the code handling it needs to set the child->ctxt pointer
     * before calling this function. */
    PMC_CTXT_REGISTER_PTR(&child->ctxt, child);
    PMC_CTXT_REGISTER_PTR(&child->common_info, child);
    PMC_CTXT_REGISTER_PTR(&child->next_sibling, child);
    PMC_CTXT_REGISTER_PTR(&child->first_child, child);
    PMC_CTXT_REGISTER_PTR(&child->last_child, child);
    PMC_CTXT_REGISTER_PTR(&child->parent_handle, child);

    if (parent != NULL)
    {
        if (parent->last_child != NULL)
        {
            parent->last_child->next_sibling = child;
        }
        else
        {
            parent->first_child = child;
        }
        parent->last_child = child;
    }
    child->next_sibling = NULL;

}

/*******************************************************************************
* print_handles
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This fucntion prints out the info on all the child handles recursively
*
* INPUTS:
*   path - string of current path to print as prefix
*   h - handle to start printing
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
static void print_handles( char *path, pmc_handle_t *h )
{
    char *tail;
    pmc_handle_t *child;

    PMC_ASSERT(PMC_STRLEN(path) + PMC_STRLEN(h->tsb_name) + 1 < PMC_HANDLE_MAX_TSB_PATH_LEN, PMC_SHARED_ERR_STRCAT_FAILED, 0, 0);
    PMC_STRCAT( path, ":" );
    PMC_STRCAT( path, h->tsb_name );
    tail = path + PMC_STRLEN(path);
    
    LOG("0x%08x 0x%04x %s",h->base_address,h->tsb_mid,path+1);
    for (child = h->first_child;child != NULL;child = child->next_sibling)
    {
        print_handles( path, child );
        *tail = 0;
    }
}

/*******************************************************************************
* pmc_handle_all_handles_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This fucntion starts with any handle that has pmc_handle_t as the first
*   element and outputs them to the LOG as strings for debug
*   
* INPUTS:
*   _h - handle to start printing
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_handle_all_handles_print( void *_h )
{
  pmc_handle_t *h = (pmc_handle_t*)_h;
  char path[PMC_HANDLE_MAX_TSB_PATH_LEN];
  PMC_MEMSET(path,0,sizeof(path));
  LOG("base_addr  mid    path");
  print_handles( path, (pmc_handle_t*)h );
}

/*******************************************************************************
* pmc_handle_by_name_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a string name, find that handle with the same name starting from
*   the current handle. Note that multiple instantiations of TSBs can have the
*   same name.
*   
* INPUTS:
*   _h - handle to start printing
*   name - string name of handle to find
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   pmc_handle_t - pointer to handle found, NULL if not found.
*
* NOTES:
*******************************************************************************/
pmc_handle_t *pmc_handle_by_name_find( void *_h, const char *name )
{
    pmc_handle_t *h = (pmc_handle_t*)_h;
    pmc_handle_t *child;
    for (child = h->first_child;child != NULL;child = child->next_sibling)
    {
        if (strcmp( child->tsb_name, name) == 0)
            return child;
    }
    return NULL;
}

/*******************************************************************************
* pmc_handle_by_path_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a path and string name, find that handle starting from the current
*   handle.
*   
* INPUTS:
*   _h - handle to start printing
*   path - string path name of handle to find
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   pmc_handle_t - pointer to handle found, NULL if not found.
*
* NOTES:
*******************************************************************************/
pmc_handle_t *pmc_handle_by_path_find( void *_h, char *path )
{
    pmc_handle_t *h = (pmc_handle_t*)_h;
    pmc_handle_t *child;
    char name[PMC_HANDLE_MAX_TSB_NAME_LEN];
    char *colon = strchr(path,':');
    char *rest_of_path;
    int len;

    /* if there are no ':'s in the path then this is the final depth and the name should be an exact match. */
    if (colon == NULL)
    {
        if (strcmp( path, h->tsb_name ) == 0)
        {
            return h;
        }
        else
        {
            return NULL; /* did not find the path */
        }
    }

    /* there is a ':' so check that this module matches and look for a matching child with the rest of the path */
    /*
     * for example if we call pmc_handle_by_path_find( digi, "DIGI:MAPOTN:RGFPF" )
     * then path will be    "DIGI:MAPOTN:RGFPF"
     * len will be          5
     * name will be         "DIGI"
     * rest_of_path will be "MAPOTN:RGFPF"
     */
    len = colon-path;
    if (len >= PMC_HANDLE_MAX_TSB_NAME_LEN)
    {
        LOG("name too long in pmc_handle_by_path_find( \"%s\" ) (max name length is %d)", path, PMC_HANDLE_MAX_TSB_NAME_LEN );
        return NULL;
    }

    /* extract the first name */
    strncpy(name,path,colon-path);
    name[len] = 0;
    rest_of_path = colon+1;
    if (strcmp( name, h->tsb_name ) == 0)
    {
        for (child = h->first_child;child != NULL;child = child->next_sibling)
        {
            pmc_handle_t *result;

            /* look for the rest of the path */
            result = pmc_handle_by_path_find(child,rest_of_path);
            if (result != NULL)
                return result;
        }
    }
    return NULL; /* did not find the path */
}

/*******************************************************************************
* pmc_handle_by_address_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a module id and address, find that handle starting from the current
*   handle.
*   
* INPUTS:
*   _h - handle to start printing
*   mid - module id
*   base_address - base address of the block
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   pmc_handle_t - pointer to handle found, NULL if not found.
*
* NOTES:
*******************************************************************************/
pmc_handle_t *pmc_handle_by_address_find( void *_h, UINT32 mid, UINT32 base_address )
{
    pmc_handle_t *h = (pmc_handle_t*)_h;
    pmc_handle_t *child;

    /* current implementation just tests every handle */
    if (h->base_address == base_address)
    {
        if ((mid == h->tsb_mid) || (mid == 0))
            return h;
    }
    for (child = h->first_child;child != NULL;child = child->next_sibling)
    {
        pmc_handle_t *result;
        result = pmc_handle_by_address_find(child,mid,base_address);
        if (result != NULL)
            return result;
    }
    return NULL;
}

/*********************************************************************
* pmc_handle_parent_get
* ____________________________________________________________________
*
* DESCRIPTION:
*   Operator to get the immediate parent module for a module
*
* INPUTS:
*   *_h - handle for which to find immediate parent
*
* OUTPUTS: 
*   None
*
* RETURNS:
*    Pointer to pmc_handle_t of the immediate parent
*
* NOTES:
* 
**********************************************************************/
pmc_handle_t * pmc_handle_parent_get( void *_h )
{
    pmc_handle_t *h = (pmc_handle_t*)_h;
    return h->parent_handle;
    
} /* pmc_handle_parent_get() */

/*********************************************************************
* pmc_handle_ultimate_parent_get
* ____________________________________________________________________
*
* DESCRIPTION:
*   Operator to get the ultimate (highest) parent module for a module
*
* INPUTS:
*   *_h - handle for which to find ultimate parent
*
* OUTPUTS: 
*   None
*
* RETURNS:
*    Pointer to pmc_handle_t of the ultimate parent
*
* NOTES:
* 
**********************************************************************/
pmc_handle_t * pmc_handle_ultimate_parent_get( void *_h )
{
    pmc_handle_t *h = (pmc_handle_t*)_h;
    pmc_handle_t *parent;
    pmc_handle_t *current_handle;
    
    /* start identify our immediate handle */
    current_handle = h;
    
    while(1)
    {
        /*get parent handle for current_handle
        if NULL - return current handle pointer
        else set current_handle to the parent_handle*/
        parent = pmc_handle_parent_get(current_handle);
        if(parent == NULL)
        {
            return current_handle;
        }
        else 
        {
            current_handle = parent;
        }

    } /* while */
}

/******************************************************************************
* FUNCTION: pmc_handle_fix_sys_handle_ptrs
* ____________________________________________________________________________
*
* DESCRIPTION:
*      A recursive function that sets the sys_handle pointer in all children of
*      the given pmc_handle to be the same as the sys_handle pointer in the
*      given pmc_handle.  This can be used to change the sys_handle pointer
*      across all pmc_handles by setting the sys_handle pointer in the topmost
*      parent pmc_handle and then calling this function with that pmc_handle.
*
* INPUTS:
*      pmc_handle - pointer to the pmc_handle with the new sys_handle value.
*
* OUTPUTS:
*      none
*
* RETURNS:
*      none
*
* NOTES:
******************************************************************************/
PUBLIC void pmc_handle_fix_sys_handle_ptrs(pmc_handle_t *pmc_handle)
{
    pmc_handle_t *child;
    
    for (child = pmc_handle->first_child; child != NULL; child = child->next_sibling) 
    {
        child->sys_handle = pmc_handle->sys_handle;
        pmc_handle_fix_sys_handle_ptrs(child);
    };
};

/* End of File */
