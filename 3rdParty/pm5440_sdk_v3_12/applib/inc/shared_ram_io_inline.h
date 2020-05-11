/******************************************************************************
 *   COPYRIGHT (C) 2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
 * -------------------------------------------------------------------------
 *   DESCRIPTION:
 *     contains the inline implementation of all the field accessor functions
 *     and register accessor functions for the shared_ram block
 *****************************************************************************/
#ifndef _SHARED_RAM_IO_INLINE_H
#define _SHARED_RAM_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "shared_ram_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SHARED_RAM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for shared_ram
 * ==================================================================================
 */
/* if logging is disabled then so is IO_LOGGING */
#ifndef NO_IO_LOGGING
# ifdef LOGGING_DISABLED
#  define NO_IO_LOGGING
# endif
#endif

/* log messages in the generated files log the caller's file/line/function instead of itself.*/
#ifndef IOLOG
# ifdef NO_IO_LOGGING
#  define IOLOG(...)
# else
#  define IOLOG(...) LOG( __VA_ARGS__)
# endif
#endif
#ifndef IO_RANGE_CHECK
# ifdef NO_IO_LOGGING
#  define IO_RANGE_CHECK(...)
# else
#  define IO_RANGE_CHECK(...) LOG( "** range check ** " __VA_ARGS__)
# endif
#endif
#ifndef IOLOG_DATA
# ifdef NO_IO_LOGGING
#  define IOLOG_DATA(...)
# else
#  define IOLOG_DATA(...) LOG_DATA( __VA_ARGS__)
# endif
#endif
typedef struct {
    coalesce_buffer_t coalesce_handle[1]; /* only used if register coalescing is enabled */
    lineotn_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} shared_ram_buffer_t;
static INLINE void shared_ram_buffer_init( shared_ram_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A ) ALWAYS_INLINE;
static INLINE void shared_ram_buffer_init( shared_ram_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "shared_ram_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void shared_ram_buffer_flush( shared_ram_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shared_ram_buffer_flush( shared_ram_buffer_t *b_ptr )
{
    IOLOG( "shared_ram_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 shared_ram_reg_read( shared_ram_buffer_t *b_ptr,
                                          lineotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 shared_ram_reg_read( shared_ram_buffer_t *b_ptr,
                                          lineotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg);
    return value;
}
static INLINE void shared_ram_reg_write( shared_ram_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_reg_write( shared_ram_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value);
}

static INLINE void shared_ram_field_set( shared_ram_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_set( shared_ram_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_field_set2( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          mask,
                          unused_mask,
                          ofs,
                          value);
    else
    {
        if ((mask | unused_mask) == 0xffffffff)
        {
            l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                             mem_type,
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void shared_ram_action_on_write_field_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_action_on_write_field_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void shared_ram_burst_read( shared_ram_buffer_t *b_ptr,
                                          lineotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void shared_ram_burst_read( shared_ram_buffer_t *b_ptr,
                                          lineotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void shared_ram_burst_write( shared_ram_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void shared_ram_burst_write( shared_ram_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE shared_ram_poll( shared_ram_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE shared_ram_poll( shared_ram_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds )
{
    if (b_ptr != NULL)
        return l1sys_poll( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
    else
        return l2sys_poll( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for shared_ram
 * ==================================================================================
 */

static INLINE void shared_ram_reg_MASTER_CONFIG_write( shared_ram_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_reg_MASTER_CONFIG_write( shared_ram_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "shared_ram_reg_MASTER_CONFIG_write", value );
    shared_ram_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_SHARED_RAM_REG_MASTER_CONFIG,
                          value);
}

static INLINE void shared_ram_reg_MASTER_CONFIG_field_set( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_reg_MASTER_CONFIG_field_set( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "shared_ram_reg_MASTER_CONFIG_field_set", A, mask, ofs, value );
    shared_ram_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_SHARED_RAM_REG_MASTER_CONFIG,
                          mask,
                          PMC_SHARED_RAM_REG_MASTER_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 shared_ram_reg_MASTER_CONFIG_read( shared_ram_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_reg_MASTER_CONFIG_read( shared_ram_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = shared_ram_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_CONFIG,
                                     PMC_SHARED_RAM_REG_MASTER_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "shared_ram_reg_MASTER_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void shared_ram_reg_LOWPWR_CONFIG_write( shared_ram_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_reg_LOWPWR_CONFIG_write( shared_ram_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "shared_ram_reg_LOWPWR_CONFIG_write", value );
    shared_ram_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_SHARED_RAM_REG_LOWPWR_CONFIG,
                          value);
}

static INLINE void shared_ram_reg_LOWPWR_CONFIG_field_set( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_reg_LOWPWR_CONFIG_field_set( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "shared_ram_reg_LOWPWR_CONFIG_field_set", A, mask, ofs, value );
    shared_ram_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_SHARED_RAM_REG_LOWPWR_CONFIG,
                          mask,
                          PMC_SHARED_RAM_REG_LOWPWR_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 shared_ram_reg_LOWPWR_CONFIG_read( shared_ram_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_reg_LOWPWR_CONFIG_read( shared_ram_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = shared_ram_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_CONFIG,
                                     PMC_SHARED_RAM_REG_LOWPWR_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "shared_ram_reg_LOWPWR_CONFIG_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void shared_ram_field_FEC_SELECT_set( shared_ram_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_FEC_SELECT_set( shared_ram_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_FEC_SELECT_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_FEC_SELECT_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_FEC_SELECT_set", A, value );

    /* (0x00001000 bits 4:1) bits 0:3 use field FEC_SELECT of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    shared_ram_reg_MASTER_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_MSK,
                                            SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_FEC_SELECT_get( shared_ram_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_FEC_SELECT_get( shared_ram_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_FEC_SELECT_get", A, 2);
    /* (0x00001000 bits 4:1) bits 0:3 use field FEC_SELECT of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    reg_value = shared_ram_reg_MASTER_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_MSK) >> SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_FEC_SELECT_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_FEC_SELECT_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_FEC_SELECT_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_FEC_SELECT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_FEC_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_FEC_SELECT_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_FEC_SELECT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001000 bits 4:1) bits 0:3 use field FEC_SELECT of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001000 bits 4:1) bits 0:3 use field FEC_SELECT of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
        shared_ram_reg_MASTER_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF + subfield_offset),
                                                SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_FEC_SELECT_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_FEC_SELECT_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_FEC_SELECT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_FEC_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_FEC_SELECT_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001000 bits 4:1) bits 0:3 use field FEC_SELECT of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    reg_value = shared_ram_reg_MASTER_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_MSK)
                  >> SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_MSK, SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_FEC_SELECT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_RESET_set( shared_ram_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_RESET_set( shared_ram_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_RESET_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_RESET_set", A, value );

    /* (0x00001000 bits 0 ) field RESET of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    shared_ram_reg_MASTER_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_MSK,
                                            SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_RESET_get( shared_ram_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_RESET_get( shared_ram_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_RESET_get", A, 2);
    /* (0x00001000 bits 0 ) field RESET of register PMC_SHARED_RAM_REG_MASTER_CONFIG */
    reg_value = shared_ram_reg_MASTER_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_MSK) >> SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_RESET_get", A, value );

    return value;
}
static INLINE void shared_ram_field_REGION_F3_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_F3_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F3_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_F3_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_F3_LOWPWR_set", A, value );

    /* (0x00001028 bits 31:28) bits 0:3 use field REGION_F3_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_F3_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_F3_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F3_LOWPWR_get", A, 2);
    /* (0x00001028 bits 31:28) bits 0:3 use field REGION_F3_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_F3_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_F3_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_F3_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F3_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F3_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F3_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F3_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 31:28) bits 0:3 use field REGION_F3_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 31:28) bits 0:3 use field REGION_F3_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_F3_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_F3_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F3_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F3_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F3_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 31:28) bits 0:3 use field REGION_F3_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F3_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_F2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_F2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F2_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_F2_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_F2_LOWPWR_set", A, value );

    /* (0x00001028 bits 27:24) bits 0:3 use field REGION_F2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_F2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_F2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F2_LOWPWR_get", A, 2);
    /* (0x00001028 bits 27:24) bits 0:3 use field REGION_F2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_F2_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_F2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_F2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F2_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F2_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F2_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F2_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 27:24) bits 0:3 use field REGION_F2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 27:24) bits 0:3 use field REGION_F2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_F2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_F2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F2_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F2_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F2_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 27:24) bits 0:3 use field REGION_F2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F2_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_F1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_F1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F1_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_F1_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_F1_LOWPWR_set", A, value );

    /* (0x00001028 bits 23:20) bits 0:3 use field REGION_F1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_F1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_F1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_F1_LOWPWR_get", A, 2);
    /* (0x00001028 bits 23:20) bits 0:3 use field REGION_F1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_F1_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_F1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_F1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F1_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F1_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F1_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F1_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 23:20) bits 0:3 use field REGION_F1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 23:20) bits 0:3 use field REGION_F1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_F1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_F1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_F1_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_F1_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_F1_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 23:20) bits 0:3 use field REGION_F1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_F1_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_E_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_E_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_E_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_E_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_E_LOWPWR_set", A, value );

    /* (0x00001028 bits 19:16) bits 0:3 use field REGION_E_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_E_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_E_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_E_LOWPWR_get", A, 2);
    /* (0x00001028 bits 19:16) bits 0:3 use field REGION_E_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_E_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_E_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_E_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_E_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_E_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_E_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_E_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 19:16) bits 0:3 use field REGION_E_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 19:16) bits 0:3 use field REGION_E_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_E_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_E_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_E_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_E_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_E_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 19:16) bits 0:3 use field REGION_E_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_E_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_D2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_D2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_D2_LOWPWR_set", A, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_D2_LOWPWR_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_D2_LOWPWR_set", A, value );

    /* (0x00001028 bits 15:13) bits 0:2 use field REGION_D2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_D2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_D2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_D2_LOWPWR_get", A, 2);
    /* (0x00001028 bits 15:13) bits 0:2 use field REGION_D2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_D2_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_D2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_D2_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_D2_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_D2_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_D2_LOWPWR_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_D2_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 15:13) bits 0:2 use field REGION_D2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 15:13) bits 0:2 use field REGION_D2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_D2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_D2_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_D2_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_D2_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_D2_LOWPWR_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 15:13) bits 0:2 use field REGION_D2_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_D2_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_D1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_D1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_D1_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_D1_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_D1_LOWPWR_set", A, value );

    /* (0x00001028 bits 12:9) bits 0:3 use field REGION_D1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_D1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_D1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_D1_LOWPWR_get", A, 2);
    /* (0x00001028 bits 12:9) bits 0:3 use field REGION_D1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_D1_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_D1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_D1_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_D1_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_D1_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_D1_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_D1_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 12:9) bits 0:3 use field REGION_D1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 12:9) bits 0:3 use field REGION_D1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_D1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_D1_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_D1_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_D1_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_D1_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 12:9) bits 0:3 use field REGION_D1_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_D1_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_C_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_C_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_C_LOWPWR_set", A, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_C_LOWPWR_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_C_LOWPWR_set", A, value );

    /* (0x00001028 bits 8:6) bits 0:2 use field REGION_C_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_C_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_C_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_C_LOWPWR_get", A, 2);
    /* (0x00001028 bits 8:6) bits 0:2 use field REGION_C_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_C_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_C_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_C_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_C_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_C_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_C_LOWPWR_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_C_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 8:6) bits 0:2 use field REGION_C_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 8:6) bits 0:2 use field REGION_C_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_C_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_C_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_C_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_C_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_C_LOWPWR_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 8:6) bits 0:2 use field REGION_C_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_C_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_B_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_B_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_B_LOWPWR_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_B_LOWPWR_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_B_LOWPWR_set", A, value );

    /* (0x00001028 bits 5:4) bits 0:1 use field REGION_B_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_B_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_B_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_B_LOWPWR_get", A, 2);
    /* (0x00001028 bits 5:4) bits 0:1 use field REGION_B_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_B_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_B_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_B_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_B_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_B_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_B_LOWPWR_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_B_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 5:4) bits 0:1 use field REGION_B_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 5:4) bits 0:1 use field REGION_B_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_B_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_B_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_B_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_B_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_B_LOWPWR_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 5:4) bits 0:1 use field REGION_B_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_B_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void shared_ram_field_REGION_A_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_REGION_A_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_A_LOWPWR_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shared_ram_field_REGION_A_LOWPWR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "shared_ram_field_REGION_A_LOWPWR_set", A, value );

    /* (0x00001028 bits 3:0) bits 0:3 use field REGION_A_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_MSK,
                                            SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 shared_ram_field_REGION_A_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_REGION_A_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_REGION_A_LOWPWR_get", A, 2);
    /* (0x00001028 bits 3:0) bits 0:3 use field REGION_A_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_MSK) >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "shared_ram_field_REGION_A_LOWPWR_get", A, value );

    return value;
}
static INLINE void shared_ram_field_range_REGION_A_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void shared_ram_field_range_REGION_A_LOWPWR_set( shared_ram_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_A_LOWPWR_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_A_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_A_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_A_LOWPWR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001028 bits 3:0) bits 0:3 use field REGION_A_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001028 bits 3:0) bits 0:3 use field REGION_A_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
        shared_ram_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF + subfield_offset),
                                                SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 shared_ram_field_range_REGION_A_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shared_ram_field_range_REGION_A_LOWPWR_get( shared_ram_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "shared_ram_field_range_REGION_A_LOWPWR_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "shared_ram_field_range_REGION_A_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "shared_ram_field_range_REGION_A_LOWPWR_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001028 bits 3:0) bits 0:3 use field REGION_A_LOWPWR of register PMC_SHARED_RAM_REG_LOWPWR_CONFIG */
    reg_value = shared_ram_reg_LOWPWR_CONFIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_MSK)
                  >> SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_MSK, SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "shared_ram_field_range_REGION_A_LOWPWR_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHARED_RAM_IO_INLINE_H */
