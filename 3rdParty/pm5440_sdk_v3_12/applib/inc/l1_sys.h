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
*     system access layer 1 (provides register coalescing)
*
*   NOTES:
*
*     each read is saved and if the next read is to the same address then the old value is used.
* 
*     field writes are buffered and if you write the the same register it is combined with the previous write.
* 
*     writes are flushed either:
*       - explicity with a call to FLUSH (e.g. TSB_io_flush()).
*       - or before a read,
*       - or before writing to a different register,
* 
*     writes invalidate the read buffer.
* 
*     calling flush() also invalidates the read buffer.
*
********************************************************************************/
#ifndef l1sys_H
#define l1sys_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * the level 1 system layer coalesces field read and writes to the same register.
 */
#include "pmc_types.h"
#include "pmc_err.h"
#include "pmc_handle.h"
#include "l2_sys.h"
#include "pmc_log.h"
#include "shadow_registers.h"

#ifdef NO_COALESCING

typedef struct {
    pmc_sys_handle_t *sys_handle;
} coalesce_buffer_t;

/*******************************************************************************
* STRUCTURE: l1pmc_sys_handle_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track layer 1 interface state 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct l1pmc_sys_handle_t 
{
    l2pmc_sys_handle_t l2sys_handle; /*!< interface state for L2 (next layer) */
} l1pmc_sys_handle_t;

#define l1sys_flush( cb )

#define l1sys_init( cb, _sys_handle ) {*(pmc_sys_handle_t **)&(cb)->sys_handle = (pmc_sys_handle_t *)(_sys_handle);}

#define l1sys_field_set( cb, mem_type, tsb_base_addr, reg_num, mask, ofs, value ) \
    l2sys_reg_read_modify_write( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), mask, (value) << (ofs) )
#define l1sys_field_set2( cb, mem_type, tsb_base_addr, reg_num, mask, unused_mask, ofs, value ) \
    l2sys_reg_read_modify_write( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), mask, (value) << (ofs) )
#define l1sys_force_read_modify_write( cb )

#define l1sys_action_on_write_field_set( cb, mem_type, tsb_base_addr, reg_num, mask, ofs, value ) \
    l1sys_field_set( cb, mem_type, tsb_base_addr, reg_num, 0xffffffff, ofs, value )

#define l1sys_reg_write( cb, mem_type, tsb_base_addr, reg_num, value ) \
    l2sys_reg_write( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), value )

#define l1sys_reg_read( cb, mem_type, tsb_base_addr, reg_num ) \
    l2sys_reg_read( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num) )

#define l1sys_burst_write( cb, mem_type, tsb_base_addr, reg_num, len, data ) \
    l2sys_burst_write( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), len, data )

#define l1sys_burst_read( cb, mem_type, tsb_base_addr, reg_num, len, data ) \
    l2sys_burst_read( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), len, data )

#define l1sys_poll_bit( cb, mem_type, tsb_base_addr, reg_num, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l2sys_poll_bit( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#define l1sys_poll( cb, mem_type, tsb_base_addr, reg_num, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l2sys_poll( cb->sys_handle, mem_type, (tsb_base_addr)+(reg_num), mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#else

/** structure used for field write coalescing */
#define BUFFER_NOT_USED                0
#define BUFFER_USED_FOR_READ           1
#define BUFFER_USED_FOR_WRITE          2
#define BUFFER_FORCE_READ_MODIFY_WRITE 4
#define BUFFER_FORCE_WRITE             8

/******************************************************************************
* STRUCTURE: coalesce_buffer_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track state for register coalescing 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct coalesce_buffer_t {
    pmc_sys_handle_t *sys_handle;  /*!< sys_handle to access device */  
    UINT8         buffer_state;    /*!< state of the buffer: (not_used, used_for_read, or used_for_write) */  
    UINT8         buffer_flags;    /*!< should the write be forced to be a read_modify_write or write? FORCE_READ_MODIFY_WRITE or FORCE_WRITE */  
    UINT32        addr;            /*!< if buffer is used_for_write, this is the address to be written
                                      if buffer is used_for_read, this is the address read */  
    UINT32        mask;            /*!< if buffer is used_for_write, this is the mask of bits to be written */  
    UINT32        unused_mask;     /*!< if buffer is used_for_write, this is the mask of bits that can be written to zero if that changes a RMW to a full write. */  
    UINT32        value;           /*!< if buffer is used_for_write, this is the value to be written
                                      if buffer is used_for_read, this is the value that was read */  
    int           mem_type;        /*!< used to distinguish between cachable config space or status space.  */
} coalesce_buffer_t;

/******************************************************************************
* STRUCTURE: l1pmc_sys_handle_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure for tracking the Layer 1 (L1) handle and operations
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct l1pmc_sys_handle_t {
    l2pmc_sys_handle_t l2sys_handle; /*!< Handle to the next lower layer of operations */  
#ifndef NO_IO_LOGGING
    UINT32 last_addr; /*!< last address accessed */  
    UINT32 dirty;     /*!< flag to indicate that an operation was in progress that has not been flushed */  
#endif

} l1pmc_sys_handle_t;

/*******************************************************************************
* l1sys_dirty_flush
* ___________________________________________________________________________
*
* DESCRIPTION:
*  flushes a buffer that is known to be dirty.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
static INLINE void l1sys_dirty_flush( coalesce_buffer_t *cb ) ALWAYS_INLINE;
static INLINE void l1sys_dirty_flush( coalesce_buffer_t *cb )
{
    BOOL use_read_modify_write;
#ifndef NO_IO_LOGGING
    l1pmc_sys_handle_t *h = *(l1pmc_sys_handle_t**)cb->sys_handle;
    h->dirty = FALSE;
#endif
    
    if (cb->buffer_flags == BUFFER_FORCE_WRITE)
    {
        use_read_modify_write = FALSE;
    }
    else if (cb->buffer_flags == BUFFER_FORCE_READ_MODIFY_WRITE)
    {
          use_read_modify_write = TRUE;
    }
    else
    {
        /* use a read/modify/write if you are not updating all bits (excepting the previous overrides) */
        use_read_modify_write = ((cb->mask | cb->unused_mask) != 0xffffffff);
    }


    if (use_read_modify_write)
    {
        l2sys_reg_read_modify_write( cb->sys_handle, cb->mem_type, cb->addr, cb->mask, cb->value );
    }
    else
    {
        l2sys_reg_write( cb->sys_handle, cb->mem_type, cb->addr, cb->value );
    }
    cb->buffer_state = BUFFER_NOT_USED;
}

/*******************************************************************************
* l1sys_flush
* ___________________________________________________________________________
*
* DESCRIPTION:
*  force buffered writes to be sent to the device
*  also called before performing a read since this may be affected
*  by pending writes.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
 * this function should typically be called before calling any
 * sub functions and definitely before calling any sleep or other 
 * potentially blocking functions.
 *
 * e.g. this code is wrong:
 *
 * {
 *   buffer b[1];
 *   buffer_init( b, h );
 *   field_X_set( b, value );
 *   // <-- buffer_flush( b ); required here before calling sleep() otherwise
 *   // X is not set before the sleep is called.
 *   sleep(10); 
 *   field_X_set( b, value );
 *   buffer_flush( b ); required here before the function exits.
 * }
 *
 * Note: If a write is buffered and this function is not called before 
 * the function returns then the write will not be performed.
 *
 * e.g. this code is wrong:
 *
 * {
 *   buffer b[1];
 *   buffer_init( b, h );
 *   field_X_set( b, value );
 *   // <-- buffer_flush( b ); required here before the function exits.
 * }
 * 
 * In the above example, the next l1sys call will generate the 'stale data' error message.
*******************************************************************************/
static INLINE void l1sys_flush( coalesce_buffer_t *cb ) ALWAYS_INLINE;
static INLINE void l1sys_flush( coalesce_buffer_t *cb )
{
    if (cb->buffer_state == BUFFER_USED_FOR_WRITE) {
        l1sys_dirty_flush(cb);
    } else {
        cb->buffer_state = BUFFER_NOT_USED; /* purge the read buffer */
    }
}


/*******************************************************************************
* l1sys_check_for_stale_data
* ___________________________________________________________________________
*
* DESCRIPTION:
*  check if the system handle has unflushed data
*
* INPUTS:
*   cb - pointer to coalesce buffer
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This function just prints an error message if LOGGING is enabled if 
*   the system handle has stale data, which indicates that someone perhaps
*   called a sub-function before calling l1sys_flush().
*******************************************************************************/
static INLINE void l1sys_check_for_stale_data( coalesce_buffer_t *cb ) ALWAYS_INLINE;
static INLINE void l1sys_check_for_stale_data( coalesce_buffer_t *cb )
{
#ifndef NO_IO_LOGGING
    l1pmc_sys_handle_t *h = *(l1pmc_sys_handle_t**)cb->sys_handle;
    if (h->dirty)
    {
        LOG("ERROR: stale data in l1sys coalescing buffer,... possible missing flush() addr = 0x%08x",h->last_addr);
    }
#endif
}

/*******************************************************************************
* l1sys_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  initialize a coalesce buffer.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   sys_handle - pointer to system handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
static INLINE void l1sys_init( coalesce_buffer_t *cb, pmc_sys_handle_t *sys_handle ) ALWAYS_INLINE;
static INLINE void l1sys_init( coalesce_buffer_t *cb, pmc_sys_handle_t *sys_handle )
{
    cb->sys_handle = sys_handle;
    l1sys_check_for_stale_data( cb );
    cb->buffer_state = BUFFER_NOT_USED;
    cb->buffer_flags = 0;

    cb->addr = 0;
    cb->mask = 0;
    cb->unused_mask = 0;
    cb->value = 0;
}

/*******************************************************************************
* l1sys_field_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  update a field in the coalesce buffer
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the register in the TSB
*   mask - field mask (already shifted)
*   ofs - first bit set in the field mask
*   value - unshifted value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   If the coalesce buffer has been used to write to a different register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_field_set( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 ofs, UINT32 value ) ALWAYS_INLINE;
static INLINE void l1sys_field_set( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 ofs, UINT32 value )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    if ((cb->buffer_state == BUFFER_USED_FOR_WRITE) && (cb->addr != reg_addr)) {
        l1sys_dirty_flush( cb );
    }
    if (!(cb->buffer_state == BUFFER_USED_FOR_WRITE)) {
#ifndef NO_IO_LOGGING
      l1pmc_sys_handle_t *h = *(l1pmc_sys_handle_t**)cb->sys_handle;
      h->dirty = TRUE;
      h->last_addr = reg_addr;
#endif
      cb->buffer_state = BUFFER_USED_FOR_WRITE;
      cb->mem_type = mem_type;
      cb->addr = reg_addr;
      cb->mask = 0;
      cb->unused_mask = 0;
      cb->value = 0;
    }
    cb->mask |= mask;
    cb->value |= ((value & (mask >> ofs)) << ofs);
    PMC_OS_STATS_FIELD_READ_MODIFY_WRITE();
}

/*******************************************************************************
* l1sys_force_read_modify_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  forces using a read/modify/write instead of possibly doing a full register write.
*
* INPUTS:
*   cb - coaleasce buffer
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This simplifies the optimization for the compiler 
*******************************************************************************/
static INLINE void l1sys_force_read_modify_write( coalesce_buffer_t *cb ) ALWAYS_INLINE;
static INLINE void l1sys_force_read_modify_write( coalesce_buffer_t *cb )
{
    cb->buffer_flags = BUFFER_FORCE_READ_MODIFY_WRITE;
}

/*******************************************************************************
* l1sys_field_set2
* ___________________________________________________________________________
*
* DESCRIPTION:
*  update a field in the coalesce buffer.
*  This is the same as l1sys_field_set, but adds an 'unused_mask' argument.
*  If all bits in a register other than the unused bits are updated, then a 
*  full register write is performed rather than a read/modify/write.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the register in the TSB
*   mask - field mask (already shifted)
*   unused_mask - mask of which bits in the register are unused.
*   ofs - first bit set in the field mask
*   value - unshifted value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   If the coalesce buffer has been used to write to a different register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_field_set2( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value ) ALWAYS_INLINE;
static INLINE void l1sys_field_set2( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    if ((cb->buffer_state == BUFFER_USED_FOR_WRITE) && (cb->addr != reg_addr)) {
        l1sys_dirty_flush( cb );
    }
    if (!(cb->buffer_state == BUFFER_USED_FOR_WRITE)) {
#ifndef NO_IO_LOGGING
      l1pmc_sys_handle_t *h = *(l1pmc_sys_handle_t**)cb->sys_handle;
      h->dirty = TRUE;
      h->last_addr = reg_addr;
#endif
      cb->buffer_state = BUFFER_USED_FOR_WRITE;
      cb->mem_type = mem_type;
      cb->addr = reg_addr;
      cb->mask = 0;
      cb->unused_mask = 0;
      cb->value = 0;
    }
    cb->mask |= mask;
    cb->unused_mask |= unused_mask;
    cb->value |= ((value & (mask >> ofs)) << ofs);
    PMC_OS_STATS_FIELD_READ_MODIFY_WRITE();
}

/*******************************************************************************
* l1sys_action_on_write_field_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  update a field in the coalesce buffer.
*  This is the same as l1sys_field_set, but forces a full register write
*  rather than a read/modify/write.
*  bits that are not updated are written with 0's regardless of what the
*  previous value of the bits were.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the register in the TSB
*   mask - field mask (already shifted)
*   ofs - first bit set in the field mask
*   value - unshifted value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   If the coalesce buffer has been used to write to a different register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_action_on_write_field_set( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 ofs, UINT32 value ) ALWAYS_INLINE;
static INLINE void l1sys_action_on_write_field_set( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 ofs, UINT32 value )
{
    mask = mask; /* disable compiler warning */
    cb->buffer_flags = BUFFER_FORCE_WRITE;
    l1sys_field_set( cb, mem_type, tsb_base_addr, reg_num, mask, ofs, value );
}

/*******************************************************************************
* l1sys_reg_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  write to a register
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the register in the TSB
*   value - register value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   If the coalesce buffer has been used to write to a different register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_reg_write( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 value ) ALWAYS_INLINE;
static INLINE void l1sys_reg_write( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 value )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    l1sys_flush(cb);
    l2sys_reg_write( cb->sys_handle, mem_type, reg_addr, value );
}

/*******************************************************************************
* l1sys_reg_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a register
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the register in the TSB
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - register value.
*
* NOTES:
*   If the coalesce buffer has been used to write to a different register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE UINT32 l1sys_reg_read( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num ) ALWAYS_INLINE;
static INLINE UINT32 l1sys_reg_read( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    if (cb->buffer_state == BUFFER_USED_FOR_WRITE) {
        l1sys_dirty_flush( cb );
    }
    if ((cb->buffer_state != BUFFER_USED_FOR_READ) || (cb->addr != reg_addr)) {
      cb->buffer_state = BUFFER_USED_FOR_READ;
      cb->mem_type = mem_type;
      cb->addr = reg_addr;
      cb->value = l2sys_reg_read( cb->sys_handle, mem_type, reg_addr );
    }
    PMC_OS_STATS_FIELD_READ();
    return cb->value;
}

/*******************************************************************************
* l1sys_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  write a contiguous span of registers
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the beginning of the span of registers in the TSB
*   len - number of 32 bit registers to write.
*   data - data[0..(len-1)] - register values.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   If the coalesce buffer has been used to write to a register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_burst_write( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 len, UINT32 *data ) ALWAYS_INLINE;
static INLINE void l1sys_burst_write( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 len, UINT32 *data )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    l1sys_flush(cb);
    l2sys_burst_write( cb->sys_handle, mem_type, reg_addr, len, data );
}


/*******************************************************************************
* l1sys_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a contiguous span of registers
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the first register in the TSB
*   len - number of registers to read (each register is a 32 bit word)
*   data - pointer to at least 'len' UINT32's
*
* OUTPUTS:
*   *data - data[0..(len-1)] - value of each register
*
* RETURNS:
*   None
*
* NOTES:
*   If the coalesce buffer has been used to write to a register then
*   the previous write is flushed.
*   
*******************************************************************************/
static INLINE void l1sys_burst_read( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 len, UINT32 *data ) ALWAYS_INLINE;
static INLINE void l1sys_burst_read( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 len, UINT32 *data )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;
    l1sys_flush(cb);
    l2sys_burst_read( cb->sys_handle, mem_type, reg_addr, len, data );
}

/*******************************************************************************
* l1sys_poll_bit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a register until the indicated field is the specified value, or until the 
*  maximum number of reads is performed without satisfying the condition.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the first register in the TSB
*   mask - shifted field mask.
*   value - shifted value.
*   max_count - maximum number of reads before returning an PMC_SHARED_ERR_CODE_POLL_TIMEOUT
*   num_failed_polls - pointer to a UINT32 to return the number of failed polls, or NULL
*   delay_between_polls_in_microseconds - delay between polls in microseconds.
*
* OUTPUTS:
*   *num_failed_polls - number of register reads that returned failed. (if num_failed_polls is not NULL)
*
* RETURNS:
*   PMC_SUCCESS - poll succeeded
*   PMC_SHARED_ERR_CODE_POLL_TIMEOUT - poll failed
*
* NOTES:
*   If the coalesce buffer has been used to write to a register then
*   the previous write is flushed.
*
*   The successful register read is not included in the number of failed polls, so if the first
*   register read satisfies the poll condition then *num_failed_polls is 0.
*******************************************************************************/
static INLINE PMC_ERROR l1sys_poll_bit( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_ERROR l1sys_poll_bit( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;

    l1sys_flush(cb);
    return l2sys_poll_bit( cb->sys_handle, mem_type, reg_addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds );
}

/*******************************************************************************
* l1sys_poll
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a register until the field satisfies the condition, or until the 
*  maximum number of reads is performed without satisfying the condition.
*
* INPUTS:
*   cb - pointer to coalesce buffer
*   mem_type - memory type
*   tsb_base_addr - base address of the TSB
*   reg_num - offset of the first register in the TSB
*   mask - shifted field mask.
*   value - shifted value.
*   cmp - comparison type - (PMC_POLL_EQUAL, etc)
*   max_count - maximum number of reads before returning an PMC_SHARED_ERR_CODE_POLL_TIMEOUT
*   num_failed_polls - pointer to a UINT32 to return the number of failed polls or NULL
*   delay_between_polls_in_microseconds - delay between polls in microseconds.
*
* OUTPUTS:
*   *num_failed_polls - number of register reads that returned failed. (if num_failed_polls is not NULL)
*
* RETURNS:
*   PMC_SUCCESS - poll succeeded
*   PMC_SHARED_ERR_CODE_POLL_TIMEOUT - poll failed
*
* NOTES:
*   If the coalesce buffer has been used to write to a register then
*   the previous write is flushed.
*
*   The successful register read is not included in the number of failed polls, so if the first
*   register read satisfies the poll condition then *num_failed_polls is 0.
*******************************************************************************/
static INLINE PMC_POLL_RETURN_TYPE l1sys_poll( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1sys_poll( coalesce_buffer_t *cb, int mem_type, UINT32 tsb_base_addr, UINT32 reg_num, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    UINT32 reg_addr = tsb_base_addr + reg_num;

    l1sys_flush(cb);
    return l2sys_poll( cb->sys_handle, mem_type, reg_addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds );
}

#endif /* NO_COALESCING */


void l1sys_allocate(pmc_sys_handle_t *customer_sys_handle);

void l1sys_deallocate(pmc_sys_handle_t *customer_sys_handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* l1sys_H */

