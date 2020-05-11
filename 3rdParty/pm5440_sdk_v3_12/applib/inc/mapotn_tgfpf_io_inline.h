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
 *     and register accessor functions for the mapotn_tgfpf block
 *****************************************************************************/
#ifndef _MAPOTN_TGFPF_IO_INLINE_H
#define _MAPOTN_TGFPF_IO_INLINE_H

#include "mapotn_loc.h"
#include "mapotn_tgfpf_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAPOTN_TGFPF_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mapotn_tgfpf
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
    mapotn_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mapotn_tgfpf_buffer_t;
static INLINE void mapotn_tgfpf_buffer_init( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_buffer_init( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mapotn_tgfpf_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mapotn_tgfpf_buffer_flush( mapotn_tgfpf_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_buffer_flush( mapotn_tgfpf_buffer_t *b_ptr )
{
    IOLOG( "mapotn_tgfpf_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mapotn_tgfpf_reg_read( mapotn_tgfpf_buffer_t *b_ptr,
                                            mapotn_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_read( mapotn_tgfpf_buffer_t *b_ptr,
                                            mapotn_handle_t *h_ptr,
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
static INLINE void mapotn_tgfpf_reg_write( mapotn_tgfpf_buffer_t *b_ptr,
                                           mapotn_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_write( mapotn_tgfpf_buffer_t *b_ptr,
                                           mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tgfpf_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                           mapotn_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                           mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tgfpf_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tgfpf_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                            mapotn_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                            mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tgfpf_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                             mapotn_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                             mapotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
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
 *  register access functions for mapotn_tgfpf
 * ==================================================================================
 */

static INLINE void mapotn_tgfpf_reg_COMMON_CFG_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_COMMON_CFG_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_COMMON_CFG_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_COMMON_CFG,
                            value);
}

static INLINE void mapotn_tgfpf_reg_COMMON_CFG_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_COMMON_CFG_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_COMMON_CFG_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_COMMON_CFG,
                            mask,
                            PMC_TGFPF96_CORE_REG_COMMON_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_COMMON_CFG_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_COMMON_CFG_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_COMMON_CFG);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_COMMON_CFG_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32  M,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32  M,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 18)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write", ofs, len, 18 );
    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write", 18, M, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM(ofs,M),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  M,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  M,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- M=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_field_set", M, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM(N,M),
                            mask,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  M,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  M,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 18)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read", ofs, len, 18 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM(ofs,M),
                             len,
                             value);

    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read", 18, M, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM(N,M));

    IOLOG( "%s -> 0x%08x; M=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_read", reg_value, M);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_RST_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_RST_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_TX_CHAN_RST_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_RST(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_RST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_RST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_TX_CHAN_RST_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_RST(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_RST_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_RST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_RST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_TX_CHAN_RST(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_TX_CHAN_RST_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_CHAN_CFG_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_CHAN_CFG_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_CHAN_CFG_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_CHAN_CFG(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_CHAN_CFG_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_CHAN_CFG_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_CHAN_CFG_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_CHAN_CFG(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_CHAN_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_CHAN_CFG_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_CHAN_CFG_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_CHAN_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_CHAN_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PMON_CTRL_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PMON_CTRL_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_PMON_CTRL_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_PMON_CTRL,
                            value);
}

static INLINE void mapotn_tgfpf_reg_PMON_CTRL_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PMON_CTRL_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PMON_CTRL_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_TGFPF96_CORE_REG_PMON_CTRL,
                            mask,
                            PMC_TGFPF96_CORE_REG_PMON_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_CTRL_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_CTRL_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_TGFPF96_CORE_REG_PMON_CTRL);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PMON_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MISC_INT_E_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MISC_INT_E_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_MISC_INT_E_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MISC_INT_E,
                            value);
}

static INLINE void mapotn_tgfpf_reg_MISC_INT_E_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MISC_INT_E_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MISC_INT_E_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MISC_INT_E,
                            mask,
                            PMC_TGFPF96_CORE_REG_MISC_INT_E_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_MISC_INT_E_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MISC_INT_E_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MISC_INT_E);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MISC_INT_E_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MISC_INT_EVENT_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MISC_INT_EVENT_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_MISC_INT_EVENT_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MISC_INT_EVENT,
                            value);
}

static INLINE void mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_MISC_INT_EVENT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_MISC_INT_EVENT_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MISC_INT_EVENT_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MISC_INT_EVENT);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MISC_INT_EVENT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MISC_INT_EVENT_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MISC_INT_EVENT_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_MISC_INT_EVENT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MISC_INT_EVENT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 ofs,
                                                                                  UINT32 len,
                                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 ofs,
                                                                                  UINT32 len,
                                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                mapotn_handle_t *h_ptr,
                                                                                                UINT32 N,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                mapotn_handle_t *h_ptr,
                                                                                                UINT32 N,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                           mapotn_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                           mapotn_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                              mapotn_handle_t *h_ptr,
                                                                                              UINT32 N,
                                                                                              UINT32 mask,
                                                                                              UINT32 ofs,
                                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                              mapotn_handle_t *h_ptr,
                                                                                              UINT32 N,
                                                                                              UINT32 mask,
                                                                                              UINT32 ofs,
                                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 N,
                                                                                         UINT32 mask,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 N,
                                                                                         UINT32 mask,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 ofs,
                                                                               UINT32 len,
                                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                             mapotn_handle_t *h_ptr,
                                                                                             UINT32 N,
                                                                                             UINT32 mask,
                                                                                             UINT32 ofs,
                                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                             mapotn_handle_t *h_ptr,
                                                                                             UINT32 N,
                                                                                             UINT32 mask,
                                                                                             UINT32 ofs,
                                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                        mapotn_handle_t *h_ptr,
                                                                                        UINT32 N,
                                                                                        UINT32 mask,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                        mapotn_handle_t *h_ptr,
                                                                                        UINT32 N,
                                                                                        UINT32 mask,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                               mapotn_handle_t *h_ptr,
                                                                                               UINT32 N,
                                                                                               UINT32 mask,
                                                                                               UINT32 ofs,
                                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                               mapotn_handle_t *h_ptr,
                                                                                               UINT32 N,
                                                                                               UINT32 mask,
                                                                                               UINT32 ofs,
                                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 ofs,
                                                                                UINT32 len,
                                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                          mapotn_handle_t *h_ptr,
                                                                                          UINT32 N,
                                                                                          UINT32 mask,
                                                                                          UINT32 value,
                                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                                          UINT32 max_count,
                                                                                          UINT32 *num_failed_polls,
                                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                          mapotn_handle_t *h_ptr,
                                                                                          UINT32 N,
                                                                                          UINT32 mask,
                                                                                          UINT32 value,
                                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                                          UINT32 max_count,
                                                                                          UINT32 *num_failed_polls,
                                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 ofs,
                                                                                  UINT32 len,
                                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 ofs,
                                                                                  UINT32 len,
                                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                mapotn_handle_t *h_ptr,
                                                                                                UINT32 N,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                mapotn_handle_t *h_ptr,
                                                                                                UINT32 N,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                             mapotn_handle_t *h_ptr,
                                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                           mapotn_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                           mapotn_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                     mapotn_handle_t *h_ptr,
                                                                                     UINT32 ofs,
                                                                                     UINT32 len,
                                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                     mapotn_handle_t *h_ptr,
                                                                                     UINT32 ofs,
                                                                                     UINT32 len,
                                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write", 3, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT(ofs),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                   mapotn_handle_t *h_ptr,
                                                                                                   UINT32 N,
                                                                                                   UINT32 mask,
                                                                                                   UINT32 ofs,
                                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_action_on_write_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                                   mapotn_handle_t *h_ptr,
                                                                                                   UINT32 N,
                                                                                                   UINT32 mask,
                                                                                                   UINT32 ofs,
                                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_action_on_write_field_set", mask, ofs, value );
    mapotn_tgfpf_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT(N),
                                            mask,
                                            ofs,
                                            value);

}

static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 ofs,
                                                                                    UINT32 len,
                                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 ofs,
                                                                                    UINT32 len,
                                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read", ofs, len, 3 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                              mapotn_handle_t *h_ptr,
                                                                                              UINT32 N,
                                                                                              UINT32 mask,
                                                                                              UINT32 value,
                                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                                              UINT32 max_count,
                                                                                              UINT32 *num_failed_polls,
                                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                              mapotn_handle_t *h_ptr,
                                                                                              UINT32 N,
                                                                                              UINT32 mask,
                                                                                              UINT32 value,
                                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                                              UINT32 max_count,
                                                                                              UINT32 *num_failed_polls,
                                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT(N),
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  M,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  M,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write", ofs, len, 16 );
    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write", 16, M, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_EXT_HDR_RAM(ofs,M),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- M=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_field_set", M, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_EXT_HDR_RAM(N,M),
                            mask,
                            PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read", ofs, len, 16 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_EXT_HDR_RAM(ofs,M),
                             len,
                             value);

    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read", 16, M, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_EXT_HDR_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  M,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_EXT_HDR_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  M,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_EXT_HDR_RAM(N,M));

    IOLOG( "%s -> 0x%08x; M=%d", "mapotn_tgfpf_reg_EXT_HDR_RAM_array_read", reg_value, M);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  M,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  M,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 32)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write", ofs, len, 32 );
    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write", 32, M, ofs, len);
    mapotn_tgfpf_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM(ofs,M),
                              len,
                              value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  M,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  M,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- M=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_field_set", M, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM(N,M),
                            mask,
                            PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  M,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  M,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 32)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read", ofs, len, 32 );
    mapotn_tgfpf_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM(ofs,M),
                             len,
                             value);

    IOLOG( "%s of %d words M=%d ofs=%d len=%d", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read", 32, M, ofs, len);
}

static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM(N,M));

    IOLOG( "%s -> 0x%08x; M=%d", "mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_read", reg_value, M);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG(N),
                            value);
}

static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set", N, mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG(N),
                            mask,
                            PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_0_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_0_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_0_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_1_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_1_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_2_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_2_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_3_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_3_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_3(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_3_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_4_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_4_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_4(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_4_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_5_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_5_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_5(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_5_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_6_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_6_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_6(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_6_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_7_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_7_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_7(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_7_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_8_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_8_array_read( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_8(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_tgfpf_reg_PMON_8_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mapotn_tgfpf_reg_PMON_STATUS_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_PMON_STATUS_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_PMON_STATUS);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_PMON_STATUS_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PMON_STATUS_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PMON_STATUS_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tgfpf_reg_PMON_STATUS_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tgfpf_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_TGFPF96_CORE_REG_PMON_STATUS,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tgfpf_reg_CFG_MEM_MUTEX_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_CFG_MEM_MUTEX_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_CFG_MEM_MUTEX_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX,
                            value);
}

static INLINE void mapotn_tgfpf_reg_CFG_MEM_MUTEX_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_CFG_MEM_MUTEX_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_CFG_MEM_MUTEX_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX,
                            mask,
                            PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_CFG_MEM_MUTEX_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_CFG_MEM_MUTEX_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_CFG_MEM_MUTEX_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tgfpf_reg_MGMT_FRM_MUTEX_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRM_MUTEX_write( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tgfpf_reg_MGMT_FRM_MUTEX_write", value );
    mapotn_tgfpf_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX,
                            value);
}

static INLINE void mapotn_tgfpf_reg_MGMT_FRM_MUTEX_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_reg_MGMT_FRM_MUTEX_field_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tgfpf_reg_MGMT_FRM_MUTEX_field_set", mask, ofs, value );
    mapotn_tgfpf_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX,
                            mask,
                            PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRM_MUTEX_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_reg_MGMT_FRM_MUTEX_read( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tgfpf_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX);

    IOLOG( "%s -> 0x%08x;", "mapotn_tgfpf_reg_MGMT_FRM_MUTEX_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_ORD_SET_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_ORD_SET_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_ORD_SET_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_ORD_SET_EN_set", value );

    /* (0x000a0000 bits 16) field ORD_SET_EN of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    mapotn_tgfpf_reg_COMMON_CFG_field_set( b_ptr,
                                           h_ptr,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_MSK,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tgfpf_field_ORD_SET_EN_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_ORD_SET_EN_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0000 bits 16) field ORD_SET_EN of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    reg_value = mapotn_tgfpf_reg_COMMON_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_MSK) >> TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_ORD_SET_EN_get", value );

    return value;
}
static INLINE void mapotn_tgfpf_field_ORD_SET_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_ORD_SET_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_ORD_SET_UPI_set", value, 255);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_ORD_SET_UPI_set", value );

    /* (0x000a0000 bits 15:8) field ORD_SET_UPI of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    mapotn_tgfpf_reg_COMMON_CFG_field_set( b_ptr,
                                           h_ptr,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_MSK,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tgfpf_field_ORD_SET_UPI_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_ORD_SET_UPI_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0000 bits 15:8) field ORD_SET_UPI of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    reg_value = mapotn_tgfpf_reg_COMMON_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_MSK) >> TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_ORD_SET_UPI_get", value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_ORD_SET_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_ORD_SET_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_ORD_SET_UPI_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_ORD_SET_UPI_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_ORD_SET_UPI_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0000 bits 15:8) field ORD_SET_UPI of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000a0000 bits 15:8) field ORD_SET_UPI of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
        mapotn_tgfpf_reg_COMMON_CFG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF + subfield_offset),
                                               TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_ORD_SET_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_ORD_SET_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_ORD_SET_UPI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_ORD_SET_UPI_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000a0000 bits 15:8) field ORD_SET_UPI of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    reg_value = mapotn_tgfpf_reg_COMMON_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_MSK)
                  >> TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_MSK, TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_ORD_SET_UPI_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_GFPF_MAP_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_GFPF_MAP_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_GFPF_MAP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_GFPF_MAP_EN_set", value );

    /* (0x000a0000 bits 0) field GFPF_MAP_EN of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    mapotn_tgfpf_reg_COMMON_CFG_field_set( b_ptr,
                                           h_ptr,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_MSK,
                                           TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tgfpf_field_GFPF_MAP_EN_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_GFPF_MAP_EN_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0000 bits 0) field GFPF_MAP_EN of register PMC_TGFPF96_CORE_REG_COMMON_CFG */
    reg_value = mapotn_tgfpf_reg_COMMON_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_MSK) >> TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_GFPF_MAP_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_M_size4__N_burstsize18
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  M,
                                                            UINT32 value[18] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  M,
                                                            UINT32 value[18] )
{
    if (M > 3)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set", M, 3);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set", M);
    mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        M,
                                                        0,
                                                        18,
                                                        value);
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  M,
                                                            UINT32 value[18] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  M,
                                                            UINT32 value[18] )
{
    if (M > 3)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_get", M, 3);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_get", M);
    mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       M,
                                                       0,
                                                       18,
                                                       (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  M,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 3)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set", M, 3);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set", M, start_bit, stop_bit, value);
    mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_field_set( b_ptr,
                                                      h_ptr,
                                                      M,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  M,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  M,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 3)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get", M, 3);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_read( b_ptr,
                                                             h_ptr,
                                                             M,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get", M, start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_PAUSE_TIME_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PAUSE_TIME_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_TIME_set", N, 3);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PAUSE_TIME_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_TIME_set", N, value );

    /* ((0x000a0010 + (N) * 0x4) bits 15:0) field PAUSE_TIME of register PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME index N=0..3 */
    mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_MSK,
                                                     TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF,
                                                     value);
}

static INLINE UINT32 mapotn_tgfpf_field_PAUSE_TIME_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PAUSE_TIME_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_TIME_get", N, 3);
    /* ((0x000a0010 + (N) * 0x4) bits 15:0) field PAUSE_TIME of register PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME index N=0..3 */
    reg_value = mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_MSK) >> TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_TIME_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PAUSE_TIME_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PAUSE_TIME_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_TIME_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_TIME_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_TIME_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_TIME_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a0010 + (N) * 0x4) bits 15:0) field PAUSE_TIME of register PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME index N=0..3 */
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
        /* ((0x000a0010 + (N) * 0x4) bits 15:0) field PAUSE_TIME of register PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME index N=0..3 */
        mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF + subfield_offset),
                                                         TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_TIME_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_TIME_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_TIME_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_TIME_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_TIME_get", stop_bit, 15 );
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
    /* ((0x000a0010 + (N) * 0x4) bits 15:0) field PAUSE_TIME of register PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME index N=0..3 */
    reg_value = mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_MSK)
                  >> TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_MSK, TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_TIME_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_TX_CHAN_RESET_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_TX_CHAN_RESET_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_CHAN_RESET_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_TX_CHAN_RESET_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_TX_CHAN_RESET_set", N, value );

    /* ((0x000a0080 + (N) * 0x4) bits 0) field TX_CHAN_RESET of register PMC_TGFPF96_CORE_REG_TX_CHAN_RST index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_RST_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_MSK,
                                                  TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_OFF,
                                                  value);
}

static INLINE UINT32 mapotn_tgfpf_field_TX_CHAN_RESET_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_CHAN_RESET_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_CHAN_RESET_get", N, 95);
    /* ((0x000a0080 + (N) * 0x4) bits 0) field TX_CHAN_RESET of register PMC_TGFPF96_CORE_REG_TX_CHAN_RST index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_RST_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_MSK) >> TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_CHAN_RESET_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 23:16) field PREFETCH_FIFO_THRESH of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 23:16) field PREFETCH_FIFO_THRESH of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a0800 + (N) * 0x4) bits 23:16) field PREFETCH_FIFO_THRESH of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x000a0800 + (N) * 0x4) bits 23:16) field PREFETCH_FIFO_THRESH of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
        mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF + subfield_offset),
                                                   TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a0800 + (N) * 0x4) bits 23:16) field PREFETCH_FIFO_THRESH of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_MSK)
                  >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_MSK, TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_CPB_MAP_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_CPB_MAP_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_CPB_MAP_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_CPB_MAP_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_CPB_MAP_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 15) field CPB_MAP of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_CPB_MAP_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_CPB_MAP_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_CPB_MAP_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 15) field CPB_MAP of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_CPB_MAP_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 14) field AUTO_FLOW_CTRL_EN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 14) field AUTO_FLOW_CTRL_EN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 13:12) field PAUSE_FRM_TEMPLATE_XON of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 13:12) field PAUSE_FRM_TEMPLATE_XON of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a0800 + (N) * 0x4) bits 13:12) field PAUSE_FRM_TEMPLATE_XON of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
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
        /* ((0x000a0800 + (N) * 0x4) bits 13:12) field PAUSE_FRM_TEMPLATE_XON of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
        mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF + subfield_offset),
                                                   TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get", stop_bit, 1 );
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
    /* ((0x000a0800 + (N) * 0x4) bits 13:12) field PAUSE_FRM_TEMPLATE_XON of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_MSK)
                  >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_MSK, TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 11:10) field PAUSE_FRM_TEMPLATE_XOFF of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 11:10) field PAUSE_FRM_TEMPLATE_XOFF of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a0800 + (N) * 0x4) bits 11:10) field PAUSE_FRM_TEMPLATE_XOFF of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
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
        /* ((0x000a0800 + (N) * 0x4) bits 11:10) field PAUSE_FRM_TEMPLATE_XOFF of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
        mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF + subfield_offset),
                                                   TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get", stop_bit, 1 );
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
    /* ((0x000a0800 + (N) * 0x4) bits 11:10) field PAUSE_FRM_TEMPLATE_XOFF of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_MSK)
                  >> TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_MSK, TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 9) field INSERT_FRM_PRIORITY of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 9) field INSERT_FRM_PRIORITY of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 8) field MGMT_FRM_PRIORITY of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_PRIORITY_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_PRIORITY_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MGMT_FRM_PRIORITY_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 8) field MGMT_FRM_PRIORITY of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_PRIORITY_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_MGMT_FRM_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MGMT_FRM_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MGMT_FRM_LEN_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_LEN_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_LEN_set", N, value );

    /* ((0x000a0800 + (N) * 0x4) bits 7:0) field MGMT_FRM_LEN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_MSK,
                                               TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MGMT_FRM_LEN_get", N, 95);
    /* ((0x000a0800 + (N) * 0x4) bits 7:0) field MGMT_FRM_LEN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_MSK) >> TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_LEN_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_MGMT_FRM_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_MGMT_FRM_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a0800 + (N) * 0x4) bits 7:0) field MGMT_FRM_LEN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x000a0800 + (N) * 0x4) bits 7:0) field MGMT_FRM_LEN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
        mapotn_tgfpf_reg_CHAN_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF + subfield_offset),
                                                   TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_MGMT_FRM_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_MGMT_FRM_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a0800 + (N) * 0x4) bits 7:0) field MGMT_FRM_LEN of register PMC_TGFPF96_CORE_REG_CHAN_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_CHAN_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_MSK)
                  >> TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_MSK, TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MGMT_FRM_LEN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set", N, value );

    /* ((0x000a1008 + (N) * 0x20) bits 15:8) field PACKER_FIFO_LOW_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_get", N, 95);
    /* ((0x000a1008 + (N) * 0x20) bits 15:8) field PACKER_FIFO_LOW_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1008 + (N) * 0x20) bits 15:8) field PACKER_FIFO_LOW_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x000a1008 + (N) * 0x20) bits 15:8) field PACKER_FIFO_LOW_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a1008 + (N) * 0x20) bits 15:8) field PACKER_FIFO_LOW_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set", N, value );

    /* ((0x000a1008 + (N) * 0x20) bits 7:0) field PACKER_FIFO_HIGH_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_get", N, 95);
    /* ((0x000a1008 + (N) * 0x20) bits 7:0) field PACKER_FIFO_HIGH_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1008 + (N) * 0x20) bits 7:0) field PACKER_FIFO_HIGH_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x000a1008 + (N) * 0x20) bits 7:0) field PACKER_FIFO_HIGH_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a1008 + (N) * 0x20) bits 7:0) field PACKER_FIFO_HIGH_THRESH of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_PMON_UPDATE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PMON_UPDATE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_PMON_UPDATE_set", value );

    /* (0x000a0004 bits 0) field PMON_UPDATE of register PMC_TGFPF96_CORE_REG_PMON_CTRL */
    mapotn_tgfpf_reg_PMON_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_MSK,
                                          TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_OFF,
                                          value);
}

static INLINE UINT32 mapotn_tgfpf_field_PMON_UPDATE_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_UPDATE_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0004 bits 0) field PMON_UPDATE of register PMC_TGFPF96_CORE_REG_PMON_CTRL */
    reg_value = mapotn_tgfpf_reg_PMON_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_MSK) >> TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge_N_burstsize3
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_set");
    mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write( b_ptr,
                                                            h_ptr,
                                                            0,
                                                            3,
                                                            value);
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_get");
    mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read( b_ptr,
                                                           h_ptr,
                                                           0,
                                                           3,
                                                           (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_field_set( b_ptr,
                                                          h_ptr,
                                                          word_number,
                                                          mask,
                                                          field_ofs,
                                                          value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_read( b_ptr,
                                                                 h_ptr,
                                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_REQ_set");
    mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write( b_ptr,
                                                           h_ptr,
                                                           0,
                                                           3,
                                                           value);
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_REQ_get");
    mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read( b_ptr,
                                                          h_ptr,
                                                          0,
                                                          3,
                                                          (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_field_set( b_ptr,
                                                         h_ptr,
                                                         word_number,
                                                         mask,
                                                         field_ofs,
                                                         value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_read( b_ptr,
                                                                h_ptr,
                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_set", value );

    /* (0x000a0318 bits 1) field PMON_TRANSFER_END_INT_E of register PMC_TGFPF96_CORE_REG_MISC_INT_E */
    mapotn_tgfpf_reg_MISC_INT_E_field_set( b_ptr,
                                           h_ptr,
                                           TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_MSK,
                                           TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0318 bits 1) field PMON_TRANSFER_END_INT_E of register PMC_TGFPF96_CORE_REG_MISC_INT_E */
    reg_value = mapotn_tgfpf_reg_MISC_INT_E_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_MSK) >> TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_get", value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_set", value );

    /* (0x000a0318 bits 0) field PMON_TRANSFER_START_INT_E of register PMC_TGFPF96_CORE_REG_MISC_INT_E */
    mapotn_tgfpf_reg_MISC_INT_E_field_set( b_ptr,
                                           h_ptr,
                                           TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_MSK,
                                           TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0318 bits 0) field PMON_TRANSFER_START_INT_E of register PMC_TGFPF96_CORE_REG_MISC_INT_E */
    reg_value = mapotn_tgfpf_reg_MISC_INT_E_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_MSK) >> TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_burstsize3
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_set");
    mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write( b_ptr,
                                                            h_ptr,
                                                            0,
                                                            3,
                                                            value);
}

static INLINE void mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_get");
    mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read( b_ptr,
                                                           h_ptr,
                                                           0,
                                                           3,
                                                           (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_field_set( b_ptr,
                                                          h_ptr,
                                                          word_number,
                                                          mask,
                                                          field_ofs,
                                                          value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_read( b_ptr,
                                                                 h_ptr,
                                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_LEN_ERR_INT_E_set");
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               3,
                                                               value);
}

static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_LEN_ERR_INT_E_get");
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              3,
                                                              (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_field_set( b_ptr,
                                                             h_ptr,
                                                             word_number,
                                                             mask,
                                                             field_ofs,
                                                             value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_read( b_ptr,
                                                                    h_ptr,
                                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_set");
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              3,
                                                              value);
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_get");
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read( b_ptr,
                                                             h_ptr,
                                                             0,
                                                             3,
                                                             (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_field_set( b_ptr,
                                                            h_ptr,
                                                            word_number,
                                                            mask,
                                                            field_ofs,
                                                            value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_read( b_ptr,
                                                                   h_ptr,
                                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_set");
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write( b_ptr,
                                                             h_ptr,
                                                             0,
                                                             3,
                                                             value);
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_get");
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read( b_ptr,
                                                            h_ptr,
                                                            0,
                                                            3,
                                                            (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                      mapotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_field_set( b_ptr,
                                                           h_ptr,
                                                           word_number,
                                                           mask,
                                                           field_ofs,
                                                           value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_read( b_ptr,
                                                                  h_ptr,
                                                                  word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_set");
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               3,
                                                               value);
}

static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_get");
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              3,
                                                              (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_field_set( b_ptr,
                                                             h_ptr,
                                                             word_number,
                                                             mask,
                                                             field_ofs,
                                                             value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_read( b_ptr,
                                                                    h_ptr,
                                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_set");
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write( b_ptr,
                                                                h_ptr,
                                                                0,
                                                                3,
                                                                value);
}

static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_get");
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               3,
                                                               (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_field_set( b_ptr,
                                                              h_ptr,
                                                              word_number,
                                                              mask,
                                                              field_ofs,
                                                              value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_read( b_ptr,
                                                                     h_ptr,
                                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_set");
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write( b_ptr,
                                                                  h_ptr,
                                                                  0,
                                                                  3,
                                                                  value);
}

static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_get");
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read( b_ptr,
                                                                 h_ptr,
                                                                 0,
                                                                 3,
                                                                 (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_set", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_field_set( b_ptr,
                                                                h_ptr,
                                                                word_number,
                                                                mask,
                                                                field_ofs,
                                                                value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_read( b_ptr,
                                                                       h_ptr,
                                                                       word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_set_to_clear", value );

    /* (0x000a0328 bits 1) field PMON_TRANSFER_END_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_MSK,
                                                               TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0328 bits 1) field PMON_TRANSFER_END_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    reg_value = mapotn_tgfpf_reg_MISC_INT_EVENT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_MSK) >> TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_poll", value );

    /* (0x000a0328 bits 1) field PMON_TRANSFER_END_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    return mapotn_tgfpf_reg_MISC_INT_EVENT_poll( b_ptr,
                                                 h_ptr,
                                                 TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_MSK,
                                                 (value<<TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                              mapotn_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_set_to_clear", value );

    /* (0x000a0328 bits 0) field PMON_TRANSFER_START_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_MSK,
                                                               TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0328 bits 0) field PMON_TRANSFER_START_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    reg_value = mapotn_tgfpf_reg_MISC_INT_EVENT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_MSK) >> TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                      mapotn_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                      mapotn_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_poll", value );

    /* (0x000a0328 bits 0) field PMON_TRANSFER_START_INT_I of register PMC_TGFPF96_CORE_REG_MISC_INT_EVENT */
    return mapotn_tgfpf_reg_MISC_INT_EVENT_poll( b_ptr,
                                                 h_ptr,
                                                 TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_MSK,
                                                 (value<<TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_burstsize3
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_LEN_ERR_INT_I_set_to_clear");
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write( b_ptr,
                                                                   h_ptr,
                                                                   0,
                                                                   3,
                                                                   value);
}

static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_LEN_ERR_INT_I_get");
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read( b_ptr,
                                                                  h_ptr,
                                                                  0,
                                                                  3,
                                                                  (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 word_number,
                                                                                 mask,
                                                                                 field_ofs,
                                                                                 value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_read( b_ptr,
                                                                        h_ptr,
                                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll( b_ptr,
                                                                   h_ptr,
                                                                   word_number,
                                                                   mask,
                                                                   ( value << field_ofs),
                                                                   cmp,
                                                                   max_count,
                                                                   num_failed_polls,
                                                                   delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_set_to_clear");
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write( b_ptr,
                                                                 h_ptr,
                                                                 0,
                                                                 3,
                                                                 value);
}

static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_get");
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read( b_ptr,
                                                                h_ptr,
                                                                0,
                                                                3,
                                                                (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                               h_ptr,
                                                                               word_number,
                                                                               mask,
                                                                               field_ofs,
                                                                               value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_read( b_ptr,
                                                                      h_ptr,
                                                                      word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                        mapotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                        mapotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll( b_ptr,
                                                                 h_ptr,
                                                                 word_number,
                                                                 mask,
                                                                 ( value << field_ofs),
                                                                 cmp,
                                                                 max_count,
                                                                 num_failed_polls,
                                                                 delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                         mapotn_handle_t *h_ptr,
                                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_set_to_clear");
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write( b_ptr,
                                                                h_ptr,
                                                                0,
                                                                3,
                                                                value);
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_get");
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               3,
                                                               (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                               mapotn_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                              h_ptr,
                                                                              word_number,
                                                                              mask,
                                                                              field_ofs,
                                                                              value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_read( b_ptr,
                                                                     h_ptr,
                                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                       mapotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                       mapotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll( b_ptr,
                                                                h_ptr,
                                                                word_number,
                                                                mask,
                                                                ( value << field_ofs),
                                                                cmp,
                                                                max_count,
                                                                num_failed_polls,
                                                                delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_set_to_clear");
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write( b_ptr,
                                                                  h_ptr,
                                                                  0,
                                                                  3,
                                                                  value);
}

static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_get");
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read( b_ptr,
                                                                 h_ptr,
                                                                 0,
                                                                 3,
                                                                 (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                                h_ptr,
                                                                                word_number,
                                                                                mask,
                                                                                field_ofs,
                                                                                value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_read( b_ptr,
                                                                       h_ptr,
                                                                       word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll( b_ptr,
                                                                  h_ptr,
                                                                  word_number,
                                                                  mask,
                                                                  ( value << field_ofs),
                                                                  cmp,
                                                                  max_count,
                                                                  num_failed_polls,
                                                                  delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_set_to_clear");
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write( b_ptr,
                                                                   h_ptr,
                                                                   0,
                                                                   3,
                                                                   value);
}

static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_get");
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read( b_ptr,
                                                                  h_ptr,
                                                                  0,
                                                                  3,
                                                                  (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                  mapotn_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 word_number,
                                                                                 mask,
                                                                                 field_ofs,
                                                                                 value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_read( b_ptr,
                                                                        h_ptr,
                                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                          mapotn_handle_t *h_ptr,
                                                                                          UINT32 start_bit,
                                                                                          UINT32 stop_bit,
                                                                                          UINT32 value,
                                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                                          UINT32 max_count,
                                                                                          UINT32 *num_failed_polls,
                                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                          mapotn_handle_t *h_ptr,
                                                                                          UINT32 start_bit,
                                                                                          UINT32 stop_bit,
                                                                                          UINT32 value,
                                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                                          UINT32 max_count,
                                                                                          UINT32 *num_failed_polls,
                                                                                          UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll( b_ptr,
                                                                   h_ptr,
                                                                   word_number,
                                                                   mask,
                                                                   ( value << field_ofs),
                                                                   cmp,
                                                                   max_count,
                                                                   num_failed_polls,
                                                                   delay_between_polls_in_microseconds);
}

static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_set_to_clear");
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write( b_ptr,
                                                                      h_ptr,
                                                                      0,
                                                                      3,
                                                                      value);
}

static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_get");
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read( b_ptr,
                                                                     h_ptr,
                                                                     0,
                                                                     3,
                                                                     (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                 mapotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit, value);
    mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_action_on_write_field_set( b_ptr,
                                                                                    h_ptr,
                                                                                    word_number,
                                                                                    mask,
                                                                                    field_ofs,
                                                                                    value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_read( b_ptr,
                                                                           h_ptr,
                                                                           word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit);
    return mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll( b_ptr,
                                                                      h_ptr,
                                                                      word_number,
                                                                      mask,
                                                                      ( value << field_ofs),
                                                                      cmp,
                                                                      max_count,
                                                                      num_failed_polls,
                                                                      delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_M_size96__N_burstsize16
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_EXT_HDR_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  M,
                                                        UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_EXT_HDR_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  M,
                                                        UINT32 value[16] )
{
    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_EXT_HDR_MEM_set", M, 95);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_EXT_HDR_MEM_set", M);
    mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    M,
                                                    0,
                                                    16,
                                                    value);
}

static INLINE void mapotn_tgfpf_lfield_EXT_HDR_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  M,
                                                        UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_EXT_HDR_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  M,
                                                        UINT32 value[16] )
{
    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_EXT_HDR_MEM_get", M, 95);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_EXT_HDR_MEM_get", M);
    mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   M,
                                                   0,
                                                   16,
                                                   (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  M,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  M,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set", M, 95);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set", M, start_bit, stop_bit, value);
    mapotn_tgfpf_reg_EXT_HDR_RAM_array_field_set( b_ptr,
                                                  h_ptr,
                                                  M,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  M,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  M,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get", M, 95);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_EXT_HDR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         M,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get", M, start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_M_size96__N_burstsize32
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  M,
                                                           UINT32 value[32] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  M,
                                                           UINT32 value[32] )
{
    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_MGMT_FRAME_MEM_set", M, 95);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_MGMT_FRAME_MEM_set", M);
    mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       M,
                                                       0,
                                                       32,
                                                       value);
}

static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  M,
                                                           UINT32 value[32] ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  M,
                                                           UINT32 value[32] )
{
    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_MGMT_FRAME_MEM_get", M, 95);
    IOLOG( "%s  M=%d", "mapotn_tgfpf_lfield_MGMT_FRAME_MEM_get", M);
    mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      M,
                                                      0,
                                                      32,
                                                      (UINT32 *)value);
}

static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  M,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set", M, 95);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set", M, start_bit, stop_bit, value);
    mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     M,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  M,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                   mapotn_handle_t *h_ptr,
                                                                   UINT32  M,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (M > 95)
        IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get", M, 95);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get", start_bit, stop_bit );
    }
    reg_value = mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_read( b_ptr,
                                                            h_ptr,
                                                            M,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d value=0x%08x", "mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get", M, start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_EXT_HDR_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_EXT_HDR_EN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_EXT_HDR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_EXT_HDR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_EXT_HDR_EN_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 31) field EXT_HDR_EN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_EXT_HDR_EN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_EXT_HDR_EN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_EXT_HDR_EN_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 31) field EXT_HDR_EN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_EXT_HDR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_EXT_HDR_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_EXT_HDR_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_EXT_HDR_LEN_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_EXT_HDR_LEN_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_EXT_HDR_LEN_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 30:24) field EXT_HDR_LEN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_EXT_HDR_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_EXT_HDR_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_EXT_HDR_LEN_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 30:24) field EXT_HDR_LEN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_EXT_HDR_LEN_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_EXT_HDR_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_EXT_HDR_LEN_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_EXT_HDR_LEN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 30:24) field EXT_HDR_LEN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        /* ((0x000a1000 + (N) * 0x20) bits 30:24) field EXT_HDR_LEN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_EXT_HDR_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_EXT_HDR_LEN_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_EXT_HDR_LEN_get", stop_bit, 6 );
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
    /* ((0x000a1000 + (N) * 0x20) bits 30:24) field EXT_HDR_LEN of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_EXT_HDR_LEN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_TYPE_HDR_PTI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_TYPE_HDR_PTI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_PTI_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_PTI_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_PTI_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 22:20) field TYPE_HDR_PTI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_PTI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_PTI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_PTI_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 22:20) field TYPE_HDR_PTI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_PTI_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_PTI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_PTI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 22:20) field TYPE_HDR_PTI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        /* ((0x000a1000 + (N) * 0x20) bits 22:20) field TYPE_HDR_PTI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_PTI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_PTI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_get", stop_bit, 2 );
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
    /* ((0x000a1000 + (N) * 0x20) bits 22:20) field TYPE_HDR_PTI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_PTI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_TYPE_HDR_EXI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_TYPE_HDR_EXI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_EXI_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_EXI_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_EXI_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 19:16) field TYPE_HDR_EXI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_EXI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_EXI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_EXI_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 19:16) field TYPE_HDR_EXI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_EXI_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_EXI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_EXI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 19:16) field TYPE_HDR_EXI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        /* ((0x000a1000 + (N) * 0x20) bits 19:16) field TYPE_HDR_EXI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_EXI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_EXI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_get", stop_bit, 3 );
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
    /* ((0x000a1000 + (N) * 0x20) bits 19:16) field TYPE_HDR_EXI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_EXI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_TYPE_HDR_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_TYPE_HDR_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_UPI_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_UPI_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_UPI_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 15:8) field TYPE_HDR_UPI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TYPE_HDR_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TYPE_HDR_UPI_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 15:8) field TYPE_HDR_UPI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TYPE_HDR_UPI_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_TYPE_HDR_UPI_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 15:8) field TYPE_HDR_UPI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x000a1000 + (N) * 0x20) bits 15:8) field TYPE_HDR_UPI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TYPE_HDR_UPI_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a1000 + (N) * 0x20) bits 15:8) field TYPE_HDR_UPI of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TYPE_HDR_UPI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_FCS_INS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_FCS_INS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_FCS_INS_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_FCS_INS_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_FCS_INS_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 7:6) field FCS_INS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_FCS_INS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_FCS_INS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_FCS_INS_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 7:6) field FCS_INS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_FCS_INS_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_FCS_INS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_FCS_INS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_FCS_INS_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_FCS_INS_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_FCS_INS_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_FCS_INS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 7:6) field FCS_INS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        /* ((0x000a1000 + (N) * 0x20) bits 7:6) field FCS_INS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_FCS_INS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_FCS_INS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_FCS_INS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_FCS_INS_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_FCS_INS_get", stop_bit, 1 );
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
    /* ((0x000a1000 + (N) * 0x20) bits 7:6) field FCS_INS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_FCS_INS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_FCS_INS_POS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_FCS_INS_POS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_FCS_INS_POS_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_FCS_INS_POS_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_FCS_INS_POS_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 5:4) field FCS_INS_POS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_FCS_INS_POS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_FCS_INS_POS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_FCS_INS_POS_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 5:4) field FCS_INS_POS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_FCS_INS_POS_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_FCS_INS_POS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_FCS_INS_POS_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_FCS_INS_POS_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_FCS_INS_POS_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_FCS_INS_POS_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_FCS_INS_POS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a1000 + (N) * 0x20) bits 5:4) field FCS_INS_POS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
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
        /* ((0x000a1000 + (N) * 0x20) bits 5:4) field FCS_INS_POS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_FCS_INS_POS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_FCS_INS_POS_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_FCS_INS_POS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_FCS_INS_POS_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_FCS_INS_POS_get", stop_bit, 1 );
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
    /* ((0x000a1000 + (N) * 0x20) bits 5:4) field FCS_INS_POS of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_MSK)
                  >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_MSK, TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_FCS_INS_POS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_UPI_SEL_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_UPI_SEL_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_UPI_SEL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_UPI_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_UPI_SEL_set", N, value );

    /* ((0x000a1000 + (N) * 0x20) bits 3) field UPI_SEL of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_UPI_SEL_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_UPI_SEL_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_UPI_SEL_get", N, 95);
    /* ((0x000a1000 + (N) * 0x20) bits 3) field UPI_SEL of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_UPI_SEL_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_SCRMBL_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_SCRMBL_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_SCRMBL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_SCRMBL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_SCRMBL_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 7) field SCRMBL of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_SCRMBL_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_SCRMBL_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_SCRMBL_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 7) field SCRMBL of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_SCRMBL_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_REV_OBYTE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_REV_OBYTE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_OBYTE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBYTE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBYTE_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 6) field PFCS_REV_OBYTE of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_OBYTE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_OBYTE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_OBYTE_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 6) field PFCS_REV_OBYTE of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBYTE_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_REV_OBIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_REV_OBIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_OBIT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBIT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBIT_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 5) field PFCS_REV_OBIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_OBIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_OBIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_OBIT_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 5) field PFCS_REV_OBIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_OBIT_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_REV_IBIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_REV_IBIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_IBIT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_REV_IBIT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_IBIT_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 4) field PFCS_REV_IBIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_IBIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_REV_IBIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_REV_IBIT_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 4) field PFCS_REV_IBIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_REV_IBIT_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_CRPT_INV_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_CRPT_INV_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_CRPT_INV_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_CRPT_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_CRPT_INV_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 3) field PFCS_CRPT_INV of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_CRPT_INV_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_CRPT_INV_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_CRPT_INV_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 3) field PFCS_CRPT_INV of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_CRPT_INV_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_INV_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_INV_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_INV_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_INV_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 2) field PFCS_INV of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_INV_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_INV_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_INV_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 2) field PFCS_INV of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_INV_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_PFCS_INIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_PFCS_INIT_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_INIT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_PFCS_INIT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_INIT_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 1) field PFCS_INIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_PFCS_INIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PFCS_INIT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PFCS_INIT_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 1) field PFCS_INIT of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PFCS_INIT_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_DC_BALANCE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_DC_BALANCE_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_DC_BALANCE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_DC_BALANCE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_DC_BALANCE_set", N, value );

    /* ((0x000a1004 + (N) * 0x20) bits 0) field DC_BALANCE of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_MSK,
                                                       TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_DC_BALANCE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_DC_BALANCE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_DC_BALANCE_get", N, 95);
    /* ((0x000a1004 + (N) * 0x20) bits 0) field DC_BALANCE of register PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_MSK) >> TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_DC_BALANCE_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_MAX_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MAX_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MAX_FRM_LENGTH_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MAX_FRM_LENGTH_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_MAX_FRM_LENGTH_set", N, value );

    /* ((0x000a100c + (N) * 0x20) bits 31:16) field MAX_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_MSK,
                                                       TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_MAX_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MAX_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MAX_FRM_LENGTH_get", N, 95);
    /* ((0x000a100c + (N) * 0x20) bits 31:16) field MAX_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_MSK) >> TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_MAX_FRM_LENGTH_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a100c + (N) * 0x20) bits 31:16) field MAX_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
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
        /* ((0x000a100c + (N) * 0x20) bits 31:16) field MAX_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
        mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get", stop_bit, 15 );
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
    /* ((0x000a100c + (N) * 0x20) bits 31:16) field MAX_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_MSK)
                  >> TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_MSK, TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_MIN_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MIN_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MIN_FRM_LENGTH_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MIN_FRM_LENGTH_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_tgfpf_field_MIN_FRM_LENGTH_set", N, value );

    /* ((0x000a100c + (N) * 0x20) bits 15:0) field MIN_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_MSK,
                                                       TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF,
                                                       value);
}

static INLINE UINT32 mapotn_tgfpf_field_MIN_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MIN_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MIN_FRM_LENGTH_get", N, 95);
    /* ((0x000a100c + (N) * 0x20) bits 15:0) field MIN_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_MSK) >> TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_MIN_FRM_LENGTH_get", N, value );

    return value;
}
static INLINE void mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000a100c + (N) * 0x20) bits 15:0) field MIN_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
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
        /* ((0x000a100c + (N) * 0x20) bits 15:0) field MIN_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
        mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF + subfield_offset),
                                                           TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get", stop_bit, 15 );
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
    /* ((0x000a100c + (N) * 0x20) bits 15:0) field MIN_FRM_LENGTH of register PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG index N=0..95 */
    reg_value = mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_MSK)
                  >> TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_MSK, TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_TX_BYTE_CNT0_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_BYTE_CNT0_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_BYTE_CNT0_get", N, 95);
    /* ((0x000a8000 + (N) * 0x40) bits 31:0) field TX_BYTE_CNT0 of register PMC_TGFPF96_CORE_REG_PMON_0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_0_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_MSK) >> TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_BYTE_CNT0_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT0_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT0_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TX_BYTE_CNT0_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TX_BYTE_CNT0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TX_BYTE_CNT0_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8000 + (N) * 0x40) bits 31:0) field TX_BYTE_CNT0 of register PMC_TGFPF96_CORE_REG_PMON_0 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_0_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_MSK)
                  >> TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_MSK, TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TX_BYTE_CNT0_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_TX_BYTE_CNT1_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_BYTE_CNT1_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_BYTE_CNT1_get", N, 95);
    /* ((0x000a8004 + (N) * 0x40) bits 7:0) field TX_BYTE_CNT1 of register PMC_TGFPF96_CORE_REG_PMON_1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_1_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_MSK) >> TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_BYTE_CNT1_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT1_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT1_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TX_BYTE_CNT1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TX_BYTE_CNT1_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TX_BYTE_CNT1_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8004 + (N) * 0x40) bits 7:0) field TX_BYTE_CNT1 of register PMC_TGFPF96_CORE_REG_PMON_1 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_1_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_MSK)
                  >> TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_MSK, TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TX_BYTE_CNT1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MGMT_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_MGMT_FRM_CNT_get", N, 95);
    /* ((0x000a8008 + (N) * 0x40) bits 31:0) field MGMT_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_2_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_MSK) >> TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_MGMT_FRM_CNT_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_MGMT_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_MGMT_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_MGMT_FRM_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_MGMT_FRM_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_MGMT_FRM_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8008 + (N) * 0x40) bits 31:0) field MGMT_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_2 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_2_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_MSK)
                  >> TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_MSK, TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_MGMT_FRM_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_FRM_CNT_get", N, 95);
    /* ((0x000a800c + (N) * 0x40) bits 31:0) field TX_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_3 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_3_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_MSK) >> TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_FRM_CNT_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TX_FRM_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a800c + (N) * 0x40) bits 31:0) field TX_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_3 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_3_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_MSK)
                  >> TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_MSK, TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TX_FRM_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PAUSE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_PAUSE_FRM_CNT_get", N, 95);
    /* ((0x000a8010 + (N) * 0x40) bits 31:0) field PAUSE_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_4 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_4_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_MSK) >> TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_PAUSE_FRM_CNT_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8010 + (N) * 0x40) bits 31:0) field PAUSE_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_4 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_4_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_MSK)
                  >> TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_MSK, TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_IDLE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_IDLE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_IDLE_FRM_CNT_get", N, 95);
    /* ((0x000a8014 + (N) * 0x40) bits 31:0) field IDLE_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_5 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_5_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_MSK) >> TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_IDLE_FRM_CNT_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_IDLE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_IDLE_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_IDLE_FRM_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_IDLE_FRM_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_IDLE_FRM_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8014 + (N) * 0x40) bits 31:0) field IDLE_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_5 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_5_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_MSK)
                  >> TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_MSK, TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_IDLE_FRM_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_ERR_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_ERR_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_ERR_FRM_CNT_get", N, 95);
    /* ((0x000a8018 + (N) * 0x40) bits 31:0) field ERR_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_6 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_6_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_MSK) >> TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_ERR_FRM_CNT_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_ERR_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_ERR_FRM_CNT_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                               mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_ERR_FRM_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_ERR_FRM_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_ERR_FRM_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8018 + (N) * 0x40) bits 31:0) field ERR_FRM_CNT of register PMC_TGFPF96_CORE_REG_PMON_6 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_6_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_MSK)
                  >> TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_MSK, TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_ERR_FRM_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_UNDERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_UNDERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_FRM_CNT_UNDERSIZE_get", N, 95);
    /* ((0x000a801c + (N) * 0x40) bits 31:0) field TX_FRM_CNT_UNDERSIZE of register PMC_TGFPF96_CORE_REG_PMON_7 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_7_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_MSK) >> TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_FRM_CNT_UNDERSIZE_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                        mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a801c + (N) * 0x40) bits 31:0) field TX_FRM_CNT_UNDERSIZE of register PMC_TGFPF96_CORE_REG_PMON_7 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_7_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_MSK)
                  >> TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_MSK, TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_OVERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_TX_FRM_CNT_OVERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                 mapotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_TX_FRM_CNT_OVERSIZE_get", N, 95);
    /* ((0x000a8020 + (N) * 0x40) bits 31:0) field TX_FRM_CNT_OVERSIZE of register PMC_TGFPF96_CORE_REG_PMON_8 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_8_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_MSK) >> TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_tgfpf_field_TX_FRM_CNT_OVERSIZE_get", N, value );

    return value;
}
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get( mapotn_tgfpf_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x000a8020 + (N) * 0x40) bits 31:0) field TX_FRM_CNT_OVERSIZE of register PMC_TGFPF96_CORE_REG_PMON_8 index N=0..95 */
    reg_value = mapotn_tgfpf_reg_PMON_8_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_MSK)
                  >> TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_MSK, TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 mapotn_tgfpf_field_PMON_TIP_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_PMON_TIP_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0008 bits 0) field PMON_TIP of register PMC_TGFPF96_CORE_REG_PMON_STATUS */
    reg_value = mapotn_tgfpf_reg_PMON_STATUS_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_MSK) >> TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_PMON_TIP_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TIP_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TIP_poll( mapotn_tgfpf_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mapotn_tgfpf_field_PMON_TIP_poll", value );

    /* (0x000a0008 bits 0) field PMON_TIP of register PMC_TGFPF96_CORE_REG_PMON_STATUS */
    return mapotn_tgfpf_reg_PMON_STATUS_poll( b_ptr,
                                              h_ptr,
                                              TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_MSK,
                                              (value<<TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset test_set
 * ==================================================================================
 */
static INLINE void mapotn_tgfpf_field_MTX_CONFIG_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MTX_CONFIG_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MTX_CONFIG_MEM_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_MTX_CONFIG_MEM_set", value );

    /* (0x000a0380 bits 0) field MTX_CONFIG_MEM of register PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX */
    mapotn_tgfpf_reg_CFG_MEM_MUTEX_field_set( b_ptr,
                                              h_ptr,
                                              TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_MSK,
                                              TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_OFF,
                                              value);
}

static INLINE UINT32 mapotn_tgfpf_field_MTX_CONFIG_MEM_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MTX_CONFIG_MEM_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a0380 bits 0) field MTX_CONFIG_MEM of register PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX */
    reg_value = mapotn_tgfpf_reg_CFG_MEM_MUTEX_read(  b_ptr, h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_MSK) >> TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_MTX_CONFIG_MEM_get", value );

    return value;
}
static INLINE void mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_set( mapotn_tgfpf_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_set", value );

    /* (0x000a03a0 bits 0) field MTX_MGMT_FRAME_MEM of register PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX */
    mapotn_tgfpf_reg_MGMT_FRM_MUTEX_field_set( b_ptr,
                                               h_ptr,
                                               TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_MSK,
                                               TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_OFF,
                                               value);
}

static INLINE UINT32 mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_get( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000a03a0 bits 0) field MTX_MGMT_FRAME_MEM of register PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX */
    reg_value = mapotn_tgfpf_reg_MGMT_FRM_MUTEX_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_MSK) >> TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_TGFPF_IO_INLINE_H */
