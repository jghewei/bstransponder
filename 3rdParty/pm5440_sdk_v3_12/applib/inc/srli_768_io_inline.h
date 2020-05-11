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
 *     and register accessor functions for the srli_768 block
 *****************************************************************************/
#ifndef _SRLI_768_IO_INLINE_H
#define _SRLI_768_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "srli_768_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SRLI_768_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for srli_768
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
    sdh_pmg_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} srli_768_buffer_t;
static INLINE void srli_768_buffer_init( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void srli_768_buffer_init( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "srli_768_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void srli_768_buffer_flush( srli_768_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void srli_768_buffer_flush( srli_768_buffer_t *b_ptr )
{
    IOLOG( "srli_768_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 srli_768_reg_read( srli_768_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 srli_768_reg_read( srli_768_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                b_ptr->base_address,
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                ((pmc_handle_t *)h_ptr)->base_address + reg);
    return value;
}
static INLINE void srli_768_reg_write( srli_768_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_reg_write( srli_768_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         b_ptr->base_address,
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                         value);
}

static INLINE void srli_768_field_set( srli_768_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_field_set( srli_768_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
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
                          b_ptr->base_address,
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
                             ((pmc_handle_t *)h_ptr)->base_address + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void srli_768_action_on_write_field_set( srli_768_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_action_on_write_field_set( srli_768_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         b_ptr->base_address,
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                         value<<ofs);
}

static INLINE void srli_768_burst_read( srli_768_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void srli_768_burst_read( srli_768_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          b_ptr->base_address,
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          ((pmc_handle_t *)h_ptr)->base_address + reg,
                          len,
                          value);
}

static INLINE void srli_768_burst_write( srli_768_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void srli_768_burst_write( srli_768_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           b_ptr->base_address,
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           ((pmc_handle_t *)h_ptr)->base_address + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE srli_768_poll( srli_768_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE srli_768_poll( srli_768_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
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
                           b_ptr->base_address,
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
                           ((pmc_handle_t *)h_ptr)->base_address + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for srli_768
 * ==================================================================================
 */

static INLINE void srli_768_reg_PGM_CLK_CFG_write( srli_768_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_reg_PGM_CLK_CFG_write( srli_768_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "srli_768_reg_PGM_CLK_CFG_write", value );
    srli_768_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SRLI_768_REG_PGM_CLK_CFG,
                        value);
}

static INLINE void srli_768_reg_PGM_CLK_CFG_field_set( srli_768_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_reg_PGM_CLK_CFG_field_set( srli_768_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "srli_768_reg_PGM_CLK_CFG_field_set", mask, ofs, value );
    srli_768_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SRLI_768_REG_PGM_CLK_CFG,
                        mask,
                        PMC_SRLI_768_REG_PGM_CLK_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 srli_768_reg_PGM_CLK_CFG_read( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 srli_768_reg_PGM_CLK_CFG_read( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = srli_768_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SRLI_768_REG_PGM_CLK_CFG);

    IOLOG( "%s -> 0x%08x;", "srli_768_reg_PGM_CLK_CFG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void srli_768_field_INTERLEAVEEN_set( srli_768_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_field_INTERLEAVEEN_set( srli_768_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "srli_768_field_INTERLEAVEEN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "srli_768_field_INTERLEAVEEN_set", value );

    /* (0x00000080 bits 15:12) bits 0:3 use field INTERLEAVEEN of register PMC_SRLI_768_REG_PGM_CLK_CFG */
    srli_768_reg_PGM_CLK_CFG_field_set( b_ptr,
                                        h_ptr,
                                        SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_MSK,
                                        SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF,
                                        value);
}

static INLINE UINT32 srli_768_field_INTERLEAVEEN_get( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 srli_768_field_INTERLEAVEEN_get( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 15:12) bits 0:3 use field INTERLEAVEEN of register PMC_SRLI_768_REG_PGM_CLK_CFG */
    reg_value = srli_768_reg_PGM_CLK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_MSK) >> SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF;
    IOLOG( "%s -> 0x%08x", "srli_768_field_INTERLEAVEEN_get", value );

    return value;
}
static INLINE void srli_768_field_range_INTERLEAVEEN_set( srli_768_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void srli_768_field_range_INTERLEAVEEN_set( srli_768_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "srli_768_field_range_INTERLEAVEEN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "srli_768_field_range_INTERLEAVEEN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "srli_768_field_range_INTERLEAVEEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 15:12) bits 0:3 use field INTERLEAVEEN of register PMC_SRLI_768_REG_PGM_CLK_CFG */
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
        /* (0x00000080 bits 15:12) bits 0:3 use field INTERLEAVEEN of register PMC_SRLI_768_REG_PGM_CLK_CFG */
        srli_768_reg_PGM_CLK_CFG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF + subfield_offset),
                                            SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 srli_768_field_range_INTERLEAVEEN_get( srli_768_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 srli_768_field_range_INTERLEAVEEN_get( srli_768_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "srli_768_field_range_INTERLEAVEEN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "srli_768_field_range_INTERLEAVEEN_get", stop_bit, 3 );
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
    /* (0x00000080 bits 15:12) bits 0:3 use field INTERLEAVEEN of register PMC_SRLI_768_REG_PGM_CLK_CFG */
    reg_value = srli_768_reg_PGM_CLK_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_MSK)
                  >> SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_MSK, SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "srli_768_field_range_INTERLEAVEEN_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SRLI_768_IO_INLINE_H */
