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
*     shadow registers is used to store a cache of values written to memory locations.
*     the two main functions are sr_read and sr_write.
*
*     if you call sr_write( addr, data )
*     and later call sr_read(addr),... you'll get the data back.
*
*     yup,... that's it,.. you can think of this module as an array of memory.
*
*     it does no memory allocation until you access a memory location, and at
*     that point it allocates a buffer for that location.
*
*     it caches the last page accessed so the call to sr_read() is typically
*     21 instructions,... but unfortunately that cache is not thread safe so
*     we'll have to work on optimizing this function while keeping it thread safe.
*
*   NOTES:
*    Comments about this file are enclosed in [Author] [~Author] blocks.
*
******************************************************************************/

#ifndef SHADOW_REGISTERS_H
#define SHADOW_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
* STRUCTURE: memory_default
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Values to use as defaults when initializing a range of addresses in a memory 
*   region.
*
* ELEMENTS:
*   addr - Starting address
*   len - number of words to initialize
*   data - array of words to use to initialize of length len.
* 
*******************************************************************************/
typedef struct {
    UINT32 addr;
    UINT32 len;
    UINT32 *data;
} memory_default;

/*******************************************************************************
* STRUCTURE: simple_memory_default
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Values to use as defaults when initializing a single address in a memory 
*   region.
*
* ELEMENTS:
*   addr - Address
*   data - Data to use to initialize.
* 
*******************************************************************************/
typedef struct {
    UINT32 addr;
    UINT32 data;
} simple_memory_defaults;

/** Opaque type */
typedef void *sr_memory_region_t;

sr_memory_region_t sr_create_memory_region( UINT32 extra_space );

void sr_delete_memory_region( sr_memory_region_t public_handle );

void sr_init_defaults( sr_memory_region_t r, memory_default *defaults, int num_defaults );

void sr_init_simple_defaults( sr_memory_region_t r, simple_memory_defaults *defaults, int num_defaults );

/** show statistics on what pages were used */
void sr_dump_memory_usage( sr_memory_region_t r ) ATTRIBUTE_COLD;

/* reset memory range tracking to 'unused' */
void sr_clear_access_flags(sr_memory_region_t r );


/*
 * returns a range of addresses used
 *
 * To iterate over a range of addresses, use the following code:
 *
 * {
 *     UINT32 start_addr = 0;
 *     UINT32 end_addr;
 *     while (sr_next_range_used( r, &start_addr, &end_addr ))
 *     {
 *         LOG("range used: 0x%08x .. 0x%08x",start_addr,end_addr-4);
 *         start_addr = end_addr;
 *     }
 * }
 */
BOOL sr_next_range_used(sr_memory_region_t public_handle, UINT32 *start_addr, UINT32 *end_addr );

/*
 * returns the address of the extra space allocated for an address.
 */
void *sr_get_extra_space( sr_memory_region_t r, UINT32 addr );

UINT32 sr_read( sr_memory_region_t r, UINT32 addr ) ATTRIBUTE_WARN_UNUSED_RESULT ATTRIBUTE_HOT;

void sr_write( sr_memory_region_t r, UINT32 addr, UINT32 value ) ATTRIBUTE_HOT;

void sr_burst_read( sr_memory_region_t r, UINT32 addr, UINT32 len, UINT32 *value ) ATTRIBUTE_HOT;

void sr_burst_write( sr_memory_region_t r, UINT32 addr, UINT32 len, UINT32 *value ) ATTRIBUTE_HOT;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
