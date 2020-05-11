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
 *     and register accessor functions for the pmpm block
 *****************************************************************************/
#ifndef _PMPM_IO_INLINE_H
#define _PMPM_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmpm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PMPM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for pmpm
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
    cpb_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} pmpm_buffer_t;
static INLINE void pmpm_buffer_init( pmpm_buffer_t *b_ptr,
                                     cpb_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void pmpm_buffer_init( pmpm_buffer_t *b_ptr,
                                     cpb_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "pmpm_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void pmpm_buffer_flush( pmpm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void pmpm_buffer_flush( pmpm_buffer_t *b_ptr )
{
    IOLOG( "pmpm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 pmpm_reg_read( pmpm_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 pmpm_reg_read( pmpm_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
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
static INLINE void pmpm_reg_write( pmpm_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_reg_write( pmpm_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
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

static INLINE void pmpm_field_set( pmpm_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_field_set( pmpm_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
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

static INLINE void pmpm_action_on_write_field_set( pmpm_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_action_on_write_field_set( pmpm_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
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

static INLINE void pmpm_burst_read( pmpm_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void pmpm_burst_read( pmpm_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
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

static INLINE void pmpm_burst_write( pmpm_buffer_t *b_ptr,
                                     cpb_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void pmpm_burst_write( pmpm_buffer_t *b_ptr,
                                     cpb_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE pmpm_poll( pmpm_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pmpm_poll( pmpm_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
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
 *  register access functions for pmpm
 * ==================================================================================
 */

static INLINE void pmpm_reg_RAM_POWER_CONFIG_array_write( pmpm_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_reg_RAM_POWER_CONFIG_array_write( pmpm_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pmpm_reg_RAM_POWER_CONFIG_array_write", value );
    pmpm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PMPM_REG_RAM_POWER_CONFIG(N),
                    value);
}

static INLINE void pmpm_reg_RAM_POWER_CONFIG_array_field_set( pmpm_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_reg_RAM_POWER_CONFIG_array_field_set( pmpm_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "pmpm_reg_RAM_POWER_CONFIG_array_field_set", A, N, mask, ofs, value );
    pmpm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PMPM_REG_RAM_POWER_CONFIG(N),
                    mask,
                    PMC_PMPM_REG_RAM_POWER_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 pmpm_reg_RAM_POWER_CONFIG_array_read( pmpm_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pmpm_reg_RAM_POWER_CONFIG_array_read( pmpm_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = pmpm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PMPM_REG_RAM_POWER_CONFIG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "pmpm_reg_RAM_POWER_CONFIG_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size32
 * ==================================================================================
 */
static INLINE void pmpm_field_PWR_MODE_set( pmpm_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_field_PWR_MODE_set( pmpm_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_PWR_MODE_set", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_PWR_MODE_set", N, 31);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pmpm_field_PWR_MODE_set", value, 3);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "pmpm_field_PWR_MODE_set", A, N, value );

    /* ((0x00068100 + (N) * 0x04) bits 17:16) field PWR_MODE of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    pmpm_reg_RAM_POWER_CONFIG_array_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               N,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_MSK,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF,
                                               value);
}

static INLINE UINT32 pmpm_field_PWR_MODE_get( pmpm_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pmpm_field_PWR_MODE_get( pmpm_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_PWR_MODE_get", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_PWR_MODE_get", N, 31);
    /* ((0x00068100 + (N) * 0x04) bits 17:16) field PWR_MODE of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    reg_value = pmpm_reg_RAM_POWER_CONFIG_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_MSK) >> PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "pmpm_field_PWR_MODE_get", A, N, value );

    return value;
}
static INLINE void pmpm_field_range_PWR_MODE_set( pmpm_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_field_range_PWR_MODE_set( pmpm_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_range_PWR_MODE_set", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_range_PWR_MODE_set", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pmpm_field_range_PWR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pmpm_field_range_PWR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "pmpm_field_range_PWR_MODE_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00068100 + (N) * 0x04) bits 17:16) field PWR_MODE of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
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
        /* ((0x00068100 + (N) * 0x04) bits 17:16) field PWR_MODE of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
        pmpm_reg_RAM_POWER_CONFIG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   subfield_mask << (PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF + subfield_offset),
                                                   PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 pmpm_field_range_PWR_MODE_get( pmpm_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pmpm_field_range_PWR_MODE_get( pmpm_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_range_PWR_MODE_get", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_range_PWR_MODE_get", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pmpm_field_range_PWR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pmpm_field_range_PWR_MODE_get", stop_bit, 1 );
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
    /* ((0x00068100 + (N) * 0x04) bits 17:16) field PWR_MODE of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    reg_value = pmpm_reg_RAM_POWER_CONFIG_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    field_value = (reg_value & PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_MSK)
                  >> PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_MSK, PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "pmpm_field_range_PWR_MODE_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void pmpm_field_DYNAMIC_ADDR_LS_EN_set( pmpm_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_field_DYNAMIC_ADDR_LS_EN_set( pmpm_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_DYNAMIC_ADDR_LS_EN_set", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_DYNAMIC_ADDR_LS_EN_set", N, 31);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pmpm_field_DYNAMIC_ADDR_LS_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "pmpm_field_DYNAMIC_ADDR_LS_EN_set", A, N, value );

    /* ((0x00068100 + (N) * 0x04) bits 9) field DYNAMIC_ADDR_LS_EN of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    pmpm_reg_RAM_POWER_CONFIG_array_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               N,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_MSK,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_OFF,
                                               value);
}

static INLINE UINT32 pmpm_field_DYNAMIC_ADDR_LS_EN_get( pmpm_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pmpm_field_DYNAMIC_ADDR_LS_EN_get( pmpm_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_DYNAMIC_ADDR_LS_EN_get", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_DYNAMIC_ADDR_LS_EN_get", N, 31);
    /* ((0x00068100 + (N) * 0x04) bits 9) field DYNAMIC_ADDR_LS_EN of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    reg_value = pmpm_reg_RAM_POWER_CONFIG_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_MSK) >> PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "pmpm_field_DYNAMIC_ADDR_LS_EN_get", A, N, value );

    return value;
}
static INLINE void pmpm_field_DYNAMIC_LS_EN_set( pmpm_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void pmpm_field_DYNAMIC_LS_EN_set( pmpm_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_DYNAMIC_LS_EN_set", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_DYNAMIC_LS_EN_set", N, 31);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pmpm_field_DYNAMIC_LS_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "pmpm_field_DYNAMIC_LS_EN_set", A, N, value );

    /* ((0x00068100 + (N) * 0x04) bits 8) field DYNAMIC_LS_EN of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    pmpm_reg_RAM_POWER_CONFIG_array_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               N,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_MSK,
                                               PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_OFF,
                                               value);
}

static INLINE UINT32 pmpm_field_DYNAMIC_LS_EN_get( pmpm_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pmpm_field_DYNAMIC_LS_EN_get( pmpm_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "pmpm_field_DYNAMIC_LS_EN_get", A, 2);
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pmpm_field_DYNAMIC_LS_EN_get", N, 31);
    /* ((0x00068100 + (N) * 0x04) bits 8) field DYNAMIC_LS_EN of register PMC_PMPM_REG_RAM_POWER_CONFIG index N=0..31 */
    reg_value = pmpm_reg_RAM_POWER_CONFIG_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_MSK) >> PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "pmpm_field_DYNAMIC_LS_EN_get", A, N, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PMPM_IO_INLINE_H */
