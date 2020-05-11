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
*
*   NOTES:
*
********************************************************************************/

#ifndef NO_CONFIG_CACHE

/*
 * this module converts read/modify/writes of config space into register writes.
 */
#include "pmc_types.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include "shadow_registers.h"
#include "config_cache.h"
#include "l4_sys.h"

/*
 */
/******************************************************************************
* STRUCTURE: cache_stats_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Stats structure per register location.
*
* ELEMENTS:
*   cached - 0 means that the register has not been cached and a read of the
*               device should be performed.  1 means that the register value is
*               valid.
*
* NOTE:
*******************************************************************************/
typedef struct {
    int cached;
} cache_stats_t;

/*******************************************************************************
* get_cache_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the l4sys_stats_t record associated with that address
*   (or NULL if that address is not defined in the defaults.h file)
*
* INPUTS:
*   config_cache - config cache to get stats from
*   reg_addr - address to get stats for
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cache_stats_t - pointer to the l4sys_stats_t record associated with that
*                   address or NULL if that address is not defined in the
*                   defaults.h file.
*
* NOTES:
*******************************************************************************/
static cache_stats_t *get_cache_stats( config_cache_t config_cache, UINT32 reg_addr )
{
    return (cache_stats_t *)sr_get_extra_space( config_cache, reg_addr );
}

/*******************************************************************************
* config_cache_create
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Create a new config cache.
*
* INPUTS:
*   None.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cache_cache_t - The newly created config cache.
*
* NOTES:
*******************************************************************************/
config_cache_t config_cache_create()
{
    return sr_create_memory_region(sizeof( cache_stats_t ));
}

/*******************************************************************************
* config_cache_delete
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Destroy a config cache.
*
* INPUTS:
*   config_cache - The config cache to destroy.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void config_cache_delete(config_cache_t config_cache)
{
    sr_delete_memory_region(config_cache);
}

/*******************************************************************************
* config_cache_init_simple_defaults
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a config cache.
*
* INPUTS:
*   config_cache - The config cache to initialize.
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
void config_cache_init_simple_defaults( config_cache_t config_cache, simple_memory_defaults *defaults, UINT32 len )
{
    sr_init_simple_defaults(config_cache,defaults,len);
}

/*******************************************************************************
* config_cache_update
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Updates a config cache.
*
* INPUTS:
*   config_cache - The config cache to update.
*   reg_addr - Address being updated
*   value - Value to update with.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void config_cache_update( config_cache_t config_cache, UINT32 reg_addr, UINT32 value )
{
    cache_stats_t *cache_stats = get_cache_stats( config_cache, reg_addr );
    cache_stats->cached = 1;
    sr_write( config_cache, reg_addr, value );
    PMC_OS_STATS_CONFIG_CACHE_WRITE();
    LOG ("CONFIG CACHE UPDATE 0x%08x <- 0x%08x",reg_addr,value);
}

/*******************************************************************************
* config_cache_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Updates a config cache.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   config_cache - The config cache to get a value from.
*   reg_addr - Address to get the value from.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The value in the config cache for the given address.
*
* NOTES:
*******************************************************************************/
UINT32 config_cache_read( void *sys_handle, config_cache_t config_cache, UINT32 reg_addr )
{
    UINT32 regval;
    cache_stats_t *cache_stats = get_cache_stats( config_cache, reg_addr );
    if (!cache_stats->cached) {
        regval = l4sys_reg_read( (void**)sys_handle, reg_addr );
        config_cache_update( config_cache, reg_addr, regval );
    }
    regval = sr_read( config_cache, reg_addr );
    PMC_OS_STATS_CONFIG_CACHE_READ();
    LOG ("CONFIG CACHE RD 0x%08x -> 0x%08x",reg_addr,regval);
    return regval;
}

/*******************************************************************************
* config_cache_convert_RMW_to_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Use the value stored in the config cache to determine the current value of
*   the register instead of reading the register to determine the current value.
*   Mask this and write back the modified value.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   config_cache - The config cache to get the value from.
*   reg_addr - Address being written.
*   mask - Bits being modified
*   value - New value to be written.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The new value of the register.
*
* NOTES:
*******************************************************************************/
UINT32 config_cache_convert_RMW_to_write( void *sys_handle, config_cache_t config_cache, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    UINT32 regval;
    regval = config_cache_read( sys_handle, config_cache, reg_addr );
    PMC_OS_STATS_CONFIG_CACHE_READ_MODIFY_WRITE();
    LOG("CONFIG CACHE RMW-RD 0x%08x -> 0x%08x",reg_addr,regval);
    regval &= ~mask;
    regval |= mask & value;
    LOG("CONFIG CACHE UPDATE 0x%08x <- 0x%08x (msk=0x%08x val=0x%08x)",reg_addr,regval,mask,value);
    sr_write( config_cache, reg_addr, regval );
    return regval;
}

#endif /* ifndef NO_CONFIG_CACHE */
