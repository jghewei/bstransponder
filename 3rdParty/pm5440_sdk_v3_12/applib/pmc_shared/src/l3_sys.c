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
*      The level 3 system layer caches the config space registers (converts
*      read/modify/writes of config space into writes).
* 
*   NOTES:
*
********************************************************************************/

#include "l3_sys.h"

#ifndef NO_CONFIG_CACHE

/*******************************************************************************
* l3sys_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the L3 layer.  Sets a few defaults, initializes the interrupt
*   cache and initializes the L4 layer.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l3sys_init(  pmc_sys_handle_t *sys_handle )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    l4sys_init(sys_handle);
    h->use_config_cache = FALSE;
    h->config_cache     = NULL;

    /* Initialize Interrupt dynamic cache */
    int_cache_init(&h->int_cache);
}

/*******************************************************************************
* l3sys_halt
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Unitialize the L3 layer.  Calls L4 halt, deletes the config cache if it
*   exists and unitializes the interrupt cache.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l3sys_halt(  pmc_sys_handle_t *sys_handle )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    l4sys_halt(sys_handle);
    if (h->config_cache != NULL)
    {
        config_cache_delete(h->config_cache);
    }
    int_cache_uninit(&h->int_cache);
}

/*******************************************************************************
* sys_config_cache_init_defaults
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the config cache, creating it if it hasn't been created yet.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   defaults - pointer to defaults to use
*   len - The number of defaults to use
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_config_cache_init_defaults( pmc_sys_handle_t *sys_handle, simple_memory_defaults *defaults, UINT32 len )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    if (h->config_cache == NULL)
        h->config_cache = config_cache_create();
    config_cache_init_simple_defaults(h->config_cache,defaults,len);
}

/*******************************************************************************
* sys_config_cache_disable
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Disables the config cache but doesn't destroy it so it can be re-enabled
*  later without losing data.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_config_cache_disable( pmc_sys_handle_t *sys_handle )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    /* temporarily disable caching,... but don't throw away cached registers */
    h->use_config_cache = FALSE;
}

/*******************************************************************************
* sys_config_cache_flush
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Deletes the config cache and recreates it.  This will also create it if it
*  doesn't exist.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_config_cache_flush( pmc_sys_handle_t *sys_handle )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    /* delete the existing cache and recreate it */
    if (h->config_cache != NULL)
    {
        config_cache_delete(h->config_cache);
    }
    h->config_cache = config_cache_create();
}

/*******************************************************************************
* sys_config_cache_enable
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Enables the config cache.  This will also create it if it doesn't exist.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_config_cache_enable( pmc_sys_handle_t *sys_handle )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    /* also create the cache if not yet created */
    if (h->config_cache == NULL)
    {
        h->config_cache = config_cache_create();
    }
    h->use_config_cache = TRUE;
}

/*******************************************************************************
* l3sys_reg_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a register write.  Calls l4sys_reg_write and updates the cache if
*  it's enabled.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   mem_type - the memory type
*   reg_addr - the address to write
*   value - the value to write to the address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l3sys_reg_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 value )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    
    l4sys_reg_write( sys_handle, reg_addr, value );
    if ((h->config_cache != NULL) && (mem_type == MEM_TYPE_CONFIG)) {
      config_cache_update( h->config_cache, reg_addr, value );
    }
}


/*******************************************************************************
* l3sys_reg_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a register read.  If the config cache is enabled, it is used.
*  Otherwise, calls l4sys_reg_read and uses the interrupt cache if
*  it's enabled.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   mem_type - the memory type
*   reg_addr - the address to write
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 -  The value of the register.
*
* NOTES:
*******************************************************************************/
UINT32 l3sys_reg_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    UINT32 regval;
    if (h->use_config_cache && (mem_type == MEM_TYPE_CONFIG))
    {
      regval = config_cache_read( sys_handle, h->config_cache, reg_addr );
    }
    else
    {
        /* check if the interrupt cache is enabled */
        if (int_cache_enable_get(sys_handle,&h->int_cache) == TRUE)
        {            
            if ( int_cache_reg_read(&h->int_cache,reg_addr, &regval) != TRUE)
            {
                regval = l4sys_reg_read( sys_handle, reg_addr );
                int_cache_reg_add(&h->int_cache,reg_addr, regval);
            }
        } else
        {
            regval = l4sys_reg_read( sys_handle, reg_addr );
        }
    }
    return regval;
}

/*******************************************************************************
* l3sys_reg_read_modify_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a register read/modify/write.  If the config cache is enabled, it is
*  used instead of performing a read and then calls l4sys_reg_write. Otherwise,
*  calls l4sys_reg_read_modify_write.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   mem_type - the memory type
*   reg_addr - the address to write
*   mask - The bits to modify
*   value - the value to write to the address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
void l3sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    UINT32 regval;

    /* convert read/modify/writes on the device into read of cache, and write to device. */
    if ((h->use_config_cache) && (mem_type == MEM_TYPE_CONFIG)) {
        regval = config_cache_convert_RMW_to_write( sys_handle, h->config_cache, reg_addr, mask, value );
        l4sys_reg_write( sys_handle, reg_addr, regval );
        return;
    }
    l4sys_reg_read_modify_write( sys_handle, reg_addr, mask, value );
}

/*******************************************************************************
* l3sys_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a burst write.  If the memory type is MEM_TYPE_CONFIG and the cache
*  exists, this actually results in individual writes for each word.  
*  Otherwise, l4sys_burst_write is called.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   mem_type - the memory type
*   reg_addr - start address to write
*   len - The number of words to write
*   data - pointer to an array of words to write.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
void l3sys_burst_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    
    /* also write to config cache */

    /* note: this is done even if the config cache is not currently enabled,...
     * it is done so that the current value of the config space is always accurate.
     */
    if ((h->config_cache != NULL) && (mem_type == MEM_TYPE_CONFIG)) {
        /* for now just do single reads and writes for cached memory */
        UINT32 i;
        for (i=0;i<len;i++)
        {
             l3sys_reg_write( sys_handle, mem_type, reg_addr+i*0x4, data[i]);
        }
        return;
    }
    l4sys_burst_write( sys_handle, reg_addr, len, data );
}


/*******************************************************************************
* l3sys_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a burst read.  If the memory type is MEM_TYPE_CONFIG and the cache
*  is enabled, the cache is used.
*  Otherwise, l4sys_burst_read is called.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   mem_type - the memory type
*   reg_addr - start address to read
*   len - The number of words to read
*
* OUTPUTS:
*   data - pointer to an array of words to write.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
void l3sys_burst_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
    
    /* read from config cache instead of the real device. */
    if ((h->use_config_cache) && (mem_type == MEM_TYPE_CONFIG)) {
        /* for now just do single reads and writes for cached memory */
        UINT32 i;
        for (i=0;i<len;i++)
            data[i] = l3sys_reg_read( sys_handle, mem_type, reg_addr+i*0x4 );
        return;
    }
    l4sys_burst_read( sys_handle, reg_addr, len, data );
}

#endif /* NO_CONFIG_CACHE */
