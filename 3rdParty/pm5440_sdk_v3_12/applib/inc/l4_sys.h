/*******************************************************************************
*   COPYRIGHT (C) 2011-2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                  
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
******************************************************************************/

#ifndef L4_SYS_H
#define L4_SYS_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * the level 4 system layer tracks statistics and adds logging before calling the system access functions.
 */
#include "pmc_types.h"
#include "pmc_err.h"
#include "l5_sys.h"
#include "pmc_log.h"
#include "shadow_registers.h"
#ifdef PMC_TESTS
#include <setjmp.h>
#endif

/* memory types */
#define MEM_TYPE_CONFIG 1
#define MEM_TYPE_STATUS 2
#define MEM_TYPE_ACTION_ON_WRITE ?
#define MEM_TYPE_CONFIG_EDGE ?
#define MEM_TYPE_MAYBE_SOME_INTERRUPT_TYPES_GET_A_SPECIAL_MEMORY_TYPE ?
#define MEM_TYPE_MAYBE_A_DIFFERENT_MEMORY_ID_FOR_EACH_INDIRECT_SPACE ?

/******************************************************************************
* STRUCTURE: pmc_os_stats_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track system accesses.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    UINT32 num_reg_read;                          /*!< number of register read operations */
    UINT32 num_field_read;                        /*!< number of field reads */
    UINT32 num_reg_write;                         /*!< number of register write operations */
    UINT32 num_reg_read_modify_write;             /*!< number of register read/modify/write operations */
    UINT32 num_field_read_modify_write;           /*!< number of field updates */
    UINT32 num_reg_read_modify_write_duplicate;   /*!< number of register read/modify/writes that were to the same address as the previous read/modify/write operation */
    UINT32 num_reg_read_modify_write_no_effect;   /*!< number of register read/modify/write operations that did not change the field */
    UINT32 num_poll_bit;                          /*!< number of calls to poll_bit */
    UINT32 num_reg_burst_read;                    /*!< number of calls to burst_read */
    UINT32 num_reg_burst_write;                   /*!< number of calls to burst_write  */
    UINT32 num_reg_burst_read_words;              /*!< number of words read using burst_read (sum of len) */
    UINT32 num_reg_burst_write_words;             /*!< number of words written using burst_write (sum of len) */
    UINT32 num_config_cache_read;                 /*!< number of register read operations to config registers */
    UINT32 num_config_cache_write;                /*!< number of register write operations to config registers */
    UINT32 num_config_cache_read_modify_write;    /*!< number of read/modify/write operations to config registers */
    UINT32 num_config_cache_burst_read;           /*!< number of burst_read operations to config registers */
    UINT32 num_config_cache_burst_write;          /*!< number of burst_write operations to config registers */
    UINT32 num_config_cache_burst_read_words;     /*!< number of words read with burst_read to config registers */
    UINT32 num_config_cache_burst_write_words;    /*!< number of words written with burst_write to config registers */
    UINT32 num_sleep_calls;                       /*!< number of calls to PMC_OS_SLEEP */
    UINT32 num_msleep_calls;                      /*!< number of calls to PMC_OS_MSLEEP */
    UINT32 num_usleep_calls;                      /*!< number of calls to PMC_OS_USLEEP */
    UINT32 num_nanosleep_calls;                   /*!< number of calls to PMC_OS_NANOSLEEP */
    UINT32 num_seconds_slept;                     /*!< number of seconds slept in PMC_OS_SLEEP */
    UINT64 num_milliseconds_slept;                /*!< number of milliseconds slept in PMC_OS_MSLEEP */
    UINT64 num_microseconds_slept;                /*!< number of microseconds slept in PMC_OS_USLEEP */
    UINT64 num_nanoseconds_slept;                 /*!< number of nanoseconds slept in PMC_OS_NANOSLEEP */
    UINT32 num_queued_write;                      /*!< number of registers written with queue_write */
    UINT32 num_queued_rmw_short;                  /*!< number of read/modify/write operations done with queue_rmw_short (less than 12 bits) */
    UINT32 num_queued_rmw_long;                   /*!< number of read/modify/write operations done with queue_rmw_long (more than 12 bits) */
    UINT32 num_queued_read;                       /*!< number of read operations done with queue_read */
    UINT32 num_queued_usleep;                     /*!< number of PMC_Q_USLEEP operations performed with queue_usleep */
    UINT64 num_microseconds_queued;               /*!< number of microseconds slept using PMC_Q_USLEEP */
    UINT32 num_queued_poll;                       /*!< number of register polls performed with queue_poll */
} pmc_os_stats_t;

extern pmc_os_stats_t digi120_pmc_os_stats; /*!< global variable that tracks system accesses */

#ifdef PMC_TESTS
extern BOOL8 pmc_sys_crash_event_required();
/*******************************************************************************
* STRUCTURE: pmc_sys_crash_event_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to schedule a crash event in Applib
*
* ELEMENTS:
*    is_setup      - Crash event has been setup
*    is__scheduled - A crash event is scheduled
*    crash_at_write - global pmc_os_stats_t stats are used to identify when a \n
*                     crash will occur 
*    buf           - Crash event will jump to the location stored in \n
*                    this buffer 
*******************************************************************************/
typedef struct pmc_sys_crash_event_t
{
    BOOL8          is_setup;
    BOOL8          is_scheduled;
    UINT32         crash_at_write;
    jmp_buf        buf;
} pmc_sys_crash_event_t;
/*! Crash event */
 extern pmc_sys_crash_event_t g_crash_event; 
#endif

#ifndef ENABLE_PMC_OS_STATS
# ifndef LOGGING_DISABLED
#  define ENABLE_PMC_OS_STATS
# endif
#endif

#ifndef PMC_TESTS
#define PMC_EMULATE_CRASH_EVENT()
#define PMC_OS_STAT_EMULATE_CRASH_EVENT()
#else 
#define PMC_EMULATE_CRASH_EVENT() longjmp(g_crash_event.buf,PMC_SHARED_ERR_CRASH_EMULATION) 
/***************************************************************************
* MACRO: PMC_EMULATE_CRASH_EVENT
* __________________________________________________________________________
*
* DESCRIPTION:
*   This macro emulates a crash event. It jump at the entry of the first 
*   applib function called in the stack. Then, this function returns with 
*   the PMC_TEST_CRASH_EVENT error code
*
****************************************************************************/
#define PMC_OS_STAT_EMULATE_CRASH_EVENT() \
    do {                                                                \
        if (pmc_sys_crash_event_required() == TRUE)                     \
        {                                                               \
            PMC_EMULATE_CRASH_EVENT();                                  \
        }                                                               \
    } while(0)

#endif

#ifndef ENABLE_PMC_OS_STATS
#define PMC_OS_STATS_USLEEP(x)
#define PMC_OS_STATS_MSLEEP(x)
#define PMC_OS_STATS_SLEEP(x)
#define PMC_OS_STATS_NANOSLEEP(x)
#define PMC_OS_STATS_REG_READ() 
#define PMC_OS_STATS_FIELD_READ()
#define PMC_OS_STATS_REG_WRITE() 
#define PMC_OS_STATS_POLL_BIT() 
#define PMC_OS_STATS_FIELD_READ_MODIFY_WRITE()
#define PMC_OS_STATS_REG_READ_MODIFY_WRITE()
#define PMC_OS_STATS_REG_READ_MODIFY_WRITE_DUPLICATE()
#define PMC_OS_STATS_REG_READ_MODIFY_WRITE_NO_EFFECT()
#define PMC_OS_STATS_BURST_READ(len)
#define PMC_OS_STATS_BURST_WRITE(len) 

#define PMC_OS_STATS_CONFIG_CACHE_READ() 
#define PMC_OS_STATS_CONFIG_CACHE_WRITE() 
#define PMC_OS_STATS_CONFIG_CACHE_READ_MODIFY_WRITE()
#define PMC_OS_STATS_CONFIG_CACHE_BURST_READ(len)
#define PMC_OS_STATS_CONFIG_CACHE_BURST_WRITE(len) 

#define PMC_OS_STATS_QUEUE_WRITE()
#define PMC_OS_STATS_QUEUE_RMW_SHORT()
#define PMC_OS_STATS_QUEUE_RMW_LONG()
#define PMC_OS_STATS_QUEUE_READ() 
#define PMC_OS_STATS_QUEUE_USLEEP(x) 
#define PMC_OS_STATS_QUEUE_POLL()

#else

#define PMC_OS_STATS_MSLEEP(x) do { \
    digi120_pmc_os_stats.num_msleep_calls++; \
    digi120_pmc_os_stats.num_milliseconds_slept += x; \
} while (0)

#define PMC_OS_STATS_USLEEP(x) do { \
    digi120_pmc_os_stats.num_usleep_calls++; \
    digi120_pmc_os_stats.num_microseconds_slept += x; \
} while (0)

#define PMC_OS_STATS_NANOSLEEP(x) do { \
    digi120_pmc_os_stats.num_nanosleep_calls++; \
    digi120_pmc_os_stats.num_nanoseconds_slept += x; \
} while (0)

#define PMC_OS_STATS_SLEEP(x) do { \
    digi120_pmc_os_stats.num_sleep_calls++; \
    digi120_pmc_os_stats.num_seconds_slept += x; \
} while (0)

#define PMC_OS_STATS_REG_READ() do { \
    digi120_pmc_os_stats.num_reg_read++; \
} while (0)

#define PMC_OS_STATS_FIELD_READ() do { \
    digi120_pmc_os_stats.num_field_read++; \
} while (0)

#define PMC_OS_STATS_POLL_BIT() do { \
    digi120_pmc_os_stats.num_poll_bit++; \
} while (0)

#define PMC_OS_STATS_REG_READ_MODIFY_WRITE() do { \
    digi120_pmc_os_stats.num_reg_read_modify_write++; \
} while (0)

#define PMC_OS_STATS_FIELD_READ_MODIFY_WRITE() do { \
    digi120_pmc_os_stats.num_field_read_modify_write++; \
} while (0)

#define PMC_OS_STATS_REG_READ_MODIFY_WRITE_DUPLICATE() do { \
    digi120_pmc_os_stats.num_reg_read_modify_write_duplicate++; \
} while (0)

#define PMC_OS_STATS_REG_READ_MODIFY_WRITE_NO_EFFECT() do { \
    digi120_pmc_os_stats.num_reg_read_modify_write_no_effect++; \
} while (0)

#define PMC_OS_STATS_REG_WRITE() do { \
    digi120_pmc_os_stats.num_reg_write++;           \
    PMC_OS_STAT_EMULATE_CRASH_EVENT();      \
} while (0)

#define PMC_OS_STATS_BURST_READ(len) do { \
    digi120_pmc_os_stats.num_reg_burst_read++; \
    digi120_pmc_os_stats.num_reg_burst_read_words += len; \
} while (0)

#define PMC_OS_STATS_BURST_WRITE(len) do { \
    digi120_pmc_os_stats.num_reg_burst_write++; \
    digi120_pmc_os_stats.num_reg_burst_write_words += len; \
} while (0)

#define PMC_OS_STATS_CONFIG_CACHE_READ() do { \
    digi120_pmc_os_stats.num_config_cache_read++; \
} while (0)

#define PMC_OS_STATS_CONFIG_CACHE_READ_MODIFY_WRITE() do { \
    digi120_pmc_os_stats.num_config_cache_read_modify_write++; \
} while (0)

#define PMC_OS_STATS_CONFIG_CACHE_WRITE() do { \
    digi120_pmc_os_stats.num_config_cache_write++; \
} while (0)

#define PMC_OS_STATS_CONFIG_CACHE_BURST_READ(len) do { \
    digi120_pmc_os_stats.num_config_cache_burst_read++; \
    digi120_pmc_os_stats.num_config_cache_burst_read_words += len; \
} while (0)

#define PMC_OS_STATS_CONFIG_CACHE_BURST_WRITE(len) do { \
    digi120_pmc_os_stats.num_config_cache_burst_write++; \
    digi120_pmc_os_stats.num_config_cache_burst_write_words += len; \
} while (0)

#define PMC_OS_STATS_QUEUE_WRITE() do { \
    digi120_pmc_os_stats.num_queued_write++; \
} while (0)

#define PMC_OS_STATS_QUEUE_RMW_SHORT() do { \
    digi120_pmc_os_stats.num_queued_rmw_short++; \
} while (0)

#define PMC_OS_STATS_QUEUE_RMW_LONG() do { \
    digi120_pmc_os_stats.num_queued_rmw_long++; \
} while (0)

#define PMC_OS_STATS_QUEUE_READ() do { \
    digi120_pmc_os_stats.num_queued_read++; \
} while (0)

#define PMC_OS_STATS_QUEUE_USLEEP(x) do { \
    digi120_pmc_os_stats.num_queued_usleep++; \
    digi120_pmc_os_stats.num_microseconds_queued += x; \
} while (0)

#define PMC_OS_STATS_QUEUE_POLL() do { \
    digi120_pmc_os_stats.num_queued_poll++; \
} while (0)
#endif

void pmc_os_stats_init();
void pmc_os_stats_snapshot( pmc_os_stats_t *snapshot );  /*  takes a snapshot. */
void pmc_os_stats_show( pmc_os_stats_t *snapshot ); /*  logs the counter differences since the snapshot. */

#ifdef NO_IO_LOGGING

/*******************************************************************************
* STRUCTURE: l4pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L4 state for configuration cache 
*
* ELEMENTS:
*   See typedef.
* 
* NOTES:
*   In future, may want to store some flags to enable/disable logging on a
*   particular region
*******************************************************************************/
typedef struct l4pmc_sys_handle_t
{
    l5pmc_sys_handle_t  l5sys_handle; /* handle to lower layer interface */
    int dummy ;        /* reserved for future storage of the stats structure */
    
} l4pmc_sys_handle_t;

#define l4sys_init( sys_handle )  \
    l5sys_init(sys_handle)

#define l4sys_halt( sys_handle )  \
    l5sys_halt(sys_handle)

void l4sys_init( pmc_sys_handle_t *sys_handle );

void l4sys_halt( pmc_sys_handle_t *sys_handle );

#ifdef ENABLE_PMC_OS_STATS
void l4sys_reg_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value );

UINT32 l4sys_reg_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr ) ATTRIBUTE_WARN_UNUSED_RESULT;

/* l4sys_reg_read_modify_write is not called unless the l1 coalescing is disabled by defining NO_COALESCING . */
void l4sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value );

void l4sys_burst_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data );

void l4sys_burst_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data );

PMC_ERROR l4sys_poll_bit( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

PMC_POLL_RETURN_TYPE l4sys_poll( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );
#else

#define l4sys_reg_write( sys_handle, reg_addr, value ) do {\
    l5sys_reg_write( sys_handle, reg_addr, value ); \
} while(0)

#define l4sys_reg_read( sys_handle, reg_addr ) ({ \
    l5sys_reg_read( sys_handle, reg_addr ); })

/* these functions (l4sys_set_field & l4sys_action_on_write_set_field) are not called unless the layer1 coalescing is disabled. */
#define l4sys_reg_read_modify_write( sys_handle, reg_addr, mask, value ) do {\
    l5sys_reg_read_modify_write( sys_handle, reg_addr, mask, value, NULL ); \
} while(0)

#define l4sys_burst_write( sys_handle, reg_addr, len, data ) do {\
    l5sys_burst_write( sys_handle, reg_addr, len, data ); \
} while(0)

#define l4sys_burst_read( sys_handle, reg_addr, len, data ) do {\
    l5sys_burst_read( sys_handle, reg_addr, len, data ); \
} while(0)

#define l4sys_poll_bit( sys, addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds ) ({ \
    pmc_sys_poll_bit( sys, addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds ); })

#define l4sys_poll( sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )({ \
    l5sys_poll( sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ); })
#endif

#define pmc_sys_dump_memory_usage(sys, just_changes)

#define pmc_sys_init_simple_defaults_with_descriptions( sys, defaults, num_defaults, descriptions, reg_bit_descriptions )

#define pmc_sys_get_description( sys, addr ) ""

#define pmc_sys_get_description_and_value( sys, addr, mask, value ) ""

#define pmc_sys_get_description_with_deltas( sys, addr,  value ) ""

#define pmc_sys_get_reg_bit_descriptions( sys, addr ) ""

#define pmc_sys_set_address_range( sys, min_addr, max_addr )

#define pmc_sys_is_valid_addr( sys, addr ) TRUE

/* begin logging all register accesses to the specified file */
#define l4sys_reg_log_open( file_name )

/* close the register access log file */
#define l4sys_reg_log_close()

/* enable or disable logging (without closing the log file) */
#define l4sys_reg_log_enable() ""
#else /* NO_IO_LOGGING */

/*******************************************************************************
* STRUCTURE: l4pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L4 state for configuration cache 
*
* ELEMENTS:
*   See typedef.
* 
* NOTES:
*   In future, may want to store some flags to enable/disable logging on a
*   particular region
*******************************************************************************/
typedef struct l4pmc_sys_handle_t
{
    l5pmc_sys_handle_t l5sys_handle; /*!< handle for next lower layer */
    UINT32 min_addr; /*!< minimum valid address */
    UINT32 max_addr; /*!< maximum valid address */
    sr_memory_region_t stats_and_descriptions; /*!< tracks statistics and register descriptions (each memory location holds a l4sys_stats_t structure) */
    int num_descriptions; /*!< The number of registers this was initialized with */
    pmc_os_stats_t     snapshot; /*!< take a snapshot of the stats when the handle is created and display stats as part of dumping memory usage */
    UINT32             last_rmw_addr; /*!< last address that was used with a read_modify_write (used to display the message "consider coalescing..." */
} l4pmc_sys_handle_t;

/*
 * set the valid address range for this system handle.
 */
void pmc_sys_set_address_range(pmc_sys_handle_t *sys, UINT32 min_addr, UINT32 max_addr );

BOOL pmc_sys_is_valid_addr(pmc_sys_handle_t *sys, UINT32 addr);

/*
 * dump a list of pages accessed.
 * If you just want variables that have had RD/WR/WRM to them, 
 * set just_changes to TRUE. 
 */
void pmc_sys_dump_memory_usage(pmc_sys_handle_t *sys, BOOL just_changes);

/* initialize the simulation memory with a set of default values */
void pmc_sys_init_defaults_and_descriptions( pmc_sys_handle_t *sys, simple_memory_defaults *defaults, int num_defaults, const char *descriptions[], const char *reg_bit_descriptions[] );

/*
 * returns the name of the register at that address
 * e.g. "<register name>"
 * DO NOT USE THIS FUNCTION IN PRODUCTION CODE,... it is only valid if PMC_SW_SIMULATION is defined
 */
const char *pmc_sys_get_description( pmc_sys_handle_t *sys, UINT32 addr );

/*
 * displays all fields represented by mask 
 * e.g. "<register name> <field>=<value> <field2>=<value2>"
 * DO NOT USE THIS FUNCTION IN PRODUCTION CODE,... it is only valid if PMC_SW_SIMULATION is defined
 */
const char *pmc_sys_get_description_and_value( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value );

/*
 * displays fields that have changed since the last time this register was displayed
 * e.g. "<register name> <field>=<value> <field2>=<value2>" 
 * DO NOT USE THIS FUNCTION IN PRODUCTION CODE,... it is only valid if PMC_SW_SIMULATION is defined
 */
const char *pmc_sys_get_description_with_deltas( pmc_sys_handle_t *sys, UINT32 addr, UINT32 value );

/*
 * returns the register bit descriptions string from the defaults.h file,
 * note: this format is subject to change.
 * currently this function is used to identify which registers should be used to simulate indirect memory.
 * DO NOT USE THIS FUNCTION IN PRODUCTION CODE,... it is only valid if PMC_SW_SIMULATION is defined
 */
const char *pmc_sys_get_reg_bit_descriptions( pmc_sys_handle_t *sys, UINT32 addr );

void l4sys_init( pmc_sys_handle_t *sys_handle );

void l4sys_halt( pmc_sys_handle_t *sys_handle );

void l4sys_reg_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value );

UINT32 l4sys_reg_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr ) ATTRIBUTE_WARN_UNUSED_RESULT;

/* l4sys_reg_read_modify_write is not called unless the l1 coalescing is disabled by defining NO_COALESCING . */
void l4sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value );

void l4sys_burst_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data );

void l4sys_burst_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 len, UINT32 *data );

PMC_ERROR l4sys_poll_bit( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

PMC_POLL_RETURN_TYPE l4sys_poll( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

/* begin logging all register accesses to the specified file */
void l4sys_reg_log_open( void *file_name );

/* close the register access log file */
void l4sys_reg_log_close( );

/* enable or disable logging (without closing the log file) */
int  l4sys_reg_log_enable( BOOL enable );

#endif /* NO_IO_LOGGING */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* l4_SYS_H */
