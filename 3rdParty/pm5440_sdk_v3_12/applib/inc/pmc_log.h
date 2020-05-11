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
*  DESCRIPTION:
*     This header file provides the types, macros, inline functions, and
*     prototypes for the logging operations used throughout the software.
*
*  NOTES:
*     None.
*******************************************************************************/
#ifndef _PMC_LOG_H
#define _PMC_LOG_H

#ifdef __cplusplus
extern "C" { 
#endif

/*
** Include Files
*/

#include "pmc_types.h"
#include "pmc_err.h"
#include "pmc_handle.h"
#include <stdarg.h>
    
#ifdef LOGGING_DISABLED 
    #define PMC_LOG_GLOBAL_SEV_HIGH
#endif
/* Legacy code to support the old logging facility */
/* In all cases, legacy log calls call the current macro with a special medium
 * severity and a special log code for legacy calls */
#ifndef LOGGING_DISABLED
    #define LOG( fmt, ... ) do {\
        PMC_LOG_NOINFO(PMC_LOG_SEV_LOWEST,PMC_SHARED_LOG_CODE_LEGACY,0,0,"                         "); /* Create indent/prefix */ \
        PMC_LOG_NOINFO(PMC_LOG_SEV_LOWEST,PMC_SHARED_LOG_CODE_LEGACY,0,0, fmt, ##__VA_ARGS__);\
        PMC_LOG_NOINFO(PMC_LOG_SEV_LOWEST,PMC_SHARED_LOG_CODE_LEGACY,0,0,"\n");\
        } while(0)
    
    #define LOG_DATA( addr, data, len, fmt, ... )     do {\
        PMC_LOG(PMC_LOG_SEV_LOW,PMC_SHARED_LOG_CODE_LEGACY,0,0, fmt, ##__VA_ARGS__);\
        PMC_LOG_NOINFO(PMC_LOG_SEV_LOWEST,PMC_SHARED_LOG_CODE_LEGACY,0,0,"\n");\
        } while(0)
    
    #define LOG_FUNCTION( file,line,function, fmt, ... ) do {\
        PMC_LOG(PMC_LOG_SEV_LOW,PMC_SHARED_LOG_CODE_LEGACY,0,0, fmt, ##__VA_ARGS__);\
        PMC_LOG_NOINFO(PMC_LOG_SEV_LOWEST,PMC_SHARED_LOG_CODE_LEGACY,0,0,"\n");\
        } while(0)
#else
    
    #define LOG( fmt, ... ) do {\
        } while(0)
    #define LOG_DATA( addr, data, len, fmt, ... )     do {\
        } while(0)
    #define LOG_FUNCTION( file,line,function, fmt, ... ) do {\
        } while(0)
#endif

/* PMC_ENTRY/PMC_RETURN makes these extra arguments not useful...
#define LOG_CALLER_DEFN const char *_file_name, int _line_num, const char *_function_name,
#define LOG_CALLER_ARGS _file_name, _line_num, _function_name,
#define LOG_CALLER      __FILE__,__LINE__,__FUNCTION__,
*/

#define LOG_CALLER_DEFN 
#define LOG_CALLER_ARGS 
#define LOG_CALLER      

/* if logging is disabled then so is IO_LOGGING */
#ifndef NO_IO_LOGGING
# ifdef LOGGING_DISABLED
#  define NO_IO_LOGGING
# endif
#endif

#ifndef IOLOG
# ifdef NO_IO_LOGGING
#  define IOLOG(...)
# else
#  define IOLOG(...) vaLog( __FILE__,__LINE__,__FUNCTION__, __VA_ARGS__)
# endif
#endif
#ifndef IO_RANGE_CHECK
# ifdef NO_IO_LOGGING
#  define IO_RANGE_CHECK(...)
# else
#  define IO_RANGE_CHECK(...) vaLog( __FILE__,__LINE__,__FUNCTION__, "** ERROR: range check ** " __VA_ARGS__)
# endif
#endif
#ifndef IOLOG_DATA
# ifdef NO_IO_LOGGING
#  define IOLOG_DATA(...)
# else
#  define IOLOG_DATA(...) vaLogData( __FILE__,__LINE__,__FUNCTION__, __VA_ARGS__)
# endif
#endif

/***************************************************************************
* vaLog
* __________________________________________________________________________
*
* DESCRIPTION:
*   Variable arguments log - legacy logging which is mapped to PMC_LOG
*
* INPUTS:
*   file_name - name of the file being logged from
*   line_num - line number where logging
*   function_name - name of the function logging
*   fmt - printf style format
*   ... - a variable number of arguments can be passed as arguments.
*         These arguments are passed to printf()/etc. via PMC_LOG()/
*         PMC_PRINT();
* 
* OUTPUTS:
*   None
* 
* NOTES:
****************************************************************************/
void vaLog( const char *file_name, int line_num, const char *function_name, const char *fmt, ... ) ATTRIBUTE_COLD ATTRIBUTE_PRINTF(4,5); /* printf() allowed in porting function */

#ifdef USE_FW_LOG
/***************************************************************************
* fwLog
* __________________________________________________________________________
*
* DESCRIPTION:
*   FW arguments log
*
* INPUTS:
*   fmt - printf style format
*   ... - a variable number of arguments can be passed as arguments.
*         These arguments are passed to printf()/etc. via PMC_LOG()/
*         PMC_PRINT();
* 
* OUTPUTS:
*   None
* 
* NOTES:
****************************************************************************/
void fwLog( const char *fmt, ... ) ATTRIBUTE_PRINTF(1,2);

#define FW_LOG(fmt,...) fwLog(fmt,##__VA_ARGS__)
#else
#define FW_LOG(fmt,...) PMC_PRINT(fmt,##__VA_ARGS__)
#endif

/* end legacy code */

/* Enable strings for logging - to turn off the string portion of the log messages
   comment out the following line. Note this is more difficult for development
   so it is only recommended for when development and debug is complete. */
#define PMC_LOG_STRINGS

/*
** Enumerated Types
*/
/***************************************************************************
* ENUM: pmc_log_sev_enum
* __________________________________________________________________________
*
* DESCRIPTION:
*   This enumeration specifies event severity levels.  The pmc_log_sev_disable
*   level allows logging to be disabled.
*
* ELEMENTS:
*   PMC_LOG_SEV_DISABLE - Disables logging.
*   PMC_LOG_SEV_HIGHEST - Only filters in events of highest severity
*   PMC_LOG_SEV_FATAL   - Fatal events
*   PMC_LOG_SEV_HIGH    - Filters in events of high severity and up
*   PMC_LOG_SEV_MEDIUM  - Filters in events of medium severity and up
*   PMC_LOG_SEV_LOW     - Filters in events of low severity and up
*   PMC_LOG_SEV_LOWEST  - Filters in events of lowest severity and up 
*                       
*   PMC_LOG_SEV_NUM      - Maximum number of severity levels <= 2^4 .
*                          Do NOT hard-code this number.
*
* NOTE:
*   To check that the maximum number of severity levels has not been exceeded,
*   it should be verified that pmc_log_sev_num <= (PMC_LOG_SEV_MASK + 1).
****************************************************************************/
typedef enum
{
    PMC_LOG_SEV_DISABLE=0,
    PMC_LOG_SEV_HIGHEST,
    PMC_LOG_SEV_FATAL,
    PMC_LOG_SEV_HIGH,
    PMC_LOG_SEV_MEDIUM,
    PMC_LOG_SEV_LOW,
    PMC_LOG_SEV_LOWEST,

    PMC_LOG_SEV_NUM

} pmc_log_sev_t;

/***************************************************************************
* STRUCTURE: pmc_log_app_entry_struct
* __________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining an application log entry.
*
* ELEMENTS:
*   log_word0   - Log word 0 holds the severity
*   seq_num     - Lower 32 bit of sequence number for corresponding entry
*                 in OSF log.
*   log_code    - 32-bit log code, shall be in the format:
*                 [16-bit module ID | 16-bit user defined field]
*   log_word2   - Generic 32-bit log word
*   log_word3   - Generic 32-bit log word
*   log_word4   - Generic 32-bit log word
*
* NOTE:
* 
****************************************************************************/
typedef struct pmc_log_app_entry_struct
{
    
    UINT32  ts_s;
    UINT32  ts_u;
    UINT32  log_word0;
    UINT32  log_code;
    UINT32  log_word2;
    UINT32  log_word3; 
    UINT32  log_word4;
} pmc_log_app_entry_struct;

/*
** Constants
*/
/* Maximum number of registered modules to log */
#define PMC_LOG_MAX_NUM_REGISTERED_MODULES  128
/* Number of error strings */
#define PMC_LOG_STRING_SIZEOF 128
/* Default log size (in bytes) */
#define PMC_LOG_DEFAULT_SIZE 1024

#define PMC_ENTRY_LOG_SEV   PMC_LOG_SEV_LOWEST
#define PMC_RETURN_LOG_SEV  PMC_LOG_SEV_LOWEST


/*
** Macro Definitions
*/
#ifndef PMC_DOX_FOR_SWIG
/***************************************************************************
* MACRO: PMC_LOG_TRACE
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used for tracing the location in code as well as
*   outputting program output. It uses PMC_LOG() to add a LOW level severity
*   output so tracing can be disabled as well as the line and file
*   information based on the PMC_LOG configuration. This macro should
*   be used rather than any direct calls to PMC_PRINT() since it can be
*   filtered/disabled.\n
* 
*   The log_code PMC_SHARED_LOG_TRACE is used by this macro but it
*   can be overloaded within a module by doing a redefinition:\n
*       # ifdef PMC_SHARED_LOG_TRACE      \n
*       # undef PMC_SHARED_LOG_TRACE      \n
*       # define PMC_SHARED_LOG_TRACE   MODULE_LOG_TRACE_CODE  \n
*       # endif                           \n
* *
* INPUTS:
*   ...         - a variable number of arguments can be passed as arguments.
*                 These arguments are passed to printf()/etc. via PMC_LOG()/
*                 PMC_PRINT();
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#ifdef LOGGING_DISABLED
#define PMC_LOG_TRACE( ... ) do {\
        } while(0)
#else
#define PMC_LOG_TRACE(...) \
    PMC_LOG(PMC_LOG_SEV_LOW, PMC_SHARED_LOG_TRACE, 0, 0, __VA_ARGS__ )
#endif

/***************************************************************************
* MACRO: PMC_LOG_NOTRACE
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used outputting program output.
*   It uses PMC_LOG_NOINFO() to add a LOW level severity
*   output so tracing can be disabled as well as the line and file
*   information based on the PMC_LOG configuration. This macro should
*   be used rather than any direct calls to PMC_PRINT() since it can be
*   filtered/disabled.\n
* 
* *
* INPUTS:
*   ...         - a variable number of arguments can be passed as arguments.
*                 These arguments are passed to printf()/etc. via PMC_LOG()/
*                 PMC_PRINT();
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#ifdef LOGGING_DISABLED
#define PMC_LOG_NOTRACE( ... ) do {\
        } while(0)
#else
#define PMC_LOG_NOTRACE(...) PMC_LOG_NOINFO(PMC_LOG_SEV_LOW, PMC_SHARED_LOG_TRACE, 0, 0, __VA_ARGS__ )
#endif

/***************************************************************************
* MACRO: PMC_LOG_TRACE_NO_INDENT
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used for tracing the location in code as well as
*   outputting program output. It uses PMC_LOG() to add a LOW level severity
*   output so tracing can be disabled as well as the line and file
*   information based on the PMC_LOG configuration. This macro should
*   be used rather than any direct calls to PMC_PRINT() since it can be
*   filtered/disabled.\n
* 
*   The log_code PMC_SHARED_LOG_TRACE is used by this macro but it
*   can be overloaded within a module by doing a redefinition:\n
*       # ifdef PMC_SHARED_LOG_TRACE      \n
*       # undef PMC_SHARED_LOG_TRACE      \n
*       # define PMC_SHARED_LOG_TRACE   MODULE_LOG_TRACE_CODE  \n
*       # endif                           \n
* *
* INPUTS:
*   ...         - a variable number of arguments can be passed as arguments.
*                 These arguments are passed to printf()/etc. via PMC_LOG()/
*                 PMC_PRINT();
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#ifdef LOGGING_DISABLED
#define PMC_LOG_TRACE_NO_INDENT( ... ) do {\
        } while(0)
#else
#define PMC_LOG_TRACE_NO_INDENT(...) PMC_LOG_NO_INDENT(PMC_LOG_SEV_LOW, PMC_SHARED_LOG_TRACE, 0, 0, __VA_ARGS__ )
#endif

/***************************************************************************
* MACRO: PMC_LOG
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is defined in a number of different ways depending on whether
*   an environment with stdout/stderr capability, embedded environment with
*   only a log buffer or a PMC verification environment. This macro takes in
*   the parameters described in inputs and then performs operations specific
*   to the environment.
*
* INPUTS:
*   sev         - Event severity (5 levels from highest(0) to lowest(4)) 
*   log_code    - 32-bit log code.  The format is:
*                 [ Module ID (16-bit) | Generic (16-bit) ]
*   word2       - 32-bit log word 2.  Generic.
*   word3       - 32-bit log word 3.  Generic.
*   ...         - if PMC_SW_SIMULATION is defined, a variable number of arguments can
*                 be passed as arguments. These arguments are passed to printf() 
*                 to be printed out to stdout.
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/

#if defined(PMC_SW_SIMULATION) | defined (PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
/* 
 * VA_NUM_ARGS returns the number of arguments passed to it. 
 * The maximum number of arguments supported is 10.
 * If no arguments is passed to it, VA_NUM_ARGS will return 1, i.e. x = VA_NUM_ARGS() will 
 * cause x to be set to 1.
 */
#define PMC_LOG_SPACES_PER_INDENT 1
/* the maximum filename is approximatly 52 charcters */
/* added the get indent method, if the get_indent macro isn't defined
 * pmc_log_indent_get() returns 0 */
#define MAX_FILENAME_LENGTH 56

#define PMC_LOG(sev, log_code, word2, word3, ...) \
    do {                                                                \
    if ( pmc_log_writable(sev,log_code) == TRUE) {                      \
        if(digi120_pmc_log_data.print_to_stdout) \
        {                                       \
            if (NULL == digi120_pmc_log_data.active_handle) \
            { \
                /* PMC_PRINT_NO_INDENT allowed in porting function */ \
                PMC_PRINT("%s:%d: In function '%s': '%s", __FILE__, __LINE__, __FUNCTION__, pmc_log_string_get(log_code)); \
            } \
            else                                \
            {  \
                PMC_PRINT("DIGI %0llx: %s:%d: In function '%s': '%s", (UINT64)((PMC_UINTPTR)digi120_pmc_log_data.active_handle), __FILE__, __LINE__, __FUNCTION__, pmc_log_string_get(log_code)); \
            } \
            PMC_PRINT_NO_INDENT(" " __VA_ARGS__ );    \
        } \
        pmc_log_app_event3(sev, 4, log_code, word2, word3, __LINE__); /* 4 = num log values */\
    }} while(0)      
       
#else
#define PMC_LOG(sev, log_code, word2, word3, ...) \
    do {\
    if ( pmc_log_writable(sev,log_code) == TRUE) {\
        pmc_log_app_event3(sev, 4 /* num log values */, log_code, word2, word3, __LINE__); \
    }} while(0)
#endif

/***************************************************************************
* MACRO: PMC_LOG_NOINFO
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is defined in a number of different ways depending on whether
*   an environment with stdout/stderr capability, embedded environment with
*   only a log buffer or a PMC verification environment. This macro takes in
*   the parameters described in inputs and then performs operations specific
*   to the environment. Tracing information such as file name, line number, 
*   and function name are not logged.
*
* INPUTS:
*   sev         - Event severity (5 levels from highest(0) to lowest(4)) 
*   log_code    - 32-bit log code.  The format is:
*                 [ Module ID (16-bit) | Generic (16-bit) ]
*   word2       - 32-bit log word 2.  Generic.
*   word3       - 32-bit log word 3.  Generic.
*   ...         - if PMC_SW_SIMULATION is defined, a variable number of arguments can
*                 be passed as arguments. These arguments are passed to printf()
*                 to be printed out to stdout.
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/

#if defined(PMC_SW_SIMULATION) | defined (PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
/* 
 * VA_NUM_ARGS returns the number of arguments passed to it. 
 * The maximum number of arguments supported is 10.
 * If no arguments is passed to it, VA_NUM_ARGS will return 1, i.e. x = VA_NUM_ARGS() will 
 * cause x to be set to 1.
 */
#define PMC_LOG_SPACES_PER_INDENT 1
/* the maximum filename is approximatly 52 charcters */
/* added the get indent method, if the get_indent macro isn't defined
 * pmc_log_indent_get() returns 0 */
#define MAX_FILENAME_LENGTH 56

#define PMC_LOG_NOINFO(sev, log_code, word2, word3, ...) \
    do {                                                                \
    if ( pmc_log_writable(sev,log_code) == TRUE) {                      \
        PMC_PRINT_NO_INDENT(__VA_ARGS__ ); /* PMC_PRINT_NO_INDENT allowed in porting function */  \
    }} while(0)      
       
#else
#define PMC_LOG_NOINFO(sev, log_code, word2, word3, ...)
#endif


/***************************************************************************
* MACRO: PMC_LOG_NO_INDENT
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is defined in a number of different ways depending on whether
*   an environment with stdout/stderr capability, embedded environment with
*   only a log buffer or a PMC verification environment. This macro takes in
*   the parameters described in inputs and then performs operations specific
*   to the environment.  Tracing information such as file name, line number, 
*   and function name are not logged. Text is not indented.
*
* INPUTS:
*   sev         - Event severity (5 levels from highest(0) to lowest(4)) 
*   log_code    - 32-bit log code.  The format is:
*                 [ Module ID (16-bit) | Generic (16-bit) ]
*   word2       - 32-bit log word 2.  Generic.
*   word3       - 32-bit log word 3.  Generic.
*   ...         - if PMC_SW_SIMULATION is defined, a variable number of arguments can
*                 be passed as arguments. These arguments are passed to printf()
*                 to be printed out to stdout.
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/

#if defined(PMC_SW_SIMULATION) | defined (PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
/* 
 * VA_NUM_ARGS returns the number of arguments passed to it. 
 * The maximum number of arguments supported is 10.
 * If no arguments is passed to it, VA_NUM_ARGS will return 1, i.e. x = VA_NUM_ARGS() will 
 * cause x to be set to 1.
 */
#define PMC_LOG_SPACES_PER_INDENT 1
/* the maximum filename is approximatly 52 charcters */
/* added the get indent method, if the get_indent macro isn't defined
 * pmc_log_indent_get() returns 0 */
#define MAX_FILENAME_LENGTH 56
#define PMC_LOG_NO_INDENT(sev, log_code, word2, word3, ...) \
    do {\
    if ( pmc_log_writable(sev,log_code) == TRUE) {\
        PMC_PRINT_NO_INDENT("%s:%d: In function '%s': '%s", __FILE__, __LINE__, __FUNCTION__, pmc_log_string_get(log_code)); /* PMC_PRINT_NO_INDENT allowed in porting function */ \
        PMC_PRINT_NO_INDENT(" " __VA_ARGS__ ); /* PMC_PRINT_NO_INDENT allowed in porting function */\
        PMC_PRINT_NO_INDENT(" "); /* PMC_PRINT_NO_INDENT allowed in porting function */\
        pmc_log_app_event3(sev, 4 /* num log values */, log_code, word2, word3, __LINE__); \
    }} while(0)
#else
#define PMC_LOG_NO_INDENT(sev, log_code, word2, word3, ...) \
    do {\
    if ( pmc_log_writable(sev,log_code) == TRUE) {\
        pmc_log_app_event3(sev, 4 /* num log values */, log_code, word2, word3, __LINE__); \
    }} while(0)
#endif
/***************************************************************************
* MACRO: PMC_DEBUG
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used only during development and is disabled in the embedded
*   systems. Log operations should be converted to permanent PMC_LOG or
*   removed at Beta since they will have no field use. These log codes are
*   also hard-coded to the highest severity level so they always output.
*
* INPUTS:
*   see pmc_log_app_event3()
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#if defined(PMC_SW_SIMULATION) | defined(PMC_SPECMAN) | defined (PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
#define PMC_DEBUG(log_code, word2, word3) \
            do { \
                PMC_PRINT("PMC_DEBUG: "); PMC_LOG(PMC_LOG_SEV_HIGHEST, log_code, word2, word3); /* PMC_PRINT allowed in porting function */\
            } \
            while(0)
#else
#define PMC_DEBUG(log_code, word2, word3)
#endif

/***************************************************************************
* MACRO: PMC_RETURN
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to log return point of the method. These log codes
*   are also hard-coded to the highest severity level so they always output.
*
* INPUTS:
*   There may be return value to log if return type of the function is not void.
*
* NOTES:
*   This macro should be called with the parameter if return type of the function
*   is not void (PMC_RETURN(rc)), and without the parameter if return type of
*   the function is void (PMC_RETURN()).
****************************************************************************/
#if defined(PMC_SW_SIMULATION) | defined (PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
#ifdef LOGGING_DISABLED
#define PMC_RETURN(...) do { return __VA_ARGS__; } while(0)
#else
#define PMC_RETURN(...) do {                                                \
    pmc_log_indent_dec();                                               \
    if (PMC_RETURN_LOG_SEV <= pmc_log_verbosity_level_get())  \
    {                                                         \
       if (NULL == digi120_pmc_log_data.active_handle) \
       { \
           PMC_PRINT("PMC_RETURN: %s():%d, ", __FUNCTION__, __LINE__); \
       } \
       else \
       { \
           PMC_PRINT("DIGI %0llx: PMC_RETURN: %s():%d, ", (UINT64)((PMC_UINTPTR)digi120_pmc_log_data.active_handle),__FUNCTION__, __LINE__); \
       } \
       PMC_PRINT_NO_INDENT("ret_val 0x%llx\n",  (UINT64)(PMC_UINTPTR)__VA_ARGS__+0);\
    }                                                                       \
    return __VA_ARGS__;                                                     \
} while(0)
#endif
#endif




/***************************************************************************
* MACRO: PMC_ENTRY
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro is used to log entry to function. 
*
* INPUTS:
*   
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#if defined(PMC_SW_SIMULATION) | defined(PMC_COMEXPRESS) | defined (PMC_CUSTOMER_OS)
#ifdef LOGGING_DISABLED
#define PMC_ENTRY()
#else
#define PMC_ENTRY() do {\
    if (PMC_ENTRY_LOG_SEV <= pmc_log_verbosity_level_get()) \
    {    \
       if (NULL == digi120_pmc_log_data.active_handle) \
       { \
           PMC_PRINT("PMC_ENTRY: %s()\n", __FUNCTION__); \
       } \
       else \
       { \
           PMC_PRINT("DIGI %0llx: PMC_ENTRY: %s()\n", (UINT64)((PMC_UINTPTR)digi120_pmc_log_data.active_handle),__FUNCTION__); \
       } \
    }                                                                       \
    pmc_log_indent_inc();                                               \
} while(0)
#endif
#endif


/***************************************************************************
* MACRO: PMC_ATOMIC_ENTRY
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro locks the global applib mutex and call PMC_ENTRY
*
* INPUTS:
*   
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#ifndef PMC_TESTS
#ifdef PMC_NO_ATOMIC_FUNCTION_STATS
#define PMC_ATOMIC_ENTRY(pmc_digi_handle) \
    do {\
      PMC_ASSERT(pmc_digi_handle!= NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0); \
      pmc_atomic_internal_start((pmc_sys_handle_t *)pmc_digi_handle);   \
      pmc_set_log_handle((pmc_handle_t *)pmc_digi_handle);              \
      PMC_ENTRY();                              \
} while(0)
#else
#define PMC_ATOMIC_ENTRY(pmc_digi_handle) \
    PMC_SYS_STAT_DECLARE_FOR_THIS_FUNCTION(this_function_stats); \
    do {\
        PMC_ASSERT(pmc_digi_handle!= NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0); \
        pmc_atomic_internal_start((pmc_sys_handle_t *)pmc_digi_handle); \
        pmc_set_log_handle((pmc_handle_t *)pmc_digi_handle);            \
        pmc_sys_stat_mark_start(this_function_stats);                   \
        PMC_ENTRY();                      \
} while(0)
#endif
#else 
#define PMC_ATOMIC_ENTRY_VOID(pmc_digi_handle) \
    PMC_SYS_STAT_DECLARE_FOR_THIS_FUNCTION(this_function_stats); \
    do {\
        PMC_ASSERT(pmc_digi_handle!= NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0); \
        pmc_atomic_internal_start((pmc_sys_handle_t *)pmc_digi_handle); \
        pmc_set_log_handle((pmc_handle_t *)pmc_digi_handle);            \
        pmc_sys_stat_mark_start(this_function_stats);                   \
        PMC_ENTRY();                      \
} while(0)
#define PMC_ATOMIC_ENTRY(pmc_digi_handle) \
    PMC_SYS_STAT_DECLARE_FOR_THIS_FUNCTION(this_function_stats); \
    do {\
        PMC_ASSERT(pmc_digi_handle!= NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);    \
        if (g_crash_event.is_scheduled == TRUE && g_crash_event.is_setup == FALSE) \
          {                                                             \
            UINT32 setjmp_ret = setjmp(g_crash_event.buf);              \
            g_crash_event.is_setup = TRUE;                              \
            if ( setjmp_ret ==  0)                                      \
            {                                                         \
                pmc_atomic_internal_start((pmc_sys_handle_t *)pmc_digi_handle); \
                pmc_set_log_handle((pmc_handle_t *)pmc_digi_handle);    \
            } else {                                                    \
                pmc_sys_stat_mark_start(this_function_stats);             \
                PMC_ENTRY();                                            \
                PMC_LOG(PMC_LOG_SEV_HIGH, PMC_SHARED_LOG_TRACE,0,0,"MID API CRASH (%d): %s\n",setjmp_ret,__FUNCTION__); \
                ((pmc_handle_t *)pmc_digi_handle)->mutex_ctxt.recursive_level = 1; \
                g_crash_event.is_setup = FALSE;                         \
                g_crash_event.is_scheduled = FALSE;                     \
                PMC_ATOMIC_RETURN(pmc_digi_handle,PMC_SHARED_ERR_CRASH_EMULATION); \
            }                                                           \
          } else {                                                      \
          pmc_atomic_internal_start((pmc_sys_handle_t *)pmc_digi_handle); \
          pmc_set_log_handle((pmc_handle_t *)pmc_digi_handle);          \
        }                                                               \
        pmc_sys_stat_mark_start(this_function_stats);                   \
        PMC_ENTRY();                                        \
    } while(0)
       
#endif 


/***************************************************************************
* MACRO: PMC_ATOMIC_END
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro releases the global applib mutex and syncs the FW QUEUE if
*   necessary
*
* INPUTS:
*   
*
* NOTES:
*
****************************************************************************/
#define PMC_ATOMIC_END(pmc_digi_handle,...) do {    \
        if (!pmc_atomic_is_nested((pmc_sys_handle_t *)pmc_digi_handle)) { \
            l5sys_abort_if_queue_errors(pmc_digi_handle); \
        }\
        pmc_release_log_handle((pmc_handle_t *)pmc_digi_handle);\
        pmc_atomic_internal_end((pmc_sys_handle_t *)pmc_digi_handle);    \
} while(0)

/***************************************************************************
* MACRO: PMC_ATOMIC_RETURN
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro calls PMC_RETURN and releases the global applib mutex
*   This macro should be used if you are returning a status code
*   the status code is changed to PMC_SHARED_ERR_POLL_FAILED if any poll failed.
*
* INPUTS:
*   
*
* NOTES:
*
****************************************************************************/
#ifndef PMC_TESTS
#ifdef PMC_NO_ATOMIC_FUNCTION_STATS 
#define PMC_ATOMIC_RETURN(pmc_digi_handle,...) do {    \
    if (!pmc_atomic_is_nested((pmc_sys_handle_t *)pmc_digi_handle)) {   \
      l5sys_abort_if_queue_errors(pmc_digi_handle);                     \
    }                                                                   \
    pmc_release_log_handle((pmc_handle_t *)pmc_digi_handle);            \
    pmc_atomic_internal_end((pmc_sys_handle_t *)pmc_digi_handle);       \
    PMC_RETURN(__VA_ARGS__);                                            \
} while(0)
#else
#define PMC_ATOMIC_RETURN(pmc_digi_handle,...) do {    \
    if (!pmc_atomic_is_nested((pmc_sys_handle_t *)pmc_digi_handle)) {   \
      l5sys_abort_if_queue_errors(pmc_digi_handle);                     \
    }                                                                   \
    pmc_release_log_handle((pmc_handle_t *)pmc_digi_handle);            \
    pmc_sys_stat_mark_end(this_function_stats);                         \
    pmc_atomic_internal_end((pmc_sys_handle_t *)pmc_digi_handle);       \
    PMC_RETURN(__VA_ARGS__);                                            \
} while(0)
#endif
#else 
#define PMC_ATOMIC_RETURN(pmc_digi_handle,...) do {    \
    if (!pmc_atomic_is_nested((pmc_sys_handle_t *)pmc_digi_handle)) {   \
      l5sys_abort_if_queue_errors(pmc_digi_handle);                     \
    }                                                                   \
    pmc_release_log_handle((pmc_handle_t *)pmc_digi_handle);            \
    pmc_sys_stat_mark_end(this_function_stats);                         \
    pmc_atomic_internal_end((pmc_sys_handle_t *)pmc_digi_handle);       \
    if (g_crash_event.is_scheduled == TRUE && ((pmc_handle_t *)pmc_digi_handle)->mutex_ctxt.recursive_level == 0) \
    {                                                                 \
        g_crash_event.is_setup = FALSE;                                 \
    }                                                                   \
    PMC_RETURN(__VA_ARGS__);                           \
} while(0)    
#endif


/***************************************************************************
* MACRO: PMC_ATOMIC_YIELD
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro release and and lock the global applib mutex 
*
* INPUTS:
*   
*
* NOTES:
*   This function performs much more slowly when outputting to the screen.
****************************************************************************/
#define PMC_ATOMIC_YIELD(pmc_digi_handle, ...)        \
    do {                                                                \
      if (TRUE == pmc_atomic_continue((pmc_handle_t *)pmc_digi_handle))   \
      { \
        if (((pmc_handle_t *)pmc_digi_handle)->mutex_ctxt.recursive_level > 0) { \
            if (  ((UINT64)(PMC_UINTPTR)__VA_ARGS__+0) != 0    )  {     \
                if (!pmc_atomic_is_nested((pmc_sys_handle_t *)pmc_digi_handle)) { \
                    l5sys_abort_if_queue_errors(pmc_digi_handle);       \
                }                                                       \
            }                                                           \
            if (pmc_atomic_yield_is_allowed((pmc_sys_handle_t *)pmc_digi_handle)  == TRUE) \
            {                                                           \
                pmc_atomic_internal_force((pmc_sys_handle_t *)pmc_digi_handle,FALSE); \
            }                                                           \
            if (  ((UINT64)(PMC_UINTPTR)__VA_ARGS__+0) != 0    )  {     \
                PMC_OS_USLEEP(__VA_ARGS__);                             \
            }                                                           \
            if (pmc_atomic_yield_is_allowed((pmc_sys_handle_t *)pmc_digi_handle)  == TRUE) \
            {                                                           \
                pmc_atomic_internal_force((pmc_sys_handle_t *)pmc_digi_handle,TRUE); \
            }                                                           \
        }                                                               \
      }                                                                 \
    } while(0)

/***************************************************************************
* temp structure
**************************************************************************/
#ifndef IO_RANGE_CHECK
    #ifdef NO_IO_LOGGING
        #define IO_RANGE_CHECK(...)
    #else
        #define IO_RANGE_CHECK(...) do {                                    \
            vaLog( __FILE__,__LINE__,__FUNCTION__, "** ERROR: range check ** " __VA_ARGS__); \
            PMC_ASSERT(FALSE,PMC_SHARED_LOG_CODE_LEGACY,0,0); \
        }while(0);
    #endif
#endif

#endif /*PMC_DOX_FOR_SWIG*/
/*
** Structures and Unions
*/


typedef pmc_log_app_entry_struct pmc_buf_item_t; /*!< The datatype for a log entry is a generic buffer type */

/***************************************************************************
* STRUCTURE: pmc_circbuf_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   This structure holds the pointer to the error code id's and lists that
*   are registered by modules with the log module.
*
* ELEMENTS:
*   not_empty - track if the buffer is not empty/empty
*   rd_idx - read index into the circular buffer
*   wr_idx - write index into the circular buffer
*   size - size of the circular buffer
*   buf - pointer to the start of the buffer
*****************************************************************************/
typedef struct pmc_circbuf_t 
{
    UINT8 not_empty;
    UINT32 rd_idx;
    UINT32 wr_idx;
    UINT32 size;
    pmc_buf_item_t *buf;
} pmc_circbuf_t;

/***************************************************************************
* STRUCTURE: pmc_log_data_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   This structure holds the pointer to the error code id's and lists that
*   are registered by modules with the log module.
*
* ELEMENTS:
*   string_list_ptr - 
*   base_code_list  - 
*   last_code_list  - 
*   verbosity_list  - 
*   num_registered  - 
*   verbosity       - 
*****************************************************************************/
typedef struct pmc_log_data_t 
{
    const char *string_list_ptr[PMC_LOG_MAX_NUM_REGISTERED_MODULES]; /*!< Array of pointers to strings */
    UINT32 base_code_list[PMC_LOG_MAX_NUM_REGISTERED_MODULES]; /*!< Lowest number in the list of codes, used for checking if error is in the range */
    UINT32 last_code_list[PMC_LOG_MAX_NUM_REGISTERED_MODULES]; /*!< Highest number in the list of codes, used for checking if error is in the range */
    pmc_log_sev_t verbosity_list[PMC_LOG_MAX_NUM_REGISTERED_MODULES]; /*!< severity per module verbosity. */
    UINT32 num_registered;  /*!< Number of registered codes */
    pmc_log_sev_t verbosity; /*!<  verbosity level currently set, global verbosity*/
    void * active_handle; /*!< currently active handle writing to the log */
    pmc_circbuf_t *default_buf; /*!< circular buffer for startup */
    BOOL8 print_to_stdout;      /*!< set to true if it is allowed to dump the log output. */
} pmc_log_data_t;
  
/*
** Global variables
*/

/* default buffer size */
#ifdef PMC_LOG_STRINGS
extern const char PMC_SHARED_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char PMC_SHARED_LOG_ERR_STRINGS[1][1];
#endif /* PMC_LOG_STRINGS */

extern pmc_log_data_t digi120_pmc_log_data;


/*
** Function Prototypes
*/

/* pmc atomic mutex pointer functions */
extern PUBLIC void pmc_atomic_start(void *mutex_ptr);
extern PUBLIC void pmc_atomic_end(void *mutex_ptr);

/* atomic entry and exit helper functinos that set the active handle */
PUBLIC void pmc_set_log_handle(pmc_handle_t * pmc_handle);
PUBLIC void pmc_release_log_handle(pmc_handle_t * pmc_handle);

/* circular buffer functions */
PUBLIC pmc_circbuf_t * pmc_circbuf_init(void* digi_handle);
PUBLIC void pmc_circbuf_destroy(void * digi_handle, pmc_circbuf_t * buf);
void pmc_circbuf_wr_idx_inc(pmc_circbuf_t * buf);
void pmc_circbuf_rd_idx_inc(pmc_circbuf_t * buf);

PUBLIC void  pmc_default_buf_size_set(UINT32 size);
PUBLIC UINT32 pmc_default_buf_size_get();

PUBLIC void pmc_circbuf_rd(pmc_circbuf_t * buf, pmc_buf_item_t * retval);
PUBLIC void pmc_circbuf_wr(pmc_circbuf_t * buf, pmc_buf_item_t itm);

UINT32 pmc_circbuf_inc_wrap(UINT32 count, UINT32 wrap);

/* logging functions */
PUBLIC void pmc_log_app_event3(pmc_log_sev_t event_sev,
                               UINT8         num_words,
                               UINT32        log_code,
                               UINT32        log_word2,
                               UINT32        log_word3,
                               UINT32        log_word4);
PUBLIC UINT32 pmc_log_app_print(UINT32 num_lines);
PUBLIC pmc_circbuf_t * pmc_log_app_get();
PUBLIC BOOL pmc_log_writable( pmc_log_sev_t event_sev,
                              UINT32        log_code ); 
PUBLIC void pmc_log_verbosity_level_set(pmc_log_sev_t level);
PUBLIC void pmc_log_module_verbosity_level_set(pmc_log_sev_t level, UINT32 log_code);
PUBLIC void pmc_log_all_module_verbosity_level_set(pmc_log_sev_t level);
PUBLIC pmc_log_sev_t pmc_log_verbosity_level_get();
PUBLIC pmc_log_sev_t pmc_log_module_verbosity_level_get(UINT32 log_code);
PUBLIC void pmc_log_init(UINT32 size);

PUBLIC const char *pmc_log_string_get(PMC_ERROR log_code);
PUBLIC void pmc_log_block_strings_register(const char *strings_ptr, UINT32 base_code, UINT32 last_code);

/* function decleration for indents */
PUBLIC UINT32 pmc_log_indent_get(void * stack_ptr);
PUBLIC void pmc_log_indent_inc(void);
PUBLIC void pmc_log_indent_dec(void);
PUBLIC BOOL8 pmc_is_logger_initialized(void);
PUBLIC void pmc_logger_mutex_set(BOOL8 lock);


#ifdef __cplusplus
}
#endif

#endif /* _PMC_LOG_H */

/*
** end of file
*/
