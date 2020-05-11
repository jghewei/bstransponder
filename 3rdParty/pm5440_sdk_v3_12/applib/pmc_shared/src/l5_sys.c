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
*     The level 5 system layer determines whether FW should execute the
*     read, writes, read-modify-writes or they should be executed directly.
* 
*   NOTES:
*
********************************************************************************/

#include "l5_sys.h"
#include "pmc_sys.h"

static void pipe_check_sum( pmc_sys_handle_t *sys_handle );
static void pipe_self_test( pmc_sys_handle_t *sys_handle, UINT32 len );
static UINT32 pipe_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr );
static void pipe_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value );
static void pipe_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value );
static UINT32 queue_message( pmc_sys_handle_t *sys_handle, UINT32 * msg, UINT32 msg_size ) ATTRIBUTE_HOT;
static UINT32 queue_message_and_data( pmc_sys_handle_t *sys_handle, UINT32 * msg, UINT32 msg_size, UINT32 *data, UINT32 data_size ) ATTRIBUTE_HOT;
static BOOL is_queue_empty( pmc_sys_handle_t *sys_handle );
static void wait_queue_done( pmc_sys_handle_t *sys_handle, pmc_sys_stat_t *stat );
static PMC_POLL_RETURN_TYPE pipe_poll(pmc_sys_handle_t *hndl, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );
static void pipe_burst_write(pmc_sys_handle_t *sys_handle, UINT32 addr, UINT32 len, UINT32 *data );
static void pipe_burst_read(pmc_sys_handle_t *sys, UINT32 addr, UINT32 len, UINT32 *data );
static BOOL8 check_pcie_alive(pmc_sys_handle_t *sys_handle);

#ifdef L5SYS_PROFILING
static l5sys_queue_message_t *add_message( UINT32 *msg, UINT32 msg_size, UINT32 *data, UINT32 data_size );
#endif
static void dump_queue(pmc_sys_handle_t *sys_handle,const char *where,const char *caller,UINT64 stall_time);
static void l5sys_process_id_check(pmc_sys_handle_t *sys_handle);

#ifdef L5SYS_ADD_CHECKSUM_AFTER_EVERY_FLUSH
#define INCREMENT_CHECKSUM(h,val) h->checksum += (val)
#else
#define INCREMENT_CHECKSUM(h,val) 
#endif

#define OP_ADDR(op,addr) ( ( (op << 25) | (addr & 0x1fffffc) ) )

#define MIN_FLUSH_SIZE 3 /* initially flush every 2^3 = 8 words */
#define MAX_FLUSH_SIZE 6 /* flush at least every 2^6 = 64 words */

/* set a trace level for queue debugging:
   8 - too many messages.
   6 - lots of messages.
   */
#define L5SYS_TRACE_LEVEL 0
#if L5SYS_TRACE_LEVEL > 0
#define L5SYS_TRACE(level,frmt,...) do { \
      if (level<=L5SYS_TRACE_LEVEL) { \
        PMC_PRINT("L5SYS_TRACE%d: "frmt,level,##__VA_ARGS__); \
        if (L5SYS_TRACE_LEVEL > 6) usleep(5000); \
      } \
  } while(0)
#else
#define L5SYS_TRACE(level,frmt,...) do { } while(0)
#endif

#ifdef L5SYS_PROFILING
/*******************************************************************************
* now_usec
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the time of day in microseconds
*
* INPUTS:
*   None.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT64 - time of day in microseconds.
*
* NOTES:
*******************************************************************************/
static UINT64 now_usec() {
    pmc_sys_timeval_t tv;
    UINT64 microseconds;

    PMC_TIMEVAL_READ(&tv);
    microseconds = tv.tv_sec;
    microseconds *= 1000000;
    microseconds += tv.tv_usec;
    return (microseconds);
}
#endif

/*******************************************************************************
* find_first_bit_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the number of the first bit set in the mask
*
* INPUTS:
*   mask - the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The position of the first bit set.
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 find_first_bit_set(UINT32 mask)
{

    UINT32 value = 0;

    if ((mask & 0xFFFF) == 0)
    {
        value+= 16;
        mask >>= 16;
    }
    if ((mask & 0xFF) == 0)
    {
        value += 8;
        mask >>= 8;
    }
    if ((mask & 0xF) == 0)
    {
        value += 4;
        mask >>= 4;
    }
    if ((mask & 0x3) == 0)
    {
        value += 2;
        mask >>= 2;
    }
    if ((mask & 0x1) == 0)
    {
        value += 1;
    }
    return value;
}

/*******************************************************************************
* is_mask_less_12_bits
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Determines if the mask is fewer than 12 bits
*
* INPUTS:
*   mask - the mask
*   first_set_bit - the first bit set in the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE if the mask is fewer than 12 bits, FALSE otherwise.
*
* NOTES:
*******************************************************************************/
static INLINE BOOL is_mask_less_12_bits(UINT32 mask, UINT32 first_set_bit){
    return ((mask >> first_set_bit) < (1 << 12));
}

/*******************************************************************************
* l5sys_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the L5 layer.
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
void l5sys_init( pmc_sys_handle_t *sys_handle )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    /* l6sys_init(sys_handle); */

    PMC_ENTRY();
    h->queue_stall_callback_function = NULL;
    h->queue_stall_callback_param = NULL;
    h->use_pipe = 0;
    h->pipe_created = 0;  
    h->disable_count = 0;    
    h->shared_host_memory_ptr = NULL;
    h->ignore_poll_errors = FALSE; /* defaults to ASSERT if a poll error occurs */

    memset(&(h->pmcfw_log_buffer_index),  0, sizeof(h->pmcfw_log_buffer_index));
    memset(&(h->pmcfw_dma_queue_index),   0, sizeof(h->pmcfw_dma_queue_index));
#ifndef LOGGING_DISABLED
    h->queue_history_is_cmd = NULL;
#ifdef L5SYS_PROFILING
    h->active = NULL;
    h->link = NULL;
    h->messages = NULL;
#endif
#endif
    h->pmcfw_dma_queue_index.flush_size = MIN_FLUSH_SIZE;
    PMC_RETURN();
}


/*******************************************************************************
* l5sys_init_queue
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the firmware queue (called by digi_fw_init_firmware_queue).
*
* INPUTS:
*   handle - pointer to any pmc handle
*   spram_address_of_host_index - spram address of host index in firmware memory
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PUBLIC void l5sys_init_queue(void *handle, UINT32 spram_address_of_host_index)
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

    PMC_ENTRY();

    memset(&(h->pmcfw_log_buffer_index),  0, sizeof(h->pmcfw_log_buffer_index));
    memset(&(h->pmcfw_dma_queue_index),   0, sizeof(h->pmcfw_dma_queue_index));
    h->use_pipe = TRUE;
    h->pipe_created = TRUE;
    h->pmcfw_dma_queue_index.spram_address_of_host_index = spram_address_of_host_index;

    PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE, 0, 0, "firmware queue enabled\n" );
    PMC_RETURN();
}


/*******************************************************************************
* l5sys_wb_invalidate_cache
* ___________________________________________________________________________
*
* DESCRIPTION:
*   invalidate the cache lines for the "shared host memory".
*  
*   note: firmware writes to these locations so we either need this memory in "uncached" memory,
*       or we need to explicity instruct the cache to refill the cache line
*  
*   invalidate the 12 bytes of shared memory.
*  
*   Note: we will need finer grained control when we put buffers in host memory;
*   for now invalidating 4 bytes is the same as invalidating 12 bytes e.g. 1 cache line
*   so invalidating the entire region is fine.
*
* INPUTS:
*   h - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing
*
* NOTES:
*******************************************************************************/
static INLINE void l5sys_wb_invalidate_cache(l5pmc_sys_handle_t *h)
{
    pmc_sys_wb_invalidate_cache((void *)h->shared_host_memory_ptr,sizeof(l5sys_shared_host_memory_t));
}

/*******************************************************************************
* l5sys_read_result
* ___________________________________________________________________________
*
* DESCRIPTION:
*   get the read_result from shared host memory
*
* INPUTS:
*   h - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - the read result
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 l5sys_read_result(l5pmc_sys_handle_t *h)
{
    UINT32 x;

    l5sys_wb_invalidate_cache(h);
    x = h->shared_host_memory_ptr->read_result;
    L5SYS_TRACE(5,"l5sys_read_result %x\n",x);
    return PMC_LE_TO_CPU_32(x);
}

/*******************************************************************************
* l5sys_dma_queue_fw_index
* ___________________________________________________________________________
*
* DESCRIPTION:
*   get the dma_queue_fw_index from shared host memory 
*
* INPUTS:
*   h - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - the driver index
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 l5sys_dma_queue_fw_index(l5pmc_sys_handle_t *h)
{
    UINT32 x;

    l5sys_wb_invalidate_cache(h);
    x = h->shared_host_memory_ptr->dma_queue_fw_index;
    L5SYS_TRACE(9,"l5sys_dma_queue_fw_index %d\n",x);
    return PMC_LE_TO_CPU_32(x);
}



/*******************************************************************************
* l5sys_poll_result
* ___________________________________________________________________________
*
* DESCRIPTION:
*   get the poll result from shared host memory 
*
* INPUTS:
*   h - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - the poll result
*
* NOTES:
*******************************************************************************/
/* get the poll_result from shared host memory */
static INLINE UINT32 l5sys_poll_result(l5pmc_sys_handle_t *h)
{
    UINT32 x;

    l5sys_wb_invalidate_cache(h);
    x = h->shared_host_memory_ptr->poll_result;
    return PMC_LE_TO_CPU_32(x);
}

/*******************************************************************************
* l5sys_should_be_0x12345678
* ___________________________________________________________________________
*
* DESCRIPTION:
*   get 0x12345678 from shared host memory
*
* INPUTS:
*   h - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - Hopefully 0x12345678
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 l5sys_should_be_0x12345678(l5pmc_sys_handle_t *h)
{
    UINT32 x;

    l5sys_wb_invalidate_cache(h);
    x = h->shared_host_memory_ptr->should_be_0x12345678;
    return PMC_LE_TO_CPU_32(x);
}

/*******************************************************************************
* l5sys_halt
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Uninitialize the L5 layer.
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
void l5sys_halt( pmc_sys_handle_t *sys_handle )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    /* l6sys_halt(sys_handle); */

    PMC_ENTRY();
    h->queue_stall_callback_function = NULL;
    h->use_pipe = 0;
    h->pipe_created = 0;
    h->shared_host_memory_ptr = NULL;

#ifndef LOGGING_DISABLED
    if (h->queue_history_is_cmd != NULL)
    {
        PMC_FREE(&h->queue_history_is_cmd);
    }
#endif
    PMC_RETURN();
}
/*******************************************************************************
* l5sys_install_queue_stall_callback
* ___________________________________________________________________________
*
* DESCRIPTION:
*   install a callback that should be called whenever there is a queue stall.
*   this is used to show the state of the firmware.
*
* INPUTS:
*   handle - pointer to any pmc handle
*   queue_stall_callback_function - callback function
*   queue_stall_callback_param - callback parameter
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   The callback should not perform any queue operations,... instead it should
*   read registers directly using pmc_ calls instead of l2sys_ or l5sys_ calls.
*
*   This also prohibits firmware messages, although perhaps we could disable the
*   queue during this callback and re-enable it afterwards. (and skip the wait queue done)
*
*******************************************************************************/
PUBLIC void l5sys_install_queue_stall_callback( void *handle, l5sys_callback_t queue_stall_callback_function, void *queue_stall_callback_param )
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    /* l6sys_halt(sys_handle); */
    h->queue_stall_callback_function = queue_stall_callback_function;
    h->queue_stall_callback_param = queue_stall_callback_param;
}


/*******************************************************************************
* l5sys_install_info_functions
* ___________________________________________________________________________
*
* DESCRIPTION:
*   install various information callbacks
*
* INPUTS:
*   handle - pointer to any pmc handle
*   fw_get_function_name - callback function to get a firmware module and function name.
*   fw_get_description   - callback function to display block, register and field names.
*   fw_show_stats        - callback function to show register access statistics per block
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
PUBLIC void l5sys_install_info_functions( void *handle,
                                          l5sys_fw_get_function_name_t fw_get_function_name, 
                                          l5sys_fw_get_description_t fw_get_description, 
                                          l5sys_fw_show_stats_t fw_show_stats )
{
#ifndef LOGGING_DISABLED
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    /* l6sys_halt(sys_handle); */
    h->fw_get_function_name = fw_get_function_name;
    h->fw_get_description = fw_get_description;
    h->fw_show_stats = fw_show_stats;
#endif
}



/*******************************************************************************
* queue_sleep
* ______________________________________________________________________________
*
* DESCRIPTION:
*   send a message to firmware to insert a delay between the current pending writes
*   and subsequent writes.
*
* INPUTS:
*   sys_handle - emulation_sys_handle_t
*   usec    - The number of microseconds to wait
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   None.
*
* NOTES:
*   None.
*
*******************************************************************************/
static void queue_sleep(pmc_sys_handle_t *sys_handle,UINT32 usec)
{
    UINT32 message[2];
    BOOL8 buffered_io_save;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

    /* Set up a Sleep message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_SLEEP, 0x0);
    message[0] = usec;
    LOG("Queuing sleeping for %d microseconds\n",usec);
    PMC_OS_STATS_QUEUE_USLEEP(usec);

    /* force a buffer flush */
    buffered_io_save = h->buffered_io;
    h->buffered_io = FALSE;
    queue_message(sys_handle, message, 2);
    h->buffered_io = buffered_io_save;
}



/*******************************************************************************
* l5sys_firmware_queue_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable the firmware queue
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none
*
* NOTES:
*   l5sys_firmare_queue_disable and l5sys_firmware_queue_enable must be used
*   in pairs.
*
*   if nested calls are made, only the final l5sys_firmware_queue_enable re-enables
*   the firmware queue.
*
*   for example:
*     l5sys_firmware_queue_disable(handle);   // firmware queue disabled.
*       l5sys_firmware_queue_disable(handle); // firmware queue still disabled.
*       l5sys_firmware_queue_enable(handle);  // firmware queue still disabled.
*     l5sys_firmware_queue_enable(handle);    // firmware queue now is enabled.
*
*******************************************************************************/
PUBLIC void l5sys_firmware_queue_disable(void *handle )
{
    PMC_ENTRY();

    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:l5sys_firmware_queue_disable" );

    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if (sys_handle != NULL) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

            wait_queue_done(sys_handle,fw_queue_wait_done);
            h->disable_count++;
            if (h->pipe_created)
            {
                h->use_pipe = FALSE;
            }
        }
    }
    PMC_RETURN();
}


/*******************************************************************************
* l5sys_firmware_queue_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable the firmware queue
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none
*
* NOTES:
*   l5sys_firmare_queue_disable and l5sys_firmware_queue_enable must be used
*   in pairs.
*
*   if nested calls are made, only the final l5sys_firmware_queue_enable re-enables
*   the firmware queue.
*
*   for example:
*     l5sys_firmware_queue_disable(handle);   // firmware queue disabled.
*       l5sys_firmware_queue_disable(handle); // firmware queue still disabled.
*       l5sys_firmware_queue_enable(handle);  // firmware queue still disabled.
*     l5sys_firmware_queue_enable(handle);    // firmware queue now is enabled.
*
*******************************************************************************/
PUBLIC void l5sys_firmware_queue_enable(void *handle )
{
    PMC_ENTRY();
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if (sys_handle != NULL) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            h->disable_count--;
            if (h->pipe_created && (h->disable_count <= 0))
            {
                h->use_pipe = TRUE;
            }
        }
    }
    PMC_RETURN();
}

/*******************************************************************************
* l5sys_q_usleep
* ______________________________________________________________________________
*
* DESCRIPTION:
*   If a firmware queue is being used, this function sends the sleep message to
*   firmware.
* 
*   If a firmware queue is not being used, this function sleeps for the indicated
*   duration.
*   
* INPUTS:
*   handle - any pmc_handle
*   usec   - The number of microseconds to wait.
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void l5sys_q_usleep(void *handle, UINT32 usec)
{
    PMC_ENTRY();
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if (sys_handle != NULL) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

            /* if handle is allocated and the firmware queue is enabled, send the message to firmware */
            if (h->use_pipe )
            {
                queue_sleep(sys_handle,usec);
                PMC_RETURN();
            }
        }
    }
    /* if the firmware queue is disabled, just call PMC_OS_USLEEP() */
    PMC_OS_USLEEP(usec);
    PMC_RETURN();
}

/*******************************************************************************
* l5sys_wait_queue_done
* ______________________________________________________________________________
*
* DESCRIPTION:
*   wait for the firmware io queue to be complete
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none
*
*******************************************************************************/
PUBLIC void l5sys_wait_queue_done(void *handle)
{
    PMC_ENTRY();
    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:l5sys_wait_queue_done" );
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            if (h->use_pipe )
            {
                wait_queue_done(sys_handle,fw_queue_wait_done);
            }
        }
    }
    PMC_RETURN();
}


/*******************************************************************************
* l5sys_check_queue_errors
* ______________________________________________________________________________
*
* DESCRIPTION:
*   wait for the firmware io queue to be complete and return an error if any
*   poll operation generated an error.
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   PMC_SUCCESS or PMC_ERR_POLL_FAILED
*
*******************************************************************************/
PUBLIC PMC_ERROR l5sys_check_queue_errors(void *handle)
{
    PMC_ENTRY();
    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:l5sys_check_queue_errors" );

    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            if (h->use_pipe )
            {
                UINT32 poll_result;
                wait_queue_done(sys_handle,fw_queue_wait_done);
                poll_result = l5sys_poll_result(h);
                if (poll_result != 0) {
                    PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_POLL_FAILED, poll_result, 0, "poll failed addr=0x%08x\n",poll_result); 
                    /* clear the poll_result after acknowledging the error */
                    h->shared_host_memory_ptr->poll_result = 0;
                    PMC_RETURN( PMC_SHARED_ERR_POLL_FAILED );
                }
            }
        }
    }
    PMC_RETURN( PMC_SUCCESS );
}


/*******************************************************************************
* l5sys_ignore_poll_errors
* ______________________________________________________________________________
*
* DESCRIPTION:
*   queue'd poll errors will be ignored.  PMC_ATOMIC_RETURN will not generate an ASSERT
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none
*
*******************************************************************************/
PUBLIC void l5sys_ignore_poll_errors(void *handle)
{
    PMC_ENTRY();
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            h->ignore_poll_errors = TRUE;
        }
    }
    PMC_RETURN();
}


/*******************************************************************************
* l5sys_abort_on_poll_errors
* ______________________________________________________________________________
*
* DESCRIPTION:
*   queue'd poll errors will latch an error flag that will be tested in PMC_ATOMIC_RETURN and will generate an ASSERT
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   none
*
*******************************************************************************/
PUBLIC void l5sys_abort_on_poll_errors(void *handle)
{
    PMC_ENTRY();
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            h->ignore_poll_errors = FALSE;
        }
    }
    PMC_RETURN();
}

/*******************************************************************************
* l5sys_abort_if_queue_errors
* ______________________________________________________________________________
*
* DESCRIPTION:
*   wait for the firmware io queue to be complete and ASSERT if any
*   poll operation generated an error.
*   
* INPUTS:
*   handle - any pmc_handle
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   PMC_SUCCESS or PMC_ERR_POLL_FAILED
*
*******************************************************************************/
PUBLIC void l5sys_abort_if_queue_errors(void *handle)
{
    PMC_ENTRY();
    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:l5sys_abort_if_queue_errors" );

    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            if (h->use_pipe )
            {
                UINT32 poll_result;
                wait_queue_done(sys_handle,fw_queue_wait_done);
                poll_result = l5sys_poll_result(h);
                if (h->shared_host_memory_ptr->poll_result != 0) {
                    PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_POLL_FAILED, poll_result, 0, "poll failed addr=0x%08x\n",poll_result); 
                    dump_queue(sys_handle,"initial stats","l5sys_abort_if_queue_errors",0);
                    /* clear the poll_result after acknowledging the error */
                    h->shared_host_memory_ptr->poll_result = 0;
                }
            }
        }
    }
    PMC_RETURN();
}


/*******************************************************************************
* pipe_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Write the value and the address into the FW SPRAM address for FW to execute
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the Address to read.
*   value - the value to write into the queue
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
static void pipe_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 value )
{
    UINT32 message[2];

    /* Set up a write message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_WRITE, reg_addr);
    message[0] = value;

    PMC_OS_STATS_QUEUE_WRITE();
    queue_message(sys_handle, message, 2);
}

/*******************************************************************************
* pipe_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Read the address - Put into the FW SPRAM address for FW to execute or read
*   directly from device 
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the Address to read.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The value read
*
* NOTES:
*******************************************************************************/
static UINT32 pipe_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr )
{

    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    PMC_OS_STATS_QUEUE_READ();
    UINT32 message[2];
    UINT32 result;

    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:pipe_read" );

    L5SYS_TRACE(8,"pipe_read\n");

    if (is_queue_empty(sys_handle)) {
        /* Queue is empty, do the read directly */
        result = pmc_sys_reg_read( sys_handle, reg_addr);
    } else {
        /* Queue is not empty.. We must send the read to FW*/
        /* Set up a read message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_READ, reg_addr);
        message[0] = 0;
        queue_message(sys_handle, message, 2);
        wait_queue_done(sys_handle,fw_queue_wait_done);
        /* Return the result that FW wrote to host memory */
        result = l5sys_read_result(h);
    }
#ifndef LOGGING_DISABLED
    if (h->logging_enabled) {
        char buffer[800];
        buffer[0] = 0;
        if (h->fw_get_description != NULL) {
            h->fw_get_description(h->queue_stall_callback_param,(l5sys_operation_code_definitions_enum)0x80000000,reg_addr,0xffffffff,result,0,buffer);
        }
        L5SYS_LOG(sys_handle,"READ result: addr=0x%08x %s\n",reg_addr,buffer);
    }
#endif
    return result;
}

/******************************************************************************
*  l5sys_bench_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*     measure the time to read an address - Put into the FW SPRAM address for 
*     FW to execute or read directly from device 
*
*  INPUTS:        
*                 *sys_handle - opaque customer's handle.
*                 reg_addr    - address of the register location to be read
*                               as it appears in the hardware data sheet
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*     UINT32 - duration in microseconds.
*
*  NOTES:
******************************************************************************/
static UINT32 pipe_bench_read( pmc_sys_handle_t *sys_handle, UINT32 reg_addr )
{

    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 message[2];

    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:pipe_bench_read" );

    /* Set up a bench message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_BENCH, reg_addr);
    message[0] = 0;
    queue_message(sys_handle, message, 2);
    wait_queue_done(sys_handle,fw_queue_wait_done);
	
    /* Return the result that FW wrote to host memory */
    return l5sys_read_result(h);
}

/*******************************************************************************
* pipe_read_modify_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Write the value and the address into the FW SPRAM address for FW to 
*   execute
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   reg_addr - the Address to read.
*   mask - the mask of bits to write.
*   value - the value to write into the queue
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
static void pipe_read_modify_write( pmc_sys_handle_t *sys_handle, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    UINT32 message[4];
    UINT32 first_bit_set;
    UINT32 short_rmw_message;

    first_bit_set = find_first_bit_set(mask);
    if (is_mask_less_12_bits(mask, first_bit_set)){

        PMC_OS_STATS_QUEUE_RMW_SHORT();
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_SHORT_RMW, reg_addr);
        short_rmw_message = first_bit_set | ((value >> first_bit_set) << 5) | ((mask >> first_bit_set) << 17);
        message[0] = short_rmw_message;
        queue_message(sys_handle, message, 2);

    } else{

        PMC_OS_STATS_QUEUE_RMW_LONG();
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_LONG_RMW, reg_addr);
        message[0] = mask;
        message[2] = value;
        message[3] = 0;
        queue_message(sys_handle, message, 4);
    }

}


/*******************************************************************************
*  pipe_poll
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*     Ask the FW queue to do a poll.
*
*  INPUTS:
*                 *hndl     - customer's opaque handle.
*                 addr      - address of the register location to be read
*                             as it appears in the hardware data sheet
*                 mask      - field being examined.
*                 value     - desired value of the field (note: this value
*                             must be pre-shifted)
*                 cmp       - type of comparison
*                 max_count - maximum number of times to read the register.
*                 delay_between_polls_in_microseconds - how long to wait between register reads.
*
*  OUTPUTS:
*                 *num_failed_polls - number of register reads that were
*                                     not the desired value.
*
*  RETURNS:
*    PMC_POLL_RETURN_TYPE -  PMC_SUCCESS
*                            PMC_ERR_POLL_TIMEOUT
*
*  NOTES:
*
*******************************************************************************/
static PMC_POLL_RETURN_TYPE pipe_poll(pmc_sys_handle_t *hndl, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    UINT32 message[6];
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    UINT32 timeout_in_microseconds;
    BOOL8 buffered_io_save;

    PMC_OS_STATS_QUEUE_POLL();
    /* Set up a POLL message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_POLL, addr);
    message[0] = mask;
    message[2] = value;
    message[3] = (cmp & 0xff) | (h->ignore_poll_errors << 8);

    /* add 2 to delay_between_polls_in_microseconds
       so that the timeout is the same with the queue as without the queue.
       This is because when not using the queue, a single register read takes a bit over
       2 microseconds which extends the total poll duration. 

       Note: a wait of 1 microsecond actually waits for the "next" microsecond,
       so a 1 microsecond delay between polls takes up the remainder of the 3 microseconds
       as long as the read takes more than 2 and less than 3 microseconds. 

       also note: the firmware's algorithm for polling just takes a timeout in microseconds,
       it does not try to emulate the "delay between polls".
    */
    timeout_in_microseconds = (delay_between_polls_in_microseconds+2)*max_count;
    message[4] = timeout_in_microseconds;
    message[5] = 0;


    /* force a buffer flush */
    buffered_io_save = h->buffered_io;
    h->buffered_io = FALSE;
    queue_message(hndl, message, 6);
    h->buffered_io = buffered_io_save;

    /* always return PMC_SUCCESS */
    return PMC_SUCCESS;
}



#ifndef LOGGING_DISABLED
/*******************************************************************************
* dump_message
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Display the message.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   index      - index into the queue
*   display_index - what index are we pretending to display (dump_message can display an old message when used with the 'playback' function).
*   prefix     - leading string for debug messages.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing
*
* NOTES:
*******************************************************************************/
static void dump_message(pmc_sys_handle_t *sys_handle, UINT32 index, UINT32 display_index, const char *prefix)
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    const UINT32 queue_size = L5SYS_HOST_QUEUE_SIZE_IN_WORDS;
#define QUEUE_DATA(x) h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer[(index+x)%queue_size]
    l5sys_operation_code_definitions_enum opcode = (l5sys_operation_code_definitions_enum)(UINT32)((QUEUE_DATA(1) & 0xfe000000) >> 25);
    UINT32 addr   = (QUEUE_DATA(1) & 0x01fffffc);
    const char *opcode_str;
    char buffer[800];

    buffer[0]=0;

    L5SYS_TRACE(8,"&msg[index] = %p\n",&QUEUE_DATA(1));

	switch(opcode) {
    case OP_CODE_WRITE     :
        opcode_str = "WRITE    ";
        {
            UINT32 mask = 0xffffffff;
            UINT32 value = QUEUE_DATA(0);

            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,addr,mask,value,0,buffer);
            }

            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x, value=0x%08x %s\n",opcode_str,addr,value,buffer);
            break;
        }
    case OP_CODE_BURST_WRITE     :
        opcode_str = "BURST_WRITE ";
        {
            UINT32 mask = 0xffffffff;
            UINT32 length = QUEUE_DATA(0);
            UINT32 i;

            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x, len=%d\n",opcode_str,addr,length);
            for (i=0;i<length;i++) {
                UINT32 value = QUEUE_DATA(i+2);

                if (h->fw_get_description != NULL) {
                    h->fw_get_description(h->queue_stall_callback_param,opcode,addr+i*4,mask,value,0,buffer);
                }

                L5SYS_LOG(sys_handle,"  value[%d]=0x%08x %s\n",i,value,buffer);
            }
            break;
        }

    case OP_CODE_SHORT_RMW :
        {
            UINT32 mask;
            UINT32 value;
            UINT32 shift = QUEUE_DATA(0) & 0x1f;
            value = ((QUEUE_DATA(0) & 0x0001ffe0) >> 5) << shift;
            mask  = ((QUEUE_DATA(0) & 0xfffe0000) >> 17) << shift;
            opcode_str = "SHORT_RMW"; 

            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,addr,mask,value,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x, mask=0x%08x, value=0x%08x %s\n",opcode_str,addr,mask,value,buffer);
            break;
        }
    case OP_CODE_LONG_RMW  :
        {
            UINT32 mask = QUEUE_DATA(0);
            UINT32 value = QUEUE_DATA(2);
            opcode_str = "LONG_RMW "; 

            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,addr,mask,value,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x, mask=0x%08x, value=0x%08x %s\n",opcode_str,addr,mask,value,buffer);
            break;
        }
    case OP_CODE_READ      :
        opcode_str = "READ     "; 

        if (h->fw_get_description != NULL) {
            h->fw_get_description(h->queue_stall_callback_param,opcode,addr,0xffffffff,0,0,buffer);
        }
        L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
        L5SYS_LOG(sys_handle,"%s addr=0x%08x %s\n",opcode_str,addr,buffer);
        break;
    case OP_CODE_POLL      :
        {
            UINT32 mask = QUEUE_DATA(0);
            UINT32 value = QUEUE_DATA(2);
            opcode_str = "POLL     "; 

            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,addr,mask,value,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x, mask=0x%08x, value=0x%08x, cmp=%d, timeout=%d %s\n",opcode_str,addr,mask,value,QUEUE_DATA(3),QUEUE_DATA(4),buffer);

        }
        break;
    case OP_CODE_SLEEP     :
        {
            UINT32 timeout = QUEUE_DATA(0);
            opcode_str = "SLEEP    "; 
            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,0,0,timeout,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s timeout=%d\n",opcode_str,timeout);
            break;
        }
    case OP_CODE_BENCH      :
        opcode_str = "BENCH    "; 
        L5SYS_LOG(sys_handle,"%s addr=0x%08x\n",opcode_str,addr);
        break;
    case OP_CODE_BURST_READ     :
        {
            opcode_str = "BURST_RD "; 

            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,addr,0xffffffff,0,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s addr=0x%08x len=%d offset=0x%0x %s\n",opcode_str,addr,QUEUE_DATA(2),QUEUE_DATA(0),buffer);

            break;
        }
    case OP_CODE_FIRMWARE_CMD      :
        {
            UINT32 cmd_len = addr / 4;
            UINT32 i;

            opcode_str = "FW_CMD   "; 
            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,QUEUE_DATA(2),QUEUE_DATA(3),0,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s",opcode_str);
            if (h->fw_get_function_name != NULL) {
                const char *mod_name;
                const char *sub_type;

                h->fw_get_function_name(QUEUE_DATA(2),QUEUE_DATA(3),&mod_name,&sub_type);
                L5SYS_LOG(sys_handle," %s %s",mod_name,sub_type);
            }
            L5SYS_LOG(sys_handle," len=%d",cmd_len);
            for (i=2;i<cmd_len;i++) {
                L5SYS_LOG(sys_handle," %x",QUEUE_DATA(i));
            }
            L5SYS_LOG(sys_handle,"\n");
            break;
        }
    case OP_CODE_FIRMWARE_QUERY      :
        {
            UINT32 cmd_len = addr / 4;
            UINT32 response_addr = QUEUE_DATA(0);
            UINT32 response_len = QUEUE_DATA(2);
            UINT32 i;

            opcode_str = "FW_QUERY ";
            if (h->fw_get_description != NULL) {
                h->fw_get_description(h->queue_stall_callback_param,opcode,QUEUE_DATA(3),QUEUE_DATA(4),0,0,buffer);
            }
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s",opcode_str);
            if (h->fw_get_function_name != NULL) {
                const char *mod_name;
                const char *sub_type;

                h->fw_get_function_name(QUEUE_DATA(3),QUEUE_DATA(4),&mod_name,&sub_type);
                L5SYS_LOG(sys_handle," %s %s",mod_name,sub_type);
            }
            L5SYS_LOG(sys_handle," len=%d response_addr=0x%08x response_len=%d",cmd_len,response_addr,response_len);
            for (i=3;i<cmd_len;i++) {
                L5SYS_LOG(sys_handle," %x",QUEUE_DATA(i));
            }
            L5SYS_LOG(sys_handle,"\n");
            break;
        }
    case OP_CODE_BATCH_READ      :
        {
            UINT32 cmd_len = addr / 4;
            UINT32 response_addr = QUEUE_DATA(0);
            UINT32 i;

            opcode_str = "BATCH_READ "; 
            L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
            L5SYS_LOG(sys_handle,"%s len=%d response_addr=0x%08x",opcode_str,cmd_len,response_addr);
            for (i=2;i<cmd_len;i++) {
                UINT32 reg_addr = QUEUE_DATA(i);
                L5SYS_LOG(sys_handle," 0x%x",reg_addr);
            }

            L5SYS_LOG(sys_handle,"\n");

            for (i=2;i<cmd_len;i++) {
                UINT32 reg_addr = QUEUE_DATA(i);
                if (h->fw_get_description != NULL) {
                    h->fw_get_description(h->queue_stall_callback_param,opcode,reg_addr,0xffffffff,0,0,buffer);
                    L5SYS_LOG(sys_handle," 0x%x - %s\n",reg_addr,buffer);
                }
            }
            break;
        }
    case OP_CODE_PADDING      :
        opcode_str = "PADDING  "; 
        L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
        L5SYS_LOG(sys_handle,"%s pad=%d\n",opcode_str,QUEUE_DATA(0));
        break;
    case OP_CODE_SELF_TEST      :
        opcode_str = "SELF_TST "; 
        L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
        L5SYS_LOG(sys_handle,"%s len=%d, pattern=0x%x\n",opcode_str,addr/4,QUEUE_DATA(0));
        break;
    case OP_CODE_CHECK_SUM      :
        opcode_str = "CHECK_SUM"; 
        L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
        L5SYS_LOG(sys_handle,"%s checksum=0x%x\n",opcode_str,QUEUE_DATA(0));
        break;
    default:
        L5SYS_LOG(sys_handle,"%s%x: ",prefix,display_index);
        L5SYS_LOG(sys_handle,"unknown command 0x%08x 0x%08x\n",QUEUE_DATA(1),QUEUE_DATA(0));
        break;
    }
}

#endif


/*******************************************************************************
* dump_queue
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Display the queue.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   where - used in debug message   
*   caller - used in debug message  
*   stall_time - stall delay in microseconds   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing
*
* NOTES:
*  This displays the contents of the firmware queue,
*  shows whether each message has been received or not.
*  it also displays a few specific registers,
*  and displays firmware statistics and resets firmware
*  statistics in case the queue remains stalled.
*
*  If the queue is stalled for another 10 seconds, then
*  the firmware statistics will show just the activity
*  in the last 10 seconds.
*
*  This function is just for debugging, a queue stall
*  should not happen on a production system.
*
*   firmware queue stall - wait_queue_done called by l5sys_check_queue_errors
*   displaying messages from index 0..43                                     
*   0: received   WRITE     addr=0x01801330, value=0x00000001               
*   2: received   READ      addr=0x018001a0                                 
*   4: received   WRITE     addr=0x018001a0, value=0x01020304               
*   6: received   READ      addr=0x018001a0                                 
*   8: received   WRITE     addr=0x018001a0, value=0xf0e0d0c0               
*   10: received   READ      addr=0x018001a0                                
*   12: received   WRITE     addr=0x018001a0, value=0xfeedcafe              
*   14: received   POLL      addr=0x01801230, mask=0x00000001, value=0x00000001, cmp=0, timeout=2
*   14: received   POLL      current value of addr=0x01801230 is value=0x00000001                
*   20: received   WRITE     addr=0x01dbfd00, value=0x83020003                                   
*   22: received   WRITE     addr=0x01dbfd04, value=0x01000002                                   
*   24: received   WRITE     addr=0x01dbfd08, value=0x02000000                                   
*   26: received   WRITE     addr=0x01801230, value=0x00000000                                   
*   ** INCONSISTENT SPRAM **   28: PENDING   unknown command 0xd00df00d 0xd00df00d                 
*   28: PENDING   POLL      addr=0x01801330, mask=0x0000ffff, value=0x00000001, cmp=0, timeout=3000000
*   28: PENDING   POLL      current value of addr=0x01801330 is value=0x00000000                      
*   34: received   WRITE     addr=0x01801330, value=0x00000001                                      
*   36: received   WRITE     addr=0x018001a0, value=0xfeedcafe                                      
*   38: received   POLL      addr=0x018001a0, mask=0xffffffff, value=0x12345678, cmp=0, timeout=140000000
*   38: received   POLL      current value of addr=0x018001a0 is value=0xfeedcafe                        
*   end of queue                                                                                          
*   firmware queue stall - wait_queue_done called by l5sys_check_queue_errors                             
*   driver_index = 38 (0x26)                                                                              
*   host_index   = 44 (0x2c)                                                                              
*   use_pipe     = TRUE                                                                                   
*   pipe_created     = TRUE                                                                               
*   queue_size(bytes) = 1024 (0x400)                                                                      
*   spram_start   = 0x01dbf800                                                                            
*   shared_host_memory_ptr = 0x0x7f167e8d3000                                                             
*   physical_addr = 0x2000000                                                                             
*   PMC_BIG_ENDIAN is not defined                                                                         
*   OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_0 addr=0x01809080 value=0x10000000                                 
*   OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_0 addr=0x01809090 value=0x00000000                                 
*   OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_0 addr=0x018090a0 value=0x00000000                          
*   OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_0 addr=0x018090b0 value=0x00000000                          
*   OUTBOUND_SIZE_MASK_LOWER_WINDOW_0 addr=0x018090c0 value=0x00000000                                    
*   OUTBOUND_SIZE_MASK_UPPER_WINDOW_0 addr=0x018090d0 value=0x00000000                                    
*   OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_1 addr=0x01809084 value=0x12000000                                 
*   OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_1 addr=0x01809094 value=0x00000000                                 
*   OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_1 addr=0x018090a4 value=0x02000000                          
*   OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_1 addr=0x018090b4 value=0x00000000                          
*   OUTBOUND_SIZE_MASK_LOWER_WINDOW_1 addr=0x018090c4 value=0x00000001                                    
*   OUTBOUND_SIZE_MASK_UPPER_WINDOW_1 addr=0x018090d4 value=0x00000000                                    
*   APP_SCRATCH_1 addr=0x01800188 value=0x9bd047a0                                                        
*   APP_SCRATCH_2 status addr=0x0180018c value=0x00000000                                                 
*   APP_SCRATCH_3 cause addr=0x01800190 value=0x00000000                                                  
*   APP_SCRATCH_4 EPC addr=0x01800194 value=0x00000000                                                    
*   APP_SCRATCH_5 return addr addr=0x01800198 value=0x00000000                                            
*   APP_SCRATCH_6 addr=0x0180019c value=0x00000000                                                        
*   APP_SCRATCH_7 addr=0x018001a0 value=0xfeedcafe                                                        
*   APP_USER0 addr=0x018001a4 value=0x00000000                                                            
*   APP_USER1 addr=0x018001a8 value=0x00000081                                                            
*   SOFT_RESET_BITS addr=0x01800000 value=0x00000000                                                      
*   previous host message was: MODID_OPSA_HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST                      
*   current host message is: MODID_OPSA_HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST                        
*   waiting for: firmware flush statistics                                                                
*   done: firmware flush statistics                                                                       
*   FIRMWARE: DIGI_0 number of statistics 66                                                              
*   FIRMWARE: DIGI_0 KERN_PER_TASK duration: n=4431, total=34.69 milliseconds, min=3.507, max=148.3, avg=7.829 usec and one in progress 
*   FIRMWARE: DIGI_0 KERN_PER_TASK period  : n=4430, total=1.108 seconds, min=248.3, max=251.6, avg=250.1 usec                          
*   FIRMWARE: DIGI_0 KERN_PER_TASK percent : 3.131%  
*   ...                                                                                   
*   FIRMWARE: DIGI_0 QUEUE_DELAYS duration: n=513773, total=547.2 milliseconds, min=1.023, max=150.3, avg=1.065 usec                     
*   FIRMWARE: DIGI_0 QUEUE_DELAYS period  : n=513772, total=1.108 seconds, min=1.917, max=151.4, avg=2.156 usec                          
*   FIRMWARE: DIGI_0 QUEUE_DELAYS percent : 49.39%                                                                                       
*   waiting for: firmware reset statistics                                                                                             
*   done: firmware reset statistics                                                                                                    
*                                                                                                                                      
*   /proj/digi_bby_sw/users/wattbyro/trunk/applib/pmc_shared/src/l5_sys.c:644: In function 'l5sys_check_queue_errors': 'PMC_FW_QUEUE: poll failed poll failed addr=0x018001a0                                                                                                                                                                   
*   firmware queue stall - wait_queue_done called by l5sys_abort_if_queue_errors                                                                                         
*   displaying messages from index 0..51                                                                                                                                 
*   0: received   WRITE     addr=0x01801330, value=0x00000001                                                                                                           
*   2: received   READ      addr=0x018001a0                                                                                                                             
*   4: received   WRITE     addr=0x018001a0, value=0x01020304                                                                                                           
*   6: received   READ      addr=0x018001a0                                                                                                                             
*   8: received   WRITE     addr=0x018001a0, value=0xf0e0d0c0                                                                                                           
*   10: received   READ      addr=0x018001a0                                                                                                                            
*   12: received   WRITE     addr=0x018001a0, value=0xfeedcafe                                                                                                          
*   14: received   POLL      addr=0x01801230, mask=0x00000001, value=0x00000001, cmp=0, timeout=2                                                                       
*   14: received   POLL      current value of addr=0x01801230 is value=0x00000001                                                                                       
*   20: received   WRITE     addr=0x01dbfd00, value=0x83020003                                                                                                          
*   22: received   WRITE     addr=0x01dbfd04, value=0x01000002                                                                                                          
*   24: received   WRITE     addr=0x01dbfd08, value=0x02000000                                                                                                          
*   26: received   WRITE     addr=0x01801230, value=0x00000000                                                                                                          
*   ** INCONSISTENT SPRAM **   28: PENDING   unknown command 0xd00df00d 0xd00df00d                                                                                        
*   28: PENDING   POLL      addr=0x01801330, mask=0x0000ffff, value=0x00000001, cmp=0, timeout=3000000                                                                    
*   28: PENDING   POLL      current value of addr=0x01801330 is value=0x00000000                                                                                          
*   34: received   WRITE     addr=0x01801330, value=0x00000001                                                                                                          
*   36: received   WRITE     addr=0x018001a0, value=0xfeedcafe                                                                                                          
*   38: received   POLL      addr=0x018001a0, mask=0xffffffff, value=0x12345678, cmp=0, timeout=140000000                                                               
*   38: received   POLL      current value of addr=0x018001a0 is value=0xfeedcafe                                                                                       
*   44: received   WRITE     addr=0x018001a0, value=0xfeedcafe                                                                                                          
*   46: received   POLL      addr=0x018001a0, mask=0xffffffff, value=0x12345678, cmp=0, timeout=4000000                                                                 
*   46: received   POLL      current value of addr=0x018001a0 is value=0xfeedcafe                                                                                       
*   end of queue                                                                                                                                                         
*   firmware queue stall - wait_queue_done called by l5sys_abort_if_queue_errors                                                                                         
*   driver_index = 46 (0x2e)                                                                                                                                             
*   host_index   = 52 (0x34)                                                                                                                                             
*   use_pipe     = TRUE                                                                                                                                                  
*   pipe_created     = TRUE                                                                                                                                              
*   queue_size(bytes) = 1024 (0x400)                                                                                                                                     
*   spram_start   = 0x01dbf800                                                                                                                                           
*   shared_host_memory_ptr = 0x0x7f167e8d3000                                                                                                                            
*   physical_addr = 0x2000000                                                                                                                                            
*   PMC_BIG_ENDIAN is not defined                                                                                                                                        
*   OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_0 addr=0x01809080 value=0x10000000                                                                                                
*   OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_0 addr=0x01809090 value=0x00000000                                                                                                
*   OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_0 addr=0x018090a0 value=0x00000000                                                                                         
*   OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_0 addr=0x018090b0 value=0x00000000                                                                                         
*   OUTBOUND_SIZE_MASK_LOWER_WINDOW_0 addr=0x018090c0 value=0x00000000                                                                                                   
*   OUTBOUND_SIZE_MASK_UPPER_WINDOW_0 addr=0x018090d0 value=0x00000000                                                                                                   
*   OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_1 addr=0x01809084 value=0x12000000                                                                                                
*   OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_1 addr=0x01809094 value=0x00000000                                                                                                
*   OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_1 addr=0x018090a4 value=0x02000000                                                                                         
*   OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_1 addr=0x018090b4 value=0x00000000                                                                                         
*   OUTBOUND_SIZE_MASK_LOWER_WINDOW_1 addr=0x018090c4 value=0x00000001                                                                                                   
*   OUTBOUND_SIZE_MASK_UPPER_WINDOW_1 addr=0x018090d4 value=0x00000000                                                                                                   
*   APP_SCRATCH_1 addr=0x01800188 value=0x9bd047a0                                                                                                                       
*   APP_SCRATCH_2 status addr=0x0180018c value=0x00000000                                                                                                                
*   APP_SCRATCH_3 cause addr=0x01800190 value=0x00000000                                                                                                                 
*   APP_SCRATCH_4 EPC addr=0x01800194 value=0x00000000                                                                                                                   
*   APP_SCRATCH_5 return addr addr=0x01800198 value=0x00000000                                                                                                           
*   APP_SCRATCH_6 addr=0x0180019c value=0x00000000                                                                                                                       
*   APP_SCRATCH_7 addr=0x018001a0 value=0xfeedcafe                                                                                                                       
*   APP_USER0 addr=0x018001a4 value=0x00000000                                                                                                                           
*   APP_USER1 addr=0x018001a8 value=0x00000081                                                                                                                           
*   SOFT_RESET_BITS addr=0x01800000 value=0x00000000                                                                                                                     
*   previous host message was: MODID_OPSA_HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST                                                                                     
*   current host message is: MODID_OPSA_HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST                                                                                       
*   waiting for: firmware flush statistics                                                                                                                               
*   done: firmware flush statistics                                                                                                                                      
*   FIRMWARE: DIGI_0 number of statistics 66                                                                                                                             
*   FIRMWARE: DIGI_0 KERN_PER_TASK duration: n=44000, total=343.9 milliseconds, min=3.623, max=147.9, avg=7.815 usec and one in progress                                   
*   FIRMWARE: DIGI_0 KERN_PER_TASK period  : n=43999, total=11.00 seconds, min=249.6, max=250.4, avg=250.0 usec                                                            
*   FIRMWARE: DIGI_0 KERN_PER_TASK percent : 3.126%         
*   ...
*   FIRMWARE: DIGI_0 QUEUE_DELAYS duration: n=5086818, total=5.412 seconds, min=1.023, max=149.9, avg=1.064 usec and one in progress
*   FIRMWARE: DIGI_0 QUEUE_DELAYS period  : n=5086817, total=11.00 seconds, min=1.933, max=151.0, avg=2.162 usec
*   FIRMWARE: DIGI_0 QUEUE_DELAYS percent : 49.20%
*   waiting for: firmware reset statistics
*   done: firmware reset statistics
* 
*******************************************************************************/
static void dump_queue(pmc_sys_handle_t *sys_handle,const char *where,const char *caller,UINT64 stall_time)
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
#ifndef LOGGING_DISABLED
    const UINT32 queue_size = L5SYS_HOST_QUEUE_SIZE_IN_WORDS;
    const UINT32 max_messages_to_display = L5SYS_MAX_MESSAGES_TO_DISPLAY;
    UINT32 i;
    UINT32 num_messages = 0;
    UINT32 min_index;

    if (h->pmcfw_dma_queue_index.index < queue_size) {
        min_index = 0;
    } else {
        min_index = h->pmcfw_dma_queue_index.index - queue_size;
    }
    if (min_index < h->pmcfw_dma_queue_index.last_index_displayed) {
        min_index = h->pmcfw_dma_queue_index.last_index_displayed;
    }

    if (max_messages_to_display > 0) {
        /* decrement the index until we see 'max_messages_to_display' commands */
        for (i=h->pmcfw_dma_queue_index.index-1;(i>min_index) && (num_messages < max_messages_to_display);i--) {
            if (h->queue_history_is_cmd[(i%queue_size)/32] & (1 << (i%32))) {
                num_messages ++;
            }
        }
    } else {
        i = min_index;
    }

	if (stall_time > 0) {
        PMC_PRINT("start of queue dump: %s called by %s (%d seconds)\n",where,caller,(UINT32)(stall_time/1000000));
    } else {
        PMC_PRINT("start of queue dump: %s called by %s\n",where,caller);
    }
    PMC_PRINT("displaying messages from index 0x%x..0x%x\n",i,h->pmcfw_dma_queue_index.index-1);
    
    for (;i<h->pmcfw_dma_queue_index.index;i++) {
        if (h->queue_history_is_cmd[(i%queue_size)/32] & (1 << (i%32))) {
            dump_message(sys_handle,i,i,"  dump: ");
            h->pmcfw_dma_queue_index.last_index_displayed = i;
        }
    }

    PMC_PRINT("end of queue\n");
#endif

	PMC_PRINT("queue_dump: %s called by %s\n",where,caller);
    PMC_PRINT("use_pipe      = %s\n",h->use_pipe?"TRUE":"FALSE");
    PMC_PRINT("pipe_created  = %s\n",h->pipe_created?"TRUE":"FALSE");
    PMC_PRINT("physical_addr = 0x%llx\n",h->physical_address);
    PMC_PRINT("host_index   = 0x%08x\n",h->pmcfw_dma_queue_index.index);
    PMC_PRINT("driver_index = 0x%08x\n",h->shared_host_memory_ptr->dma_queue_fw_index);
    PMC_PRINT("\n");
#ifdef PMC_BIG_ENDIAN
PMC_PRINT("PMC_BIG_ENDIAN is defined\n");
#else
PMC_PRINT("PMC_BIG_ENDIAN is not defined\n");
#endif

    l5sys_process_id_check(sys_handle);
#ifndef LOGGING_DISABLED
    if (h->queue_stall_callback_function != NULL)
    {
        h->queue_stall_callback_function(h->queue_stall_callback_param,TRUE);
    }
#endif

    PMC_PRINT("\n");
	if (stall_time > 0) {
        PMC_PRINT("end of queue dump: %s called by %s (%d seconds)\n",where,caller,(UINT32)(stall_time/1000000));
    } else {
        PMC_PRINT("end of queue dump: %s called by %s\n",where,caller);
    }
	fflush(stdout);
}

/*******************************************************************************
* l5sys_display_queue
* ______________________________________________________________________________
*
* DESCRIPTION:
*   wait for the firmware io queue to be complete and ASSERT if any
*   poll operation generated an error.
*   
* INPUTS:
*   handle - any pmc_handle
*   where  - why is this being called
*   caller - function name of the caller.
*
* OUTPUTS:
*   none
*   
* RETURNS:
*   PMC_SUCCESS or PMC_ERR_POLL_FAILED
*
*******************************************************************************/
PUBLIC void l5sys_display_queue(void *handle,const char *where, const char *caller)
{
    PMC_ENTRY();
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            if (h->use_pipe )
            {
                dump_queue(sys_handle,where,caller,0);
            }
        }
    }
    PMC_RETURN();
}


/*******************************************************************************
* wait_needed
* ___________________________________________________________________________
*
* DESCRIPTION:
*   a quick inline check to see if there is enough space in the queue.
*
* INPUTS:
*   h - l5pmc_sys_handle_t
*   wait_index - wait until driver_index to be greater than or equal to wait_index
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE -  should call wait_for_firmware, FALSE - there is enough space in the queue already.
*
* NOTES:
*******************************************************************************/
static INLINE BOOL8 wait_needed(l5pmc_sys_handle_t *h, UINT32 wait_index)
{
    if ((INT32)(wait_index - h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index) > 0)
        return TRUE;
    return FALSE;
}

#ifdef L5SYS_PROFILING
/*******************************************************************************
* check_if_active_operations_have_completed
* ___________________________________________________________________________
*
* DESCRIPTION:
*   used to measure timing, this function checks if the active operation has completed.
*
* INPUTS:
*   h - pointer to l5sys_pmc_sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
static void check_if_active_operations_have_completed(l5pmc_sys_handle_t *h)
{
    if (h->active->index < h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index) {
        UINT64 now = now_usec();
        while (h->active != NULL && ((h->active->started_timestamp == 0) || h->active->index < h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index)) {
            h->active->done_timestamp = now;
            h->active = h->active->next;
        }
    }
}
#endif

/*******************************************************************************
* check_pcie_alive
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Check if pcie is alive via accessing a device scratch register.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if pcie is working normally, otherwise FALSE.
*
*******************************************************************************/
static BOOL8 check_pcie_alive(pmc_sys_handle_t *sys_handle)
{
    UINT32 addr_APP_USER_0 = 0x18001a4;
    UINT32 val_APP_USER_0;
    UINT32 wr_val, rd_val;
    BOOL8 is_pcie_working = TRUE;

    val_APP_USER_0 = pmc_sys_reg_read(sys_handle,addr_APP_USER_0);

    wr_val = ~val_APP_USER_0;
    (void)pmc_sys_reg_write(sys_handle, addr_APP_USER_0, wr_val);
    rd_val = pmc_sys_reg_read(sys_handle, addr_APP_USER_0);
    is_pcie_working = (rd_val==wr_val)?TRUE:FALSE;

    /* write the origin value back */
    (void)pmc_sys_reg_write(sys_handle, addr_APP_USER_0, val_APP_USER_0);

    return is_pcie_working;
}

/*******************************************************************************
* wait_for_firmware
* ___________________________________________________________________________
*
* DESCRIPTION:
*   wait until firmware queue is more than the specified index
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   wait_index - wait until driver_index to be greater than or equal to wait_index
*   stat       - what statistic to update for delays.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
static void wait_for_firmware( pmc_sys_handle_t *sys_handle, UINT32 wait_index, pmc_sys_stat_t *stat )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 wait_count = 0;
    UINT32 wait_needed = FALSE;
    BOOL8 queue_stalled = FALSE; /* has a long time passed */
    UINT64 total_wait_time_in_usec = 0;
    UINT32 start_firmware_index = 0;
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    UINT64 debug_delay = 1000*1000*60; /* display queue stall information after 1 minute if using L5SYS_PROFILING */
#else
#ifdef PMC_SW_SIMULATION
    UINT64 debug_delay = 1000*1000*5; /* how long to wait before displaying debug information (in microseconds) */
#else
    UINT64 debug_delay = 1000*1000; /* how long to wait before displaying debug information (in microseconds) */
#endif
#endif
#else 
    UINT64 debug_delay = 1000*1000*5; /* how long to wait before displaying debug information (in microseconds) */
#endif

#if (L5SYS_TRACE_LEVEL >= 5)
#ifndef PMC_SW_SIMULATION
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        h->queue_stall_callback_function(h->queue_stall_callback_param,FALSE);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
#endif
#endif
    L5SYS_TRACE(8,"wait_for_firmware %d, %d\n",wait_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
    /* e.g. break if fw read index is greater than or equal to the wait index */
    while ((INT32)(wait_index - h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index) > 0)
    {

        wait_count++;
        if (wait_count == 2) {
            if (!wait_needed) {
                wait_needed = TRUE;
                pmc_sys_stat_mark_start(stat);
            }
        }

         /* if the queue is not empty, do an OS sleep to let other processes run */
        if (wait_count > 2) {
            UINT32 estimated_delay = (wait_index - h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index) / 20;
            if (estimated_delay > 0) {
                PMC_OS_USLEEP(estimated_delay);
                total_wait_time_in_usec += estimated_delay;
            }
        }
        
        /* if a queue stall was indicated on the previous loop 
            and this loop still indicates that the firmware is still stalled,
            then display the debug information */ 
        if (queue_stalled) {
            UINT64 total_elapsed_time = pmc_sys_stat_running_time(stat);
            pmc_sys_timeval_t now;
            PMC_TIMEVAL_READ(&now);
            PMC_PRINT_NO_INDENT("queue stall in wait_for_firmware\n");
            PMC_PRINT_NO_INDENT("queue stall now.sec    = %d\n",now.tv_sec);
            PMC_PRINT_NO_INDENT("queue stall start.sec  = %d\n",stat->mark.tv_sec);
            PMC_PRINT_NO_INDENT("queue stall now.usec   = %d\n",now.tv_usec);
            PMC_PRINT_NO_INDENT("queue stall start.usec = %d\n",stat->mark.tv_usec);
            PMC_PRINT_NO_INDENT("queue stall wait_index           = 0x%x (%d)\n",wait_index,wait_index);
            PMC_PRINT_NO_INDENT("queue stall now_fw_index         = 0x%x (%d)\n",h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
            PMC_PRINT_NO_INDENT("queue stall start_firmware_index = 0x%x (%d)\n",start_firmware_index,start_firmware_index);
            PMC_PRINT_NO_INDENT("queue stall total_wait_time passed to PMC_OS_USLEEP = %lld usec\n",total_wait_time_in_usec);
            PMC_PRINT_NO_INDENT("queue stall total_elapsed_time = %lld usec\n",total_elapsed_time);
            PMC_PRINT_NO_INDENT("queue stall number of loops = %d\n",wait_count);
 
            debug_delay *= 10;
            if (debug_delay >= 120 * 1000 * 1000) {
                debug_delay = 120 * 1000 * 1000;
            }
            queue_stalled = FALSE;
            if (total_elapsed_time > 120 * 1000 * 1000) { /* about 2 minutes */
#ifndef LOGGING_DISABLED   
                dump_queue(sys_handle,"firmware queue stall - queue_message","queue_message",pmc_sys_stat_running_time(stat));
#endif
                PMC_PRINT_NO_INDENT("wait_for_firmware():PMC_ASSERT(FALSE,PMC_SHARED_ERR_QUEUE_STALL)\n");
                PMC_ASSERT(FALSE,PMC_SHARED_ERR_QUEUE_STALL,wait_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
            }
        }
        if (wait_needed) {
            UINT64 total_elapsed_time = pmc_sys_stat_running_time(stat);
            if (total_elapsed_time > debug_delay) {
                queue_stalled = TRUE; /* generate a queue stall if the next read of of the firmware queue index is still not acceptable */
            }
        }

        if (wait_needed && pmc_sys_stat_running_time(stat)>10*1000) {
            /* maybe pcie is not working, so check it */
            if (!check_pcie_alive(sys_handle)) {
                PMC_PRINT_NO_INDENT("wait_for_firmware():PMC_ASSERT( FALSE, PMC_SHARED_ERR_PCIE_FAILURE)\n");
                PMC_ASSERT( FALSE, PMC_SHARED_ERR_PCIE_FAILURE, 0, 0 );
            }
        }

        h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index = l5sys_dma_queue_fw_index(h);
#ifdef L5SYS_PROFILING
        if (h->active != NULL) {
            check_if_active_operations_have_completed(h);
        }
#endif

        if (labs((INT32)(wait_index - h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index)) > L5SYS_HOST_QUEUE_SIZE_IN_WORDS ) {
            UINT64 total_elapsed_time = pmc_sys_stat_running_time(stat);
            pmc_sys_timeval_t now;
            PMC_TIMEVAL_READ(&now);
            PMC_PRINT_NO_INDENT("large variance in wait_for_firmware\n");
            PMC_PRINT_NO_INDENT("large variance now.sec    = %d\n",now.tv_sec);
            PMC_PRINT_NO_INDENT("large variance start.sec  = %d\n",stat->mark.tv_sec);
            PMC_PRINT_NO_INDENT("large variance now.usec   = %d\n",now.tv_usec);
            PMC_PRINT_NO_INDENT("large variance start.usec = %d\n",stat->mark.tv_usec);
            PMC_PRINT_NO_INDENT("large variance wait_index           = 0x%x (%d)\n",wait_index,wait_index);
            PMC_PRINT_NO_INDENT("large variance now_fw_index         = 0x%x (%d)\n",h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
            PMC_PRINT_NO_INDENT("large variance start_firmware_index = 0x%x (%d)\n",start_firmware_index,start_firmware_index);
            PMC_PRINT_NO_INDENT("large variance total_wait_time passed to PMC_OS_USLEEP = %lld usec\n",total_wait_time_in_usec);
            PMC_PRINT_NO_INDENT("large variance total_elapsed_time = %lld usec\n",total_elapsed_time);
            PMC_PRINT_NO_INDENT("large variance number of loops = %d\n",wait_count);
            
            PMC_PRINT("large variance in firmware index and host index: driver_index=0x%x (%d), wait_index=0x%x (%d), delta=%d\n",
                      h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,
                      wait_index,wait_index,
                      wait_index-h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
            dump_queue(sys_handle,"weird driver_index",stat->description,0);
            PMC_PRINT("large variance in firmware index and host index: driver_index=0x%x (%d), wait_index=0x%x (%d), delta=%d\n",
                      h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index,
                      wait_index,wait_index,
                      wait_index-h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
            PMC_PRINT_NO_INDENT("wait_for_firmware():PMC_ASSERT(0,PMC_ERR_INVALID_PARAMETERS)\n");
            PMC_ASSERT(0,PMC_ERR_INVALID_PARAMETERS,wait_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
        }
#if (L5SYS_TRACE_LEVEL >= 5)
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        h->queue_stall_callback_function(h->queue_stall_callback_param,FALSE);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
#endif
    }
    if (wait_needed) {
        pmc_sys_stat_mark_end(stat);
    }
    L5SYS_TRACE(8,"wait_for_firmware %d, %d\n",wait_index,h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index);
}

/*******************************************************************************
* queue_message
* ___________________________________________________________________________
*
* DESCRIPTION:
*   add a message to the host queue.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   msg - message to send
*   msg_size - length of the message (must be even)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - index of message in the host queue.
*
* NOTES:
*******************************************************************************/
static UINT32 queue_message( pmc_sys_handle_t *sys_handle, UINT32 * msg, UINT32 msg_size )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 index = h->pmcfw_dma_queue_index.index;
    UINT32 total_message_size = msg_size; /* round up to next multiple of 2 */

    UINT32 wait_index = index + total_message_size - L5SYS_HOST_QUEUE_SIZE_IN_WORDS;
    UINT32 msg_index = index;
    UINT32 flushed = FALSE; /* just used for timing */

    /** Stat for how long we wait for space in the queue */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_for_space1, "fw_queue_wait_for_space1" );

    msg_index = msg_index; /* possibly unused if logging_disabled */


    /* wait for driver index to be at least 'wait_index' before continuing */
    if (wait_needed(h,wait_index)) {
        wait_for_firmware(sys_handle,wait_index,fw_queue_wait_for_space1);
    }
#ifndef LOGGING_DISABLED
    if (h->queue_history_is_cmd == NULL)
    {
        h->queue_history_is_cmd = (UINT32 *)PMC_CALLOC(L5SYS_HOST_QUEUE_SIZE_IN_WORDS/32 * sizeof(UINT32)); /* 1 bit per word */
    }
#endif
    {
        UINT32 i;
        UINT32 *circular_buffer = (UINT32 *)h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer;
#ifndef LOGGING_DISABLED
        {
            UINT32 temp_index = index;

            for (i=0;i<total_message_size;i++,temp_index++)
            {
                L5SYS_TRACE(8,"[%d] = 0x%08x\n",temp_index,msg[i]);
                if (i==0) {
                    /* set the "start of command" bit */
                    h->queue_history_is_cmd[(temp_index% L5SYS_HOST_QUEUE_SIZE_IN_WORDS)/32] |= (1 << (temp_index%32));
                } else {
                    /* clear the "start of command" bit */
                    h->queue_history_is_cmd[(temp_index% L5SYS_HOST_QUEUE_SIZE_IN_WORDS)/32] &= ~(1 << (temp_index%32));
                }
            }
        }
#endif
#if defined(PMC_BIG_ENDIAN)
        for (i=0;i<msg_size;i++,index++)
        {
            circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS] = PMC_LE_TO_CPU_32(msg[i]);
            INCREMENT_CHECKSUM(h,msg[i]);
        }
#else
        /* use 64 bit writes to memory for speed */
        for (i=0;i<msg_size;i+=2,index+=2) {
            *(UINT64*)(&(circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS])) = *(UINT64*)(&msg[i]);
            INCREMENT_CHECKSUM(h,msg[i] + msg[i+1]);
        }
#endif
    }
    /* if we are using 'buffered_io', then only update the queue index if we've past a 'flush' boundary
     * this reduces the number of unnecessary pcie writes and increases the 'batching' performed by DMA.
     *
     * note: the index is also written in the wait_for_firmware().
     */
    if ((h->pmcfw_dma_queue_index.index >> h->pmcfw_dma_queue_index.flush_size ) != (index >> h->pmcfw_dma_queue_index.flush_size)
        || !(h->buffered_io) ) {
        /* write new index to spram */
        h->pmcfw_dma_queue_index.last_index_written_to_spram = index;
        pmc_sys_reg_write(sys_handle,h->pmcfw_dma_queue_index.spram_address_of_host_index,index);
        if (h->pmcfw_dma_queue_index.flush_size < MAX_FLUSH_SIZE) {
            h->pmcfw_dma_queue_index.flush_size++;
        }
        flushed = TRUE;
    }
    h->pmcfw_dma_queue_index.index = index;

#if (L5SYS_TRACE_LEVEL >= 8)
    L5SYS_TRACE(8,"%s &msg[index] = %p\n",__FUNCTION__,&h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer[(msg_index + 1) % L5SYS_HOST_QUEUE_SIZE_IN_WORDS]);
    dump_message(sys_handle,msg_index,msg_index,"  queue_message: ");
#endif

#ifndef LOGGING_DISABLED
    {
#ifdef L5SYS_PROFILING
        l5sys_operation_code_definitions_enum opcode = (l5sys_operation_code_definitions_enum)(UINT32)((msg[1] & 0xfe000000) >> 25);
#endif
        if (h->logging_enabled) {
            UINT32 i;
            L5SYS_LOG( sys_handle, "Q: %d", total_message_size );
            for (i=0;i<msg_size;i++) {
                L5SYS_LOG( sys_handle, " %08x", msg[i] );
            }
            L5SYS_LOG( sys_handle, "\n" );
            dump_message(sys_handle,msg_index,msg_index,"L5SYS: ");
        }
#ifdef L5SYS_PROFILING
        if (opcode != OP_CODE_CHECK_SUM) {
            if (h->link != NULL) {
                h->link->next = add_message(msg,msg_size,NULL,0);
                h->link = h->link->next;
                h->link->started_timestamp = now_usec();
                h->link->index = msg_index;
                if (h->active == NULL) {
                    h->active = h->link;
                }
            }
        }
        if (flushed) {
            if (h->active != NULL) {
                /* note: this read from shared memory adds 100 nanoseconds */
                h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index = l5sys_dma_queue_fw_index(h);
                check_if_active_operations_have_completed(h);
            }
        }
#endif
    }
#endif
#ifdef L5SYS_ADD_CHECKSUM_AFTER_EVERY_FLUSH
    if (flushed) {
        l5sys_operation_code_definitions_enum opcode = (l5sys_operation_code_definitions_enum)(UINT32)((msg[1] & 0xfe000000) >> 25);
        if (opcode != OP_CODE_CHECK_SUM) {
            pipe_check_sum(sys_handle); /* also send a checksum message */
        }
    }
#endif
    return msg_index;
}

/*******************************************************************************
* queue_message_and_data
* ___________________________________________________________________________
*
* DESCRIPTION:
*   add a message with auxillary data to the host queue.
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   msg - message to send
*   msg_size - length of the message (must be even)
*   data - extra words in message
*   data_size - length of extra words in the message
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - index of message in the host queue.
*
* NOTES:
*******************************************************************************/
static UINT32 queue_message_and_data( pmc_sys_handle_t *sys_handle, UINT32 * msg, UINT32 msg_size, UINT32 *data, UINT32 data_size )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 index = h->pmcfw_dma_queue_index.index;
    UINT32 total_message_size = msg_size + data_size + (data_size & 1); /* round up to next multiple of 2 */

    UINT32 wait_index = index + total_message_size - L5SYS_HOST_QUEUE_SIZE_IN_WORDS;
    UINT32 msg_index = index;
    UINT32 flushed = FALSE; /* just used for timing */

    PMC_ENTRY();

    /** Stat for how long we wait for space in the queue */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_for_space2, "fw_queue_wait_for_space2" );

    msg_index = msg_index; /* possibly unused if logging_disabled */

    /* wait for driver index to be at least 'wait_index' before continuing */
    if (wait_needed(h,wait_index)) {
        wait_for_firmware(sys_handle,wait_index,fw_queue_wait_for_space2);
    }
#ifndef LOGGING_DISABLED
    if (h->queue_history_is_cmd == NULL)
    {
        h->queue_history_is_cmd = (UINT32 *)PMC_CALLOC(L5SYS_HOST_QUEUE_SIZE_IN_WORDS/32 * sizeof(UINT32)); /* 1 bit per word */
    }
#endif
    {
        UINT32 i;
        UINT32 *circular_buffer = (UINT32 *)h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer;
#ifndef LOGGING_DISABLED
        {
            UINT32 temp_index = index;

            for (i=0;i<total_message_size;i++,temp_index++)
            {
                L5SYS_TRACE(8,"[%d] = 0x%08x\n",temp_index,msg[i]);
                if (i==0) {
                    /* set the "start of command" bit */
                    h->queue_history_is_cmd[(temp_index% L5SYS_HOST_QUEUE_SIZE_IN_WORDS)/32] |= (1 << (temp_index%32));
                } else {
                    /* clear the "start of command" bit */
                    h->queue_history_is_cmd[(temp_index% L5SYS_HOST_QUEUE_SIZE_IN_WORDS)/32] &= ~(1 << (temp_index%32));
                }
            }
        }
#endif
#if defined(PMC_BIG_ENDIAN)
        for (i=0;i<msg_size;i++,index++)
        {
            circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS] = PMC_LE_TO_CPU_32(msg[i]);
            INCREMENT_CHECKSUM(h,msg[i]);
        }
        for (i=0;i<data_size;i++,index++)
        {
            circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS] = PMC_LE_TO_CPU_32(data[i]);
            INCREMENT_CHECKSUM(h,data[i]);
        }
        for (i=0;i<(data_size & 1);i++,index++)
        {
            circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS] = 0;
        }
#else
        /* use 64 bit writes to memory for speed */
        for (i=0;i<msg_size;i+=2,index+=2) {
            *(UINT64*)(&(circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS])) = *(UINT64*)(&msg[i]);
            INCREMENT_CHECKSUM(h,msg[i] + msg[i+1]);
        }

        /* use 64 bit writes to memory for speed */
        for (i=0;i<data_size-1;i+=2,index+=2) {
            *(UINT64*)(&(circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS])) = *(UINT64*)(&data[i]);
            INCREMENT_CHECKSUM(h,data[i] + data[i+1]);
        }
        /* write last 32 bit word, and a zero */
        if (data_size & 1)
        {
            UINT32 last_two_words[2];
            last_two_words[0]=data[data_size-1];
            last_two_words[1]=0;
            *(UINT64*)(&(circular_buffer[index % L5SYS_HOST_QUEUE_SIZE_IN_WORDS])) = *(UINT64*)(&last_two_words[0]);
            INCREMENT_CHECKSUM(h,last_two_words[0]);
            index+=2;
        }
#endif
    }
    /* if we are using 'buffered_io', then only update the queue index if we've past a 'flush' boundary
     * this reduces the number of unnecessary pcie writes and increases the 'batching' performed by DMA.
     *
     * note: the index is also written in the wait_for_firmware().
     */
    if ((h->pmcfw_dma_queue_index.index >> h->pmcfw_dma_queue_index.flush_size ) != (index >> h->pmcfw_dma_queue_index.flush_size)
        || !(h->buffered_io) ) {
        /* write new index to spram */
        h->pmcfw_dma_queue_index.last_index_written_to_spram = index;
        pmc_sys_reg_write(sys_handle,h->pmcfw_dma_queue_index.spram_address_of_host_index,index);
        if (h->pmcfw_dma_queue_index.flush_size < MAX_FLUSH_SIZE) {
            h->pmcfw_dma_queue_index.flush_size++;
        }
        flushed = TRUE;
    }
    h->pmcfw_dma_queue_index.index = index;

#if (L5SYS_TRACE_LEVEL >= 8)
    L5SYS_TRACE(8,"%s &msg[index] = %p\n",__FUNCTION__,&h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer[(msg_index + 1) % L5SYS_HOST_QUEUE_SIZE_IN_WORDS]);
    dump_message(sys_handle,msg_index,msg_index,"  queue_message: ");
#endif

#ifndef LOGGING_DISABLED
    if (h->logging_enabled) {
        UINT32 i;
        L5SYS_LOG( sys_handle, "Q: %d", total_message_size );
        for (i=0;i<msg_size;i++) {
            L5SYS_LOG( sys_handle, " %08x", msg[i] );
        }
        for (i=0;i<data_size;i++) {
            L5SYS_LOG( sys_handle, " %08x", data[i] );
        }
        for (i=0;i<(data_size&1);i++) {
            L5SYS_LOG( sys_handle, " 00000000" );
        }
        L5SYS_LOG( sys_handle, "\n" );
        dump_message(sys_handle,msg_index,msg_index,"L5SYS: ");
    }
#ifdef L5SYS_PROFILING
    if (h->link != NULL) {
        h->link->next = add_message(msg,msg_size,data,data_size);
        h->link = h->link->next;
        h->link->started_timestamp = now_usec();
        h->link->index = msg_index;
        if (h->active == NULL) {
            h->active = h->link;
        }
    }
    if (flushed) {
        if (h->active != NULL) {
            /* note: this read from shared memory adds 100 nanoseconds */
            h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index = l5sys_dma_queue_fw_index(h);
            check_if_active_operations_have_completed(h);
        }
    }
#endif
#endif
#ifdef L5SYS_ADD_CHECKSUM_AFTER_EVERY_FLUSH
    if (flushed) {
        l5sys_operation_code_definitions_enum opcode = (l5sys_operation_code_definitions_enum)(UINT32)((msg[1] & 0xfe000000) >> 25);
        if (opcode != OP_CODE_CHECK_SUM) {
            pipe_check_sum(sys_handle); /* also send a checksum message */
        }
    }
#endif
    PMC_RETURN(msg_index);
}

/*******************************************************************************
* is_queue_empty
* ___________________________________________________________________________
*
* DESCRIPTION:
*   check if the firmware queue is empty
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE if the queue is empty.
*
* NOTES:
*******************************************************************************/
static BOOL is_queue_empty( pmc_sys_handle_t *sys_handle )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

    if (h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index == h->pmcfw_dma_queue_index.index) {
        L5SYS_TRACE(9,"is_queue_empty == TRUE\n");
        return TRUE;
    } else {
        return FALSE;
    }

}


/*******************************************************************************
* wait_queue_done
* ___________________________________________________________________________
*
* DESCRIPTION:
*   wait for the firmware queue to be empty 
*
* INPUTS:
*   sys_handle - pointer to the sys_handle
*   stat       - what statistic to update for delays.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
static void wait_queue_done( pmc_sys_handle_t *sys_handle, pmc_sys_stat_t *stat )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 wait_index = h->pmcfw_dma_queue_index.index;

    L5SYS_TRACE(10,"wait_queue_done %d\n",wait_index);
#ifdef L5SYS_PROFILING
    if (h->logging_enabled) {
        L5SYS_LOG(sys_handle,"wait_queue_done %s\n",stat->description);
    }
#endif

    /* ensure latest write index has been sent to firmware  */
    if (h->pmcfw_dma_queue_index.index != h->pmcfw_dma_queue_index.last_index_written_to_spram) {
        h->pmcfw_dma_queue_index.last_index_written_to_spram = h->pmcfw_dma_queue_index.index;
        pmc_sys_reg_write(sys_handle,h->pmcfw_dma_queue_index.spram_address_of_host_index,h->pmcfw_dma_queue_index.index);
    }

    wait_for_firmware(sys_handle,wait_index,stat);
    h->pmcfw_dma_queue_index.flush_size = MIN_FLUSH_SIZE;
}

/*******************************************************************************
* pipe_burst_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation burst read
*
* INPUTS:
*   sys_handle - System handle
*   addr       - First 32-bit device address to read.
*   len        - Number of registers to read in the burst.
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
static void pipe_burst_read(pmc_sys_handle_t *sys_handle, UINT32 addr, UINT32 len, UINT32 *data )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:pipe_burst_read" );

    PMC_ASSERT(len<=L5SYS_READ_BUFFER_SIZE_IN_WORDS,PMC_ERR_INVALID_PARAMETERS,len,L5SYS_READ_BUFFER_SIZE_IN_WORDS);

    if (h->use_pipe) {

        UINT32 message[4];
        size_t offset_of_read_results_from_top_of_shared_memory;

        offset_of_read_results_from_top_of_shared_memory = (size_t)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[0]) - (size_t)h->shared_host_memory_ptr;

        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_BURST_READ, addr);
        message[0] = offset_of_read_results_from_top_of_shared_memory;
        message[2] = len;
        message[3] = 0;

/*
        PMC_OS_STATS_QUEUE_BURST_READ(len);
*/
#if L5SYS_TRACE_LEVEL > 0
        /* fill results with 0xaa to avoid getting stale data during debugging */
        memset((void *)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[0]),0xaa,len*4);
#endif

        /* send message to firmware to ask for burst read */
        queue_message(sys_handle, message, 4);

        /* wait for burst read to be finished */
        wait_queue_done(sys_handle,fw_queue_wait_done);

        /* copy result from shared memory */
#if defined(PMC_BIG_ENDIAN)
        {
            UINT32 i;
            for(i=0;i<len;i++) {
                UINT32 src = h->shared_host_memory_ptr->pmcfw_dma_results.read_results[i];
                data[i] = PMC_LE_TO_CPU_32(src);
            }
        }
#else
        memcpy((void *)data,(const void *)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[0]),len*4);
#endif

    } else {
        /* just wait for the queue to be done,... then do a direct burst read. */
        wait_queue_done(sys_handle,fw_queue_wait_done);
        pmc_sys_burst_read( sys_handle,addr,len,data );
    }
}

/*******************************************************************************
* pipe_batch_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation burst read
*
* INPUTS:
*   sys_handle - System handle
*   addr       - Array of 32-bit register addresses to read.
*   len        - Number of registers to read in the batch.
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
static void pipe_batch_read(pmc_sys_handle_t *sys_handle, UINT32 len, UINT32 *addr, UINT32 *data )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 message[66];
    size_t offset_of_read_results_from_top_of_shared_memory;
    UINT32 index;
#define MAX_BATCH_READ_WORDS 64  /* maximum number of registers in a 'batch' read command - note: you can still send a bunch of these mini-batches before waiting for the initial results. */
    UINT32 message_index[L5SYS_READ_BUFFER_SIZE_IN_WORDS/MAX_BATCH_READ_WORDS];
    UINT32 message_itr;
    UINT32 num_messages = 0;

    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:pipe_batch_read" );

    PMC_ASSERT(len<=L5SYS_READ_BUFFER_SIZE_IN_WORDS,PMC_ERR_INVALID_PARAMETERS,len,L5SYS_READ_BUFFER_SIZE_IN_WORDS);

    offset_of_read_results_from_top_of_shared_memory = (size_t)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[0]) - (size_t)h->shared_host_memory_ptr;

    index = 0;
    while (index < len) {
        UINT32 num = len - index; /* number of words in this 'mini-batch' */
        UINT32 cmd_len;

        /* break it into 64 reads at a time... */
        if (num > MAX_BATCH_READ_WORDS) {
            num = MAX_BATCH_READ_WORDS;
        }
        cmd_len = num+2;
        
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_BATCH_READ, cmd_len*4);
        message[0] = offset_of_read_results_from_top_of_shared_memory + index * 4;

#if L5SYS_TRACE_LEVEL > 0
        /* fill results with 0xaa to avoid getting stale data during debugging */
        memset((void *)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[index]),0xaa,len*4);
#endif

        h->pmcfw_dma_queue_index.flush_size = 1; /* ensure each message is flushed */

        /* send message to firmware to ask for batch read */
        message_index[num_messages] = queue_message_and_data(sys_handle, message, 2, &addr[index], num);
        num_messages ++;

        index += num;
    }


    for (message_itr=0;message_itr<num_messages;message_itr++) {
        UINT32 start = message_itr*MAX_BATCH_READ_WORDS;
        UINT32 size = MAX_BATCH_READ_WORDS;
        if (start + size > len) {
            size = len - start;
        }
        wait_for_firmware(sys_handle,message_index[message_itr]+2,fw_queue_wait_done);
#if defined(PMC_BIG_ENDIAN)
        {
            UINT32 i;
            for(i=0;i<len;i++) {
                UINT32 src = h->shared_host_memory_ptr->pmcfw_dma_results.read_results[i];
                data[i] = PMC_LE_TO_CPU_32(src);
            }
        }
#else
        memcpy((void *)&(data[start]),(const void *)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[start]),size*4);
#endif
    }

    /* copy result from shared memory */
}

/*******************************************************************************
* pipe_burst_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Base emulation burst write
*
* INPUTS:
*   sys_handle - System handle
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
static void pipe_burst_write(pmc_sys_handle_t *sys_handle, UINT32 addr, UINT32 len, UINT32 *data )
{
#ifndef DO_NOT_USE_BURST_WRITES
    /* send burst writes in 100 word chunks */
    while (len > 0)
    {
        UINT32 message[2];
        UINT32 words;
       
        if (len > 100) {
            words = 100;
        } else {
            words = len;
        }
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_BURST_WRITE, addr);
        message[0] = words;

        PMC_OS_STATS_QUEUE_WRITE();
        queue_message_and_data(sys_handle, message, 2, data, words);
        addr += words * 4;
        data += words;
        len  -= words;
    }
#else
    while (len > 0)
    {
        UINT32 message[2];
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_WRITE, addr);
        message[0] = *data;

        PMC_OS_STATS_QUEUE_WRITE();
        queue_message(sys_handle, message, 2);
        addr += 4;
        data ++;
        len  --;
    }
#endif
}


/******************************************************************************
*  l5sys_reg_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function checks whether we are using the fw performance queue
*                 and then either performs a direct register read of a 32 bit
*                 wide register or gets FW to return the result.
*
*  INPUTS:        
*                 *hndl       - opaque customer's handle.
*                 addr        - address of the register location to be read
*                               as it appears in the hardware data sheet
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*    UINT32 -     32 bit value read.
*
*  NOTES:
******************************************************************************/
UINT32 l5sys_reg_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        return pipe_read( hndl, addr);
    } else {
        return pmc_sys_reg_read( hndl, addr);
    }
} /* l5sys_reg_read */

/******************************************************************************
*  tv_to_microseconds
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*     Converts the pmc_sys_timeval_t struct to a number of microseconds.
*
*  INPUTS:        
*     *tv - pointer to the timeval
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*    UINT64 - the microsecond value.
*
*  NOTES:
******************************************************************************/
static UINT64 tv_to_microseconds(pmc_sys_timeval_t *tv)
{
    UINT64 microseconds = tv->tv_sec;
    microseconds *= 1000000;
    microseconds += tv->tv_usec;
    return (microseconds);
}

/******************************************************************************
*  l5sys_bench_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function returns the time in microseconds that it takes
*                 to read the specified register.
*
*                 if the firmware queue is used it uses firmware to do the measurement.
*
*                 without the firmware queue the measurement includes the pcie delay
*                 which is not particularly useful.
*
*  INPUTS:        
*                 *hndl       - opaque customer's handle.
*                 addr        - address of the register location to be read
*                               as it appears in the hardware data sheet
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*    DOUBLE -     DOUBLE duration in microseconds.
*
*  NOTES:
******************************************************************************/
DOUBLE l5sys_bench_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        UINT32 clock_ticks;
        clock_ticks = pipe_bench_read( hndl, addr);
        return (clock_ticks / 300.0);
    } else {
        pmc_sys_timeval_t start;
        pmc_sys_timeval_t now;
        static UINT32 result;

        PMC_TIMEVAL_READ(&start);

        result = pmc_sys_reg_read( hndl, addr);

        PMC_TIMEVAL_READ(&now);

        return (tv_to_microseconds(&now) - tv_to_microseconds(&start));
    }
} /* l5sys_bench_read */


/*******************************************************************************
*  l5sys_reg_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs either a direct register write of a 32 bit
*                 wide register or writes the address and value to FW for execution.
*
*  INPUTS:        
*                 hndl        - opaque customer's handle.
*                 addr        - address of the register location to be read
*                               as it appears in the hardware data sheet
*                 value       - 32 bit value to write to the register.
*
* OUTPUTS:
*   None.
*
*  RETURNS:       
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_reg_write( pmc_sys_handle_t *hndl,
                                      UINT32 addr,
                                      UINT32 value )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        /* write address and value to FW */
        pipe_write( hndl, addr, value );
    } else {
        pmc_sys_reg_write( hndl, addr, value );
    }
} /* pmc_sys_reg_write */


/*******************************************************************************
*  l5sys_reg_read_modify_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a burst write of an array of 32 bit
*                 wide registers.
*
*  INPUTS:
*                 *hndl   - customer's opaque handle.
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
*      UINT32 -   value written to the register.
*
*  NOTES:
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
*******************************************************************************/
UINT32 l5sys_reg_read_modify_write( pmc_sys_handle_t *hndl, 
                                                    UINT32 addr,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    UINT32 *oldval )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        pipe_read_modify_write( hndl, addr, mask, value );
        return value;
    } else {
        return pmc_sys_reg_read_modify_write( hndl, addr, mask, value, oldval );
    }
} /* l5sys_reg_read_modify_write */


/*******************************************************************************
*  l5sys_poll
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function polls a register until the comparison is met
*
*  INPUTS:
*                 *sys      - customer's opaque handle.
*                 addr      - address of the register location to be read
*                             as it appears in the hardware data sheet
*                 mask      - field being examined.
*                 value     - desired value of the field (note: this value
*                             must be pre-shifted)
*                 cmp       - type of comparison
*                 max_count - maximum number of times to read the register.
*                 delay_between_polls_in_microseconds - how long to wait between register reads.
*
*  OUTPUTS:
*                 *num_failed_polls - number of register reads that were
*                                     not the desired value.
*
*  RETURNS:
*    PMC_POLL_RETURN_TYPE -  PMC_SUCCESS
*                            PMC_ERR_POLL_TIMEOUT
*
*  NOTES:
*  To wait for the value of the middle bits of a field to be 0xcafe
*  (which admittedly is strange thing to wait for) the following code could
*  be used:
*
*  UINT32 mask  = 0x00ffff00;
*  UINT32 value = 0x00cafe00;
*  UINT32 max_count = 1000;  // wait for up to 1000 register reads.
*  UINT32 num_failed_polls;
*  UINT32 delay_between_polls_in_microseconds = 10; // wait 10 microseconds between reads.
*  UINT32 result;
*
*  result = pmc_sys_poll( hndl, addr, mask, value, PMC_POLL_EQUAL, max_count, &num_failed_polls, delay_between_polls_in_microseconds );
*  if (result == PMC_ERR_POLL_TIMEOUT)
*  {
*    ... // poll timeout
*  }
*
*******************************************************************************/
PMC_POLL_RETURN_TYPE l5sys_poll(pmc_sys_handle_t *sys, 
                                UINT32 addr, 
                                UINT32 mask, 
                                UINT32 value, 
                                PMC_POLL_COMPARISON_TYPE cmp, 
                                UINT32 max_count, 
                                UINT32 *num_failed_polls, 
                                UINT32 delay_between_polls_in_microseconds )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys;
    if (h->use_pipe) {
        return pipe_poll(sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds );
    } else {
        return pmc_sys_poll(sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds );
    }
}



/*******************************************************************************
*  l5sys_burst_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs a burst read of an array of 32 bit
*                 wide registers.
*
*  INPUTS:        
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be read
*                           as it appears in the hardware data sheet
*                 len     - number of words to read.
*                 data    - array of 32 bit values read from the registers
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_burst_read( pmc_sys_handle_t *hndl,
                                       UINT32 addr,
                                       UINT32 len,
                                       UINT32 *data )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        pipe_burst_read( hndl, addr, len, data );
    } else {
        pmc_sys_burst_read( hndl, addr, len, data );
    }
} /* l5sys_burst_read */


/*******************************************************************************
*  l5sys_batch_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs a burst read of an array of 32 bit
*                 wide registers.
*
*  INPUTS:        
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be read
*                           as it appears in the hardware data sheet
*                 len     - number of words to read.
*                 data    - array of 32 bit values read from the registers
*
*  OUTPUTS:
*    None.
*
*  RETURNS:       
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_batch_read( pmc_sys_handle_t *hndl,
                                       UINT32 len,
                                       UINT32 *addr, 
                                       UINT32 *data )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        pipe_batch_read( hndl, len, addr, data );
    } else {
        UINT32 i;
        for (i=0;i<len;i++) {
            data[i] = pmc_sys_reg_read(hndl,addr[i]);
        }
    }
} /* l5sys_batch_read */

/*******************************************************************************
*  l5sys_burst_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a burst write of an array of 32 bit
*                 wide registers.
*
*  INPUTS:
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be written
*                           as it appears in the hardware data sheet
*                 len     - number of words to write.
*                 data    - array of 32 bit values to write to the registers.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_burst_write( pmc_sys_handle_t *hndl, 
                                        UINT32 addr,
                                        UINT32 len,
                                        UINT32 *data )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)hndl;
    if (h->use_pipe) {
        pipe_burst_write( hndl, addr, len, data );
    } else {
        pmc_sys_burst_write( hndl, addr, len, data );
    }
} /* pmc_sys_burst_write */



/*******************************************************************************
*  pipe_self_test
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function send a test sequence to the firmware.
*
*  INPUTS:
*                 sys_handle - any pmc_handle_t
*                 len        - number of words to write (max 256)
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*                 none.
*
*  NOTES:
*******************************************************************************/
static void pipe_self_test( pmc_sys_handle_t *sys_handle, UINT32 len )
{
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    UINT32 message[256];
    UINT32 i;
    UINT32 test_pattern = h->test_pattern;

    PMC_ENTRY();
    len = len + len % 2;
    if (len > 256) {
        len = 256;
    }
    message[0] = test_pattern++;
    /* Set up a write message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_SELF_TEST, len * 4);
    for (i=2;i<len;i++) {
        message[i] = test_pattern++;
    }
    h->test_pattern = test_pattern;

    queue_message(sys_handle, message, len);
#if L5SYS_TRACE_LEVEL >= 5
    if (h->queue_stall_callback_function != NULL)
    {
        h->queue_stall_callback_function(h->queue_stall_callback_param,FALSE);
    }
#endif
    PMC_RETURN();
} /* pipe_self_test */

/*******************************************************************************
*  pipe_check_sum
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function sends the current checksum to the firmware.
*
*  INPUTS:
*                 sys_handle - any pmc_handle_t
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*                 none.
*
*  NOTES:
*******************************************************************************/
static void pipe_check_sum( pmc_sys_handle_t *sys_handle )
{

    PMC_ENTRY();
#ifdef L5SYS_ADD_CHECKSUM_AFTER_EVERY_FLUSH
    {
        l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
        UINT32 message[2];
        message[0] = h->checksum;
        /* Set up a write message to write into SPRAM */
        message[1] = OP_ADDR(OP_CODE_CHECK_SUM, 0);

        queue_message(sys_handle, message, 2);
#if L5SYS_TRACE_LEVEL >= 5
        if (h->queue_stall_callback_function != NULL)
        {
            h->queue_stall_callback_function(h->queue_stall_callback_param,FALSE);
        }
#endif
    }
#endif
    PMC_RETURN();
} /* pipe_check_sum */


/*******************************************************************************
*  l5sys_self_test
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function send a test sequence to the firmware.
*
*  INPUTS:
*                 handle  - any pmc_handle_t
*                 len     - number of words to write (max 256)
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_self_test( void *handle, UINT32 len )
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;

    PMC_ENTRY();
    if (sys_handle != NULL) {
        l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
        if (h->use_pipe) {
            pipe_self_test( sys_handle, len );
        }
    }
    PMC_RETURN();
} /* l5sys_self_test */

/*******************************************************************************
*  l5sys_check_sum
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function sends the current checksum to the firmware.
*
*  INPUTS:
*                 handle  - any pmc_handle_t
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*                 none.
*
*  NOTES:
*******************************************************************************/
void l5sys_check_sum( void *handle )
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;

    PMC_ENTRY();
    if (sys_handle != NULL) {
        l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
        if (h->use_pipe) {
            pipe_check_sum( sys_handle );
        }
    }
    PMC_RETURN();
} /* l5sys_check_sum */

/*******************************************************************************
*  l5sys_process_id_register
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      writes the time stamp and process id into a circular buffer on the digi.
*
*  INPUTS:
*     *sys_handle - pmc_sys_handle_t
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*******************************************************************************/
static void l5sys_process_id_register(pmc_sys_handle_t *sys_handle )
{
    PMC_ENTRY();
    PMC_RETURN();
}


/*******************************************************************************
*  l5sys_process_id_check
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      displays the history of timestamps and process id's of processes that
*      have initialized the firmware queue.
*
*  INPUTS:
*     *sys_handle - pmc_sys_handle_t
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*******************************************************************************/
static void l5sys_process_id_check(pmc_sys_handle_t *sys_handle )
{
    PMC_ENTRY();
    PMC_RETURN();
}

/*******************************************************************************
*  l5sys_queue_init_test
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      this function writes to APP_SCRATCH7 using the firmware queue
*      and reads the register back.
*
*      if an error is observed it prints a few debug messages and returns a failure code.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*  PMC_ERROR - PMC_SUCCESS if test passed.
*              PMC_ERR_FAIL if test failed.
*
*  NOTES:
*******************************************************************************/
PMC_ERROR l5sys_queue_init_test( void *handle )
{
    UINT32 addr = 0x18001A0; /* test using APP_SCRATCH_7 register */
    UINT32 save_val;
    UINT32 write_val;
    UINT32 read_val;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    L5SYS_TRACE(3,"calling l5sys_queue_init_test\n");
    if (handle != NULL) {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        
        if ((sys_handle != NULL)) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;

            L5SYS_TRACE(3,"calling l2sys_reg_read\n");
            /* read the initial value of the register */
            save_val = l2sys_reg_read(sys_handle,0,addr);

            /* write a test pattern */
            L5SYS_TRACE(3,"calling l2sys_reg_write\n");
            write_val = 0x01020304;
            l2sys_reg_write(sys_handle,0,addr,write_val);

            /* read the register */
            L5SYS_TRACE(3,"calling l2sys_reg_read\n");
            read_val = l2sys_reg_read(sys_handle,0,addr);

            /* check that the read value and write value are the same. */
            if (read_val != write_val) {
                PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0, "wrote 0x%08x read 0x%08x\n",write_val,read_val);
                result = PMC_ERR_FAIL;
            }

            /* write a different test pattern */
            write_val = 0xf0e0d0c0;
            l2sys_reg_write(sys_handle,0,addr,write_val);

            /* read the register */
            read_val = l2sys_reg_read(sys_handle,0,addr);

            /* check that the read value and write value are the same. */
            if (read_val != write_val) {
                PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0, "wrote 0x%08x read 0x%08x\n",write_val,read_val);
                result = PMC_ERR_FAIL;
            }

            if (PMC_SUCCESS != result) {
                UINT32 poll_result = l5sys_poll_result(h);
                UINT32 read_result = l5sys_read_result(h);
                UINT32 should_be_0x12345678 = l5sys_should_be_0x12345678(h);
                UINT32 app_scratch_7 = pmc_sys_reg_read(sys_handle,addr);

                PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0, "error using firmware queue\n");
#define PRINT_PARAM(field) PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, field, 0, "%s is 0x%x (%d)\n",#field,field,field)
                PRINT_PARAM(poll_result);
                PRINT_PARAM(read_result);
                PRINT_PARAM(should_be_0x12345678);
                PRINT_PARAM(app_scratch_7);
                dump_queue(sys_handle,"test_failed",__FUNCTION__,0);
                PMC_OS_USLEEP(2*1000*1000);
                dump_queue(sys_handle,"test_failed",__FUNCTION__,2);
            }

            /* restore the previous value */
            l2sys_reg_write(sys_handle,0,addr,save_val);

            if (PMC_SUCCESS == result) {
                l5sys_process_id_register(sys_handle);
            }
        }
    }
    PMC_RETURN(result);
}


/*******************************************************************************
*  l5sys_queue_enabled
****************************************************************************//**
*
*  DESCRIPTION:
*      returns TRUE if the firmware queue is enabled.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*     TRUE - performance queue configured, FALSE - queue disabled.
*
*  NOTES:
*******************************************************************************/
BOOL8 l5sys_queue_enabled(void *handle)
{
    PMC_ENTRY();

    /* check input */
    PMC_ASSERT(NULL != handle, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
        
    {
        pmc_sys_handle_t *sys_handle = *(void ***)handle;
        if (sys_handle != NULL) {
            l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
            BOOL8 result = h->use_pipe;

            PMC_RETURN( result );
        }
    }

    PMC_RETURN(FALSE);
}

/*******************************************************************************
*  l5sys_fw_perform_query
****************************************************************************//**
*
*  DESCRIPTION:
*      send a message to firmware and wait for the response.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     data      - message to send
*     len       - number of words in the message to send
*     max_response_len - maximum number of words in response accepted
*
*  OUTPUTS:
*     response  - message to retreived (must be space for at least max_response_len words)
*     header    - first two message header words returned
*
*  RETURNS:
*     PMC_SUCCESS.
*
*  NOTES:
*******************************************************************************/
PMC_ERROR l5sys_fw_perform_query(void *handle, UINT32 *data, UINT32 len, UINT32 *response, UINT32 max_response_len, UINT32 header[2])
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    /** Stat for how long we wait for the queue to finish */
    PMC_SYS_STAT_DECLARE( fw_queue_wait_done, "fw_queue_wait_done:l5sys_fw_perform_query" );

    UINT32 message[4];
    UINT32 message_len;
    UINT32 offset_of_read_results_from_top_of_shared_memory;
    volatile UINT32 *read_results = h->shared_host_memory_ptr->pmcfw_dma_results.read_results;
    
    PMC_ENTRY();

    PMC_ASSERT(len <= 64,PMC_ERR_INVALID_PARAMETERS,len,0);

    message_len = 3+len;
    message_len += (message_len % 2); /* round up to even number */

    offset_of_read_results_from_top_of_shared_memory = (size_t)&(h->shared_host_memory_ptr->pmcfw_dma_results.read_results[0]) - (size_t)h->shared_host_memory_ptr;

    /* Set up a write message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_FIRMWARE_QUERY, (message_len)*4);
    message[0] = offset_of_read_results_from_top_of_shared_memory;
    message[2] = max_response_len;
    message[3] = data[0];
    queue_message_and_data(sys_handle, message, 4, &(data[1]),len-1);

    /* wait for the queue to be done and retrieve the results */
    wait_queue_done(sys_handle,fw_queue_wait_done);

    /* copy result from shared memory */
#if defined(PMC_BIG_ENDIAN)
    {
        UINT32 i;
        for(i=0;i<max_response_len;i++) {
            UINT32 src = read_results[2+i];
            response[i] = PMC_LE_TO_CPU_32(src);
        }
    }
#else
    memcpy((void *)response,(const void *)&(read_results[2]),max_response_len*4);
#endif
    if (h->logging_enabled) {
        UINT32 i;
        L5SYS_LOG(sys_handle,"RESP:");
        for (i=2;i<max_response_len+2;i++) {
            L5SYS_LOG(sys_handle," %x",read_results[i]);
        }
        L5SYS_LOG(sys_handle,"\n");
    }
    header[0] = PMC_LE_TO_CPU_32(read_results[0]);
    header[1] = PMC_LE_TO_CPU_32(read_results[1]);

    PMC_RETURN( PMC_SUCCESS );
}

/*******************************************************************************
*  l5sys_fw_send_cfg_message
****************************************************************************//**
*
*  DESCRIPTION:
*      send a message to firmware and do not wait for the response.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     data      - message to send
*     len       - number of words in the message to send
*
*  OUTPUTS:
*     None.
*
*  RETURNS:
*     PMC_SUCCESS.
*
*  NOTES:
*******************************************************************************/
PMC_ERROR l5sys_fw_send_cfg_message(void *handle,UINT32 *data,UINT32 len)
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    UINT32 message[2];
    UINT32 message_len;
    
    PMC_ENTRY();

    PMC_ASSERT(len <= 64,PMC_ERR_INVALID_PARAMETERS,len,0);

    message_len = 2+len;
    message_len += (message_len % 2); /* round up to even number */
    /* Set up a write message to write into SPRAM */
    message[1] = OP_ADDR(OP_CODE_FIRMWARE_CMD, (message_len)*4);
    message[0] = 0;

    /* send message into host memory */
    queue_message_and_data(sys_handle, message, 2, data, len);

    PMC_RETURN( PMC_SUCCESS );
}


/*******************************************************************************
*  l5sys_log_record
****************************************************************************//**
*
*  DESCRIPTION:
*      record l5sys messages to memory.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_record(void *handle)
{
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_log_free(h->messages);
    h->messages = NULL;
    h->active = NULL;
    h->link = (l5sys_queue_message_t *)&(h->messages);
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
#endif
}

/*******************************************************************************
*  l5sys_log_stop
****************************************************************************//**
*
*  DESCRIPTION:
*      stop recording l5sys messages to memory.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    l5sys_queue_message_t - list of messages that were recorded.
*
*  NOTES:
*******************************************************************************/
PUBLIC l5sys_queue_message_t *l5sys_log_stop(void *handle)
{
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_queue_message_t *ret_val = h->messages;

    PMC_ENTRY();
    h->messages = NULL;
    h->active = NULL;
    h->link = NULL;
    PMC_RETURN(ret_val); 
#else
    PMC_ENTRY();
    PMC_RETURN(NULL); 
#endif
}

#ifdef L5SYS_PROFILING
/*******************************************************************************
*  add_message
****************************************************************************//**
*
*  DESCRIPTION:
*     save the message in memory
*
*  INPUTS:
*     msg   - message
*     msg_size - number of words (UINT32's)
*     data  - extra data
*     data_size - number of words of extra data
*
*  OUTPUTS:
*    None
*
*  RETURNS:
*    message_t - message. (NOTE: this is MALLOC'd and needs to be FREE'd)
*
*  NOTES:
*******************************************************************************/
static l5sys_queue_message_t *add_message( UINT32 *msg, UINT32 msg_size, UINT32 *data, UINT32 data_size )
{
    UINT32 i;
    UINT32 index = 0;
    l5sys_queue_message_t *message;
    UINT32 num = msg_size + data_size + (data_size & 1);

    PMC_ENTRY();
    message = (l5sys_queue_message_t *)PMC_MALLOC(sizeof(l5sys_queue_message_t)+sizeof(UINT32)*num);
    message->num = num;
    message->next = NULL;
    for (i=0;i<msg_size;i++,index++) {
        message->data[index] = msg[i];
    }
    for (i=0;i<data_size;i++,index++) {
        message->data[index] = data[i];
    }
    if (data_size&1) {
        message->data[index] = 0;
    }
    message->done_timestamp = 0;
    message->started_timestamp = 0;
    message->index = 0;
    PMC_RETURN(message);
}
#endif


/*******************************************************************************
*  l5sys_log_open
****************************************************************************//**
*
*  DESCRIPTION:
*      log l5sys messages to a log file.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     file_name - log file full path name.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_open(void *handle,const char *file_name)
{
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    PMC_ENTRY();
    h->logging_enabled = TRUE;
    h->logfile = fopen(file_name,"w");
    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
#endif
}
/*******************************************************************************
*  l5sys_log_function
****************************************************************************//**
*
*  DESCRIPTION:
*      calls 'log_function' inside of L5SYS_LOG instead of writing messages to a file.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     log_function - log function to call inside L5SYS_LOG
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_function(void *handle,log_function_t log_function)
{
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    PMC_ENTRY();
    h->logging_enabled = TRUE;
    h->log_function = log_function;
    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
#endif
}

/*******************************************************************************
*  l5sys_log_close
****************************************************************************//**
*
*  DESCRIPTION:
*      close the log file.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_close(void *handle)
{
#ifndef LOGGING_DISABLED
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    PMC_ENTRY();
    if (h->fw_show_stats != NULL) {
        h->fw_show_stats(sys_handle,h->queue_stall_callback_param);
    }
    h->logging_enabled = FALSE;
    if (h->logfile != NULL) {
        fclose(h->logfile);
        h->logfile = NULL;
    }
    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
#endif
}



#ifdef L5SYS_PROFILING
/*******************************************************************************
*  parse_message
****************************************************************************//**
*
*  DESCRIPTION:
*      parse a line "num data0 data1 data2"  (The "Q: " is already skipped)
*
*  INPUTS:
*     *line   - pointer to line.
*
*  OUTPUTS:
*    None
*
*  RETURNS:
*    message_t - parsed message. (NOTE: this is MALLOC'd and needs to be FREE'd)
*
*  NOTES:
*******************************************************************************/
static l5sys_queue_message_t *parse_message( char *line )
{
    UINT32 i,num;
    l5sys_queue_message_t *message;

    PMC_ENTRY();
    while (*line == ' ') {
        line++;
    }
    num = strtol(line,&line,10);
    message = (l5sys_queue_message_t *)PMC_MALLOC(sizeof(l5sys_queue_message_t)+sizeof(UINT32)*num);
    message->num = num;
    message->next = NULL;
    for (i=0;i<num;i++) {
        while (*line == ' ') {
            line++;
        }
        message->data[i] = strtol(line,&line,16);
    }
    PMC_RETURN(message);
}
#endif

/*******************************************************************************
*  l5sys_log_read
****************************************************************************//**
*
*  DESCRIPTION:
*      read the log file and returns the list of messages
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     file_name - log file full path name.
*
*  OUTPUTS:
*    *messages - list of messages.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC PMC_ERROR l5sys_log_read(void *handle,const char *file_name, l5sys_queue_message_t **messages)
{
#ifdef L5SYS_PROFILING
    l5sys_queue_message_t *link = (l5sys_queue_message_t *)messages;
    UINT32 count = 0;
    UINT32 total_size = 0;
    FILE *file;
    

    PMC_ENTRY();
    *messages = NULL;
    file = fopen ( file_name, "r" );
    if ( file != NULL )
    {
        char line [ 1024 ]; /* or other suitable maximum line size */

        while ( fgets ( line, sizeof( line ), file ) != NULL ) /* read a line */
        {
            if (strncmp(line,"Q: ",3) == 0) {
                link->next = parse_message(line+3);
                link = link->next;
                total_size += link->num;
                count++;
            }
        }
        fclose ( file );
    }
    PMC_PRINT("%d operations, %d words\n",count,total_size);
    PMC_RETURN( PMC_SUCCESS );
#else
    PMC_ENTRY();
    *messages = NULL;
    PMC_RETURN( PMC_SUCCESS );
#endif
}

#ifdef L5SYS_PROFILING
void digi_fw_speed(double microseconds);
#endif

/*******************************************************************************
*  l5sys_log_save
****************************************************************************//**
*
*  DESCRIPTION:
*      saves the list of messages to a file.
*
*  INPUTS:
*     handle    - any pmc_handle_t.
*     messages  - list of messages.
*     file_name - file name that is created.
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_save(void *handle,l5sys_queue_message_t *messages, const char *file_name )
{
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_queue_message_t *link;

    UINT64 init_timestamp = messages->started_timestamp;
    UINT64 last_timestamp = init_timestamp;
    
    PMC_ENTRY();
    digi_fw_speed(0);

    h->logging_enabled = TRUE;
    h->logfile = fopen(file_name,"w");

    for (link = messages; link != NULL; link=link->next) {
        if (h->logging_enabled) {
            UINT32 i;
            UINT32 index = h->pmcfw_dma_queue_index.index;

            if (link->started_timestamp != 0) {
                UINT64 started = link->started_timestamp - init_timestamp;
                UINT64 finished = link->done_timestamp - init_timestamp;
                UINT64 duration = link->done_timestamp - link->started_timestamp;
                UINT64 delta_started = link->started_timestamp - last_timestamp;

                last_timestamp = link->started_timestamp;
                L5SYS_LOG( sys_handle, "started: %0.3f (delta: %0.3f) - finished %0.3f (lag: %0.3f)\n",
                           (started)/1000.0,
                           (delta_started)/1000.0,
                           (finished)/1000.0,
                           (duration)/1000.0);
            }

            L5SYS_LOG( sys_handle, "Q: %d", link->num );
            for (i=0;i<link->num;i++) {
                L5SYS_LOG( sys_handle, " %08x", link->data[i] );
                h->shared_host_memory_ptr->pmcfw_dma_queue.circular_buffer[(index+i)%L5SYS_HOST_QUEUE_SIZE_IN_WORDS] = link->data[i];
            }
            L5SYS_LOG( sys_handle, "\n" );
            dump_message(sys_handle,index,link->index,"L5SYS: ");
        }
    }

    if (h->fw_show_stats != NULL) {
        h->fw_show_stats(sys_handle,h->queue_stall_callback_param);
    }
    h->logging_enabled = FALSE;
    fclose(h->logfile);

    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
}

#ifdef L5SYS_PROFILING

#endif

/*******************************************************************************
*  l5sys_log_bench_each_message
****************************************************************************//**
*
*  DESCRIPTION:
*      send each message to the device 10,000 times and measures the time each message
*      takes.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     *messages - list of messages.
*     *file_name - filename to record bench results.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_bench_each_message(void *handle,l5sys_queue_message_t *messages, const char *file_name)
{
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_queue_message_t *link;
    l5sys_queue_message_t *prev;

    PMC_ENTRY();
    for (link = messages,prev=NULL; link != NULL; link=link->next) {
        link->prev = prev;
        prev = link;
    }

    l5sys_log_open(handle,file_name);
    l5sys_buffered(handle);
    h->logging_enabled = FALSE;
    for (link = messages; link != NULL; link=link->next) {
        double speed;
        UINT32 loop_count;
        UINT32 msg_index = 0;
        UINT32 i;
        l5sys_operation_code_definitions_enum opcode = (l5sys_operation_code_definitions_enum)(UINT32)((link->data[1] & 0xfe000000) >> 25);

        /* set flush size to every 2^31 which means it flushes every 1 billion operations */
        l5sys_buffered(handle);
        h->pmcfw_dma_queue_index.flush_size = 31;
        if (opcode == OP_CODE_FIRMWARE_CMD) {
            loop_count = 100;
        } else if (opcode == OP_CODE_POLL) {
            loop_count = 100;
        } else if (opcode == OP_CODE_SLEEP) {
            loop_count = 100;
        } else {
            loop_count = 10000;
            if (link->num >= 6) {
                loop_count = loop_count * 2 / link->num;
            }
        }
        for(i=0;i<loop_count;i++) {
            msg_index = queue_message(sys_handle,link->data,link->num);
        }

        pmc_sys_stat_t bench_stat[1] = {{NULL,1,"","",0}};

        pmc_sys_stat_mark_start(bench_stat);
        l5sys_wait_queue_done(handle);
        pmc_sys_stat_mark_end(bench_stat);
        pmc_sys_stat_set_num(bench_stat,loop_count);
        /* avg = pmc_sys_stat_show_avg(bench_stat,"");
        */
        speed = (1.0 * bench_stat->total) / bench_stat->num;
        if (opcode == OP_CODE_BURST_WRITE) {
            UINT32 num_words = link->data[0];
            speed /= num_words;
        }
        h->logging_enabled = TRUE;
        digi_fw_speed(speed);
        dump_message(sys_handle,msg_index,msg_index,"L5SYS: ");
        L5SYS_LOG(sys_handle,"SPEED: %g nanoseconds\n",(1000.0 * speed));
        h->logging_enabled = FALSE;
    }
    h->logging_enabled = TRUE;

    l5sys_log_close(handle);
    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
}
/*******************************************************************************
*  l5sys_log_timed_playback
****************************************************************************//**
*
*  DESCRIPTION:
*      send the messages to the device.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     *messages - list of messages.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_timed_playback(void *handle,l5sys_queue_message_t *messages )
{
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_queue_message_t *link;
    PMC_SYS_STAT_DECLARE(playback,"playback");
    
    PMC_ENTRY();

    digi_fw_speed(0);
    h->active = messages;
    l5sys_buffered(handle);
    for (link = messages; link != NULL; link=link->next) {
        link->index = 0xffffffff;
        link->started_timestamp = 0;
        link->done_timestamp = 0;
    }
    for (link = messages; link != NULL; link=link->next) {
        link->index = h->pmcfw_dma_queue_index.index;
        link->started_timestamp = now_usec();
        queue_message(sys_handle,link->data,link->num);
    }
    wait_queue_done(sys_handle,playback);

    while (h->active != NULL) {
        UINT64 now = now_usec();
        h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index = l5sys_dma_queue_fw_index(h);
        while (h->active != NULL && h->active->index < h->pmcfw_dma_queue_index.copy_of_dma_queue_fw_index) {
            h->active->done_timestamp = now;
            h->active = h->active->next;
        }
    }

    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
}

/*******************************************************************************
*  l5sys_log_playback
****************************************************************************//**
*
*  DESCRIPTION:
*      send the messages to the device and records approximately when each message was executed.
*
*  INPUTS:
*     *handle   - any pmc_handle_t.
*     *messages - list of messages.
*     buffer - TRUE means just write the messages to host memory.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_playback(void *handle,l5sys_queue_message_t *messages, BOOL8 buffer)
{
#ifdef L5SYS_PROFILING
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    l5sys_queue_message_t *link;

    PMC_ENTRY();

    l5sys_buffered(handle);
    if (buffer) {
        h->pmcfw_dma_queue_index.flush_size = 31;
    }
    for (link = messages; link != NULL; link=link->next) {
        queue_message(sys_handle,link->data,link->num);
    }

    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
}

/*******************************************************************************
*  l5sys_log_free
****************************************************************************//**
*
*  DESCRIPTION:
*      frees the memory associated with the messages
*
*  INPUTS:
*     *messages - list of messages.
*
*  OUTPUTS:
*    None.
*
*  RETURNS:
*    None.
*
*  NOTES:
*******************************************************************************/
PUBLIC void l5sys_log_free(l5sys_queue_message_t *messages)
{
#ifdef L5SYS_PROFILING
    l5sys_queue_message_t *link;
    l5sys_queue_message_t *next;

    PMC_ENTRY();

    for (link = messages; link != NULL; link=next) {
        next = link->next;
        PMC_FREE(&link);
    }

    PMC_RETURN();
#else
    PMC_ENTRY();
    PMC_RETURN();
#endif
} /* l5sys_log_free() */

/*******************************************************************************
* l5sysLog
* ___________________________________________________________________________
*
* DESCRIPTION:
*  print a line to the firmware log file "fwlog.txt"
*
* INPUTS:
*    sys_handle - pmc_sys_handle_t
*    fmt - format string similar to printf()
*    ... - a variable number of arguments similar to printf()
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void l5sysLog( pmc_sys_handle_t *sys_handle, const char *fmt, ... )
{
#ifndef LOGGING_DISABLED
    va_list ap; 
    
#ifdef L5SYS_PROFILING
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
#endif
    PMC_ENTRY();

#ifdef L5SYS_PROFILING
    if (h->logging_enabled) {
        /* only write message to the log open log file */
        va_start(ap,fmt);
        if (h->log_function != NULL) {
            h->log_function(fmt,ap);
        } else {
            vfprintf(h->logfile,fmt,ap); /* vfprintf() allowed in porting function */
            fflush(h->logfile); /* fflush() allowed in porting function */ 
        }
    } else
#endif
    {
        /* just display message to stdout */
        va_start(ap,fmt);
        vprintf(fmt,ap); /* vprintf() allowed in porting function */
    }

    va_end(ap); 
    PMC_RETURN();
#endif   
}


/*******************************************************************************
* l5sys_buffered
* ___________________________________________________________________________
*
* DESCRIPTION:
*  let the firmware notification of new messages be delayed to use less
*  pcie bandwidth
*
* INPUTS:
*    handle - any pmc_handle_t
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void l5sys_buffered(void *handle)
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    h->buffered_io = TRUE;
}

/*******************************************************************************
* l5sys_unbuffered
* ___________________________________________________________________________
*
* DESCRIPTION:
*  always notify firmware of new messages,... this is needed if anyone writes
*  a register outside of a PMC_ATOMIC_ENTRY/PMC_ATOMIC_RETURN.
*
* INPUTS:
*    handle - any pmc_handle_t
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void l5sys_unbuffered(void *handle)
{
    pmc_sys_handle_t *sys_handle = *(void ***)handle;
    l5pmc_sys_handle_t *h = *(l5pmc_sys_handle_t**)sys_handle;
    h->buffered_io = FALSE;
}
