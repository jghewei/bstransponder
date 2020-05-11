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
*     This file contains the definitions and inline fucntions for the pmc
*     software test mode. PMC software test mode provides a memory space
*     to provide the equivalent of register access to the device. This simulation
*     does not have the operations of the device so status values and reads
*     will only return what was written.
* 
*   NOTES:
*
*******************************************************************************/

#ifndef PMC_SW_SIMULATION_H
#define PMC_SW_SIMULATION_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pmc_types.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "shadow_registers.h"
#include "pmc_ctxt_mgmt.h"
#include "pmc_handle.h"
#include <math.h>
#include <sys/time.h>

#define PMC_FLOOR(arg) floor(arg)
#define PMC_CEIL(arg) ceil(arg)
#define PMC_MATH_LOG(arg) log(arg)
#define PMC_ABS(arg) abs(arg)
#define PMC_POW(a,b) pow(a,b)
#define PMC_FABS(arg) fabs(arg)
/* for float */
#define PMC_FLOORF(arg) floor(arg)
#define PMC_CEILF(arg) ceil(arg)
#define PMC_MATH_LOGF(arg) log(arg)
#define PMC_POWF(a,b) pow(a,b)
#define PMC_FABSF(arg) fabs(arg)
#define PMC_ROUND(x)   ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

/***************************************************************************
* MACRO: PMC_CALLOC
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to alloc local memory and set to zero. 
*
* INPUTS:
*   size - number of bytes to allocate
* 
* RETURNS:
*   *void - allocated buffer set to zero. Asserts on failure
* NOTES:
*   
****************************************************************************/
#define PMC_CALLOC(size)       pmc_calloc_function(size)

/*! Define the simulation function function typedef */
typedef void (*simulation_function_t)( pmc_sys_handle_t *sys, UINT32 addr, void *param, UINT32 read_flag, UINT32 *value );

/***************************************************************************
* STRUCTURE: simulation_sys_handle_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   This structure is the system level info to provide the simulation
*   operation.
*   DO NOT CHANGE MEMBER ORDER IN THIS STRUCTURE.
*
* ELEMENTS:
*   See typedef.
* 
* NOTES:
*   Enhancement: instead of calculating global stats,do it per sys_handle
************************************************************************/
typedef struct simulation_sys_handle_t 
{
    void *internal_sys_handle; /*!< Sys handle structure must start with this internal PMC pointer */
    struct hostmsg_handle_t *hostmsg_handle;  /*!< Host message specific information. */
    sr_memory_region_t simulation;        /*!< saves writes,... reads return last value written */
} simulation_sys_handle_t;

/*******************************************************************************
* simulation_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Create a handle to a software simulation of the device, currently this
*   just remembers the last value written to a register and returns it when
*   the register is read.
* 
* INPUTS:
*   None
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   pmc_sys_handle_t - handle created
*
* NOTES:
*
*******************************************************************************/
pmc_sys_handle_t *simulation_create(void);

/*******************************************************************************
* simulation_sys_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Create a handle to a software simulation of the device, currently this
*   just remembers the last value written to a register and returns it when
*   the register is read.
* 
* INPUTS:
*   None
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   pmc_sys_handle_t - handle created
*
* NOTES:
*
*******************************************************************************/
pmc_sys_handle_t *simulation_sys_create(void);

/*******************************************************************************
* simulation_sys_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Free memory associated with the simulation.
* 
* INPUTS:
*   sys - sys handle used for the simulation.
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
void simulation_sys_destroy(pmc_sys_handle_t **sys);

PUBLIC void pmc_usleep(UINT64 usec);

/*******************************************************************************
* pmc_assert_function
* ______________________________________________________________________________
*
* DESCRIPTION:
*   When using the PMC_ASSERT() macro the following function must be 
*   instantiated somewhere in the code base.
* 
* INPUTS:
*   error_code - error code for the assert
*   *file_ptr - filename string (to match with typical assert(), not usually
*           used in fielded system)
*   line - line number where assert occurred
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Should never return from this function
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_assert_function(PMC_ERROR error_code, const char *file_ptr, UINT32 line) ATTRIBUTE_NORETURN;

/***************************************************************************
* MACRO: PMC_PRINT()
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used instead of printf(). 
*
* INPUTS:
*   format - standard printf() 'format' entry
*   args - varaible arguments list for standard printf()
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_PRINT(format, args...) do { /* PMC_PRINT allowed in porting function */ \
    void * stackptr2 = 0x00; \
    UINT32 indents2 = pmc_log_indent_get(&stackptr2) * PMC_LOG_SPACES_PER_INDENT; \
    fprintf(stdout, "%*s", indents2, ""); /* fprintf() allowed in porting function */\
    fprintf(stdout, format, ## args);     /* fprintf() allowed in porting function */\
} while(0)    

/***************************************************************************
* MACRO: PMC_PRINT_NO_INDENT()
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used instead of printf() or PMC_PRINT() if you do not want
*   indentation
*
* INPUTS:
*   format - standard printf() 'format' entry
*   args   - variable arguments list for standard printf()
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_PRINT_NO_INDENT(format, args...) do { /* PMC_PRINT_NO_INDENT() allowed in porting function */\
    fprintf(stdout, format, ## args); /* fprintf() allowed in porting function */\
} while(0)

/***************************************************************************
* MACRO: PMC_FPRINTF()
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used instead of fprintf(). 
*
* INPUTS:
*   file_ptr - output file
*   args     - varaible arguments list for standard printf()
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_FPRINTF(file_ptr, args...) \
    fprintf(file_ptr, ## args);     /* fprintf() allowed in porting function */

/***************************************************************************
* MACRO: PMC_ASSERT
* __________________________________________________________________________
*
* DESCRIPTION:
*   The following performs the assert operation for various environments. 
*   The condition is checked and if FALSE then information is logged and
*   a fatal exit is invoked in an environment dependent manner.
*   
* INPUTS:
*   condition - check to perform.  If FALSE then the code will halt or
*               in the verification case return to the calling program.
*   log_code - see pmc_log_app_event3()
*   word2 - see pmc_log_app_event3()
*   word3 - see pmc_log_app_event3()
* 
* NOTES:
*   This function expects the application to define a pmc_assert_function
*   which typically includes an assert(). Allowing the assert to be a 
*   define results in the proper line and file info output since it is
*   put inline where the check is occurs.
****************************************************************************/
#define PMC_ASSERT(condition, log_code, word2, word3) \
    do {if (!(condition)) { \
        PMC_LOG(PMC_LOG_SEV_HIGHEST, log_code, word2, word3); \
        pmc_assert_function(log_code, __FILE__, __LINE__); \
        }} while(0)


/***************************************************************************
* MACRO: PMC_MALLOC
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to allocate local memory.
*
* INPUTS:
*   size - number of bytes to allocate
* 
* RETURNS:
*   *void - allocated buffer. Asserts on failure
*
* NOTES:
*   
****************************************************************************/
static INLINE void *PMC_MALLOC(UINT32 size)
{
   void *ptr = (char *)malloc(size);
   PMC_ASSERT(ptr != NULL, PMC_SHARED_ERR_MALLOC_FAILED, 0, 0 );
   return (void *)ptr;
}


/***************************************************************************
* pmc_calloc_function
* __________________________________________________________________________
*
* DESCRIPTION:
*   The PMC_CALLOC macro and the function it calls pmc_calloc_function are
*   used to allocate local memory and set to zero. 
*
* INPUTS:
*   size - number of bytes to allocate
* 
* RETURNS:
*   *void - allocated buffer set to zero. Asserts on failure
* 
* NOTES:
*   
****************************************************************************/
static INLINE void *pmc_calloc_function(UINT32 size)
{
    char *ptr = (char *)calloc(1,size);
    PMC_ASSERT( ptr != NULL, PMC_SHARED_ERR_MALLOC_FAILED, 0, 0 );
    return (void *)ptr;
}


/***************************************************************************
* MACRO: PMC_CTXT_CALLOC
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to allocate memory from the context.  Since PMC_CALLOC is
*   used to allocate the context in the first place, the memory will be set to
*   0.
*
* INPUTS:
*   size - number of bytes to allocate
*   pmc_handle - a pointer to a pmc_handle, which contains a pointer to the
*   context
* 
* RETURNS:
*   *void - allocated buffer set to zero. Asserts on failure
* NOTES:
*   
****************************************************************************/
#ifdef USE_PMC_CTXT_CALLOC
# define PMC_CTXT_CALLOC(size, pmc_handle) pmc_ctxt_calloc_helper(size, (pmc_handle_t *)pmc_handle, __FUNCTION__, __LINE__);
#else
# define PMC_CTXT_CALLOC(size, pmc_handle) PMC_CALLOC(size);
#endif

#ifdef USE_PMC_CTXT_CALLOC
static INLINE void *pmc_ctxt_calloc_helper(UINT32 size,pmc_handle_t *pmc_handle, const char* func_name, UINT32 line_number)
{
    return(pmc_ctxt_calloc_function(size, pmc_handle->ctxt, func_name, line_number));
}
#endif

/***************************************************************************
* MACRO: PMC_CTXT_REGISTER_PTR
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to register a pointer within the context so it can be
*   fixed up if the context is moved.
*
* INPUTS:
*   ptr - location of the pointer to register
*   pmc_handle - a pointer to a pmc_handle, which contains a pointer to the
*   context
* 
* RETURNS:
*   nothing
* NOTES:
*   
****************************************************************************/
#ifdef USE_PMC_CTXT_CALLOC
# define PMC_CTXT_REGISTER_PTR(ptr, pmc_handle) pmc_ctxt_ptr_register_helper(ptr, (pmc_handle_t *) pmc_handle);
#else
# define PMC_CTXT_REGISTER_PTR(ptr, pmc_handle);
#endif

#ifdef USE_PMC_CTXT_CALLOC
static INLINE void pmc_ctxt_ptr_register_helper(void *ptr, pmc_handle_t* pmc_handle)
{
    pmc_ctxt_ptr_register(ptr, pmc_handle->ctxt);
}
#endif

#if (USE_PMC_MEM_TRACK == TRUE)  /* track mem allocations to DB */
#define PMC_FREE(ptr) \
        pmc_alloc_track_log_free_to_db((UINT32*)*ptr,__FILE__,(char*)__FUNCTION__,__LINE__); \
        pmc_free_function_track((void **)ptr);  
/***************************************************************************
* pmc_free_function
* __________________________________________________________________________
*
* DESCRIPTION:
*   The PMC_FREE macro and the function it calls pmc_free_function() are
*   used to free local memory. If the pinter is already NULL
*   then a warning is logged. The pointer is also set to NULL when returned.
*
* INPUTS:
*   **ptr - pointer to free Same as standard library free as this macro simply replace the name
*   in the function call.
* 
* OUTPUTS:
*    **ptr - set to NULL
* 
****************************************************************************/
static INLINE void pmc_free_function_track(void **ptr)
{
    if (ptr == NULL || *ptr == NULL) 
    { 
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0);
    }
    else
    {
        free(*ptr);
        *ptr = NULL;
    }
}

#else   /* track mem allocations to DB  disabled */

#define PMC_FREE(ptr)       pmc_free_function((void **)(ptr))
/***************************************************************************
* pmc_free_function
* __________________________________________________________________________
*
* DESCRIPTION:
*   The PMC_FREE macro and the function it calls pmc_free_function() are
*   used to free local memory. If the pinter is already NULL
*   then a warning is logged. The pointer is also set to NULL when returned.
*
* INPUTS:
*   **ptr - pointer to free Same as standard library free as this macro simply replace the name
*   in the function call.
* 
* OUTPUTS:
*    **ptr - set to NULL
* 
****************************************************************************/
static INLINE void pmc_free_function(void **ptr)
{
    if (ptr == NULL || *ptr == NULL) 
    { 
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0, "\n");
    }
    else
    {
        free(*ptr);
        *ptr = NULL;
    }
}

#endif

/***************************************************************************
* MACRO: PMC_CTXT_FREE
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to free context memory. If the pointer is already NULL
*   then a warning is logged. The pointer is also set to NULL when returned.
*
* INPUTS:
*   **ptr - pointer to free.
*   pmc_handle - a pointer to a pmc_handle, which contains a pointer to the
*   context
* 
* OUTPUTS:
*    **ptr - set to NULL
* 
****************************************************************************/
/* The following #define is necessary when using newer compilers like gcc 4.4.3 which
   will warn on passing a double pointer being passed that isn't a pointer to a (void *)*/
#ifdef USE_PMC_CTXT_CALLOC
# define PMC_CTXT_FREE(ptr, pmc_handle) pmc_ctxt_free_helper((void **)(ptr),(pmc_handle_t *)pmc_handle, __func__);
#else
# define PMC_CTXT_FREE(ptr, pmc_handle) PMC_FREE((void **)(ptr));
#endif

#ifdef USE_PMC_CTXT_CALLOC
static INLINE void pmc_ctxt_free_helper(void **ptr, pmc_handle_t* pmc_handle, const char *func)
{
    pmc_ctxt_free_function(ptr,pmc_handle->ctxt, func);
}
#endif

/***************************************************************************
* MACRO: PMC_MEMSET
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to fill memory area with same data pattern .
*
* INPUTS:
*   mem_ptr - start of memory area to fill with the patern.
*   fill - patern to use when filling memory area.
*   num_bytes - number of bytes to write to memory area.
* 
* RETURNS: None.
*
* NOTES:
*   
****************************************************************************/
#define PMC_MEMSET(mem_ptr, fill, num_bytes) do {\
    PMC_ASSERT( mem_ptr != NULL, PMC_SHARED_ERR_MEMSET_FAILED, 0, 0 );\
    PMC_ASSERT( num_bytes != 0, PMC_SHARED_ERR_MEMSET_FAILED, 0, 0 );\
    memset( mem_ptr, fill, num_bytes );\
} while(0)


/***************************************************************************
* MACRO: PMC_MEMCPY
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to copy one memory area to another.
*
* INPUTS:
*   dest      - Destination memory address
*   src       - Source memory address
*   num_bytes - number of bytes to copy
* 
* RETURNS: None.
*
* NOTES:
*   
****************************************************************************/
#define PMC_MEMCPY(dest, src, num_bytes) do {\
    PMC_ASSERT( dest != NULL, PMC_SHARED_ERR_MEMCPY_FAILED, 0, 0 );\
    PMC_ASSERT( src != NULL, PMC_SHARED_ERR_MEMCPY_FAILED, 0, 0 );\
    PMC_ASSERT( num_bytes != 0, PMC_SHARED_ERR_MEMCPY_FAILED, 0, 0 );\
    memcpy( dest, src, num_bytes );\
} while(0)

/***************************************************************************
* MACRO: PMC_MEMCMP
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to compare one memory area to another.
*
* INPUTS:
*   dest      - Source memory address 1
*   src       - Source memory address 2
*   num_bytes - number of bytes to compare
* 
* RETURNS: None.
*   0 - strings match
*   !0 - strings do not match
* 
* NOTES:
*   
****************************************************************************/
static INLINE INT32 PMC_MEMCMP(const void *dest, const void *src, size_t num_bytes)
{
    PMC_ASSERT( dest != NULL, PMC_SHARED_ERR_MEMCMP_FAILED, 0, 0);
    PMC_ASSERT( src != NULL, PMC_SHARED_ERR_MEMCMP_FAILED, 0, 0);
    return(memcmp(dest, src, num_bytes));
} /* PMC_MEMCMP() */

/***************************************************************************
* MACRO: PMC_STRLEN
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to calculate the length of the string src_ptr, 
*   not including the terminating '\0' character
*
* INPUTS:
*   src_ptr   - Source address for the start of the string.
* 
* RETURNS: 
*   The length of the string not including the termination '\0'.
* 
* NOTES:
*   
****************************************************************************/
static INLINE INT32 PMC_STRLEN(const void *src_ptr)
{
    PMC_ASSERT( src_ptr != NULL, PMC_SHARED_ERR_STRLEN_FAILED, 0, 0);
    return(strlen((const char*)src_ptr));
} /* PMC_STRLEN() */
  
/***************************************************************************
* MACRO: PMC_STRCPY
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to copy one string to another (or new memory loc)
*   
*
* INPUTS:
*   src_ptr   - Source address for the start of the string.
*   dst_ptr   - the address where you want the string to be copied to
*               must not overlab with source
* RETURNS: 
*   The destination pointer '\0'.
* 
*NOTES:
*   
****************************************************************************/
static INLINE char * PMC_STRCPY(char * dst_ptr,const char *src_ptr)
{
    PMC_ASSERT( src_ptr != NULL, PMC_SHARED_ERR_STRCPY_FAILED, 0, 0);
    PMC_ASSERT( dst_ptr != NULL, PMC_SHARED_ERR_STRCPY_FAILED, 0, 0);
    return(strcpy(dst_ptr,src_ptr));
};
/***************************************************************************
* MACRO: PMC_STRCAT
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to concat 2 strings and stores them at dst 
*
* INPUTS:
*   src_ptr   - Source address for the start of the string.
*   dst_ptr   - new location for the combined string to be stored
*               
* RETURNS: 
*   The destination pointer '\0'.
* 
* NOTES:
*   
****************************************************************************/
static INLINE char * PMC_STRCAT(char * dst_ptr,const char *src_ptr)
{
    PMC_ASSERT( src_ptr != NULL, PMC_SHARED_ERR_STRCAT_FAILED, 0, 0);
    PMC_ASSERT( dst_ptr != NULL, PMC_SHARED_ERR_STRCAT_FAILED, 0, 0);
    return(strcat(dst_ptr,src_ptr));
};
/***************************************************************************
* MACRO: PMC_SPRINTF
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used instead of sprintf. 
*
* INPUTS:
*   dst    - ptr to the location for the new string
*   format - standard printf 'format' entry
*   args - varaible arguments list for standard printf
*
* returns : int, size of the new string
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_SPRINTF(dst,format, args...) sprintf(dst, format, ## args)
/* Maximum length for formatted timestamp;
 */
#define PMC_LOG_TIMESTAMP_LEN 64


/***************************************************************************
* MACRO: PMC_TIMEVAL_READ
* __________________________________________________________________________
*
* DESCRIPTION:
*   fetches time of day
*
* INPUTS:
*       ptimeval = pointer to pmc_sys_timeval_t structure
* returns : nothing
*
****************************************************************************/
#define PMC_TIMEVAL_READ(ptimeval) do { \
    struct timeval ts;\
    gettimeofday(&ts,NULL); \
    (ptimeval)->tv_sec  = (UINT32)ts.tv_sec;\
    (ptimeval)->tv_usec = ts.tv_usec;\
}while(0)

/***************************************************************************
* MACRO: PMC_TIME_GET
* __________________________________________________________________________
*
* DESCRIPTION:
*   returns seconds and usec for the logger
*
* INPUTS:
*       sec  = place for seconds to be assigned
*       usec = place for usec to be assigned
* returns : nothing
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_TIME_GET(sec,usec) do { \
    sec  = pmc_log_seq_counter++;\
}while(0);

/***************************************************************************
* MACRO: PMC_LOG_TIME2STR
* __________________________________________________________________________
*
* DESCRIPTION:
*   used to turn the timestamp into a user readable string
*
* INPUTS:
*   buf : pointer to buffer to put the formated string into
*   sec : seconds to turn into string
*   usec: microseconds to turn into string
* returns : nothing
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/

#define PMC_LOG_TIME2STR(buf,sec,usec) do { \
    PMC_SPRINTF(buf,"%d",sec);\
}while(0);

/*******************************************************************************
*  simulation_sys_reg_read_modify_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a burst write of an array of 32 bit
*                 wide registers.
*
*  INPUTS:
*                 sys     - customer's opaque handle.
*                 addr    - start address of the register location to be written
*                           as it appears in the hardware data sheet
*                 mask    - bits to update.
*                 value   - new value for the updated field (note: this value
*                           must be pre-shifted)
*
*  OUTPUTS:
*                 *oldval  - previous value of the register.
*                            note: NULL may be passed in for oldval.
*
*  RETURNS:
*                 UINT32 - value written to the register.
*
*  EXAMPLE:
*  suppose the previous value of the register was 0x12345678 and
*  we want to update the middle bits to 0xcafe.
*
*  UINT32 mask  = 0x00ffff00;
*  UINT32 value = 0x00cafe00;
*  UINT32 newval;
*  UINT32 oldval;
*
*  newval = pmc_sys_reg_read_modify_write( hndl, addr, mask, value, &oldval );
*
*  // at this point oldval and new val would be:
*  // oldval = 0x12345678
*  // newval = 0x12cafe78
*
******************************************************************************/
UINT32 simulation_sys_reg_read_modify_write( pmc_sys_handle_t *sys, 
                                             UINT32 addr,
                                             UINT32 mask,
                                             UINT32 value,
                                             UINT32 *oldval );


/*******************************************************************************
* simulation_sys_reg_read
* ______________________________________________________________________________
*
* DESCRIPTION:
    Simulation read.
*
* INPUTS:
*   sys    - System handle
*   addr   - 32-bit device address to read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - 32-bit register data
*
* NOTES:
*
*******************************************************************************/
UINT32 simulation_sys_reg_read( pmc_sys_handle_t *sys, UINT32 addr );

/*******************************************************************************
* simulation_sys_reg_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Simulation write.
*
* INPUTS:
*   sys    - System handle
*   addr   - 32-bit device address to write
*   value  - 32-bit register value to write
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
void simulation_sys_reg_write( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value );

/*******************************************************************************
* simulation_sys_burst_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Simulation burst read
*
* INPUTS:
*   sys    - System handle
*   addr   - First 32-bit device address to read.
*   len    - Number of registers to read in the burst.
*
* OUTPUTS:
*   value   - Array of 32-bit unsigned integers.
*             Space must be allocated for len entries.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
void simulation_sys_burst_read(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *value );

/*******************************************************************************
* simulation_sys_mem_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a memory write function for simulation to transfer
*   byte-array values (e.g. firmware image). This operation transfers is
*   intended to be an endian agnostic transfer and transfers a byte length.
*
* INPUTS:
*   sys    - System handle
*   addr   - First device address to write.
*   len    - Number of bytes to write.
*   data_ptr   - Array of len bytes.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
void simulation_sys_mem_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT8 *data_ptr );

/*******************************************************************************
* simulation_sys_burst_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Simulation burst write
*
* INPUTS:
*   sys    - System handle
*   addr   - First 32-bit device address to write.
*   len    - Number of registers to write in the burst.
*   value  - Array of len 32-bit unsigned integers.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
void simulation_sys_burst_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *value );

/*******************************************************************************
*  simulation_sys_poll_bit
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function polls a register field until the desired value is read.
*
*  INPUTS:
*    sys       - opaque simulation handle.
*    addr      - address of the register location to be read
*                as it appears in the hardware data sheet
*    mask      - field being examined.
*    value     - desired value of the field (note: this value
*                must be pre-shifted)
*    max_count - maximum number of times to read the register.
*    delay_between_polls_in_microseconds - how long to
*                wait between register reads.
*
*  OUTPUTS:
*    *num_failed_polls - number of register reads that were
*                        not the desired value.
*
*  RETURNS:
*   PMC_SUCCESS - if successful, otherwise PMC_ERR_POLL_TIMEOUT
* 
*  NOTES:
******************************************************************************/
PMC_ERROR simulation_sys_poll_bit( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

/*******************************************************************************
*  simulation_init_simple_defaults
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*    Initialize the simulation memory with a set of default values
*
*  INPUTS:
*    sys - opaque simulation handle.
*    defaults - array of defautls to use in address/data format
*    num_defaults - size of the defaults array
*
*  OUTPUTS:
*    None
* 
*  RETURNS:
*    None
* 
*  NOTES:
******************************************************************************/
void simulation_init_simple_defaults( pmc_sys_handle_t *sys, simple_memory_defaults *defaults, int num_defaults );

/*******************************************************************************
*  simulation_add_hook
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*    Calls fn() whenever addr is read or written
*
*  INPUTS:
*    sys - opaque simulation handle.
*    addr - address of the register location to trigger the callback on
*    fn - function to call
*    param - parameter to pass to fn when called
*
*  OUTPUTS:
*    None
*
*  RETURNS:
*   None
* 
*  NOTES:
******************************************************************************/
void simulation_add_hook( pmc_sys_handle_t *sys, UINT32 addr, simulation_function_t fn, void *param );

/*******************************************************************************
*  simulation_sys_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*    *path_ptr   - path to file
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to streem, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* simulation_sys_file_open(const char* path_ptr);

/*******************************************************************************
*  simulation_sys_output_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens an output file.
*
*  INPUTS:        
*    *path_ptr   - path to file
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to streem, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* simulation_sys_output_file_open(const char* path_ptr);

/*******************************************************************************
*  simulation_sys_file_close
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function closes a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*     *stream_ptr  - file stream pointer
*
*  OUTPUTS:       
*     None.
*
*  RETURNS:
*     PMC_SUCCESS - on SUCCESS. Error code otherwise. 
*
*******************************************************************************/
PUBLIC PMC_ERROR simulation_sys_file_close(FILE* stream_ptr);

/*******************************************************************************
*  simulation_sys_file_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function reads 4-bytes from the file at the specified pointer 
*    location. Content must be updated by user for their specific system 
*    requirements.
*
*  INPUTS:        
*    *f_ptr       - file stream pointer
*    *buffer_ptr  - buffer pointer
*    *offset_ptr  - offset into the buffer pointer
*    *rd_len_ptr  - Read length pointer
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:        
*    PMC_SUCCESS on Success, failure otherwise.
*
******************************************************************************/
PUBLIC PMC_ERROR simulation_sys_file_read(FILE* f_ptr, UINT8* buffer_ptr, UINT32* offset_ptr, UINT8* rd_len_ptr);

#ifndef __USE_BSD
#define __USE_BSD
#endif
#include <unistd.h> /* for usleep() */
#include <time.h> /*  for struct timespec */

#define PMC_OS_USLEEP(time_in_microseconds)  do {\
    LOG("Sleeping for %d microseconds",time_in_microseconds);\
    pmc_usleep(time_in_microseconds);\
    PMC_OS_STATS_USLEEP(time_in_microseconds);\
} while(0)

#define PMC_OS_USLEEP_NO_LOG(time_in_microseconds)  do {\
    pmc_usleep(time_in_microseconds);\
    PMC_OS_STATS_USLEEP(time_in_microseconds);\
} while(0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PMC_SW_SIMULATION_H */
