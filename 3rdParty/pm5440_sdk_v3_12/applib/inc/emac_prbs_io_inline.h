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
 *     and register accessor functions for the emac_prbs block
 *****************************************************************************/
#ifndef _EMAC_PRBS_IO_INLINE_H
#define _EMAC_PRBS_IO_INLINE_H

#include "emac_loc.h"
#include "emac_prbs_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EMAC_PRBS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for emac_prbs
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
    emac_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} emac_prbs_buffer_t;
static INLINE void emac_prbs_buffer_init( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void emac_prbs_buffer_init( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "emac_prbs_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void emac_prbs_buffer_flush( emac_prbs_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void emac_prbs_buffer_flush( emac_prbs_buffer_t *b_ptr )
{
    IOLOG( "emac_prbs_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 emac_prbs_reg_read( emac_prbs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_reg_read( emac_prbs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
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
static INLINE void emac_prbs_reg_write( emac_prbs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_write( emac_prbs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
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

static INLINE void emac_prbs_field_set( emac_prbs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_set( emac_prbs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
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

static INLINE void emac_prbs_action_on_write_field_set( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_action_on_write_field_set( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
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

static INLINE void emac_prbs_burst_read( emac_prbs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_prbs_burst_read( emac_prbs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
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

static INLINE void emac_prbs_burst_write( emac_prbs_buffer_t *b_ptr,
                                          emac_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_prbs_burst_write( emac_prbs_buffer_t *b_ptr,
                                          emac_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE emac_prbs_poll( emac_prbs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_prbs_poll( emac_prbs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
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
 *  register access functions for emac_prbs
 * ==================================================================================
 */

static INLINE void emac_prbs_reg_PRBS_CTRL_write( emac_prbs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_PRBS_CTRL_write( emac_prbs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_prbs_reg_PRBS_CTRL_write", value );
    emac_prbs_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_PRBS_CTRL,
                         value);
}

static INLINE void emac_prbs_reg_PRBS_CTRL_field_set( emac_prbs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_PRBS_CTRL_field_set( emac_prbs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_prbs_reg_PRBS_CTRL_field_set", mask, ofs, value );
    emac_prbs_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_PRBS_CTRL,
                         mask,
                         PMC_PRBS_REG_PRBS_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_prbs_reg_PRBS_CTRL_read( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_reg_PRBS_CTRL_read( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_prbs_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_PRBS_REG_PRBS_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_prbs_reg_PRBS_CTRL_read", reg_value);
    return reg_value;
}

static INLINE UINT32 emac_prbs_reg_PRBS_STATUS_read( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_reg_PRBS_STATUS_read( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_prbs_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_PRBS_REG_PRBS_STATUS);

    IOLOG( "%s -> 0x%08x;", "emac_prbs_reg_PRBS_STATUS_read", reg_value);
    return reg_value;
}

static INLINE void emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_write( emac_prbs_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_write( emac_prbs_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_write", value );
    emac_prbs_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_LANE_PRBS31_ERR_CNT(N),
                         value);
}

static INLINE void emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set( emac_prbs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set( emac_prbs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set", N, mask, ofs, value );
    emac_prbs_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_LANE_PRBS31_ERR_CNT(N),
                         mask,
                         PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read( emac_prbs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read( emac_prbs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = emac_prbs_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_PRBS_REG_LANE_PRBS31_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_write( emac_prbs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_write( emac_prbs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_write", value );
    emac_prbs_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_LANE_PRBS9_ERR_CNT(N),
                         value);
}

static INLINE void emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set( emac_prbs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set( emac_prbs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set", N, mask, ofs, value );
    emac_prbs_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_PRBS_REG_LANE_PRBS9_ERR_CNT(N),
                         mask,
                         PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read( emac_prbs_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read( emac_prbs_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = emac_prbs_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_PRBS_REG_LANE_PRBS9_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void emac_prbs_field_PRBS9_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_PRBS9_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_prbs_field_PRBS9_ENA_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_prbs_field_PRBS9_ENA_set", value );

    /* (0x0000f800 bits 23:12) bits 0:11 use field PRBS9_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    emac_prbs_reg_PRBS_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_MSK,
                                       PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF,
                                       value);
}

static INLINE UINT32 emac_prbs_field_PRBS9_ENA_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_PRBS9_ENA_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000f800 bits 23:12) bits 0:11 use field PRBS9_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    reg_value = emac_prbs_reg_PRBS_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_MSK) >> PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF;
    IOLOG( "%s -> 0x%08x", "emac_prbs_field_PRBS9_ENA_get", value );

    return value;
}
static INLINE void emac_prbs_field_range_PRBS9_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_range_PRBS9_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS9_ENA_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS9_ENA_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS9_ENA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000f800 bits 23:12) bits 0:11 use field PRBS9_ENA of register PMC_PRBS_REG_PRBS_CTRL */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000f800 bits 23:12) bits 0:11 use field PRBS9_ENA of register PMC_PRBS_REG_PRBS_CTRL */
        emac_prbs_reg_PRBS_CTRL_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF + subfield_offset),
                                           PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_prbs_field_range_PRBS9_ENA_get( emac_prbs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_range_PRBS9_ENA_get( emac_prbs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS9_ENA_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS9_ENA_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000f800 bits 23:12) bits 0:11 use field PRBS9_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    reg_value = emac_prbs_reg_PRBS_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_MSK)
                  >> PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_MSK, PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS9_ENA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_prbs_field_PRBS31_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_PRBS31_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_prbs_field_PRBS31_ENA_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_prbs_field_PRBS31_ENA_set", value );

    /* (0x0000f800 bits 11:0) bits 0:11 use field PRBS31_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    emac_prbs_reg_PRBS_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_MSK,
                                       PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF,
                                       value);
}

static INLINE UINT32 emac_prbs_field_PRBS31_ENA_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_PRBS31_ENA_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000f800 bits 11:0) bits 0:11 use field PRBS31_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    reg_value = emac_prbs_reg_PRBS_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_MSK) >> PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF;
    IOLOG( "%s -> 0x%08x", "emac_prbs_field_PRBS31_ENA_get", value );

    return value;
}
static INLINE void emac_prbs_field_range_PRBS31_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_range_PRBS31_ENA_set( emac_prbs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS31_ENA_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS31_ENA_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS31_ENA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000f800 bits 11:0) bits 0:11 use field PRBS31_ENA of register PMC_PRBS_REG_PRBS_CTRL */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000f800 bits 11:0) bits 0:11 use field PRBS31_ENA of register PMC_PRBS_REG_PRBS_CTRL */
        emac_prbs_reg_PRBS_CTRL_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF + subfield_offset),
                                           PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_prbs_field_range_PRBS31_ENA_get( emac_prbs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_range_PRBS31_ENA_get( emac_prbs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS31_ENA_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS31_ENA_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000f800 bits 11:0) bits 0:11 use field PRBS31_ENA of register PMC_PRBS_REG_PRBS_CTRL */
    reg_value = emac_prbs_reg_PRBS_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_MSK)
                  >> PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_MSK, PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS31_ENA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_prbs_field_PRBS_ALL_ONES_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_PRBS_ALL_ONES_get( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000f864 bits 11:0) bits 0:11 use field PRBS_ALL_ONES of register PMC_PRBS_REG_PRBS_STATUS */
    reg_value = emac_prbs_reg_PRBS_STATUS_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_MSK) >> PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_OFF;
    IOLOG( "%s -> 0x%08x", "emac_prbs_field_PRBS_ALL_ONES_get", value );

    return value;
}
static INLINE UINT32 emac_prbs_field_range_PRBS_ALL_ONES_get( emac_prbs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_range_PRBS_ALL_ONES_get( emac_prbs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS_ALL_ONES_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS_ALL_ONES_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000f864 bits 11:0) bits 0:11 use field PRBS_ALL_ONES of register PMC_PRBS_REG_PRBS_STATUS */
    reg_value = emac_prbs_reg_PRBS_STATUS_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_MSK)
                  >> PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_MSK, PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS_ALL_ONES_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void emac_prbs_field_PRBS31_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_PRBS31_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_PRBS31_ERR_CNT_set", N, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_prbs_field_PRBS31_ERR_CNT_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "emac_prbs_field_PRBS31_ERR_CNT_set", N, value );

    /* ((0x0000f804 + (N) * 4) bits 15:0) bits 0:15 use field PRBS31_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS31_ERR_CNT index N=0..11 */
    emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK,
                                                       PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF,
                                                       value);
}

static INLINE UINT32 emac_prbs_field_PRBS31_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_PRBS31_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_PRBS31_ERR_CNT_get", N, 11);
    /* ((0x0000f804 + (N) * 4) bits 15:0) bits 0:15 use field PRBS31_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS31_ERR_CNT index N=0..11 */
    reg_value = emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK) >> PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "emac_prbs_field_PRBS31_ERR_CNT_get", N, value );

    return value;
}
static INLINE void emac_prbs_field_range_PRBS31_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_range_PRBS31_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_range_PRBS31_ERR_CNT_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS31_ERR_CNT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS31_ERR_CNT_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS31_ERR_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000f804 + (N) * 4) bits 15:0) bits 0:15 use field PRBS31_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS31_ERR_CNT index N=0..11 */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x0000f804 + (N) * 4) bits 15:0) bits 0:15 use field PRBS31_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS31_ERR_CNT index N=0..11 */
        emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF + subfield_offset),
                                                           PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_prbs_field_range_PRBS31_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_range_PRBS31_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_range_PRBS31_ERR_CNT_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS31_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS31_ERR_CNT_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000f804 + (N) * 4) bits 15:0) bits 0:15 use field PRBS31_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS31_ERR_CNT index N=0..11 */
    reg_value = emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK)
                  >> PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK, PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS31_ERR_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_prbs_field_PRBS9_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_PRBS9_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_PRBS9_ERR_CNT_set", N, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_prbs_field_PRBS9_ERR_CNT_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "emac_prbs_field_PRBS9_ERR_CNT_set", N, value );

    /* ((0x0000f834 + (N) * 4) bits 15:0) bits 0:15 use field PRBS9_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS9_ERR_CNT index N=0..11 */
    emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK,
                                                      PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF,
                                                      value);
}

static INLINE UINT32 emac_prbs_field_PRBS9_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_PRBS9_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_PRBS9_ERR_CNT_get", N, 11);
    /* ((0x0000f834 + (N) * 4) bits 15:0) bits 0:15 use field PRBS9_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS9_ERR_CNT index N=0..11 */
    reg_value = emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK) >> PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "emac_prbs_field_PRBS9_ERR_CNT_get", N, value );

    return value;
}
static INLINE void emac_prbs_field_range_PRBS9_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_prbs_field_range_PRBS9_ERR_CNT_set( emac_prbs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_range_PRBS9_ERR_CNT_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS9_ERR_CNT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS9_ERR_CNT_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS9_ERR_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000f834 + (N) * 4) bits 15:0) bits 0:15 use field PRBS9_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS9_ERR_CNT index N=0..11 */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x0000f834 + (N) * 4) bits 15:0) bits 0:15 use field PRBS9_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS9_ERR_CNT index N=0..11 */
        emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF + subfield_offset),
                                                          PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_prbs_field_range_PRBS9_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_prbs_field_range_PRBS9_ERR_CNT_get( emac_prbs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_prbs_field_range_PRBS9_ERR_CNT_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_prbs_field_range_PRBS9_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_prbs_field_range_PRBS9_ERR_CNT_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000f834 + (N) * 4) bits 15:0) bits 0:15 use field PRBS9_ERR_CNT of register PMC_PRBS_REG_LANE_PRBS9_ERR_CNT index N=0..11 */
    reg_value = emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK)
                  >> PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK, PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_prbs_field_range_PRBS9_ERR_CNT_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_PRBS_IO_INLINE_H */
