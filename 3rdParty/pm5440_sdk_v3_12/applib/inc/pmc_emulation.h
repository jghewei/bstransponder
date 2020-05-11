/*******************************************************************************
*  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*     This file contains the definitions and inline fucntions for the pmc
*     emulation board system. Included are the emulation sys_handle, read,
*     write, burst read, burst write, and read-modify-write.
* 
*  NOTES:
*     None.
*******************************************************************************/

#ifndef PMC_EMULATION_OS_H
#define PMC_EMULATION_OS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "pmc_types.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_ctxt_mgmt.h"
#include "pmc_handle.h"

/* Make sure we are using GCC helpers for memcpy and memset. These can catch
   some nasty size problems in compile time. */
#undef memcpy
#define bos0(dest) __builtin_object_size (dest, 0)
#define memcpy(dest, src, n) \
    __builtin___memcpy_chk (dest, src, n, bos0 (dest))

#undef memset
#define bos0(dest) __builtin_object_size (dest, 0)
#define memset(dest, data, n) \
    __builtin___memset_chk (dest, data, n, bos0 (dest))

/* For double */
#define PMC_FLOOR(arg) floor(arg)
#define PMC_CEIL(arg) ceil(arg)
#define PMC_MATH_LOG(arg) log(arg)
#define PMC_ABS(arg) abs(arg)
#define PMC_POW(a,b) pow(a,b)
#define PMC_FABS(arg) fabs(arg)
/* For float */
#define PMC_FLOORF(arg) floor(arg)
#define PMC_CEILF(arg) ceil(arg)
#define PMC_MATH_LOGF(arg) log(arg)
#define PMC_POWF(a,b) pow(a,b)
#define PMC_FABSF(arg) fabs(arg)

#define PMC_ROUND(x)   ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define DIGI_EMUL_MMAP_SIZE         0x02000000

#define PMC_CALLOC(size) pmc_calloc_function(size)
#define PMC_FREE(ptr)  pmc_free_function((void **)(ptr))

/* use 64 bit reads and writes for burst reads & burst writes */
#define PMC_64BIT_PCIE


#define EMUL_MAX_DIGI_NAME_SIZE 12


/*******************************************************************************
* ENUM: pmc_event_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI events.
*
* ELEMENTS:
*       PMC_EVENT_ID_NONE - ignored.
*       PMC_EVENT_ID_OB_PCIE_IRQ_MON - Monitor top level interrupt indicators
*                                           towards PCIe.
*       PMC_EVENT_ID_LAST - must be always last.
*
*******************************************************************************/
typedef enum pmc_event_id
{
    PMC_EVENT_ID_NONE = 0,
    PMC_EVENT_ID_OB_PCIE_IRQ_MON = 1,
    PMC_EVENT_ID_LAST,
} pmc_event_id_t;

/*******************************************************************************
* STRUCT: emulation_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base system handle that is passed to all device operations.
*   Allocated with customer_sys_handle_create().
*   Freed with customer_sys_handle_destroy().
*   Initialized wtih customer_sys_init().
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
/* DO NOT CHANGE MEMBER ORDER IN THIS STRUCTURE. */
typedef struct emulation_sys_handle_t {
    
    void *internal_sys_handle; /*!< Sys handle structure must start with this internal PMC pointer */
    struct hostmsg_handle_t *hostmsg_handle;  /*!< Host message specific information. */

    /* Add customer specific information here such as base address to device */
    void       *base_addr_ptr; /*!< Base address of device (example value that can be replaced if not needed in customer system) */
    char        name_ptr[EMUL_MAX_DIGI_NAME_SIZE+1]; /*!< Name of instantiation of device (example value that can be replaced if not needed in customer system) */
    UINT32      mmap_size; /*!< size of mmap-ed memory (in bytes) */
    const char *dev_name_ptr; /*!< pointer to device name string. May be NULL if not used. */
    void       *shmem_dma_pool_ptr; /*!< pointer to DMA shared memory pool. */
    UINT32      shmem_dma_pool_size; /*!< DMA shared memory pool size (in 32-bit words). */
    UINT64      shmem_dma_pool_phys; /*!< DMA shared memory physical base address. */
    void       *pmc_mem_pool_ptr; /*!< DMA shared memory pool control structure. */
    void       *pmc_event_table_ptr; /*!< interrupt event table */
    void       *shmem_mmap_ptr; /*!< pointer to the shared memory */
    UINT32     last_write_addr; /*!< track last addr written for read/write ordering */
} emulation_sys_handle_t;

PUBLIC pmc_sys_handle_t *emulation_sys_handle_create(void);
PUBLIC PMC_ERROR emulation_sys_init(pmc_sys_handle_t *sys_handle, void *sys_cfg_ptr, const char *name_ptr);
PUBLIC void emulation_sys_handle_destroy(pmc_sys_handle_t **sys_handle);
PUBLIC void pmc_assert_function(PMC_ERROR error_code, const char *file_ptr, UINT32 line) ATTRIBUTE_NORETURN;
PUBLIC FILE* emulation_sys_file_open(const char* path_ptr);
PUBLIC FILE* emulation_sys_output_file_open(const char* path_ptr);
PUBLIC PMC_ERROR emulation_sys_file_close(FILE* stream_ptr);
PUBLIC PMC_ERROR emulation_sys_file_read(FILE* f_ptr, UINT8* buffer_ptr, UINT32* offset_ptr, UINT8* rd_len_ptr);
PUBLIC void emulation_event_init(pmc_sys_handle_t *sys_handle);
PUBLIC void emulation_event_destroy(pmc_sys_handle_t *sys_handle);
PUBLIC PMC_ERROR emulation_event_register(pmc_sys_handle_t *sys_handle, pmc_event_id_t event_id, void *event_cfg);
PUBLIC void emulation_event_monitor(pmc_sys_handle_t *sys_handle, void *event_cfg);
PUBLIC void pmc_mem_pool_destroy_all(pmc_sys_handle_t **sys_handle);
PUBLIC void pmc_mem_init(pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_usleep(UINT64 usec);


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
#define PMC_PRINT(format, args...) do { /* PMC_PRINT() allowed in porting function */\
    void * stackptr2 = 0x00; \
    UINT32 indents2 = pmc_log_indent_get(&stackptr2) * PMC_LOG_SPACES_PER_INDENT; \
    fprintf(stdout, "%*s", indents2, "");  /* fprintf() allowed in porting function */\
    fprintf(stdout, format, ## args);  /* fprintf() allowed in porting function */ \
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
*   args - varaible arguments list for standard printf()
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_PRINT_NO_INDENT(format, args...) do {  /* PMC_PRINT_NO_INDENT() allowed in porting function */\
    fprintf(stdout, format, ## args); /* fprintf() allowed in porting function */ \
} while(0)

/***********************************************
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
#define PMC_FPRINTF(file_ptr, args...)\
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
    PMC_ASSERT(size != 0, PMC_SHARED_ERR_MALLOC_FAILED, 0, 0);
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
# define PMC_CTXT_CALLOC(size, pmc_handle) pmc_calloc_function(size);
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
*   ptr - The location of the pointer to register.
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
        PMC_LOG(PMC_LOG_SEV_HIGH, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0);
    }
    else
    {
        free(*ptr);
        *ptr = NULL;
    }
}

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
# define PMC_CTXT_FREE(ptr, pmc_handle) pmc_free_function((void **)(ptr));
#endif

#ifdef USE_PMC_CTXT_CALLOC
static INLINE void pmc_ctxt_free_helper(void **ptr, pmc_handle_t* pmc_handle, const char *func)
{
    return(pmc_ctxt_free_function(ptr,pmc_handle->ctxt, func));
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
* RETURNS:
*   None.
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
    struct timeval ts;\
    gettimeofday(&ts,NULL); \
    sec  = (UINT32)ts.tv_sec;\
    usec = ts.tv_usec;\
}while(0)

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
    time_t tm = sec;\
    PMC_STRCPY(buf,ctime(&tm));\
}while(0);

#ifndef __USE_BSD
#define __USE_BSD
#endif
#include <unistd.h> /* for usleep() */
#include <time.h> /*  for struct timespec */

#define PMC_OS_USLEEP(time_in_microseconds)  do {\
    LOG("Sleeping for %d microseconds at %s:%d\n",time_in_microseconds, __FILE__, __LINE__);\
    pmc_usleep(time_in_microseconds);\
    PMC_OS_STATS_USLEEP(time_in_microseconds);\
} while(0)

#define PMC_OS_USLEEP_NO_LOG(time_in_microseconds)  do {\
    pmc_usleep(time_in_microseconds);\
    PMC_OS_STATS_USLEEP(time_in_microseconds);\
} while(0)


#define INLINE_SYS
#ifdef INLINE_SYS

/*******************************************************************************
* emulation_sys_reg_read
* ______________________________________________________________________________
*
* DESCRIPTION:
    Base emulation read.
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
static INLINE UINT32 emulation_sys_reg_read( pmc_sys_handle_t *sys, UINT32 addr ) __attribute((__always_inline__));
static INLINE UINT32 emulation_sys_reg_read( pmc_sys_handle_t *sys, UINT32 addr )
{
    UINT32 value;
    volatile UINT32 temp_value;
    emulation_sys_handle_t *emul_sys = (emulation_sys_handle_t*)sys;


    if (addr == emul_sys->last_write_addr)
    {
        /* when the last write was at the same address, we read another time to
           avoid getting an ordered RD */
        temp_value = *(volatile UINT32 *) ((PMC_UINTPTR)emul_sys->base_addr_ptr + addr);
        value = temp_value;
    }

    value = *(volatile UINT32 *) ((PMC_UINTPTR)emul_sys->base_addr_ptr + addr);

    /* the last write is always invalidated */
    emul_sys->last_write_addr = 0xFFFFFFFF;

    /* This macro swaps the bytes if the host is big endian (PMC_BIG_ENDIAN defined)
       for LE hosts this macro becomes value = value */
    value = PMC_LE_TO_CPU_32(value);

    return value;
}

#ifdef PMC_64BIT_PCIE
/*******************************************************************************
* emulation_sys_reg_read64
* ______________________________________________________________________________
*
* DESCRIPTION:
    Base emulation read.
*
* INPUTS:
*   sys    - System handle
*   addr   - 32-bit device address to read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT64 - 64-bit register data
*
* NOTES:
*
*******************************************************************************/
static INLINE UINT64 emulation_sys_reg_read64( pmc_sys_handle_t *sys, UINT32 addr )
{
    UINT64 value;
    volatile UINT64 temp_value;

    emulation_sys_handle_t *emul_sys = (emulation_sys_handle_t*)sys;

    if (addr == emul_sys->last_write_addr)
    {
        /* when the last write was at the same address, we read another time to 
           avoid getting an ordered RD */
        temp_value = *(volatile UINT64 *) ((PMC_UINTPTR)emul_sys->base_addr_ptr + addr);
        value = temp_value;
    }

    value = *(volatile UINT64 *) ((PMC_UINTPTR)emul_sys->base_addr_ptr + addr);

    /* the last write is always invalidated */
    emul_sys->last_write_addr = 0xFFFFFFFF;

    /* This macro swaps the bytes if the host is big endian (PMC_BIG_ENDIAN defined)
       for LE hosts this macro becomes value = value */
    value = PMC_LE_TO_CPU_64(value);

    return value;
}
#endif

/*******************************************************************************
* emulation_sys_reg_write
* ______________________________________________________________________________
*
* DESCRIPTION:
    Base emulation write.
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
static INLINE void emulation_sys_reg_write( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value ) ALWAYS_INLINE;
static INLINE void emulation_sys_reg_write( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value )
{
    emulation_sys_handle_t *emul_sys = (emulation_sys_handle_t*)sys;

    /* This macro swaps the bytes if the host is big endian (PMC_BIG_ENDIAN defined)
       for LE hosts this macro becomes value = value */
    value = PMC_CPU_TO_LE_32(value);

    *(volatile UINT32 *)((PMC_UINTPTR)emul_sys->base_addr_ptr + addr) = value;

    emul_sys->last_write_addr = addr;
}

#ifdef PMC_64BIT_PCIE
/*******************************************************************************
* emulation_sys_reg_write64
* ______________________________________________________________________________
*
* DESCRIPTION:
    Base emulation write.
*
* INPUTS:
*   sys    - System handle
*   addr   - 32-bit device address to write
*   value64  - 64-bit register value to write
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
static INLINE void emulation_sys_reg_write64( pmc_sys_handle_t *sys, UINT32 addr, UINT64 value64 )
{
    emulation_sys_handle_t *emul_sys = (emulation_sys_handle_t*)sys;

    /* This macro swaps the bytes if the host is big endian (PMC_BIG_ENDIAN defined)
       for LE hosts this macro becomes value = value */
    value64 = PMC_CPU_TO_LE_64(value64);

    *(volatile UINT64 *)((PMC_UINTPTR)emul_sys->base_addr_ptr + addr) = value64;

    emul_sys->last_write_addr = addr;
}
#endif

/*******************************************************************************
* emulation_sys_burst_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation burst read
*
* INPUTS:
*   sys    - System handle
*   addr   - First 32-bit device address to read.
*   len    - Number of registers to read in the burst.
*
* OUTPUTS:
*   data   - Array of 32-bit unsigned integers.
*            Space must be allocated for len entries.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
static INLINE void emulation_sys_burst_read(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *data )
{
#ifdef PMC_64BIT_PCIE
    if (len == 0) {
        return;
    }
    /* if reading from an odd address, do a single read first */
    if (addr & 0x4) {
        *data = emulation_sys_reg_read( sys, addr );
        addr += 4;
        data++;
        len --;
    }
    while (len >= 2)
    {
        /* might not work on big endian architecture ? */
        *(UINT64 *)data = emulation_sys_reg_read64( sys, addr );
        data += 2;
        addr += 8;
        len  -= 2;
    }
    /* read final word if needed. */
    if (len)
    {
        *data = emulation_sys_reg_read( sys, addr );
    }
#else
    while (len > 0)
    {
        *data = emulation_sys_reg_read(sys,addr);
        addr += 4;
        data ++;
        len  --;
    }
#endif
}

/*******************************************************************************
* emulation_sys_burst_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation burst write
*
* INPUTS:
*   sys    - System handle
*   addr   - First 32-bit device address to write.
*   len    - Number of registers to write in the burst.
*   data   - Array of len 32-bit unsigned integers.
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
static INLINE void emulation_sys_burst_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *data )
{
#ifdef PMC_64BIT_PCIE
    if (len == 0) {
        return;
    }
    /* if writing to an odd address, do a single write first */
    if (addr & 0x4) {
        emulation_sys_reg_write( sys, addr, *data );
        addr += 4;
        data++;
        len --;
    }
    while (len >= 2)
    {
        /* might not work on big endian architecture ? */
        emulation_sys_reg_write64( sys, addr, *(UINT64 *)data );
        data += 2;
        addr += 8;
        len  -= 2;
    }
    /* write final word if needed. */
    if (len)
    {
        emulation_sys_reg_write( sys, addr, *data );
    }
#else
    while (len > 0)
    {
        emulation_sys_reg_write(sys,addr,*data);
        addr += 4;
        data ++;
        len  --;
    }
#endif
}


/*******************************************************************************
* emulation_sys_reg_read_modify_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation read modify write
*
* INPUTS:
*   sys     - System handle
*   addr    - 32-bit device address to write
*   mask    - Mask of bits to be modified
*   value   - 32-bit register value to write
*
* OUTPUTS:
*   *oldval - Optional. Original value in register.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
static INLINE UINT32 emulation_sys_reg_read_modify_write( pmc_sys_handle_t *sys, 
                                                    UINT32 addr,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    UINT32 *oldval ) ALWAYS_INLINE;
static INLINE UINT32 emulation_sys_reg_read_modify_write( pmc_sys_handle_t *sys, 
                                                    UINT32 addr,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    UINT32 *oldval )
{
    UINT32 regval;
    regval = emulation_sys_reg_read(sys,addr);
    if (oldval != NULL)
    {
        *oldval = regval; /* save old value for logging purposes */
    }
    regval &= ~mask;
    regval |= (mask & value);
    emulation_sys_reg_write(sys, addr, regval );

    return regval; /* return value written, mostly for logging purposes */
}

/*******************************************************************************
* emulation_sys_mem_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a memory write function to the DIGI device used to transfer
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
static INLINE void emulation_sys_mem_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT8 *data_ptr )
{
    emulation_sys_handle_t *emul_sys = (emulation_sys_handle_t*)sys;
    UINT8 *dst_ptr;

    dst_ptr = (UINT8 *)emul_sys->base_addr_ptr + addr;

    PMC_MEMCPY(dst_ptr, data_ptr, len);
}

#else

/* If not inline, then define in pmc_emulation.c */
UINT32 emulation_sys_reg_read( pmc_sys_handle_t *sys, UINT32 addr );

void emulation_sys_reg_write( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value );

void emulation_sys_burst_read(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *value );

void emulation_sys_burst_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *value );

void emulation_sys_mem_write(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT8 *data_ptr );

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

