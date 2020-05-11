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
 *     and register accessor functions for the mp_mgen block
 *****************************************************************************/
#ifndef _MP_MGEN_IO_INLINE_H
#define _MP_MGEN_IO_INLINE_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "mp_mgen_loc.h"
#include "mp_mgen.h"
#include "mp_mgen_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MP_MGEN_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mp_mgen
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
    mp_mgen_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mp_mgen_buffer_t;
static INLINE void mp_mgen_buffer_init( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mp_mgen_buffer_init( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mp_mgen_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mp_mgen_buffer_flush( mp_mgen_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mp_mgen_buffer_flush( mp_mgen_buffer_t *b_ptr )
{
    IOLOG( "mp_mgen_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mp_mgen_reg_read( mp_mgen_buffer_t *b_ptr,
                                       mp_mgen_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_reg_read( mp_mgen_buffer_t *b_ptr,
                                       mp_mgen_handle_t *h_ptr,
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
static INLINE void mp_mgen_reg_write( mp_mgen_buffer_t *b_ptr,
                                      mp_mgen_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_write( mp_mgen_buffer_t *b_ptr,
                                      mp_mgen_handle_t *h_ptr,
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

static INLINE void mp_mgen_field_set( mp_mgen_buffer_t *b_ptr,
                                      mp_mgen_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_set( mp_mgen_buffer_t *b_ptr,
                                      mp_mgen_handle_t *h_ptr,
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

static INLINE void mp_mgen_action_on_write_field_set( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_action_on_write_field_set( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
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

static INLINE void mp_mgen_burst_read( mp_mgen_buffer_t *b_ptr,
                                       mp_mgen_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void mp_mgen_burst_read( mp_mgen_buffer_t *b_ptr,
                                       mp_mgen_handle_t *h_ptr,
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

static INLINE void mp_mgen_burst_write( mp_mgen_buffer_t *b_ptr,
                                        mp_mgen_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void mp_mgen_burst_write( mp_mgen_buffer_t *b_ptr,
                                        mp_mgen_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mp_mgen_poll( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_poll( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
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
 *  register access functions for mp_mgen
 * ==================================================================================
 */

static INLINE void mp_mgen_reg_IPT_ENTRY_array_write( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_IPT_ENTRY_array_write( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mp_mgen_reg_IPT_ENTRY_array_write", value );
    mp_mgen_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_MP_MGEN_REG_IPT_ENTRY(N),
                       value);
}

static INLINE void mp_mgen_reg_IPT_ENTRY_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                          mp_mgen_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_IPT_ENTRY_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                          mp_mgen_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mp_mgen_reg_IPT_ENTRY_array_field_set", N, mask, ofs, value );
    mp_mgen_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_MP_MGEN_REG_IPT_ENTRY(N),
                       mask,
                       PMC_MP_MGEN_REG_IPT_ENTRY_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 mp_mgen_reg_IPT_ENTRY_array_read( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_reg_IPT_ENTRY_array_read( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = mp_mgen_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_MP_MGEN_REG_IPT_ENTRY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mp_mgen_reg_IPT_ENTRY_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mp_mgen_reg_AMP_CFGS_array_write( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_AMP_CFGS_array_write( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mp_mgen_reg_AMP_CFGS_array_write", value );
    mp_mgen_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_MP_MGEN_REG_AMP_CFGS(N),
                       value);
}

static INLINE void mp_mgen_reg_AMP_CFGS_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                         mp_mgen_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_AMP_CFGS_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                         mp_mgen_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mp_mgen_reg_AMP_CFGS_array_field_set", N, mask, ofs, value );
    mp_mgen_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_MP_MGEN_REG_AMP_CFGS(N),
                       mask,
                       PMC_MP_MGEN_REG_AMP_CFGS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 mp_mgen_reg_AMP_CFGS_array_read( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_reg_AMP_CFGS_array_read( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = mp_mgen_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_MP_MGEN_REG_AMP_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mp_mgen_reg_AMP_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mp_mgen_reg_DIV_48_M_LAST_TXR_array_write( mp_mgen_buffer_t *b_ptr,
                                                              mp_mgen_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_DIV_48_M_LAST_TXR_array_write( mp_mgen_buffer_t *b_ptr,
                                                              mp_mgen_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mp_mgen_reg_DIV_48_M_LAST_TXR_array_write", value );
    mp_mgen_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR(N),
                       value);
}

static INLINE void mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                                  mp_mgen_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( mp_mgen_buffer_t *b_ptr,
                                                                  mp_mgen_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set", N, mask, ofs, value );
    mp_mgen_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR(N),
                       mask,
                       PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( mp_mgen_buffer_t *b_ptr,
                                                               mp_mgen_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( mp_mgen_buffer_t *b_ptr,
                                                               mp_mgen_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = mp_mgen_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mp_mgen_reg_DIV_48_M_LAST_TXR_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll( mp_mgen_buffer_t *b_ptr,
                                                                             mp_mgen_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll( mp_mgen_buffer_t *b_ptr,
                                                                             mp_mgen_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mp_mgen_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR(N),
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mp_mgen_field_IPT_VALID_set( mp_mgen_buffer_t *b_ptr,
                                                mp_mgen_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_IPT_VALID_set( mp_mgen_buffer_t *b_ptr,
                                                mp_mgen_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_IPT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_IPT_VALID_set", N, value );

    /* ((0x00000000 + (N) * 0x8) bits 16) field IPT_VALID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    mp_mgen_reg_IPT_ENTRY_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_MSK,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_OFF,
                                           value);
}

static INLINE UINT32 mp_mgen_field_IPT_VALID_get( mp_mgen_buffer_t *b_ptr,
                                                  mp_mgen_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_IPT_VALID_get( mp_mgen_buffer_t *b_ptr,
                                                  mp_mgen_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_VALID_get", N, 95);
    /* ((0x00000000 + (N) * 0x8) bits 16) field IPT_VALID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    reg_value = mp_mgen_reg_IPT_ENTRY_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_MSK) >> MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_IPT_VALID_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_IPT_ID_set( mp_mgen_buffer_t *b_ptr,
                                             mp_mgen_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_IPT_ID_set( mp_mgen_buffer_t *b_ptr,
                                             mp_mgen_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_IPT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_IPT_ID_set", N, value );

    /* ((0x00000000 + (N) * 0x8) bits 14:8) field IPT_ID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    mp_mgen_reg_IPT_ENTRY_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_MSK,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF,
                                           value);
}

static INLINE UINT32 mp_mgen_field_IPT_ID_get( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_IPT_ID_get( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_ID_get", N, 95);
    /* ((0x00000000 + (N) * 0x8) bits 14:8) field IPT_ID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    reg_value = mp_mgen_reg_IPT_ENTRY_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_MSK) >> MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_IPT_ID_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_IPT_ID_set( mp_mgen_buffer_t *b_ptr,
                                                   mp_mgen_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_IPT_ID_set( mp_mgen_buffer_t *b_ptr,
                                                   mp_mgen_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_IPT_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_IPT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_IPT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_IPT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x8) bits 14:8) field IPT_ID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000000 + (N) * 0x8) bits 14:8) field IPT_ID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
        mp_mgen_reg_IPT_ENTRY_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF + subfield_offset),
                                               MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_IPT_ID_get( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_IPT_ID_get( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_IPT_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_IPT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_IPT_ID_get", stop_bit, 6 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 6) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 6;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000000 + (N) * 0x8) bits 14:8) field IPT_ID of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    reg_value = mp_mgen_reg_IPT_ENTRY_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_MSK)
                  >> MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_MSK, MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_IPT_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mp_mgen_field_IPT_VA_set( mp_mgen_buffer_t *b_ptr,
                                             mp_mgen_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_IPT_VA_set( mp_mgen_buffer_t *b_ptr,
                                             mp_mgen_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_VA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_IPT_VA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_IPT_VA_set", N, value );

    /* ((0x00000000 + (N) * 0x8) bits 6:0) field IPT_VA of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    mp_mgen_reg_IPT_ENTRY_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_MSK,
                                           MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF,
                                           value);
}

static INLINE UINT32 mp_mgen_field_IPT_VA_get( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_IPT_VA_get( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_IPT_VA_get", N, 95);
    /* ((0x00000000 + (N) * 0x8) bits 6:0) field IPT_VA of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    reg_value = mp_mgen_reg_IPT_ENTRY_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_MSK) >> MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_IPT_VA_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_IPT_VA_set( mp_mgen_buffer_t *b_ptr,
                                                   mp_mgen_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_IPT_VA_set( mp_mgen_buffer_t *b_ptr,
                                                   mp_mgen_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_IPT_VA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_IPT_VA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_IPT_VA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_IPT_VA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x8) bits 6:0) field IPT_VA of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000000 + (N) * 0x8) bits 6:0) field IPT_VA of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
        mp_mgen_reg_IPT_ENTRY_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF + subfield_offset),
                                               MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_IPT_VA_get( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_IPT_VA_get( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_IPT_VA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_IPT_VA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_IPT_VA_get", stop_bit, 6 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 6) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 6;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000000 + (N) * 0x8) bits 6:0) field IPT_VA of register PMC_MP_MGEN_REG_IPT_ENTRY index N=0..95 */
    reg_value = mp_mgen_reg_IPT_ENTRY_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_MSK)
                  >> MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_MSK, MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_IPT_VA_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void mp_mgen_field_NJO_TYPE_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_NJO_TYPE_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_TYPE_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_NJO_TYPE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_NJO_TYPE_set", N, value );

    /* ((0x00000004 + (N) * 0x8) bits 25:24) field NJO_TYPE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_MSK,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF,
                                          value);
}

static INLINE UINT32 mp_mgen_field_NJO_TYPE_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_NJO_TYPE_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_TYPE_get", N, 95);
    /* ((0x00000004 + (N) * 0x8) bits 25:24) field NJO_TYPE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_MSK) >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_NJO_TYPE_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_NJO_TYPE_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_NJO_TYPE_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_TYPE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x8) bits 25:24) field NJO_TYPE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
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
        /* ((0x00000004 + (N) * 0x8) bits 25:24) field NJO_TYPE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
        mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              subfield_mask << (MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF + subfield_offset),
                                              MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_NJO_TYPE_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_NJO_TYPE_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_TYPE_get", stop_bit, 1 );
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
    /* ((0x00000004 + (N) * 0x8) bits 25:24) field NJO_TYPE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_MSK)
                  >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_MSK, MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mp_mgen_field_NJO_POS_set( mp_mgen_buffer_t *b_ptr,
                                              mp_mgen_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_NJO_POS_set( mp_mgen_buffer_t *b_ptr,
                                              mp_mgen_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_POS_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_NJO_POS_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_NJO_POS_set", N, value );

    /* ((0x00000004 + (N) * 0x8) bits 21:16) field NJO_POS of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_MSK,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF,
                                          value);
}

static INLINE UINT32 mp_mgen_field_NJO_POS_get( mp_mgen_buffer_t *b_ptr,
                                                mp_mgen_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_NJO_POS_get( mp_mgen_buffer_t *b_ptr,
                                                mp_mgen_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_POS_get", N, 95);
    /* ((0x00000004 + (N) * 0x8) bits 21:16) field NJO_POS of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_MSK) >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_NJO_POS_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_NJO_POS_set( mp_mgen_buffer_t *b_ptr,
                                                    mp_mgen_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_NJO_POS_set( mp_mgen_buffer_t *b_ptr,
                                                    mp_mgen_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_POS_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_POS_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_POS_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_POS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x8) bits 21:16) field NJO_POS of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
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
        /* ((0x00000004 + (N) * 0x8) bits 21:16) field NJO_POS of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
        mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              subfield_mask << (MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF + subfield_offset),
                                              MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_NJO_POS_get( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_NJO_POS_get( mp_mgen_buffer_t *b_ptr,
                                                      mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_POS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_POS_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_POS_get", stop_bit, 5 );
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
    /* ((0x00000004 + (N) * 0x8) bits 21:16) field NJO_POS of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_MSK)
                  >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_MSK, MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_POS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mp_mgen_field_NJO_ODTU_CYCLE_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_NJO_ODTU_CYCLE_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_ODTU_CYCLE_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_NJO_ODTU_CYCLE_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_NJO_ODTU_CYCLE_set", N, value );

    /* ((0x00000004 + (N) * 0x8) bits 11:0) field NJO_ODTU_CYCLE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_MSK,
                                          MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF,
                                          value);
}

static INLINE UINT32 mp_mgen_field_NJO_ODTU_CYCLE_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_NJO_ODTU_CYCLE_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_NJO_ODTU_CYCLE_get", N, 95);
    /* ((0x00000004 + (N) * 0x8) bits 11:0) field NJO_ODTU_CYCLE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_MSK) >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_NJO_ODTU_CYCLE_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_NJO_ODTU_CYCLE_set( mp_mgen_buffer_t *b_ptr,
                                                           mp_mgen_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_NJO_ODTU_CYCLE_set( mp_mgen_buffer_t *b_ptr,
                                                           mp_mgen_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_ODTU_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_ODTU_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_ODTU_CYCLE_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_ODTU_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (N) * 0x8) bits 11:0) field NJO_ODTU_CYCLE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
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
        /* ((0x00000004 + (N) * 0x8) bits 11:0) field NJO_ODTU_CYCLE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
        mp_mgen_reg_AMP_CFGS_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              subfield_mask << (MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF + subfield_offset),
                                              MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_NJO_ODTU_CYCLE_get( mp_mgen_buffer_t *b_ptr,
                                                             mp_mgen_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_NJO_ODTU_CYCLE_get( mp_mgen_buffer_t *b_ptr,
                                                             mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_NJO_ODTU_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_NJO_ODTU_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_NJO_ODTU_CYCLE_get", stop_bit, 11 );
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
    /* ((0x00000004 + (N) * 0x8) bits 11:0) field NJO_ODTU_CYCLE of register PMC_MP_MGEN_REG_AMP_CFGS index N=0..95 */
    reg_value = mp_mgen_reg_AMP_CFGS_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_MSK)
                  >> MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_MSK, MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_NJO_ODTU_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mp_mgen_field_LAST_TXR_BYTES_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_LAST_TXR_BYTES_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_LAST_TXR_BYTES_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_LAST_TXR_BYTES_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_LAST_TXR_BYTES_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF,
                                                   value);
}

static INLINE UINT32 mp_mgen_field_LAST_TXR_BYTES_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_LAST_TXR_BYTES_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_LAST_TXR_BYTES_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK) >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_LAST_TXR_BYTES_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_LAST_TXR_BYTES_set( mp_mgen_buffer_t *b_ptr,
                                                           mp_mgen_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_LAST_TXR_BYTES_set( mp_mgen_buffer_t *b_ptr,
                                                           mp_mgen_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_LAST_TXR_BYTES_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_LAST_TXR_BYTES_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_LAST_TXR_BYTES_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_LAST_TXR_BYTES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
        mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF + subfield_offset),
                                                       MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_LAST_TXR_BYTES_get( mp_mgen_buffer_t *b_ptr,
                                                             mp_mgen_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_LAST_TXR_BYTES_get( mp_mgen_buffer_t *b_ptr,
                                                             mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_LAST_TXR_BYTES_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_LAST_TXR_BYTES_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_LAST_TXR_BYTES_get", stop_bit, 5 );
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
    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK)
                  >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_LAST_TXR_BYTES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_field_range_LAST_TXR_BYTES_poll( mp_mgen_buffer_t *b_ptr,
                                                                            mp_mgen_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_field_range_LAST_TXR_BYTES_poll( mp_mgen_buffer_t *b_ptr,
                                                                            mp_mgen_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_LAST_TXR_BYTES_poll", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_LAST_TXR_BYTES_poll", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_LAST_TXR_BYTES_poll", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_LAST_TXR_BYTES_poll", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
        return mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF + subfield_offset),
                                                         value << (MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mp_mgen_field_LAST_TXR_BYTES_poll( mp_mgen_buffer_t *b_ptr,
                                                                      mp_mgen_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mp_mgen_field_LAST_TXR_BYTES_poll( mp_mgen_buffer_t *b_ptr,
                                                                      mp_mgen_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "mp_mgen_field_LAST_TXR_BYTES_poll", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 21:16) field LAST_TXR_BYTES of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    return mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK,
                                                     (value<<MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void mp_mgen_field_MOD_48_M_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_MOD_48_M_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_MOD_48_M_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_MOD_48_M_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_MOD_48_M_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 13:8) field MOD_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_MSK,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF,
                                                   value);
}

static INLINE UINT32 mp_mgen_field_MOD_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_MOD_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_MOD_48_M_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 13:8) field MOD_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_MSK) >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_MOD_48_M_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_MOD_48_M_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_MOD_48_M_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_MOD_48_M_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_MOD_48_M_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_MOD_48_M_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_MOD_48_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 13:8) field MOD_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 13:8) field MOD_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
        mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF + subfield_offset),
                                                       MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_MOD_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_MOD_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_MOD_48_M_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_MOD_48_M_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_MOD_48_M_get", stop_bit, 5 );
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
    /* ((0x00000400 + (N) * 0x4) bits 13:8) field MOD_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_MSK)
                  >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_MSK, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_MOD_48_M_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mp_mgen_field_DIV_48_M_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_DIV_48_M_set( mp_mgen_buffer_t *b_ptr,
                                               mp_mgen_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_DIV_48_M_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mp_mgen_field_DIV_48_M_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "mp_mgen_field_DIV_48_M_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 5:0) field DIV_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_MSK,
                                                   MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF,
                                                   value);
}

static INLINE UINT32 mp_mgen_field_DIV_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_DIV_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                 mp_mgen_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_DIV_48_M_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 5:0) field DIV_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_MSK) >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mp_mgen_field_DIV_48_M_get", N, value );

    return value;
}
static INLINE void mp_mgen_field_range_DIV_48_M_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mp_mgen_field_range_DIV_48_M_set( mp_mgen_buffer_t *b_ptr,
                                                     mp_mgen_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_DIV_48_M_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_DIV_48_M_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_DIV_48_M_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_DIV_48_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 5:0) field DIV_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 5:0) field DIV_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
        mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF + subfield_offset),
                                                       MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 mp_mgen_field_range_DIV_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mp_mgen_field_range_DIV_48_M_get( mp_mgen_buffer_t *b_ptr,
                                                       mp_mgen_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mp_mgen_field_range_DIV_48_M_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mp_mgen_field_range_DIV_48_M_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mp_mgen_field_range_DIV_48_M_get", stop_bit, 5 );
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
    /* ((0x00000400 + (N) * 0x4) bits 5:0) field DIV_48_M of register PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR index N=0..95 */
    reg_value = mp_mgen_reg_DIV_48_M_LAST_TXR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_MSK)
                  >> MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_MSK, MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mp_mgen_field_range_DIV_48_M_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MP_MGEN_IO_INLINE_H */
