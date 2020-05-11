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
*      caches/simulates the register space of the device.
*
*   ASSUMPTIONS:
*      currently assuming that accesses are word aligned (e.g. addr & 3 = 0)
*      if not and there are full words behind each address then this space could be called 1 to 4 memory.
*      where each address represents a full 4 words of memory.
*      also will need to provide separate regions for spaces that use indirect registers.
* 
*   NOTES:
*      Comments about this file are enclosed in [Author] [~Author] blocks.
*
******************************************************************************/

/*---- Compile Options -----------------------------------------------------*/

/*---- Include Files -------------------------------------------------------*/
#include "pmc_types.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include "uthash.h"
#include "shadow_registers.h"


/* undefine uthash memory allocation functions and error functions */
#undef uthash_malloc
#undef uthash_free
#undef uthash_fatal

/* re-define, specifying alternate functions */
#define uthash_malloc(sz) PMC_MALLOC(sz)
#define uthash_free(ptr,sz) PMC_FREE(&(ptr))
#define uthash_fatal(str) do {PMC_LOG_TRACE("ERROR: UTHASH FATAL: %s", str); PMC_ASSERT(FALSE, 0xFFFFFFFF, 0, 0);} while(0)

/*---- Extern Declarations -------------------------------------------------*/
/*---- Private Constant and Type Declarations ------------------------------*/

#define NUM_TABLE2_ENTRIES_IN_REGION  0x1000
#define NUM_PAGES_IN_TABLE2           0x1000
#define PAGE_SIZE                     0x100
#define PAGE_ADDR_MASK                0xffffff00

/*******************************************************************************
* STRUCTURE: shadow_memory_page
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Page of memory of size PAGE_SIZE words.
*
* ELEMENTS:
*    page_addr          - base address of this page
*    page_read_flag     - has this page been read since the defaults were applied.
*    page_write_flag    - has this page been written since the defaults were applied.
*    mem                - or could use calculated space if not using constants for page_size.
*    hh                 - hash handle
*    extra_space        - extra space for use by the caller
* 
*******************************************************************************/
typedef struct {
    UINT32 page_addr;
    int page_read_flag;
    int page_write_flag;
    UINT32 mem[PAGE_SIZE/4];
    UT_hash_handle hh;   
    UINT8  extra_space[0];
} shadow_memory_page;

/*******************************************************************************
* STRUCTURE: private_sr_memory_region_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Private version of sr_memory_region_t with extra space per memory location
*   accessed for the caller's use.  If used for simulation, this will track
*   usage statistics.
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct {
    shadow_memory_page *last_page;
    UINT32 last_page_addr;
    shadow_memory_page *hash_table;
    UINT32 extra_space;
} private_sr_memory_region_t;


/*******************************************************************************
* sr_clear_access_flags
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Clears the access flags for all the addresses stored in the given memory
*   region.
*
* INPUTS:
*   public_handle - memory region to clear the access flags for.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
void sr_clear_access_flags(sr_memory_region_t public_handle)
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
    for (page = r->hash_table; page != NULL; page = (shadow_memory_page *)page->hh.next)
    {
        page->page_read_flag = 0;
        page->page_write_flag = 0;
    }
}

/*******************************************************************************
* sort_by_addr
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Returns the difference in page_addr between a and b.
*   region.
*
* INPUTS:
*   a - The page's page_addr to subtract from
*   b - The page's page_addr to subtract
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   int - the difference.
*
* NOTES:
*******************************************************************************/
int sort_by_addr( shadow_memory_page *a, shadow_memory_page *b )
{
    return (a->page_addr - b->page_addr);
}

/*******************************************************************************
* sr_dump_memory_usage
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Prints out the addresses in use in the memory region.
*   region.
*
* INPUTS:
*   public_handle - The memory region to dump.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
void sr_dump_memory_usage(sr_memory_region_t public_handle)
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
    HASH_SORT(r->hash_table,sort_by_addr);
    for (page = r->hash_table; page != NULL; page = (shadow_memory_page*)page->hh.next)
    {
        if (page->page_read_flag || page->page_write_flag) {
            LOG("PAGE USED 0x%08x..0x%08x",page->page_addr,page->page_addr+PAGE_SIZE-1);
        }
    }
}


/*******************************************************************************
* sr_look_for_page_used
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Looks for the first used or unused page in the given memory region.
*
* INPUTS:
*   r - The memory region to search.
*   look_for_used - If TRUE, look for the first used page.  If FALSE, look for
*                   the first unused page.
*
* OUTPUTS:
*   addr - The page_addr of the page found
*
* RETURNS:
*   BOOL - If look_for_used is FALSE, always returns TRUE.  If look_for_used
*           is TRUE, returns TRUE if an unused page was found, FALSE otherwise.
*
* NOTES:
*   this function is only used for logging and does not have to be optimized for speed
*******************************************************************************/
static BOOL sr_look_for_page_used( private_sr_memory_region_t *r, UINT32 *addr, BOOL look_for_used )
{
    shadow_memory_page *page;
    /* skip forward past any page that is smaller */
    for (page = r->hash_table; (page != NULL) && (page->page_addr < *addr); page = (shadow_memory_page*)page->hh.next)
    {
    }
    /* skip forward past any page that does not meet the criteria */
    if (look_for_used)
    {
        for (; (page != NULL) ; page = (shadow_memory_page*)page->hh.next)
        {
            /* if you find a used page then break */
            if (page->page_read_flag || page->page_write_flag)
            {
                *addr = page->page_addr;
                return TRUE;
            }
        }
        return FALSE;
    }
    else
    {
        for (; (page != NULL) ; page = (shadow_memory_page*)page->hh.next)
        {
            /* if you skip a page, then that page was not used */
            if (page->page_addr > *addr)
            {
                return TRUE;
            }
            /* if you find an unused page then break */
            if (!(page->page_read_flag || page->page_write_flag))
            {
                return TRUE;
            }
            *addr += PAGE_SIZE;
        }
        return TRUE;
    }
}

/*******************************************************************************
* sr_next_range_used
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Determines the range of used addresses in the given memory region.
*
* INPUTS:
*   public_handle - The memory region to search.
*
* OUTPUTS:
*   start_addr - The starting address of the used addresses
*   end_addr - The end address of the used addresses
*
* RETURNS:
*   BOOL - If there were no used addresses, return FALSE, otherwise TRUE.
*
* NOTES:
*   this function is only used for logging and does not have to be optimized for speed
*******************************************************************************/
BOOL sr_next_range_used(sr_memory_region_t public_handle, UINT32 *start_addr, UINT32 *end_addr )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    HASH_SORT(r->hash_table,sort_by_addr);
    if (!sr_look_for_page_used( r, start_addr, TRUE ))
        return FALSE;
    *end_addr = *start_addr;
    if (!sr_look_for_page_used( r, end_addr, FALSE ))
        return FALSE;
    /* LOG("returning range 0x%08x..0x%08x",*start_addr, *end_addr); */
    return TRUE;
}

/*******************************************************************************
* sr_init_defaults
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Applies the given defaults to the given memory region.
*
* INPUTS:
*   public_handle - The memory region to search.
*   defaults - An array of defaults to apply.
*   num_defaults - The number of elements in defaults.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
void sr_init_defaults(sr_memory_region_t public_handle,memory_default *defaults,int num_defaults)
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    int i;
    for (i=0; i<num_defaults; i++)
    {
        UINT32 addr = defaults[i].addr;
        UINT32 len = defaults[i].len;
        UINT32 *data = defaults[i].data;
        int j;
        for (j=0;j<(int)len;j++)
        {
            sr_write(r,addr,data[j]);
            addr += 4;
        }
    }
}

/*******************************************************************************
* sr_init_simple_defaults
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Applies the given simple defaults to the given memory region.
*
* INPUTS:
*   public_handle - The memory region to search.
*   defaults - An array of simple efaults to apply.
*   num_defaults - The number of elements in defaults.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
void sr_init_simple_defaults(sr_memory_region_t public_handle,simple_memory_defaults *defaults,int num_defaults)
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    int i;
    for (i=0; i<num_defaults; i++)
    {
        sr_write(r,defaults[i].addr,defaults[i].data);
        if ((i > 0) && (defaults[i].addr <= defaults[i-1].addr))
            LOG("warning non-ascending default value: [%d].0x%08x = 0x%08x, [%d].0x%08x = 0x%08x"
                    ,i-1,defaults[i-1].addr,defaults[i-1].data,i,defaults[i].addr,defaults[i].data);
    }
    sr_clear_access_flags(r);
    LOG("shadow_registers: initialized %d default registers",num_defaults);
}

/*******************************************************************************
* sr_create_memory_region
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Allocates space for a memory region and sets up some defaults.
*
* INPUTS:
*   extra_space - caller-specified value to put into the extra_space field.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   The new memory region.
*
* NOTES:
*
*******************************************************************************/
sr_memory_region_t sr_create_memory_region( UINT32 extra_space )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)PMC_CALLOC(sizeof(private_sr_memory_region_t));
    r->hash_table = NULL;
    r->last_page_addr = (UINT32)0xcccccccc;
    r->extra_space = extra_space;
    return (sr_memory_region_t) r;
}
    
/*******************************************************************************
* sr_delete_memory_region
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Free the space allocated for the given memory region.
*
* INPUTS:
*   public_handle - The memory region to free.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
void sr_delete_memory_region( sr_memory_region_t public_handle )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page,*tmp;
    
    HASH_ITER(hh, r->hash_table, page, tmp) {
      HASH_DEL(r->hash_table,page);  /* delete; users advances to next */
      PMC_FREE(&page);                /* optional- if you want to free  */
    }
    r->last_page_addr = (UINT32)0xcccccccc;
    PMC_FREE(&r);
}

#if defined(__linux__) && defined(__GNUC__)
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#else
#define likely(x)      (x)
#define unlikely(x)    (x)
#endif

/*******************************************************************************
* get_sr_page
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Find the page in the given memory region that has the given address.  If no
*   page exists, allocate and initialize one.
*
* INPUTS:
*   r - The memory region to find the page in.
*   page_addr - The address to look for.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   shadow_memory_page - The page with the given address.
*
* NOTES:
*
*******************************************************************************/
shadow_memory_page *get_sr_page( private_sr_memory_region_t *r, UINT32 page_addr )
{
    shadow_memory_page *page;
    HASH_FIND_INT( r->hash_table, &page_addr, page );  
    if (page == NULL)
    {
        page = (shadow_memory_page*)PMC_CALLOC(sizeof(shadow_memory_page)+(r->extra_space * (PAGE_SIZE/4)));
        page->page_addr = page_addr;
        PMC_MEMSET(page->mem,0xcc,PAGE_SIZE);
        HASH_ADD_INT( r->hash_table, page_addr, page );  
    }
    return page;
}

/*******************************************************************************
* fast_get_sr_page
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Find the page in the given memory region that has the given address.  If no
*   page exists, allocate and initialize one.  This will also avoid doing the
*   search if the last address looked up was the same as addr.  This could be a
*   problem in a multi-threaded environment where there are multiple memory
*   regions with the same addresses.
*
* INPUTS:
*   r - The memory region to find the page in.
*   addr - The address to look for.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   shadow_memory_page - The page with the given address.
*
* NOTES:
*
*******************************************************************************/
static INLINE shadow_memory_page *fast_get_sr_page( private_sr_memory_region_t *r, UINT32 addr )
{
    UINT32 page_addr = addr & PAGE_ADDR_MASK;
    /* warning,... caching the last page accessed is not multi-thread safe */
    if (likely(r->last_page_addr == (page_addr)))
        return r->last_page;
    else
    {
        shadow_memory_page *page;
        page = get_sr_page( r, page_addr );
        r->last_page_addr = addr & PAGE_ADDR_MASK;
        r->last_page = page;
        return page;
    }
}

/*******************************************************************************
* sr_get_extra_space
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Returns the address of the extra space allocated for an address.
*
* INPUTS:
*   public_handle - The memory region to find the address in.
*   addr - The address to look for.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   void - The address of the extra space for the given address.
*
* NOTES:
*
*******************************************************************************/
void *sr_get_extra_space( sr_memory_region_t public_handle, UINT32 addr )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
    int offset;
    page = fast_get_sr_page( r, addr );
#ifndef LOGGING_DISABLED
    if (addr & 0x3) {
        LOG("shadow_registers not compiled for unaligned word accesses (address 0x%08x)",addr);
        LOG("seg-faulting to aide in debugging");
        {
            UINT32 *x = (UINT32 *)3;
            *x = 3;
        }
        exit(-1);
    }
#endif
    offset = (addr & 0xff) >> 2;
    return &(page->extra_space[offset * r->extra_space]);
}

/*******************************************************************************
* sr_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Returns the cached copy of the given address.
*
* INPUTS:
*   public_handle - The memory region to find the address in.
*   addr - The address to look for.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The cached value at the given address.
*
* NOTES:
*   Exits with a fatal error if malloc fails.
*******************************************************************************/
UINT32 sr_read( sr_memory_region_t public_handle, UINT32 addr )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
    int offset;
    page = fast_get_sr_page( r, addr );
#ifndef LOGGING_DISABLED
    if (addr & 0x3) {
        LOG("shadow_registers not compiled for unaligned word accesses (address 0x%08x)",addr);
        LOG("seg-faulting to aide in debugging");
        {
            UINT32 *x = (UINT32 *)3;
            *x = 3;
        }
        exit(-1);
    }
    page->page_read_flag = 1;
#endif
    offset = (addr & 0xff) >> 2;

    return (page->mem[offset]);
}

/*******************************************************************************
* sr_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Updates the cached copy of the given address.
*
* INPUTS:
*   public_handle - The memory region to find the address in.
*   addr - The address to update.
*   value - The new value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   Exits with a fatal error if malloc fails.
*******************************************************************************/
void sr_write( sr_memory_region_t public_handle, UINT32 addr, UINT32 value )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
    int offset;
    page = fast_get_sr_page( r, addr );
#ifndef LOGGING_DISABLED
    page->page_write_flag = 1;
    if (addr & 0x3) {
        LOG("shadow_registers: not compiled for unaligned word accesses (address 0x%08x)",addr);
        exit(-1);
    }
#endif
    offset = (addr & 0xff) >> 2;
    page->mem[offset] = value;
}


/*******************************************************************************
* sr_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Returns the cached copy of the given set of addresses.
*
* INPUTS:
*   public_handle - The memory region to find the address in.
*   addr - The starting address to look for.
*   len - The number of addresses to return - this is the number of words.
*   value - An array of values of length len.  This must be allocated by the
*           caller.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   Exits with a fatal error if malloc fails.
*******************************************************************************/
void sr_burst_read( sr_memory_region_t public_handle, UINT32 addr, UINT32 len, UINT32 *value )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
#ifndef LOGGING_DISABLED
    if (addr & 0x3) {
        LOG("shadow_registers not compiled for unaligned word accesses (address 0x%08x)",addr);
        exit(-1);
    }
#endif
    while (len > 0)
    {
        UINT32 i;
        UINT32 max;
        UINT32 offset;
        UINT32 *mem;
        page = fast_get_sr_page( r, addr );
        page->page_read_flag = 1;
        offset = (addr & 0xff) >> 2;
        max = (0x100 >> 2) - offset;
        if (max > len)
            max = len;
        mem = &(page->mem[offset]);
        for (i=max;i>0;i--) {
            *value++ = *mem++;
        }
        len -= max;
        addr += max * 4;
    }
}

/*******************************************************************************
* sr_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Updates the cached copy of the given set of addresses.
*
* INPUTS:
*   public_handle - The memory region to find the address in.
*   addr - The starting address to update.
*   len - The number of addresses to update - this is the number of words.
*   value - An array of new values of length len.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   Exits with a fatal error if malloc fails.
*******************************************************************************/
void sr_burst_write( sr_memory_region_t public_handle, UINT32 addr, UINT32 len, UINT32 *value )
{
    private_sr_memory_region_t *r = (private_sr_memory_region_t*)public_handle;
    shadow_memory_page *page;
#ifndef LOGGING_DISABLED
    if (addr & 0x3) {
        LOG("shadow_registers not compiled for unaligned word accesses (address 0x%08x)",addr);
        exit(-1);
    }
#endif
    while (len > 0)
    {
        UINT32 i;
        UINT32 max;
        UINT32 offset;
        UINT32 *mem;
        page = fast_get_sr_page( r, addr );
        page->page_write_flag = 1;
        offset = (addr & 0xff) >> 2; /* calculate offset into this page */
        max = (0x100 >> 2) - offset; /* don't write past the end of this page */
        if (max > len)
            max = len;               /* don't write more words than requested */
        mem = &(page->mem[offset]);
        for (i=max;i>0;i--) {
            *mem++ = *value++;
        }
        len -= max;
        addr += max * 4;
    }
}
