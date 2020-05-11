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
*     The level 4 system layer tracks statistics and adds logging before
*     calling the system access functions.
* 
*   NOTES:
*
********************************************************************************/

#include "l4_sys.h"

#ifndef NO_IO_LOGGING
/*******************************************************************************
* ENUM: mem_count_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Memory access type.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum {
    mem_read = 0,
    mem_write = 1,
    mem_read_modify_write = 2,
    burst_read = 3,
    burst_write = 4,
    poll_bit = 5
} mem_count_t;
#define NUM_COUNTS 6

#define MEM_READ                0x1
#define MEM_WRITE               0x2
#define MEM_READ_MODIFY_WRITE   0x4
#define BURST_READ              0x8
#define BURST_WRITE             0x10
#define POLL_BIT                0x20
#define MAX_FLAGS               0x3f

/** one global log file to generate a "terse" list of operation,addr,value */
static FILE *pmc_sys_reg_log_file = NULL;
/** Keep track of whether the log file is enabled or not */
static BOOL pmc_sys_reg_log_enabled = FALSE;



/*******************************************************************************
* l4sys_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the L4 layer. Initializes the stats and initializes the L5 layer.
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
void l4sys_init( pmc_sys_handle_t *sys_handle )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    l5sys_init(sys_handle);
    
    h->stats_and_descriptions = NULL;
    h->num_descriptions = 0;
    h->min_addr = 0;
    h->max_addr = 0xffffffff;
    h->last_rmw_addr = 0xcccccccc;
    pmc_os_stats_snapshot( &(h->snapshot) );
}

/*******************************************************************************
* l4sys_halt
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Unitialize the L4 layer.  Calls L5 halt and deletes the stats if they
*   exists.
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
void l4sys_halt( pmc_sys_handle_t *sys_handle )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    l5sys_halt(sys_handle);

    if (h->stats_and_descriptions != NULL)
    {
        sr_delete_memory_region(h->stats_and_descriptions);
    }
    h->stats_and_descriptions = NULL;
}

/*******************************************************************************
* pmc_sys_log_access
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Controls access to the log.  If the reg log is enabled, the operation is
*   logged.
*
* INPUTS:
*   operation - operation being performed
*   addr - address being accessed
*   data - data read/written/modified
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
static INLINE void pmc_sys_log_access( const char *operation, UINT32 addr, UINT32 data )
{
    if (pmc_sys_reg_log_enabled)
    {
        LOG("%s,%x,%x\n",operation,addr,data);
    }
}

#ifdef LOGGING_DISABLED
#else
/*******************************************************************************
* count_str
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Returns the name string associated with the stat counter.
*
* INPUTS:
*   count - The stat counter.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - pointer to the name string of the counter.
*
* NOTES:
*******************************************************************************/
static const char *count_str( mem_count_t count)
{
    const char *str[NUM_COUNTS] = {"RD","WR","RMW","BR","BW","POLL"};
    return str[count];
}
#endif

/******************************************************************************
* STRUCTURE: l4sys_stats_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure containing stats of accesses to a register.
*
* ELEMENTS:
*   destription_index - incrementing serial number
*   last_value  - last value read/written
*   last_snapshot - value at last snapshot
*   first_delta - used to keep track of which bits have changed since calling
*   pmc_sys_get_description_with_deltas().  Starts at 0xffffffff and set to 0
*   after the first call to pmc_sys_get_description_with_deltas().
*   flags - keeps track of whether the register has been read/written/modified
*   counts - array of stats counters for each type of access
*   description - register description
*   reg_bit_descriptions - register bit field descriptions.
*******************************************************************************/
typedef struct l4sys_stats_t
{
    int description_index;
    UINT32 last_value;
    UINT32 last_snapshot;
    UINT32 first_delta;
    UINT32 flags;
    UINT32 counts[NUM_COUNTS];
    const char *description; /*!< See title block description */
    const char *reg_bit_descriptions; /*!< See title block description */
} l4sys_stats_t;

/*******************************************************************************
* get_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the l4sys_stats_t record associated with that address
*   (or NULL if that address is not defined in the defaults.h file)
*
* INPUTS:
*   h - pointer to the l4 sys_handle
*   reg_addr - The address to get stas for
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   l4sys_stats_t - pointer to the the stats
*
* NOTES:
*******************************************************************************/
static l4sys_stats_t *get_stats( l4pmc_sys_handle_t *h, UINT32 reg_addr )
{
    if (h->stats_and_descriptions == NULL)
    {
        return NULL;
    }
    else
    {
        return (l4sys_stats_t *)sr_get_extra_space( h->stats_and_descriptions, reg_addr );
    }
}

/*******************************************************************************
* update_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*   updates the statistics with another access of the specified access_type for
*   that address.
*
* INPUTS:
*   h - pointer to the l4 sys_handle
*   reg_addr - The address to get stas for
*   access_type - the type of access
*   value - the value associated with the access.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
static void update_stats( l4pmc_sys_handle_t *h, UINT32 reg_addr, mem_count_t access_type, UINT32 *value )
{
    l4sys_stats_t *stats = get_stats( h, reg_addr );
    if (stats != NULL)
    {
        UINT32 x = 1 << (UINT32)access_type;
        stats->flags |= x;
        stats->counts[access_type]++;  /* increment the count for this type of access */
        if (value != NULL)
            stats->last_value = *value; /* record the last value read/written */

        /* hack to indicate that the memory location was used */
        sr_write( h->stats_and_descriptions, reg_addr, 0 );
    }
}

/*******************************************************************************
* pmc_sys_is_valid_addr
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns TRUE if the address is within min_addr and max_addr
*   (only relevant if pmc_sys_set_address_range was called).
*
* INPUTS:
*   sys - pointer to the sys_handle
*   addr - address to test
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE if the address is valid, FALSE otherwise.
*
* NOTES:
*******************************************************************************/
BOOL pmc_sys_is_valid_addr( pmc_sys_handle_t *sys, UINT32 addr )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if ((addr >= h->min_addr) && (addr <= h->max_addr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*******************************************************************************
* pmc_sys_set_address_range
* ___________________________________________________________________________
*
* DESCRIPTION:
*   limits statistic gathering to just the specified range of addresses.
*   addresses outside this range will not be displayed.
*
* INPUTS:
*   sys - pointer to the sys_handle
*   min_addr - low end of the range
*   max_addr - high end of the range
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_sys_set_address_range(pmc_sys_handle_t *sys, UINT32 min_addr, UINT32 max_addr )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    h->min_addr = min_addr;
    h->max_addr = max_addr;
}

/*******************************************************************************
* pmc_sys_init_defaults_and_descriptions
* ___________________________________________________________________________
*
* DESCRIPTION:
*   initialize the l4sys memory with a set of default values
*
*   note: the reg_bit_descriptions is either a string like:
*    "0x000000f0,TX_SM_BEI_BIAE_VALUE,0x00000008,TX_SM_BDI_VALUE,0x00000004,TX_SM_IAE_VALUE,0x00000003,TX_SM_RES_VALUE" 
*    or:
*    "=0x12345678"
*    where 0x12345678 is the address of another register that has the same 
*    reg_bit_descriptions.
*    the intent of this was merely to reduce the file size of the defaults.h 
*    file.
*    it doesn't actually save any code space since all compilers do common 
*    string folding.
*    limits statistic gathering to just the specified range of addresses.
*    addresses outside this range will not be displayed.
*
* INPUTS:
*   sys - pointer to the sys_handle
*   defaults - defaults
*   num_defaults - the number of defaults
*   descriptions - array of strings of register descriptions
*   reg_bit_descriptions - array of strings of register bit field descriptions.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_sys_init_defaults_and_descriptions( pmc_sys_handle_t *sys, simple_memory_defaults *defaults, int num_defaults, const char *descriptions[], const char * reg_bit_descriptions[] )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    int index;

    h->stats_and_descriptions = sr_create_memory_region( sizeof( l4sys_stats_t ) );
    h->num_descriptions = num_defaults;
    for (index=0;index<num_defaults;index++)
    {
        if (pmc_sys_is_valid_addr(sys,defaults[index].addr))
        {
            l4sys_stats_t *stats = get_stats( h, defaults[index].addr );
            stats->description_index = index+1;
            stats->last_value = defaults[index].data;
            stats->last_snapshot = defaults[index].data;
            stats->first_delta = 0xffffffff;
            stats->description = descriptions[index+1];
            stats->reg_bit_descriptions = reg_bit_descriptions[index+1];
            /* look for "=0x..." and grab the bit descriptions from that other register if the first character
              is an '=' sign */
            if (*(stats->reg_bit_descriptions) == '=')
            {
                UINT32 dup_addr;
                char *temp;
                l4sys_stats_t *dup;
                dup_addr = strtol( &(stats->reg_bit_descriptions[3]), &temp, 16 );
                dup = (l4sys_stats_t *)sr_get_extra_space( h->stats_and_descriptions, dup_addr );
                stats->reg_bit_descriptions = dup->reg_bit_descriptions;
            }
        }
    }
#ifdef PMC_SW_SIMULATION
    simulation_init_simple_defaults( sys, defaults, num_defaults );
#endif
}

/*******************************************************************************
* num_bits_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the number of bits set in the mask 
*   e.g.
*   num_bits_set( 0x0008 ) returns 1
*   num_bits_set( 0x00f0 ) returns 4
*
* INPUTS:
*   mask - the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The number of bits set in mask.
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 num_bits_set( UINT32 mask )
{
    UINT32 x = 0;
    if (mask == 0)
    {
        return 0;
    }
    while ((mask & 0x1) != 1)
    {
        mask = mask >> 1;
    }
    while (mask > 0)
    {
        x++;
        mask = mask >> 1;
    }
    return x;
}

/*******************************************************************************
* extract_value
* ___________________________________________________________________________
*
* DESCRIPTION:
*   extracts the field from a register value and updates the number of bits in the mask.
*   e.g.
*   extract_value( 0x000f, 0x1234, &mask ) returns 0x4 and sets mask_bits to 4
*   extract_value( 0x0ff0, 0x1234, &mask ) returns 0x23 and sets mask_bits to 8
*
* INPUTS:
*   mask - the mask
*   value - the value
*
* OUTPUTS:
*   mask_bits - the number of bits set in the mask
*
* RETURNS:
*   int - The value of the register bit field.
*
* NOTES:
*******************************************************************************/
static int extract_value( UINT32 mask, UINT32 value, UINT32 *mask_bits )
{
    value = value & mask;
    while (((mask & 0x1) != 1) && (value != 0))
    {
        value = value >> 1;
        mask = mask >> 1;
    }
    *mask_bits = num_bits_set(mask);
    return value;
}


/*******************************************************************************
* describe_bits
* ___________________________________________________________________________
*
* DESCRIPTION:
*   updates buffer with the string "field=<value>"
*  
*    reg_bit_descriptions is a string like:
*    "0x00000ff0,TX_SM_BEI_BIAE_VALUE,0x00000008,TX_SM_BDI_VALUE,0x00000004,TX_SM_IAE_VALUE,0x00000003,TX_SM_RES_VALUE" ) 
*    
*    The following examples assume the above reg_bit_descriptions:
*    
*    describe_bits( reg_bit_descriptions, buffer, 0xfe, 0x1234, &fields_used )
*    fields_used = 0xfff;
*    buffer = "TX_SM_BEI_BIAE_VALUE=0x23 TX_SM_BDI_VALUE=0 TX_SM_IAE_VALUE=1 TX_SM_RES_VALUE=0"
*   
*   no check is made to ensure that there is enough space in the buffer.
*   the function also returns 'buffer'.
*
* INPUTS:
*   reg_bit_descriptions - array of register bit field description strings
*   buffer - the buffer to modify
*   mask - mask of relevant bits
*   value - value being operated on
*
* OUTPUTS:
*   fields_used* - mask of which bit fields were used.
*
* RETURNS:
*   char - pointer to buffer.
*
* NOTES:
*******************************************************************************/
static const char *describe_bits( const char *reg_bit_descriptions, char *buffer, UINT32 mask, UINT32 value, UINT32 *fields_used )
{
    BOOL blank = TRUE;
    char *tail = buffer; /* tail is always pointing to the last character in buffer */
    char *desc = (char *)reg_bit_descriptions;
	UINT64 temp_bit_mask;
	
    PMC_STRCPY(tail,"{ ");
    tail += PMC_STRLEN(tail);
    while (*desc != 0)
    {
        UINT32 bit_mask;
        char *bit_descr;
        char *temp;
        int len;
        if (strncmp(desc,"0x",2) != 0)
        {
            break;
        }
        desc += 2;

        /* extract the bit mask */
        /* first check the return value*/
		temp_bit_mask = strtoul( desc, &temp, 16 );
		PMC_ASSERT(temp_bit_mask <= MAX_UINT32 , PMC_SHARED_ERR_BIT_MASK_EXTRACT_FAIL,0,0);
		bit_mask = temp_bit_mask;

        desc = strchr(desc,',');
        desc++;
        /* mark the beginning of the description of the bit field */
        bit_descr = desc;
        desc = strchr(desc,',');
        if (desc == NULL)
            desc = bit_descr+PMC_STRLEN(bit_descr);
        /* calculate the 'len' of the description of the bit field */
        len = desc - bit_descr;
        /* if this bit field should be printed */
        if (bit_mask & mask)
        {
            UINT32 mask_bits, extracted_value;
            /* update the 'fields_used' argument - a mask of which bit fields were used */
            *fields_used = *fields_used | bit_mask;
            strncat(tail,bit_descr,len);
            tail[len] = 0;
            tail += len;
            extracted_value = extract_value( bit_mask, value, &mask_bits );
            blank = FALSE;
            if (mask_bits <= 3)
                PMC_SPRINTF(tail,"=%d ",extracted_value);
            else
                PMC_SPRINTF(tail,"=0x%0*x ",(mask_bits+3)/4,extracted_value);
            tail += PMC_STRLEN(tail);
        }
        if (*desc == ',')
            desc ++;
    }
    if (blank)
        PMC_STRCPY(buffer,"");
    else
        PMC_STRCPY(tail,"}");
    return buffer;
}

/*******************************************************************************
* pmc_sys_get_description
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the name of the register
*
* INPUTS:
*   sys - pointer to the sys handle
*   addr - the address of the register
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - pointer to the name of the register.
*
* NOTES:
*******************************************************************************/
const char *pmc_sys_get_description( pmc_sys_handle_t *sys, UINT32 addr )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
    {
        return "";
    }
    else
    {
        l4sys_stats_t *stats = get_stats( h, addr );
        if (stats->description == NULL)
            return "Error: undefined register";
        else
            return stats->description;
    }
}

/*******************************************************************************
* pmc_sys_get_reg_bit_descriptions
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns a pointer to the register bit descriptions
*   something like 
*    "0x00000ff0,TX_SM_BEI_BIAE_VALUE,0x00000008,TX_SM_BDI_VALUE,0x00000004,TX_SM_IAE_VALUE,0x00000003,TX_SM_RES_VALUE"
*   returns "" if the register is not defined.
*
* INPUTS:
*   sys - pointer to the sys handle
*   addr - the address of the register
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - pointer to the register bit field descriptions.
*
* NOTES:
*******************************************************************************/
const char *pmc_sys_get_reg_bit_descriptions( pmc_sys_handle_t *sys, UINT32 addr )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
    {
        return "";
    }
    else
    {
        l4sys_stats_t *stats = get_stats( h, addr );
        return stats->reg_bit_descriptions;
    }
}

/*******************************************************************************
* pmc_sys_describe_fields
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns either "Error: undefined register"
*   or a the description of the bits,... e.g.
*   if the bit descriptions of addr is 
*    "0x00000ff0,TX_SM_BEI_BIAE_VALUE,0x00000008,TX_SM_BDI_VALUE,0x00000004,TX_SM_IAE_VALUE,0x00000003,TX_SM_RES_VALUE" ) 
*    
*    pmc_sys_describe_fields( buffer, addr, 0xfe, 0x1234, &fields_used )
*    fields_used = 0xfff;
*    buffer = "TX_SM_BEI_BIAE_VALUE=0x23 TX_SM_BDI_VALUE=0 TX_SM_IAE_VALUE=1 TX_SM_RES_VALUE=0"
*  
*   no check is made to ensure that there is enough space in the buffer.
*   the function also returns 'buffer'.
*
* INPUTS:
*   buffer - the buffer add the desciption to
*   sys - pointer to the sys handle
*   addr - the address of the register
*   mask - mask of the bits of interest
*   value - the value of the register
*
* OUTPUTS:
*   fields_used* - mask of which bit fields were used.
*
* RETURNS:
*   char - char pointer to buffer.
*
* NOTES:
*******************************************************************************/
const char *pmc_sys_describe_fields( char *buffer, pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 *fields_used )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
    {
        return "";
    }
    else
    {
        l4sys_stats_t *stats = get_stats( h, addr );
        char *tail;
        const char *reg_bit_descriptions = stats->reg_bit_descriptions;
        
        *fields_used = 0;
        if (stats->description == NULL)
        {
            *fields_used = 0xffffffff;
            return "Error: undefined register";
        }
        PMC_STRCPY(buffer,"");
        tail = buffer;
        tail += PMC_STRLEN(tail);
        describe_bits( reg_bit_descriptions, tail, mask, value, fields_used );
        return buffer;
    }
}

/*******************************************************************************
* pmc_sys_get_description_and_value
* ___________________________________________________________________________
*
* DESCRIPTION:
*   same as pmc_sys_describe_fields() but without the 'fields_used' argument.
*
* INPUTS:
*   sys - pointer to the sys handle
*   addr - the address of the register
*   mask - mask of the bits of interest
*   value - the value of the register
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - char pointer containing the description.
*
* NOTES:
*******************************************************************************/
const char *pmc_sys_get_description_and_value( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
    {
        return "";
    }
    else
    {
        l4sys_stats_t *stats = get_stats( h, addr );
        UINT32 fields_used;
        /* big honken buffer for generating field name/value strings */
        static char buffer[2048];
        char *tail;
        const char *reg_bit_descriptions = stats->reg_bit_descriptions;
        
        if (stats->description == NULL)
            return "Error: undefined register";
        PMC_STRCPY(buffer,stats->description);
        tail = buffer;
        tail += PMC_STRLEN(tail);
        PMC_STRCAT(tail," ");
        tail += PMC_STRLEN(tail);
        describe_bits( reg_bit_descriptions, tail, mask, value, &fields_used );
        return buffer;
    }
}


/*******************************************************************************
* pmc_sys_get_description_with_deltas
* ___________________________________________________________________________
*
* DESCRIPTION:
*   same as pmc_sys_describe_fields() but only displays the fields that have 
*   changed since the last call to this function.
*
* INPUTS:
*   sys - pointer to the sys handle
*   addr - the address of the register
*   value - the value of the register
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - char pointer to the description.
*
* NOTES:
*******************************************************************************/
const char *pmc_sys_get_description_with_deltas( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
        return "";
    else
    {
        l4sys_stats_t *stats = get_stats( h, addr );
        UINT32 mask = (stats->last_value ^ value) | stats->first_delta; 
        stats->last_value = value;
        stats->first_delta = 0;
        return pmc_sys_get_description_and_value( sys, addr, mask, value );
    }
}

#ifdef LOGGING_DISABLED
#else
/*******************************************************************************
* status_str
* ___________________________________________________________________________
*
* DESCRIPTION:
*   builds a list of types of memory accesses used in a field of 21 characters
*   if RD and WR accesses are used it returns       "RD WR                "
*   if RD and RMW accesses are used it returns      "RD    RMW            "
*   if BurstRead and BurstWrite are used it returns "          BR BW      "
*   if just POLL is used it returns                 "                POLL "
*   spaces are kept so that it fits well into a table.
*
* INPUTS:
*   buffer - buffer to write into
*   flags - the access types.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - char pointer to buffer.
*
* NOTES:
*******************************************************************************/
static char *status_str( char *buffer, UINT32 flags )
{
    PMC_STRCPY(buffer,((flags & MEM_READ   )       ? "RD "   : "   "));
    PMC_STRCAT(buffer,(flags & MEM_WRITE  )           ? "WR "   : "   ");
    PMC_STRCAT(buffer,(flags & MEM_READ_MODIFY_WRITE) ? "RMW "  : "    ");
    PMC_STRCAT(buffer,(flags & BURST_READ )           ? "BR "   : "   ");
    PMC_STRCAT(buffer,(flags & BURST_WRITE)           ? "BW "   : "   ");
    PMC_STRCAT(buffer,(flags & POLL_BIT   )           ? "POLL " : "     ");
    return buffer;
}
#endif

#ifdef LOGGING_DISABLED
#else
/*******************************************************************************
* values_str
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns a list of access type counts like:
*   "RD:10 RMW:15"
*
* INPUTS:
*   buffer - buffer to write into
*   counts - array of stats counters
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   char - char pointer to buffer.
*
* NOTES:
*******************************************************************************/
static char *values_str( char *buffer, UINT32 counts[NUM_COUNTS] )
{
    char *tail;
    UINT32 i;
    PMC_STRCPY(buffer,"");
    tail = buffer;
    for (i=0;i<NUM_COUNTS;i++)
        if (counts[i] > 0)
        {
            PMC_SPRINTF(tail,"%s:%d ",count_str((mem_count_t)i),counts[i]);
            tail = tail + PMC_STRLEN(tail);
        }
    return buffer;
}
#endif 

/*******************************************************************************
* pmc_sys_show_changes
* ___________________________________________________________________________
*
* DESCRIPTION:
*   displays a list of register that have been modified and the modified fields since the last time this function
*   was called.
*   The list looks like the following:
*       registers written
*       address    mask       value      register name  {deltas since last shown}
*       0x00001830 0x00000000 0x00000001 40GCOTN_POLO_DSP_PMD_ERR_FIR_TAP_RESET 
*       0x00002214 0x00000001 0x00000001 40GCOTN_POLO_DSP_SIB_CONTROL { CDC_SHARED_WRITE_FIELD=1 }
*       0x00004018 0x00000000 0x00000001 40GCOTN_POLO_DSP_CDC_FFT_H_FILT_FLIP 
*       0x00004200 0x00000000 0x007f007f 40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=0 
*       0x00004204 0xffffffff 0xfc7ffe7f 40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=1 { FC_MEM_FIELD=0xfc7ffe7f }                                                             
*       0x000043fc 0xffffffff 0x007ffe7f 40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=127 { FC_MEM_FIELD=0x007ffe7f }
*
* INPUTS:
*   sys - pointer to sys handle
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_sys_show_changes(pmc_sys_handle_t *sys)
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    {
        UINT32 start_addr;
        UINT32 end_addr;
        pmc_os_stats_show( &(h->snapshot) );  
        pmc_os_stats_snapshot( &(h->snapshot) );  
        start_addr = 0;
        LOG("registers written");
        LOG("address    mask       value      register name  {deltas since last shown}");
        while (sr_next_range_used( h->stats_and_descriptions, &start_addr, &end_addr ))
        {
            UINT32 addr;
            for (addr = start_addr; addr < end_addr; addr += 4 )
            {
                l4sys_stats_t *stats = get_stats( h, addr );
                if (stats != NULL)
                {
                    if ((stats->flags != 0) || (stats->description_index != 0))
                    {
                        if (stats->flags & (MEM_WRITE | MEM_READ_MODIFY_WRITE | BURST_WRITE))
                        {
#ifdef LOGGING_DISABLED
#else
                            UINT32 differences = stats->last_snapshot ^ stats->last_value;
                            UINT32 fields_used;
                            char buffer[2048];
                            const char *fields_description = pmc_sys_describe_fields( buffer, sys, addr, differences, stats->last_value, &fields_used );
                            LOG("0x%08x 0x%08x 0x%08x %25s %s",addr,fields_used,stats->last_value,pmc_sys_get_description(sys,addr),fields_description);
                            stats->last_snapshot = stats->last_value;
#endif
                        }
                    }
                }
                else
                {
                	LOG("Invalid address: 0x%08x\n",addr);
                }
            }
            start_addr = end_addr;
        }
    }
}

/*******************************************************************************
* pmc_sys_show_changes
* ___________________________________________________________________________
*
* DESCRIPTION:
*   shows the values of any changed registers since the last time this function was called,
*   and also displays statistics like the following:
*  
*   The format of the table is:
*   addr       access types (RD WR RMW BR BW POLL) register name, counts per access type.
*  
*        0x0000182c                      40GCOTN_POLO_DSP_PMD_ERR_CDC
*        0x00001830       RMW            40GCOTN_POLO_DSP_PMD_ERR_FIR_TAP_RESET   counts - RMW:12 
*        0x00001834                      40GCOTN_POLO_DSP_PMD_ERR_FIR_TAP_DISABLE
*        0x00002210                      40GCOTN_POLO_DSP_SIB_DSP_RESET
*        0x00002214       RMW            40GCOTN_POLO_DSP_SIB_CONTROL   counts - RMW:1 
*        0x00002218                      40GCOTN_POLO_DSP_SIB_SCRATCH
*        0x00004014                      40GCOTN_POLO_DSP_CDC_FFT_H_IRQ_FORCE_CDC
*        0x00004018       RMW            40GCOTN_POLO_DSP_CDC_FFT_H_FILT_FLIP   counts - RMW:12 
*        0x0000401c                      40GCOTN_POLO_DSP_CDC_FFT_H_FFT_REC_START
*        0x0000403c                      40GCOTN_POLO_DSP_CDC_FFT_H_COARSE_FREQ_STATUS2
*        0x00004200              BW      40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=0   counts - BW:13 
*        0x00004204              BW      40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=1   counts - BW:13 
*        ...
*        0x000043fc              BW      40GCOTN_POLO_DSP_CDC_FFT_H_FILT_COEFFS N=127   counts - BW:13 
*        0x00004400                      40GCOTN_POLO_DSP_CDC_FFT_H_FFT_RESULTS N=0
*        0x0000801c                      40GCOTN_POLO_DSP_CPR_CONST_BUFF_CTRL
*        0x00008020 RD                   40GCOTN_POLO_DSP_CPR_ESNO   counts - RD:11 
*        0x00008024                      40GCOTN_POLO_DSP_CPR_SIGNAL_H
*  
*    Then it displays global statistics of total number of registers that were used with that sort of
*    access type:
*        num_regs   RD                   1
*        num_regs         RMW            3
*        num_regs                BW      128
*  
*    The it displays the total number of accesses made by type.
*        totals   RD:11 RMW:25 BW:1664 
*  
*    Interpretation of the above statistics:
*     1 register was read,... and it was read 11 times.
*     3 registers were accessed using Read/modify/write operations, and there were a total of 25 read/modify/write operations.
*     128 registers were accessed using burst write operations, and there were 1664 words written using burst write accesses.
*
* INPUTS:
*   sys - pointer to sys handle
*   just_changes - If TRUE, don't show the stats.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void pmc_sys_dump_memory_usage(pmc_sys_handle_t *sys, BOOL just_changes)
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys;
    if (h->stats_and_descriptions == NULL)
    {
        return ;
    }
    else
    {
#ifdef LOGGING_DISABLED
#else
        char status_buffer[30];
        char values_buffer[150];
#endif
        UINT32 start_addr;
        UINT32 end_addr;
        UINT32 count_flags[MAX_FLAGS+1];
        UINT32 * count_flags_ptr;
        UINT32 totals[NUM_COUNTS];
        UINT32 * totals_ptr;
        UINT32 i;
        UINT32 flags;

        count_flags_ptr = &count_flags[0];
        totals_ptr = &totals[0];

        PMC_MEMSET( totals_ptr, 0, sizeof(totals) );
        PMC_MEMSET( count_flags_ptr, 0, sizeof(count_flags) );
        pmc_sys_show_changes( sys );
        start_addr = 0;
        while (sr_next_range_used( h->stats_and_descriptions, &start_addr, &end_addr ))
        {
            UINT32 addr;
            for (addr = start_addr; addr < end_addr; addr += 4 )
            {
                l4sys_stats_t *stats = get_stats( h, addr );
                count_flags[stats->flags] ++;
                if ((stats->flags != 0) || (stats->description_index != 0))
                {
                    for (i=0;i<NUM_COUNTS;i++)
                    {
                        totals[i] += stats->counts[i];
                    }
                    if (stats->flags != 0)
                        LOG("0x%08x %s %s   counts - %s",addr,status_str(status_buffer,stats->flags),stats->description,values_str(values_buffer,stats->counts));
                    else if (just_changes == FALSE) {
                        LOG("0x%08x %s %s",addr,status_str(status_buffer,stats->flags),stats->description);
                    }
                    for (i=0;i<NUM_COUNTS;i++)
                        stats->counts[i] = 0; /* clear stats */
                }
                stats->flags = 0; /* clear stats */
            }
            start_addr = end_addr;
        }
        sr_clear_access_flags( h->stats_and_descriptions );
        for (flags = 1;flags <= MAX_FLAGS; flags++)
        {
            if (count_flags[flags] != 0)
                LOG("num_regs   %s %d",status_str(status_buffer,flags),count_flags[flags]);
        }
        LOG("totals   %s",values_str(values_buffer,totals));
    }
}


/*******************************************************************************
* l4sys_reg_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a register write.  Calls l5sys_reg_write, logs the write and updates
*  the stats.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
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
void l4sys_reg_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;

    update_stats( h, reg_addr, mem_write, &value );
    PMC_OS_STATS_REG_WRITE();
    if ( h->last_rmw_addr== reg_addr)
    {
        LOG("consider coalescing - 0x%08x",reg_addr);
        PMC_OS_STATS_REG_READ_MODIFY_WRITE_DUPLICATE();
    }
    h->last_rmw_addr = reg_addr;
    {
        const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,0xffffffff,value);
        if (*description != 0)
        {
            LOG("WR %s",description);
        }
    }
    LOG("WR 0x%08x <- 0x%08x",reg_addr,value);
    pmc_sys_log_access("WR",reg_addr,value);
    l5sys_reg_write( sys_handle, reg_addr, value );
}

/*******************************************************************************
* l4sys_reg_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a register read.  Updates the stats, logs the access and
*  calls l5sys_reg_read.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the address to write
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The value of the register.
*
* NOTES:
*******************************************************************************/
UINT32 l4sys_reg_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    
    UINT32 regval;

    PMC_OS_STATS_REG_READ();
    h->last_rmw_addr = 0xcccccccc;
    regval = l5sys_reg_read( sys_handle, reg_addr );
    update_stats( h, reg_addr, mem_read, &regval );
    {
        const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,0xffffffff,regval);
        if (*description != 0)
        {
            LOG("RD %s",description);
        }
    }
    LOG ("RD 0x%08x -> 0x%08x",reg_addr,regval);
    pmc_sys_log_access("RD",reg_addr,regval);

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
*  Not called unless layer1 coalescing is disabled.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the address to write
*   mask - The bits to modify
*   value - the value to write to the address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void l4sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;

    if (h->last_rmw_addr== reg_addr)
    {
        LOG("consider coalescing - 0x%08x",reg_addr);
        PMC_OS_STATS_REG_READ_MODIFY_WRITE_DUPLICATE();
    }
    
    h->last_rmw_addr= reg_addr;
    /* if all bits are being set then skip the read */
    if (mask == 0xffffffff)
    {
        update_stats( h, reg_addr, mem_write, &value );
        PMC_OS_STATS_REG_WRITE();
        {
            const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,mask,value);
            if (*description != 0)
            {
                LOG("RMW-WR %s",description);
            }
        }
        LOG("RMW-WR 0x%08x <- 0x%08x",reg_addr,value);
        l5sys_reg_write( sys_handle, reg_addr, value );
        pmc_sys_log_access("RMW-WR",reg_addr,value);
        return;
    }
    else
    {
        UINT32 oldval = 0xFFFFFFFF;
        UINT32 regval;
            
        regval = l5sys_reg_read_modify_write( sys_handle, reg_addr, mask, value, &oldval );
        {
            const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,mask,value);
            if (*description != 0)
            {
                LOG("RMW %s",description);
            }
        }
        update_stats( h, reg_addr, mem_read_modify_write, &value );
        LOG("RMW-RD 0x%08x -> 0x%08x",reg_addr,oldval);
        pmc_sys_log_access("RMW-RD",reg_addr,oldval);
        LOG("RMW-WR 0x%08x <- 0x%08x (msk=0x%08x val=0x%08x)",reg_addr,regval,mask,value);
        pmc_sys_log_access("RMW-WR",reg_addr,value);
        PMC_OS_STATS_REG_READ_MODIFY_WRITE();
        if (oldval == regval)
        {
            LOG("RMW with no effect 0x%08x <- 0x%08x (msk=0x%08x val=0x%08x)",reg_addr,regval,mask,value);
            PMC_OS_STATS_REG_READ_MODIFY_WRITE_NO_EFFECT();
        }
    }
}


/*******************************************************************************
* l4sys_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a burst write.  Calls l5sys_burst_write, logs the write and updates
*  the stats.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the starting address of the write
*   len - the number of words to write
*   data - array of words to write
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l4sys_burst_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    UINT32 i;
    PMC_OS_STATS_BURST_WRITE(len);
    h->last_rmw_addr= 0xcccccccc;
    {
        const char *description = pmc_sys_get_description(sys_handle,reg_addr);
        if (*description != 0)
        {
            LOG("BURSTWRITE %s len=%d",description,len);
        }
    }
    LOG("BURSTWRITE 0x%08x len=%d",reg_addr,len);
    for (i=0;i<len;i++)
    {
        update_stats( h, reg_addr+i*0x4, burst_write, &(data[i]) );
        pmc_sys_log_access("WR",reg_addr+i*0x4,data[i]);
    }
    LOG_DATA(reg_addr,data,len,"BURSTWRITE");

    l5sys_burst_write( sys_handle, reg_addr, len, data );
}


/*******************************************************************************
* l4sys_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Performs a burst read.  Calls l5sys_burst_read, logs the read and updates
*  the stats.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the starting address of the read
*   len - the number of words to read
*
* OUTPUTS:
*   data - array of words read
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l4sys_burst_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    UINT32 i;
    l5sys_burst_read( sys_handle, reg_addr, len, data );
    {
        const char *description = pmc_sys_get_description(sys_handle,reg_addr);
        if (*description != 0)
        {
            LOG("BURSTREAD %s len=%d",description,len);
        }
    }
    
    LOG("BURSTREAD 0x%08x len=%d",reg_addr,len);
    for (i=0;i<len;i++)
    {
        update_stats( h, reg_addr+i*0x4, burst_read, &(data[i]) );
        pmc_sys_log_access("RD",reg_addr+i*0x4,data[i]);
    }
    PMC_OS_STATS_BURST_READ(len);
    h->last_rmw_addr= 0xcccccccc;
    LOG_DATA(reg_addr,data,len,"BURSTREAD");
}

/*******************************************************************************
* l4sys_poll_bit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Polls a bit until it matches.  Calls l5sys_poll, logs the poll and updates
*  the stats.  Equivalent to calling l4sys_poll with PMC_POLL_EQUAL for
*  poll_type
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the starting address of the register containing the bit to poll.
*   mask - the bits to poll
*   value - keep polling until the value == value read && mask
*   max_count - maximum number of times to poll
*   delay_between_polls_in_microseconds - dwell time of polling loop
*
* OUTPUTS:
*   num_failed_polls - number of polls where the value did not match.
*
* RETURNS:
*   PMC_ERROR - PMC_SUCCESS if the poll succeeded before max_count, an error
*   code otherwise.
*
* NOTES:
*******************************************************************************/
PMC_ERROR l4sys_poll_bit( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    PMC_ERROR result;
    UINT32 _num_failed_polls;

    h->last_rmw_addr= 0xcccccccc;
    PMC_OS_STATS_POLL_BIT();
    update_stats(h,reg_addr,poll_bit,NULL);
    if (num_failed_polls == NULL)
        num_failed_polls = &_num_failed_polls;
    {
        const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,mask,value);
        if (*description != 0)
        {
            LOG("poll_bit %s, max_count=%d, delay_in_usec=%d",description,max_count,delay_between_polls_in_microseconds);
        }
    }
    /* LOG("poll_bit 0x%08x (msk=0x%08x, val=0x%08x, max_count=%d, delay_in_usec=%d)",reg_addr,mask,value,max_count,delay_between_polls_in_microseconds); */
    result = l5sys_poll(sys_handle,reg_addr,mask,value,PMC_POLL_EQUAL,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    if (result == PMC_SUCCESS)
    {
        LOG("poll_bit 0x%08x (msk=0x%08x, val=0x%08x, max_count=%d, delay_in_usec=%d) succeeded after %d polls",reg_addr,mask,value,max_count,delay_between_polls_in_microseconds,*num_failed_polls);
    }
    else
    {
        LOG("poll_bit 0x%08x (msk=0x%08x, val=0x%08x, max_count=%d, delay_in_usec=%d) failed after %d polls",reg_addr,mask,value,max_count,delay_between_polls_in_microseconds,*num_failed_polls);
    }
    LOG("poll_bit - extra read for logging - RD 0x%08x -> 0x%08x",reg_addr,pmc_sys_reg_read( sys_handle, reg_addr ));
    pmc_sys_log_access("POLL",reg_addr,pmc_sys_reg_read( sys_handle, reg_addr ));
    return result;
}

/*******************************************************************************
* l4sys_poll
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Polls a register bit.  Calls l5sys_poll, logs the poll and updates
*  the stats.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the starting address of the register containing the bit to poll.
*   mask - the bits to poll
*   value - keep polling until the value == value read && mask
*   cmp - the poll comparison type.
*   max_count - maximum number of times to poll
*   delay_between_polls_in_microseconds - dwell time of polling loop
*
* OUTPUTS:
*   num_failed_polls - number of polls where the value did not match.
*
* RETURNS:
*   PMC_POLL_RETURN_TYPE - PMC_SUCCESS or PMC_ERR_POLL_TIMEOUT
*
* NOTES:
*******************************************************************************/
PMC_POLL_RETURN_TYPE l4sys_poll( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    l4pmc_sys_handle_t *h = *(l4pmc_sys_handle_t**)sys_handle;
    PMC_POLL_RETURN_TYPE result;
    UINT32 _num_failed_polls;

    h->last_rmw_addr= 0xcccccccc;
    PMC_OS_STATS_POLL_BIT();
    update_stats(h,reg_addr,poll_bit,NULL);
    if (num_failed_polls == NULL)
    {
        num_failed_polls = &_num_failed_polls;
    }
    {
        const char *description = pmc_sys_get_description_and_value(sys_handle,reg_addr,mask,value);
        if (*description != 0)
        {
            LOG("poll %s, cmp=%d, max_count=%d, delay_in_usec=%d",description,cmp,max_count,delay_between_polls_in_microseconds);
        }
    }
    result = l5sys_poll(sys_handle,reg_addr,mask,value,cmp,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    if (result == PMC_SUCCESS)
    {
        LOG("poll 0x%08x (msk=0x%08x, val=0x%08x, cmp=%d, max_count=%d, delay_in_usec=%d) succeeded after %d polls",reg_addr,mask,value,cmp,max_count,delay_between_polls_in_microseconds,*num_failed_polls);
    }
    else
    {
        LOG("poll 0x%08x (msk=0x%08x, val=0x%08x, cmp=%d, max_count=%d, delay_in_usec=%d) failed after %d polls",reg_addr,mask,value,cmp,max_count,delay_between_polls_in_microseconds,*num_failed_polls);
    }
    LOG("poll - extra read for logging - RD 0x%08x -> 0x%08x",reg_addr,pmc_sys_reg_read( sys_handle, reg_addr ));
    pmc_sys_log_access("POLL",reg_addr,pmc_sys_reg_read( sys_handle, reg_addr ));
    return result;
}

/*******************************************************************************
* l4sys_reg_log_open
* ___________________________________________________________________________
*
* DESCRIPTION:
*   begin logging all register accesses to the specified file 
*
* INPUTS:
*   file_name - Name of the file to log to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void l4sys_reg_log_open( void *file_name )
{
    pmc_sys_reg_log_file = fopen((const char *)file_name,"w");
    pmc_sys_reg_log_enabled = TRUE;
}

/*******************************************************************************
* l4sys_reg_log_close
* ___________________________________________________________________________
*
* DESCRIPTION:
*   close the register access log file and disable register access logging.
*
* INPUTS:
*   None.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void l4sys_reg_log_close( )
{
    if (pmc_sys_reg_log_file != NULL)
        {
            fclose(pmc_sys_reg_log_file);
            pmc_sys_reg_log_file = NULL;
    }
    pmc_sys_reg_log_enabled = FALSE;
}

/*******************************************************************************
* l4sys_reg_log_close
* ___________________________________________________________________________
*
* DESCRIPTION:
*   enable or disable logging (without closing the log file)
*
* INPUTS:
*   enable - If TRUE, enable register access logging.  If FALSE, disable.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   int - value of pmc_sys_reg_log_enabled
*
* NOTES:
*******************************************************************************/
int  l4sys_reg_log_enable( BOOL enable )
{
    BOOL result = pmc_sys_reg_log_enabled;
    pmc_sys_reg_log_enabled = enable;
    return result;
}
#else /* NO_IO_LOGGING */

#undef pmc_sys_set_address_range
void pmc_sys_set_address_range(pmc_sys_handle_t *sys, UINT32 min_addr, UINT32 max_addr ){}

#undef pmc_sys_is_valid_addr
BOOL pmc_sys_is_valid_addr(pmc_sys_handle_t *sys, UINT32 addr){ return FALSE;}

#undef pmc_sys_dump_memory_usage
void pmc_sys_dump_memory_usage(pmc_sys_handle_t *sys, BOOL just_changes){}

#undef pmc_sys_init_defaults_and_descriptions
void pmc_sys_init_defaults_and_descriptions( pmc_sys_handle_t *sys, simple_memory_defaults *defaults, int num_defaults, const char *descriptions[], const char *reg_bit_descriptions[] ){}

#undef pmc_sys_get_description
const char *pmc_sys_get_description( pmc_sys_handle_t *sys, UINT32 addr ){ return NULL;}

#undef pmc_sys_get_description_and_value
const char *pmc_sys_get_description_and_value( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value ){return NULL;}

#undef pmc_sys_get_description_with_deltas
const char *pmc_sys_get_description_with_deltas( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value ){return NULL;}


#undef pmc_sys_get_reg_bit_descriptions
const char *pmc_sys_get_reg_bit_descriptions( pmc_sys_handle_t *sys, UINT32 addr ){return NULL;}

#undef l4sys_init
void l4sys_init( pmc_sys_handle_t *sys_handle ){}

#undef l4sys_halt
void l4sys_halt( pmc_sys_handle_t *sys_handle ){}

#ifdef l4sys_reg_write
#undef l4sys_reg_write
#endif
void l4sys_reg_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value ){
    PMC_OS_STATS_REG_WRITE();
    l5sys_reg_write( sys_handle, reg_addr, value );
}

#ifdef l4sys_reg_read
#undef l4sys_reg_read
#endif
UINT32 l4sys_reg_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr ) {
    PMC_OS_STATS_REG_READ();
    return l5sys_reg_read(sys_handle, reg_addr);
}

#ifdef l4sys_reg_read_modify_write
#undef l4sys_reg_read_modify_write
#endif
void l4sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    PMC_OS_STATS_REG_READ_MODIFY_WRITE();
    if (mask == 0xffffffff) {
        l5sys_reg_write( sys_handle, reg_addr, value );
    } else {
        UINT32 oldval,regval;
        regval = l5sys_reg_read_modify_write( sys_handle, reg_addr, mask, value, &oldval );
    }
};

#ifdef l4sys_burst_write
#undef l4sys_burst_write
#endif
void l4sys_burst_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data ){
    PMC_OS_STATS_BURST_WRITE(len);
    l5sys_burst_write( sys_handle, reg_addr, len, data );    
}

#ifdef l4sys_burst_read
#undef l4sys_burst_read
#endif
void l4sys_burst_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data ){
    PMC_OS_STATS_BURST_READ(len);
    l5sys_burst_read( sys_handle, reg_addr, len, data );    
}

#ifdef l4sys_poll_bit
#undef l4sys_poll_bit
#endif
PMC_ERROR l4sys_poll_bit( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    PMC_ERROR result;
    UINT32 _num_failed_polls;
    PMC_OS_STATS_POLL_BIT();
    if (num_failed_polls == NULL)
        num_failed_polls = &_num_failed_polls;
    result = l5sys_poll(sys,addr,mask,value,PMC_POLL_EQUAL,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    return result;

}

#ifdef l4sys_poll
#undef l4sys_poll
#endif
PMC_POLL_RETURN_TYPE l4sys_poll( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds ){
    PMC_POLL_RETURN_TYPE result;
    UINT32 _num_failed_polls;
    PMC_OS_STATS_POLL_BIT();
      if (num_failed_polls == NULL)
    {
        num_failed_polls = &_num_failed_polls;
    }
    result = l5sys_poll(sys,addr,mask,value,cmp,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    return result;
}

#undef l4sys_reg_log_open
void l4sys_reg_log_open( void *file_name ){}

#undef l4sys_reg_log_close
void l4sys_reg_log_close( ){}

#endif /* NO_IO_LOGGING */

