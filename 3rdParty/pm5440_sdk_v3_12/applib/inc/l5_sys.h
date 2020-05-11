/*******************************************************************************
*   COPYRIGHT (C) 2008-2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                  
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
*    This file contains PMC system access function definitions. This file is
*    setup to support customer systems as well as the PMC EVBD, emulation,
*    simulation, and verification environment. There is no customer porting
*    required in this file that occurs in pmc_customer_os.h and
*    pmc_customer_os.c.
* 
*   NOTES: 
*
******************************************************************************/

#ifndef L5_SYS_H
#define L5_SYS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "pmc_types.h"
#include "pmc_err.h"
#include "l6_sys.h"

#ifndef L5SYS_MAX_MESSAGES_TO_DISPLAY
#define L5SYS_MAX_MESSAGES_TO_DISPLAY    (0) /*!< maximum messages to display in a queue dump */
#endif

#ifndef L5SYS_HOST_QUEUE_SIZE_IN_WORDS
#ifdef L5SYS_PROFILING
#define L5SYS_HOST_QUEUE_SIZE_IN_WORDS    (1024*256) /*!< memory usage of 1MB - that's about 256k write operations (addr+value), minimum size needed is 512 words */
#else
#define L5SYS_HOST_QUEUE_SIZE_IN_WORDS    (128*256)  /*!< memory usage of 128k bytes - that's about 16k write operations (addr+value), minimum size needed is 512 words */
#endif
#endif

#ifndef L5SYS_READ_BUFFER_SIZE_IN_WORDS
#define L5SYS_READ_BUFFER_SIZE_IN_WORDS   (32*256)  /*!< memory usage of 32k bytes - enough room for 8k register reads, minimum size needed is 64 words */
#endif

#ifndef L5SYS_FW_LOG_BUFFER_SIZE_IN_WORDS
#define L5SYS_FW_LOG_BUFFER_SIZE_IN_WORDS (0)       /*!< size in words of the firmware log buffer on the host (firmware spools messages to the host) */
#endif


#ifndef DISABLE_LOGGING
#define L5SYS_LOG(sys_handle,fmt,...) l5sysLog(sys_handle,fmt,##__VA_ARGS__)
#else
#define L5SYS_LOG(sys_handle,fmt,...) PMC_PRINT(fmt,##__VA_ARGS__)
#endif
PUBLIC void l5sysLog( pmc_sys_handle_t *sys_handle, const char *fmt, ... );

/*******************************************************************************
* ENUM: operation_code_definitions_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerations for the operation codes to write into FW SPRAM
*    bits 25:31 of the command/address word (7 bits is 128 opcodes)
*
* ELEMENTS:
*   See typedef. 
*******************************************************************************/
typedef enum
{
    OP_CODE_WRITE          = 1,   /* Do a write on the respective address */
    OP_CODE_SHORT_RMW      = 2,   /* Do a short read-modify-write on the respective address */
    OP_CODE_LONG_RMW       = 3,   /* Do a long read-modify-write on the respective address */
    OP_CODE_READ           = 4,   /* Do a read on the respective address and return the result */
    OP_CODE_POLL           = 5,   /* Poll for the register value */
    OP_CODE_SLEEP          = 6,   /* Sleep for a perscribed period of time */
    OP_CODE_BENCH          = 7,   /* measure the time to perform a read */
    OP_CODE_SEGMENT        = 8,   /* process messages from a host queue segment. */
    OP_CODE_BURST_WRITE    = 9,   /* do a burst write. */
    OP_CODE_BURST_READ     = 10,  /* do a burst read. */
    OP_CODE_BATCH_READ     = 11,  /* read a list of registers */
    OP_CODE_FIRMWARE_CMD   = 12,  /* perform a firmware command */
    OP_CODE_FIRMWARE_QUERY = 13,  /* perform a firmware query */
    OP_CODE_PADDING        = 14,  /* padding */
    OP_CODE_SELF_TEST      = 15,  /* self-test */
    OP_CODE_CHECK_SUM      = 16,  /* send a checksum message to firmware */
} l5sys_operation_code_definitions_enum;

/*******************************************************************************
* STRUCTURE: pmcfw_dma_results_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   There are 3 major users of the dma results:
*
*   batch reads (list of registers e.g. interrupt status registers)
*   burst reads (one address, multiple registers from that location)
*   firmware queury results (every firmware command writes the results back)
*
*   From the firmware's point of view, the read results are put at a particular
*   location.
*
*   I can't see an easy programming model for exposing the buffer past l5sys
*   so I don't know if we need to implement a circular buffer for read responses.
*
*   for batch reads:
*
*   A batch read that requests 8192 register will be done in multiple read requests.
*
*   so you'll send 81 100 register read requests down to firmware,
*
*   firmware will dma the 100 results 81 times to different memory locations in the
*   results buffer.
*
*   for a burst read: 
*
*   The command to perform a burst read can be handed to the dma engine without much
*   firmware interaction.
*
*   burst read requests of larger then the maximum read buffer will have to be broken
*   into multiple requests.
*
*   for firmware queries:
*
*   results are always a maximum of 64 words,... although maybe we could add a 'max_resp_len' ?
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct pmcfw_dma_results_t
{
    volatile UINT32 read_results[L5SYS_READ_BUFFER_SIZE_IN_WORDS];  /*!<  firmware places read results and firmware query results here. */
} pmcfw_dma_results_t;


/*******************************************************************************
* STRUCTURE: pmcfw_queue_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   one large queue 
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct pmcfw_dma_queue_t
{
    UINT32 circular_buffer[L5SYS_HOST_QUEUE_SIZE_IN_WORDS];
} pmcfw_dma_queue_t;


/*******************************************************************************
* STRUCTURE: pmcfw_dma_queue_index_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   index into pmcfw_dma_queue_t  (this is local to the host and not in shared memory)
*   NOTE: the space available in the queue is from index -> dma_queue_fw_index+SIZE
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct
{
    UINT32 index; /*!< what is the current write index in the operation buffer? */
    UINT32 flush_size; /*!< how often should the write index be written to shared memory? */
    UINT32 copy_of_dma_queue_fw_index; /*!< copy of the shared memory location dma_queue_fw_index */
    UINT32 last_index_written_to_spram; /*!< last value written to the spram host index */
    UINT32 spram_address_of_host_index; /*!< spram address where host index should be written on digi */
    UINT32 last_index_displayed; /*!< what was the last index displayed with dump_queue? (avoid repeating information) */
} pmcfw_dma_queue_index_t;

#if L5SYS_FW_LOG_BUFFER_SIZE_IN_WORDS > 0
/*******************************************************************************
* STRUCTURE: pmcfw_log_buffer_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Memory on host for the firmware log messages
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct pmcfw_log_buffer_t
{
    volatile UINT32 circular_buffer[L5SYS_FW_LOG_BUFFER_SIZE_IN_WORDS];
} pmcfw_log_buffer_t;
#endif

/*******************************************************************************
* STRUCTURE: pmcfw_log_buffer_index_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   indicies into pmcfw_log_buffer_t  (this is local to the host and not in shared memory)
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct pmcfw_log_buffer_index_t
{
    UINT32 read_index;  /*!< the read index only modified by the host. */
    UINT32 circular_buffer_size;  /*!< size of the circular buffer in bytes? */
    UINT32 number_of_log_entries;  /*!< number of entries in the log buffer */
} pmcfw_log_buffer_index_t;


/*******************************************************************************
* STRUCTURE: l5sys_shared_host_memory_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Memory on host shared between host and firmware.
*
*   The physical address of this structure is passed to firmware and firmware
*   performs single pcie writes and dma writes to this structure.
*
*   firmware also performs dma reads from this structure.
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct l5sys_shared_host_memory_t
{
    volatile UINT32 read_result;              /*!< result of the last READ request */
    volatile UINT32 dma_queue_fw_index;       /*!< next index that firmware will execute */
    volatile UINT32 poll_result;              /*!< address of the last poll request that failed. */
    volatile UINT32 should_be_0x12345678;     /*!< firmware sets this to 0x12345678 on initialization */
    volatile UINT32 log_buffer_write_index;   /*!< the firmware updates the write index. */
    UINT32 align_to_64_bits;                  /*!< force dma_queue to start at a 64 bit aligned address. */
    pmcfw_dma_queue_t   pmcfw_dma_queue;      /*!< host memory for firmware queue which is dma'd by firmware from host to firmware spram */
    pmcfw_dma_results_t pmcfw_dma_results;    /*!< host memory for results read by firmware which is dma'd from firmware spram to host */
#if L5SYS_FW_LOG_BUFFER_SIZE_IN_WORDS > 0
    pmcfw_log_buffer_t  pmcfw_log_buffer;     /*!< firmware log buffer */
#endif
} l5sys_shared_host_memory_t;

/*! Function pointer for queue stall callback */
typedef void (*l5sys_callback_t) (void *param, BOOL8 full);

/*! Function pointer for retrieving firmare module name and subtype name */
typedef void (*l5sys_fw_get_function_name_t)(UINT32 word0, UINT32 word1, const char **mod_name_ptr, const char**sub_type_ptr);

/*! Function pointer for retrieving block,register & field description associated with a queue operation */
typedef void (*l5sys_fw_get_description_t)(void *param, l5sys_operation_code_definitions_enum opcode, UINT32 addr, UINT32 mask, UINT32 value, UINT32 flags, char *buffer );

/*! Function pointer for showing block access statistics  */
typedef void (*l5sys_fw_show_stats_t)(pmc_sys_handle_t *sys_handle, void *param);
 

/*******************************************************************************
* STRUCTURE: l5sys_queue_message_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   a linked list of messages to be sent to the firmware queue 
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct l5sys_queue_message_t {
    struct l5sys_queue_message_t *next; /*!< next message (note: this must be the first field for various list operations to work) */
    struct l5sys_queue_message_t *prev; /*!< previous message (not initialized except in certain tests) */
    UINT32 index;                       /*!< index of this message (when called with 'timed_playback') */
    UINT64 done_timestamp;              /*!< timestamp when this message was completed (when played back with 'timed_playback') */
    UINT64 started_timestamp;           /*!< timestamp when this message was completed (when played back with 'timed_playback') */
    UINT32 num;                         /*!< number of words in this message */
    UINT32 data[0];                     /*!< variable length array for this message */
} l5sys_queue_message_t;

/*! Function pointer which is eventually set to fwvLog() */
typedef void (*log_function_t)( const char *frmt, va_list __arg );

/*******************************************************************************
* STRUCTURE: l5pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local variables to manage the firmware queue 
*
* ELEMENTS:
*   See typedef.
* 
*******************************************************************************/
typedef struct l5pmc_sys_handle_t
{
    BOOL8   pipe_created;          /*!< the firmware pipe exists */
    BOOL8   use_pipe;              /*!< enable the use of the firmware pipe */
    INT8    disable_count;         /*!< nesting level of l5sys_firmware_queue_disable */
    volatile l5sys_shared_host_memory_t *shared_host_memory_ptr; /*!< virtual address of host memory */
    UINT64  physical_address;      /*!< physical address of shared host memory */
    BOOL    ignore_poll_errors;    /*!< should poll errors be ignored? */

    BOOL8   logging_enabled;       /*!< is l5sys logging enabled? */
    log_function_t log_function;       /*!< logging function to call. */
    BOOL8   buffered_io;           /*!< should firmware notifications of new queue messages be buffered for better throughput */
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    l5sys_queue_message_t *messages; /*!< list of messages when recording messages */
    l5sys_queue_message_t *link;     /*!< last recorded message */
    l5sys_queue_message_t *active;   /*!< last message processed by firmware (used to estimate delay) */
    FILE   *logfile;               /*!< file handle for logging all l5sys messages */
#endif
    UINT32 *queue_history_is_cmd;  /*!< array of flags indicating this was the beginning of a command */
    l5sys_fw_get_function_name_t fw_get_function_name; /*!< function to extract module name and subtype from firmware message */
    l5sys_fw_get_description_t   fw_get_description; /*!< function to return register and field descriptions */
    l5sys_fw_show_stats_t        fw_show_stats; /*!< function to return register and field descriptions */
#endif
    UINT32 test_pattern;  /*!< test pattern for self-test */
#ifdef L5SYS_ADD_CHECKSUM_AFTER_EVERY_FLUSH
    UINT32 checksum;      /*!< checksum of all messages sent to firmware */
#endif

    pmcfw_dma_queue_index_t   pmcfw_dma_queue_index;     /*!< index into firmware queue        */
    pmcfw_log_buffer_index_t  pmcfw_log_buffer_index;    /*!< index into firmware log buffer   */

    l5sys_callback_t queue_stall_callback_function; /*!< function to call if there is a queue stall. */
    void *queue_stall_callback_param;    /*!< parameter to pass to function if there is a queue stall. */
    UINT32 count;
    UINT32 timer_hi;
    UINT32 timer_lo;
    UINT32 process_id;
} l5pmc_sys_handle_t;


PUBLIC void l5sys_wait_queue_done(void *handle);
PUBLIC void l5sys_install_queue_stall_callback( void *handle, l5sys_callback_t queue_stall_callback_function, void *queue_stall_callback_param );
PUBLIC void l5sys_install_info_functions( void *handle, l5sys_fw_get_function_name_t fw_get_function_name, l5sys_fw_get_description_t   fw_get_description, l5sys_fw_show_stats_t fw_show_stats );
PUBLIC PMC_ERROR l5sys_check_queue_errors(void *handle);

PUBLIC void l5sys_abort_if_queue_errors(void *handle);
PUBLIC void l5sys_ignore_poll_errors(void *handle);
PUBLIC void l5sys_abort_on_poll_errors(void *handle);
PUBLIC void l5sys_firmware_queue_disable(void *handle );
PUBLIC void l5sys_firmware_queue_enable(void *handle );

void l5sys_halt( pmc_sys_handle_t *sys_handle );

void l5sys_init( pmc_sys_handle_t *sys_handle );

PUBLIC void l5sys_init_queue( void *handle, UINT32 spram_address_of_host_index );

UINT32 l5sys_reg_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr );

DOUBLE l5sys_bench_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr );

void l5sys_reg_write( pmc_sys_handle_t *hndl,
                                      UINT32 addr,
                                      UINT32 value );

UINT32 l5sys_reg_read_modify_write( pmc_sys_handle_t *hndl, 
                                                    UINT32 addr,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    UINT32 *oldval );

PMC_POLL_RETURN_TYPE l5sys_poll(pmc_sys_handle_t *sys, 
                                UINT32 addr, 
                                UINT32 mask, 
                                UINT32 value, 
                                PMC_POLL_COMPARISON_TYPE cmp, 
                                UINT32 max_count, 
                                UINT32 *num_failed_polls, 
                                UINT32 delay_between_polls_in_microseconds );

void l5sys_burst_read( pmc_sys_handle_t *hndl,
                                       UINT32 addr,
                                       UINT32 len,
                                       UINT32 *data );

void l5sys_batch_read( pmc_sys_handle_t *hndl,
                                       UINT32 len,
                                       UINT32 *addr,
                                       UINT32 *data );

void l5sys_burst_write( pmc_sys_handle_t *hndl, 
                                        UINT32 addr,
                                        UINT32 len,
                                        UINT32 *data );

void l5sys_self_test( void *handle, UINT32 len );

void l5sys_q_usleep( void *handle, UINT32 usec );

PMC_ERROR l5sys_queue_init_test( void *handle );

BOOL8 l5sys_queue_enabled(void *handle);
PMC_ERROR l5sys_fw_perform_query(void *handle, UINT32 *data_ptr, UINT32 len, UINT32 *response, UINT32 max_response_len, UINT32 header[2]);
PMC_ERROR l5sys_fw_send_cfg_message(void *handle, UINT32 *data_ptr, UINT32 len );
PUBLIC void l5sys_display_queue(void *handle,const char *where, const char *caller);

PUBLIC void l5sys_log_record(void *handle);
PUBLIC l5sys_queue_message_t *l5sys_log_stop(void *handle);
PUBLIC void l5sys_log_function(void *handle,log_function_t log_function);
PUBLIC void l5sys_log_open(void *handle,const char *file_name);
PUBLIC void l5sys_log_close(void *handle);
PUBLIC PMC_ERROR l5sys_log_read(void *handle,const char *file_name, l5sys_queue_message_t **messages);
PUBLIC void l5sys_log_playback(void *handle,l5sys_queue_message_t *messages, BOOL8 buffer);
PUBLIC void l5sys_log_timed_playback(void *handle,l5sys_queue_message_t *messages);
PUBLIC void l5sys_log_bench_each_message(void *handle,l5sys_queue_message_t *messages, const char *file_name);
PUBLIC void l5sys_log_save(void *handle,l5sys_queue_message_t *messages, const char *file_name );
PUBLIC void l5sys_log_free(l5sys_queue_message_t *messages);

PUBLIC void l5sys_buffered(void *handle);
PUBLIC void l5sys_unbuffered(void *handle);

#define PMC_Q_USLEEP(handle,usec)  do { \
  if (usec > 2000) { \
    if (l5sys_queue_enabled(handle)) { \
        PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE, usec, 0, "large PMC_Q_USLEEP(%d)\n",usec); \
    } \
  } \
  l5sys_q_usleep(handle,usec); \
} while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* L5_SYS_H */
