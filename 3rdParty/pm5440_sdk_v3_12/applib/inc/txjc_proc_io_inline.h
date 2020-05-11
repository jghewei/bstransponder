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
 *     and register accessor functions for the txjc_proc block
 *****************************************************************************/
#ifndef _TXJC_PROC_IO_INLINE_H
#define _TXJC_PROC_IO_INLINE_H

#include "txjc_proc_loc.h"
#include "txjc_proc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TXJC_PROC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for txjc_proc
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
    txjc_proc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} txjc_proc_buffer_t;
static INLINE void txjc_proc_buffer_init( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void txjc_proc_buffer_init( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "txjc_proc_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void txjc_proc_buffer_flush( txjc_proc_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void txjc_proc_buffer_flush( txjc_proc_buffer_t *b_ptr )
{
    IOLOG( "txjc_proc_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 txjc_proc_reg_read( txjc_proc_buffer_t *b_ptr,
                                         txjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_reg_read( txjc_proc_buffer_t *b_ptr,
                                         txjc_proc_handle_t *h_ptr,
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
static INLINE void txjc_proc_reg_write( txjc_proc_buffer_t *b_ptr,
                                        txjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_write( txjc_proc_buffer_t *b_ptr,
                                        txjc_proc_handle_t *h_ptr,
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

static INLINE void txjc_proc_field_set( txjc_proc_buffer_t *b_ptr,
                                        txjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_set( txjc_proc_buffer_t *b_ptr,
                                        txjc_proc_handle_t *h_ptr,
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

static INLINE void txjc_proc_action_on_write_field_set( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_action_on_write_field_set( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
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

static INLINE void txjc_proc_burst_read( txjc_proc_buffer_t *b_ptr,
                                         txjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void txjc_proc_burst_read( txjc_proc_buffer_t *b_ptr,
                                         txjc_proc_handle_t *h_ptr,
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

static INLINE void txjc_proc_burst_write( txjc_proc_buffer_t *b_ptr,
                                          txjc_proc_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void txjc_proc_burst_write( txjc_proc_buffer_t *b_ptr,
                                          txjc_proc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE txjc_proc_poll( txjc_proc_buffer_t *b_ptr,
                                                   txjc_proc_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE txjc_proc_poll( txjc_proc_buffer_t *b_ptr,
                                                   txjc_proc_handle_t *h_ptr,
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
 *  register access functions for txjc_proc
 * ==================================================================================
 */

static INLINE void txjc_proc_reg_CHANNEL_COMMON_CFG_array_write( txjc_proc_buffer_t *b_ptr,
                                                                 txjc_proc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CHANNEL_COMMON_CFG_array_write( txjc_proc_buffer_t *b_ptr,
                                                                 txjc_proc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "txjc_proc_reg_CHANNEL_COMMON_CFG_array_write", value );
    txjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG(N),
                         value);
}

static INLINE void txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                     txjc_proc_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                     txjc_proc_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set", N, mask, ofs, value );
    txjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG(N),
                         mask,
                         PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = txjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "txjc_proc_reg_CHANNEL_COMMON_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void txjc_proc_reg_GOLDEN_CN_REG_array_write( txjc_proc_buffer_t *b_ptr,
                                                            txjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_GOLDEN_CN_REG_array_write( txjc_proc_buffer_t *b_ptr,
                                                            txjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "txjc_proc_reg_GOLDEN_CN_REG_array_write", value );
    txjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_GOLDEN_CN_REG(N),
                         value);
}

static INLINE void txjc_proc_reg_GOLDEN_CN_REG_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_GOLDEN_CN_REG_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "txjc_proc_reg_GOLDEN_CN_REG_array_field_set", N, mask, ofs, value );
    txjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_GOLDEN_CN_REG(N),
                         mask,
                         PMC_TXJC_PROC_REG_GOLDEN_CN_REG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 txjc_proc_reg_GOLDEN_CN_REG_array_read( txjc_proc_buffer_t *b_ptr,
                                                             txjc_proc_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_reg_GOLDEN_CN_REG_array_read( txjc_proc_buffer_t *b_ptr,
                                                             txjc_proc_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = txjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_TXJC_PROC_REG_GOLDEN_CN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "txjc_proc_reg_GOLDEN_CN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void txjc_proc_reg_CN_RAMP_NUMERATOR_array_write( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CN_RAMP_NUMERATOR_array_write( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "txjc_proc_reg_CN_RAMP_NUMERATOR_array_write", value );
    txjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR(N),
                         value);
}

static INLINE void txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set", N, mask, ofs, value );
    txjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR(N),
                         mask,
                         PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 txjc_proc_reg_CN_RAMP_NUMERATOR_array_read( txjc_proc_buffer_t *b_ptr,
                                                                 txjc_proc_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_reg_CN_RAMP_NUMERATOR_array_read( txjc_proc_buffer_t *b_ptr,
                                                                 txjc_proc_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = txjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "txjc_proc_reg_CN_RAMP_NUMERATOR_array_read", reg_value, N);
    return reg_value;
}

static INLINE void txjc_proc_reg_CN_RAMP_DENOMINATOR_array_write( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CN_RAMP_DENOMINATOR_array_write( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "txjc_proc_reg_CN_RAMP_DENOMINATOR_array_write", value );
    txjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR(N),
                         value);
}

static INLINE void txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                      txjc_proc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set( txjc_proc_buffer_t *b_ptr,
                                                                      txjc_proc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set", N, mask, ofs, value );
    txjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR(N),
                         mask,
                         PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read( txjc_proc_buffer_t *b_ptr,
                                                                   txjc_proc_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read( txjc_proc_buffer_t *b_ptr,
                                                                   txjc_proc_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = txjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void txjc_proc_field_SPECIAL_MODE_2_SEL_set( txjc_proc_buffer_t *b_ptr,
                                                           txjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_SPECIAL_MODE_2_SEL_set( txjc_proc_buffer_t *b_ptr,
                                                           txjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_SPECIAL_MODE_2_SEL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_SPECIAL_MODE_2_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_SPECIAL_MODE_2_SEL_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 5) field SPECIAL_MODE_2_SEL of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_MSK,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_OFF,
                                                      value);
}

static INLINE UINT32 txjc_proc_field_SPECIAL_MODE_2_SEL_get( txjc_proc_buffer_t *b_ptr,
                                                             txjc_proc_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_SPECIAL_MODE_2_SEL_get( txjc_proc_buffer_t *b_ptr,
                                                             txjc_proc_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_SPECIAL_MODE_2_SEL_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 5) field SPECIAL_MODE_2_SEL of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_MSK) >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_SPECIAL_MODE_2_SEL_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_PREV_CN_SEL_set( txjc_proc_buffer_t *b_ptr,
                                                    txjc_proc_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_PREV_CN_SEL_set( txjc_proc_buffer_t *b_ptr,
                                                    txjc_proc_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_PREV_CN_SEL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_PREV_CN_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_PREV_CN_SEL_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 4) field PREV_CN_SEL of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_MSK,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_OFF,
                                                      value);
}

static INLINE UINT32 txjc_proc_field_PREV_CN_SEL_get( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_PREV_CN_SEL_get( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_PREV_CN_SEL_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 4) field PREV_CN_SEL of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_MSK) >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_PREV_CN_SEL_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_PJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                txjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_PJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                txjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_PJO_CNT_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_PJO_CNT_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_PJO_CNT_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 3:2) field PJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_MSK,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF,
                                                      value);
}

static INLINE UINT32 txjc_proc_field_PJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_PJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_PJO_CNT_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 3:2) field PJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_MSK) >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_PJO_CNT_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_range_PJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_range_PJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_PJO_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_PJO_CNT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_PJO_CNT_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_PJO_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x10) bits 3:2) field PJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
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
        /* ((0x00000000 + (N) * 0x10) bits 3:2) field PJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
        txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF + subfield_offset),
                                                          TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 txjc_proc_field_range_PJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_range_PJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_PJO_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_PJO_CNT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_PJO_CNT_get", stop_bit, 1 );
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
    /* ((0x00000000 + (N) * 0x10) bits 3:2) field PJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_MSK)
                  >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_MSK, TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_PJO_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void txjc_proc_field_NJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                txjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_NJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                txjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_NJO_CNT_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_NJO_CNT_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_NJO_CNT_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 1:0) field NJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_MSK,
                                                      TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF,
                                                      value);
}

static INLINE UINT32 txjc_proc_field_NJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_NJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_NJO_CNT_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 1:0) field NJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_MSK) >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_NJO_CNT_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_range_NJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_range_NJO_CNT_set( txjc_proc_buffer_t *b_ptr,
                                                      txjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_NJO_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_NJO_CNT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_NJO_CNT_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_NJO_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x10) bits 1:0) field NJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
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
        /* ((0x00000000 + (N) * 0x10) bits 1:0) field NJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
        txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF + subfield_offset),
                                                          TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 txjc_proc_field_range_NJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_range_NJO_CNT_get( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_NJO_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_NJO_CNT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_NJO_CNT_get", stop_bit, 1 );
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
    /* ((0x00000000 + (N) * 0x10) bits 1:0) field NJO_CNT of register PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG index N=0..95 */
    reg_value = txjc_proc_reg_CHANNEL_COMMON_CFG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_MSK)
                  >> TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_MSK, TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_NJO_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void txjc_proc_field_CN_GOLDEN_set( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_CN_GOLDEN_set( txjc_proc_buffer_t *b_ptr,
                                                  txjc_proc_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_CN_GOLDEN_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_set", N, value );

    /* ((0x00000004 + (N) * 0x10) bits 23:0) field CN_GOLDEN of register PMC_TXJC_PROC_REG_GOLDEN_CN_REG index N=0..95 */
    txjc_proc_reg_GOLDEN_CN_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_MSK,
                                                 TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF,
                                                 value);
}

static INLINE UINT32 txjc_proc_field_CN_GOLDEN_get( txjc_proc_buffer_t *b_ptr,
                                                    txjc_proc_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_CN_GOLDEN_get( txjc_proc_buffer_t *b_ptr,
                                                    txjc_proc_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_get", N, 95);
    /* ((0x00000004 + (N) * 0x10) bits 23:0) field CN_GOLDEN of register PMC_TXJC_PROC_REG_GOLDEN_CN_REG index N=0..95 */
    reg_value = txjc_proc_reg_GOLDEN_CN_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_MSK) >> TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_range_CN_GOLDEN_set( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_range_CN_GOLDEN_set( txjc_proc_buffer_t *b_ptr,
                                                        txjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x10) bits 23:0) field CN_GOLDEN of register PMC_TXJC_PROC_REG_GOLDEN_CN_REG index N=0..95 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000004 + (N) * 0x10) bits 23:0) field CN_GOLDEN of register PMC_TXJC_PROC_REG_GOLDEN_CN_REG index N=0..95 */
        txjc_proc_reg_GOLDEN_CN_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF + subfield_offset),
                                                     TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_get( txjc_proc_buffer_t *b_ptr,
                                                          txjc_proc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_get( txjc_proc_buffer_t *b_ptr,
                                                          txjc_proc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000004 + (N) * 0x10) bits 23:0) field CN_GOLDEN of register PMC_TXJC_PROC_REG_GOLDEN_CN_REG index N=0..95 */
    reg_value = txjc_proc_reg_GOLDEN_CN_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_MSK)
                  >> TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_MSK, TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void txjc_proc_field_CN_GOLDEN_NUMERATOR_set( txjc_proc_buffer_t *b_ptr,
                                                            txjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_CN_GOLDEN_NUMERATOR_set( txjc_proc_buffer_t *b_ptr,
                                                            txjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_NUMERATOR_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_CN_GOLDEN_NUMERATOR_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_NUMERATOR_set", N, value );

    /* ((0x00000008 + (N) * 0x10) bits 23:0) field CN_GOLDEN_NUMERATOR of register PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR index N=0..95 */
    txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_MSK,
                                                     TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF,
                                                     value);
}

static INLINE UINT32 txjc_proc_field_CN_GOLDEN_NUMERATOR_get( txjc_proc_buffer_t *b_ptr,
                                                              txjc_proc_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_CN_GOLDEN_NUMERATOR_get( txjc_proc_buffer_t *b_ptr,
                                                              txjc_proc_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_NUMERATOR_get", N, 95);
    /* ((0x00000008 + (N) * 0x10) bits 23:0) field CN_GOLDEN_NUMERATOR of register PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR index N=0..95 */
    reg_value = txjc_proc_reg_CN_RAMP_NUMERATOR_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_MSK) >> TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_NUMERATOR_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set( txjc_proc_buffer_t *b_ptr,
                                                                  txjc_proc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000008 + (N) * 0x10) bits 23:0) field CN_GOLDEN_NUMERATOR of register PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR index N=0..95 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000008 + (N) * 0x10) bits 23:0) field CN_GOLDEN_NUMERATOR of register PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR index N=0..95 */
        txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF + subfield_offset),
                                                         TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000008 + (N) * 0x10) bits 23:0) field CN_GOLDEN_NUMERATOR of register PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR index N=0..95 */
    reg_value = txjc_proc_reg_CN_RAMP_NUMERATOR_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_MSK)
                  >> TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_MSK, TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void txjc_proc_field_CN_GOLDEN_DENOMINATOR_set( txjc_proc_buffer_t *b_ptr,
                                                              txjc_proc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_CN_GOLDEN_DENOMINATOR_set( txjc_proc_buffer_t *b_ptr,
                                                              txjc_proc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_DENOMINATOR_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "txjc_proc_field_CN_GOLDEN_DENOMINATOR_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_DENOMINATOR_set", N, value );

    /* ((0x0000000c + (N) * 0x10) bits 23:0) field CN_GOLDEN_DENOMINATOR of register PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR index N=0..95 */
    txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_MSK,
                                                       TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF,
                                                       value);
}

static INLINE UINT32 txjc_proc_field_CN_GOLDEN_DENOMINATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_CN_GOLDEN_DENOMINATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                txjc_proc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_CN_GOLDEN_DENOMINATOR_get", N, 95);
    /* ((0x0000000c + (N) * 0x10) bits 23:0) field CN_GOLDEN_DENOMINATOR of register PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR index N=0..95 */
    reg_value = txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_MSK) >> TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "txjc_proc_field_CN_GOLDEN_DENOMINATOR_get", N, value );

    return value;
}
static INLINE void txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set( txjc_proc_buffer_t *b_ptr,
                                                                    txjc_proc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000000c + (N) * 0x10) bits 23:0) field CN_GOLDEN_DENOMINATOR of register PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR index N=0..95 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x0000000c + (N) * 0x10) bits 23:0) field CN_GOLDEN_DENOMINATOR of register PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR index N=0..95 */
        txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF + subfield_offset),
                                                           TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                      txjc_proc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get( txjc_proc_buffer_t *b_ptr,
                                                                      txjc_proc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000000c + (N) * 0x10) bits 23:0) field CN_GOLDEN_DENOMINATOR of register PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR index N=0..95 */
    reg_value = txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_MSK)
                  >> TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_MSK, TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TXJC_PROC_IO_INLINE_H */
