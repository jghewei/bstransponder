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
 *     and register accessor functions for the cpact_obuf block
 *****************************************************************************/
#ifndef _CPACT_OBUF_IO_INLINE_H
#define _CPACT_OBUF_IO_INLINE_H

#include "cpact_obuf_loc.h"
#include "cpact_obuf_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CPACT_OBUF_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cpact_obuf
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
    cpact_obuf_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cpact_obuf_buffer_t;
static INLINE void cpact_obuf_buffer_init( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cpact_obuf_buffer_init( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cpact_obuf_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cpact_obuf_buffer_flush( cpact_obuf_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cpact_obuf_buffer_flush( cpact_obuf_buffer_t *b_ptr )
{
    IOLOG( "cpact_obuf_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cpact_obuf_reg_read( cpact_obuf_buffer_t *b_ptr,
                                          cpact_obuf_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_reg_read( cpact_obuf_buffer_t *b_ptr,
                                          cpact_obuf_handle_t *h_ptr,
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
static INLINE void cpact_obuf_reg_write( cpact_obuf_buffer_t *b_ptr,
                                         cpact_obuf_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_write( cpact_obuf_buffer_t *b_ptr,
                                         cpact_obuf_handle_t *h_ptr,
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

static INLINE void cpact_obuf_field_set( cpact_obuf_buffer_t *b_ptr,
                                         cpact_obuf_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_set( cpact_obuf_buffer_t *b_ptr,
                                         cpact_obuf_handle_t *h_ptr,
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

static INLINE void cpact_obuf_action_on_write_field_set( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_action_on_write_field_set( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
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

static INLINE void cpact_obuf_burst_read( cpact_obuf_buffer_t *b_ptr,
                                          cpact_obuf_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_burst_read( cpact_obuf_buffer_t *b_ptr,
                                          cpact_obuf_handle_t *h_ptr,
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

static INLINE void cpact_obuf_burst_write( cpact_obuf_buffer_t *b_ptr,
                                           cpact_obuf_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_burst_write( cpact_obuf_buffer_t *b_ptr,
                                           cpact_obuf_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cpact_obuf_poll( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cpact_obuf_poll( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
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
 *  register access functions for cpact_obuf
 * ==================================================================================
 */

static INLINE void cpact_obuf_reg_CPACT_CHNL_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                              cpact_obuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_CPACT_CHNL_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                              cpact_obuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpact_obuf_reg_CPACT_CHNL_CFG_array_write", value );
    cpact_obuf_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG(N),
                          value);
}

static INLINE void cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                                  cpact_obuf_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                                  cpact_obuf_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set", N, mask, ofs, value );
    cpact_obuf_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG(N),
                          mask,
                          PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 cpact_obuf_reg_CPACT_CHNL_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                               cpact_obuf_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_reg_CPACT_CHNL_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                               cpact_obuf_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = cpact_obuf_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cpact_obuf_reg_CPACT_CHNL_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void cpact_obuf_reg_SOMF_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_SOMF_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpact_obuf_reg_SOMF_CFG_array_write", value );
    cpact_obuf_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_SOMF_CFG(N),
                          value);
}

static INLINE void cpact_obuf_reg_SOMF_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_SOMF_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cpact_obuf_reg_SOMF_CFG_array_field_set", N, mask, ofs, value );
    cpact_obuf_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_SOMF_CFG(N),
                          mask,
                          PMC_CPACT_OBUF_REG_SOMF_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 cpact_obuf_reg_SOMF_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_reg_SOMF_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = cpact_obuf_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_CPACT_OBUF_REG_SOMF_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cpact_obuf_reg_SOMF_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void cpact_obuf_reg_EOMF_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_EOMF_CFG_array_write( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpact_obuf_reg_EOMF_CFG_array_write", value );
    cpact_obuf_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_EOMF_CFG(N),
                          value);
}

static INLINE void cpact_obuf_reg_EOMF_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_reg_EOMF_CFG_array_field_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cpact_obuf_reg_EOMF_CFG_array_field_set", N, mask, ofs, value );
    cpact_obuf_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CPACT_OBUF_REG_EOMF_CFG(N),
                          mask,
                          PMC_CPACT_OBUF_REG_EOMF_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 cpact_obuf_reg_EOMF_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_reg_EOMF_CFG_array_read( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = cpact_obuf_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_CPACT_OBUF_REG_EOMF_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cpact_obuf_reg_EOMF_CFG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void cpact_obuf_field_CH_MAP_TYPE_set( cpact_obuf_buffer_t *b_ptr,
                                                     cpact_obuf_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_CH_MAP_TYPE_set( cpact_obuf_buffer_t *b_ptr,
                                                     cpact_obuf_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_CH_MAP_TYPE_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpact_obuf_field_CH_MAP_TYPE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "cpact_obuf_field_CH_MAP_TYPE_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 2:0) field CH_MAP_TYPE of register PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG index N=0..95 */
    cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_MSK,
                                                   CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF,
                                                   value);
}

static INLINE UINT32 cpact_obuf_field_CH_MAP_TYPE_get( cpact_obuf_buffer_t *b_ptr,
                                                       cpact_obuf_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_CH_MAP_TYPE_get( cpact_obuf_buffer_t *b_ptr,
                                                       cpact_obuf_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_CH_MAP_TYPE_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 2:0) field CH_MAP_TYPE of register PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_CPACT_CHNL_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_MSK) >> CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cpact_obuf_field_CH_MAP_TYPE_get", N, value );

    return value;
}
static INLINE void cpact_obuf_field_range_CH_MAP_TYPE_set( cpact_obuf_buffer_t *b_ptr,
                                                           cpact_obuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_range_CH_MAP_TYPE_set( cpact_obuf_buffer_t *b_ptr,
                                                           cpact_obuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_CH_MAP_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_CH_MAP_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_CH_MAP_TYPE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_CH_MAP_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x10) bits 2:0) field CH_MAP_TYPE of register PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG index N=0..95 */
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
        /* ((0x00000000 + (N) * 0x10) bits 2:0) field CH_MAP_TYPE of register PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG index N=0..95 */
        cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF + subfield_offset),
                                                       CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 cpact_obuf_field_range_CH_MAP_TYPE_get( cpact_obuf_buffer_t *b_ptr,
                                                             cpact_obuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_range_CH_MAP_TYPE_get( cpact_obuf_buffer_t *b_ptr,
                                                             cpact_obuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_CH_MAP_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_CH_MAP_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_CH_MAP_TYPE_get", stop_bit, 2 );
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
    /* ((0x00000000 + (N) * 0x10) bits 2:0) field CH_MAP_TYPE of register PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_CPACT_CHNL_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_MSK)
                  >> CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_MSK, CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_CH_MAP_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void cpact_obuf_field_SOMF_FIRST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                       cpact_obuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_SOMF_FIRST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                       cpact_obuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_SOMF_FIRST_DT_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpact_obuf_field_SOMF_FIRST_DT_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "cpact_obuf_field_SOMF_FIRST_DT_set", N, value );

    /* ((0x00000004 + (N) * 0x10) bits 21:16) field SOMF_FIRST_DT of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    cpact_obuf_reg_SOMF_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_MSK,
                                             CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF,
                                             value);
}

static INLINE UINT32 cpact_obuf_field_SOMF_FIRST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_SOMF_FIRST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                         cpact_obuf_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_SOMF_FIRST_DT_get", N, 95);
    /* ((0x00000004 + (N) * 0x10) bits 21:16) field SOMF_FIRST_DT of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_SOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_MSK) >> CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cpact_obuf_field_SOMF_FIRST_DT_get", N, value );

    return value;
}
static INLINE void cpact_obuf_field_range_SOMF_FIRST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                             cpact_obuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_range_SOMF_FIRST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                             cpact_obuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_SOMF_FIRST_DT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_SOMF_FIRST_DT_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_SOMF_FIRST_DT_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_SOMF_FIRST_DT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x10) bits 21:16) field SOMF_FIRST_DT of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000004 + (N) * 0x10) bits 21:16) field SOMF_FIRST_DT of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
        cpact_obuf_reg_SOMF_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF + subfield_offset),
                                                 CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 cpact_obuf_field_range_SOMF_FIRST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                               cpact_obuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_range_SOMF_FIRST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                               cpact_obuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_SOMF_FIRST_DT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_SOMF_FIRST_DT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_SOMF_FIRST_DT_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000004 + (N) * 0x10) bits 21:16) field SOMF_FIRST_DT of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_SOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_MSK)
                  >> CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_MSK, CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_SOMF_FIRST_DT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void cpact_obuf_field_SOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_SOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_SOMF_CYCLE_set", N, 95);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpact_obuf_field_SOMF_CYCLE_set", value, 32767);
    IOLOG( "%s <= N=%d 0x%08x", "cpact_obuf_field_SOMF_CYCLE_set", N, value );

    /* ((0x00000004 + (N) * 0x10) bits 14:0) field SOMF_CYCLE of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    cpact_obuf_reg_SOMF_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_MSK,
                                             CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF,
                                             value);
}

static INLINE UINT32 cpact_obuf_field_SOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_SOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_SOMF_CYCLE_get", N, 95);
    /* ((0x00000004 + (N) * 0x10) bits 14:0) field SOMF_CYCLE of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_SOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_MSK) >> CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cpact_obuf_field_SOMF_CYCLE_get", N, value );

    return value;
}
static INLINE void cpact_obuf_field_range_SOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                          cpact_obuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_range_SOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                          cpact_obuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_SOMF_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_SOMF_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_SOMF_CYCLE_set", stop_bit, 14 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_SOMF_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x10) bits 14:0) field SOMF_CYCLE of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000004 + (N) * 0x10) bits 14:0) field SOMF_CYCLE of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
        cpact_obuf_reg_SOMF_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF + subfield_offset),
                                                 CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 cpact_obuf_field_range_SOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_range_SOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_SOMF_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_SOMF_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_SOMF_CYCLE_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000004 + (N) * 0x10) bits 14:0) field SOMF_CYCLE of register PMC_CPACT_OBUF_REG_SOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_SOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_MSK)
                  >> CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_MSK, CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_SOMF_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void cpact_obuf_field_EOMF_LAST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_EOMF_LAST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_EOMF_LAST_DT_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpact_obuf_field_EOMF_LAST_DT_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "cpact_obuf_field_EOMF_LAST_DT_set", N, value );

    /* ((0x00000008 + (N) * 0x10) bits 21:16) field EOMF_LAST_DT of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    cpact_obuf_reg_EOMF_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_MSK,
                                             CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF,
                                             value);
}

static INLINE UINT32 cpact_obuf_field_EOMF_LAST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_EOMF_LAST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                        cpact_obuf_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_EOMF_LAST_DT_get", N, 95);
    /* ((0x00000008 + (N) * 0x10) bits 21:16) field EOMF_LAST_DT of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_EOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_MSK) >> CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cpact_obuf_field_EOMF_LAST_DT_get", N, value );

    return value;
}
static INLINE void cpact_obuf_field_range_EOMF_LAST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_range_EOMF_LAST_DT_set( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_EOMF_LAST_DT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_EOMF_LAST_DT_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_EOMF_LAST_DT_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_EOMF_LAST_DT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000008 + (N) * 0x10) bits 21:16) field EOMF_LAST_DT of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000008 + (N) * 0x10) bits 21:16) field EOMF_LAST_DT of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
        cpact_obuf_reg_EOMF_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF + subfield_offset),
                                                 CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 cpact_obuf_field_range_EOMF_LAST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                              cpact_obuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_range_EOMF_LAST_DT_get( cpact_obuf_buffer_t *b_ptr,
                                                              cpact_obuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_EOMF_LAST_DT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_EOMF_LAST_DT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_EOMF_LAST_DT_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000008 + (N) * 0x10) bits 21:16) field EOMF_LAST_DT of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_EOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_MSK)
                  >> CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_MSK, CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_EOMF_LAST_DT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void cpact_obuf_field_EOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_EOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                    cpact_obuf_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_EOMF_CYCLE_set", N, 95);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpact_obuf_field_EOMF_CYCLE_set", value, 32767);
    IOLOG( "%s <= N=%d 0x%08x", "cpact_obuf_field_EOMF_CYCLE_set", N, value );

    /* ((0x00000008 + (N) * 0x10) bits 14:0) field EOMF_CYCLE of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    cpact_obuf_reg_EOMF_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_MSK,
                                             CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF,
                                             value);
}

static INLINE UINT32 cpact_obuf_field_EOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_EOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                      cpact_obuf_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_EOMF_CYCLE_get", N, 95);
    /* ((0x00000008 + (N) * 0x10) bits 14:0) field EOMF_CYCLE of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_EOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_MSK) >> CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cpact_obuf_field_EOMF_CYCLE_get", N, value );

    return value;
}
static INLINE void cpact_obuf_field_range_EOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                          cpact_obuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cpact_obuf_field_range_EOMF_CYCLE_set( cpact_obuf_buffer_t *b_ptr,
                                                          cpact_obuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_EOMF_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_EOMF_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_EOMF_CYCLE_set", stop_bit, 14 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_EOMF_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000008 + (N) * 0x10) bits 14:0) field EOMF_CYCLE of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000008 + (N) * 0x10) bits 14:0) field EOMF_CYCLE of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
        cpact_obuf_reg_EOMF_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF + subfield_offset),
                                                 CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 cpact_obuf_field_range_EOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpact_obuf_field_range_EOMF_CYCLE_get( cpact_obuf_buffer_t *b_ptr,
                                                            cpact_obuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cpact_obuf_field_range_EOMF_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpact_obuf_field_range_EOMF_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpact_obuf_field_range_EOMF_CYCLE_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000008 + (N) * 0x10) bits 14:0) field EOMF_CYCLE of register PMC_CPACT_OBUF_REG_EOMF_CFG index N=0..95 */
    reg_value = cpact_obuf_reg_EOMF_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_MSK)
                  >> CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_MSK, CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cpact_obuf_field_range_EOMF_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPACT_OBUF_IO_INLINE_H */
