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
 *     and register accessor functions for the sts_sw block
 *****************************************************************************/
#ifndef _STS_SW_IO_INLINE_H
#define _STS_SW_IO_INLINE_H

#include "sts_sw.h"
#include "sts_sw_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STS_SW_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sts_sw
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
    sts_sw_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sts_sw_buffer_t;
static INLINE void sts_sw_buffer_init( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sts_sw_buffer_init( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sts_sw_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sts_sw_buffer_flush( sts_sw_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sts_sw_buffer_flush( sts_sw_buffer_t *b_ptr )
{
    IOLOG( "sts_sw_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sts_sw_reg_read( sts_sw_buffer_t *b_ptr,
                                      sts_sw_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_read( sts_sw_buffer_t *b_ptr,
                                      sts_sw_handle_t *h_ptr,
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
static INLINE void sts_sw_reg_write( sts_sw_buffer_t *b_ptr,
                                     sts_sw_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_write( sts_sw_buffer_t *b_ptr,
                                     sts_sw_handle_t *h_ptr,
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

static INLINE void sts_sw_field_set( sts_sw_buffer_t *b_ptr,
                                     sts_sw_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_set( sts_sw_buffer_t *b_ptr,
                                     sts_sw_handle_t *h_ptr,
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

static INLINE void sts_sw_action_on_write_field_set( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_action_on_write_field_set( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
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

static INLINE void sts_sw_burst_read( sts_sw_buffer_t *b_ptr,
                                      sts_sw_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void sts_sw_burst_read( sts_sw_buffer_t *b_ptr,
                                      sts_sw_handle_t *h_ptr,
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

static INLINE void sts_sw_burst_write( sts_sw_buffer_t *b_ptr,
                                       sts_sw_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void sts_sw_burst_write( sts_sw_buffer_t *b_ptr,
                                       sts_sw_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sts_sw_poll( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sts_sw_poll( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
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
 *  register access functions for sts_sw
 * ==================================================================================
 */

static INLINE void sts_sw_reg_CONFIG_write( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_CONFIG_write( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sts_sw_reg_CONFIG_write", value );
    sts_sw_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_STS_SW_REG_CONFIG,
                      value);
}

static INLINE void sts_sw_reg_CONFIG_field_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_CONFIG_field_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sts_sw_reg_CONFIG_field_set", mask, ofs, value );
    sts_sw_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_STS_SW_REG_CONFIG,
                      mask,
                      PMC_STS_SW_REG_CONFIG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sts_sw_reg_CONFIG_read( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_CONFIG_read( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sts_sw_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_STS_SW_REG_CONFIG);

    IOLOG( "%s -> 0x%08x;", "sts_sw_reg_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void sts_sw_reg_IPT_array_write( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_IPT_array_write( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sts_sw_reg_IPT_array_write", value );
    sts_sw_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_STS_SW_REG_IPT(N),
                      value);
}

static INLINE void sts_sw_reg_IPT_array_field_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_IPT_array_field_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sts_sw_reg_IPT_array_field_set", N, mask, ofs, value );
    sts_sw_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_STS_SW_REG_IPT(N),
                      mask,
                      PMC_STS_SW_REG_IPT_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sts_sw_reg_IPT_array_read( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_IPT_array_read( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = sts_sw_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_STS_SW_REG_IPT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sts_sw_reg_IPT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sts_sw_reg_HO_CFG_array_write( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_HO_CFG_array_write( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sts_sw_reg_HO_CFG_array_write", value );
    sts_sw_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_HO_CFG(N),
                      value);
}

static INLINE void sts_sw_reg_HO_CFG_array_field_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_HO_CFG_array_field_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sts_sw_reg_HO_CFG_array_field_set", N, mask, ofs, value );
    sts_sw_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_HO_CFG(N),
                      mask,
                      PMC_STS_SW_REG_HO_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sts_sw_reg_HO_CFG_array_read( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_HO_CFG_array_read( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = sts_sw_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_STS_SW_REG_HO_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sts_sw_reg_HO_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sts_sw_reg_HO_CYCLE_CFG_array_write( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_HO_CYCLE_CFG_array_write( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sts_sw_reg_HO_CYCLE_CFG_array_write", value );
    sts_sw_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_HO_CYCLE_CFG(N),
                      value);
}

static INLINE void sts_sw_reg_HO_CYCLE_CFG_array_field_set( sts_sw_buffer_t *b_ptr,
                                                            sts_sw_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_HO_CYCLE_CFG_array_field_set( sts_sw_buffer_t *b_ptr,
                                                            sts_sw_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sts_sw_reg_HO_CYCLE_CFG_array_field_set", N, mask, ofs, value );
    sts_sw_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_HO_CYCLE_CFG(N),
                      mask,
                      PMC_STS_SW_REG_HO_CYCLE_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sts_sw_reg_HO_CYCLE_CFG_array_read( sts_sw_buffer_t *b_ptr,
                                                         sts_sw_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_HO_CYCLE_CFG_array_read( sts_sw_buffer_t *b_ptr,
                                                         sts_sw_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = sts_sw_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_STS_SW_REG_HO_CYCLE_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sts_sw_reg_HO_CYCLE_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sts_sw_reg_CALENDAR_CHANNEL_ID_array_write( sts_sw_buffer_t *b_ptr,
                                                               sts_sw_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_CALENDAR_CHANNEL_ID_array_write( sts_sw_buffer_t *b_ptr,
                                                               sts_sw_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sts_sw_reg_CALENDAR_CHANNEL_ID_array_write", value );
    sts_sw_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_CALENDAR_CHANNEL_ID(N),
                      value);
}

static INLINE void sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( sts_sw_buffer_t *b_ptr,
                                                                   sts_sw_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( sts_sw_buffer_t *b_ptr,
                                                                   sts_sw_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set", N, mask, ofs, value );
    sts_sw_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_STS_SW_REG_CALENDAR_CHANNEL_ID(N),
                      mask,
                      PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( sts_sw_buffer_t *b_ptr,
                                                                sts_sw_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( sts_sw_buffer_t *b_ptr,
                                                                sts_sw_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = sts_sw_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_STS_SW_REG_CALENDAR_CHANNEL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sts_sw_reg_CALENDAR_CHANNEL_ID_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void sts_sw_field_MXDX_MODE_EN_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_MXDX_MODE_EN_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_MXDX_MODE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sts_sw_field_MXDX_MODE_EN_set", value );

    /* (0x00000000 bits 8) field MXDX_MODE_EN of register PMC_STS_SW_REG_CONFIG */
    sts_sw_reg_CONFIG_field_set( b_ptr,
                                 h_ptr,
                                 STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_MSK,
                                 STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_OFF,
                                 value);
}

static INLINE UINT32 sts_sw_field_MXDX_MODE_EN_get( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_MXDX_MODE_EN_get( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field MXDX_MODE_EN of register PMC_STS_SW_REG_CONFIG */
    reg_value = sts_sw_reg_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_MSK) >> STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sts_sw_field_MXDX_MODE_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void sts_sw_field_IPT_VALID_set( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_IPT_VALID_set( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_IPT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_IPT_VALID_set", N, value );

    /* ((0x00002000 + (N) * 0x4) bits 16) field IPT_VALID of register PMC_STS_SW_REG_IPT index N=0..95 */
    sts_sw_reg_IPT_array_field_set( b_ptr,
                                    h_ptr,
                                    N,
                                    STS_SW_REG_IPT_BIT_IPT_VALID_MSK,
                                    STS_SW_REG_IPT_BIT_IPT_VALID_OFF,
                                    value);
}

static INLINE UINT32 sts_sw_field_IPT_VALID_get( sts_sw_buffer_t *b_ptr,
                                                 sts_sw_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_IPT_VALID_get( sts_sw_buffer_t *b_ptr,
                                                 sts_sw_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_VALID_get", N, 95);
    /* ((0x00002000 + (N) * 0x4) bits 16) field IPT_VALID of register PMC_STS_SW_REG_IPT index N=0..95 */
    reg_value = sts_sw_reg_IPT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_IPT_BIT_IPT_VALID_MSK) >> STS_SW_REG_IPT_BIT_IPT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_IPT_VALID_get", N, value );

    return value;
}
static INLINE void sts_sw_field_IPT_ID_set( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_IPT_ID_set( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_IPT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_IPT_ID_set", N, value );

    /* ((0x00002000 + (N) * 0x4) bits 14:8) field IPT_ID of register PMC_STS_SW_REG_IPT index N=0..95 */
    sts_sw_reg_IPT_array_field_set( b_ptr,
                                    h_ptr,
                                    N,
                                    STS_SW_REG_IPT_BIT_IPT_ID_MSK,
                                    STS_SW_REG_IPT_BIT_IPT_ID_OFF,
                                    value);
}

static INLINE UINT32 sts_sw_field_IPT_ID_get( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_IPT_ID_get( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_ID_get", N, 95);
    /* ((0x00002000 + (N) * 0x4) bits 14:8) field IPT_ID of register PMC_STS_SW_REG_IPT index N=0..95 */
    reg_value = sts_sw_reg_IPT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_IPT_BIT_IPT_ID_MSK) >> STS_SW_REG_IPT_BIT_IPT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_IPT_ID_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_IPT_ID_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_IPT_ID_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_IPT_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_IPT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_IPT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_IPT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002000 + (N) * 0x4) bits 14:8) field IPT_ID of register PMC_STS_SW_REG_IPT index N=0..95 */
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
        /* ((0x00002000 + (N) * 0x4) bits 14:8) field IPT_ID of register PMC_STS_SW_REG_IPT index N=0..95 */
        sts_sw_reg_IPT_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        subfield_mask << (STS_SW_REG_IPT_BIT_IPT_ID_OFF + subfield_offset),
                                        STS_SW_REG_IPT_BIT_IPT_ID_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_IPT_ID_get( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_IPT_ID_get( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_IPT_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_IPT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_IPT_ID_get", stop_bit, 6 );
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
    /* ((0x00002000 + (N) * 0x4) bits 14:8) field IPT_ID of register PMC_STS_SW_REG_IPT index N=0..95 */
    reg_value = sts_sw_reg_IPT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & STS_SW_REG_IPT_BIT_IPT_ID_MSK)
                  >> STS_SW_REG_IPT_BIT_IPT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_IPT_BIT_IPT_ID_MSK, STS_SW_REG_IPT_BIT_IPT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_IPT_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_IPT_VA_set( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_IPT_VA_set( sts_sw_buffer_t *b_ptr,
                                            sts_sw_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_VA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_IPT_VA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_IPT_VA_set", N, value );

    /* ((0x00002000 + (N) * 0x4) bits 6:0) field IPT_VA of register PMC_STS_SW_REG_IPT index N=0..95 */
    sts_sw_reg_IPT_array_field_set( b_ptr,
                                    h_ptr,
                                    N,
                                    STS_SW_REG_IPT_BIT_IPT_VA_MSK,
                                    STS_SW_REG_IPT_BIT_IPT_VA_OFF,
                                    value);
}

static INLINE UINT32 sts_sw_field_IPT_VA_get( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_IPT_VA_get( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_IPT_VA_get", N, 95);
    /* ((0x00002000 + (N) * 0x4) bits 6:0) field IPT_VA of register PMC_STS_SW_REG_IPT index N=0..95 */
    reg_value = sts_sw_reg_IPT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_IPT_BIT_IPT_VA_MSK) >> STS_SW_REG_IPT_BIT_IPT_VA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_IPT_VA_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_IPT_VA_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_IPT_VA_set( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_IPT_VA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_IPT_VA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_IPT_VA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_IPT_VA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002000 + (N) * 0x4) bits 6:0) field IPT_VA of register PMC_STS_SW_REG_IPT index N=0..95 */
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
        /* ((0x00002000 + (N) * 0x4) bits 6:0) field IPT_VA of register PMC_STS_SW_REG_IPT index N=0..95 */
        sts_sw_reg_IPT_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        subfield_mask << (STS_SW_REG_IPT_BIT_IPT_VA_OFF + subfield_offset),
                                        STS_SW_REG_IPT_BIT_IPT_VA_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_IPT_VA_get( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_IPT_VA_get( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_IPT_VA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_IPT_VA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_IPT_VA_get", stop_bit, 6 );
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
    /* ((0x00002000 + (N) * 0x4) bits 6:0) field IPT_VA of register PMC_STS_SW_REG_IPT index N=0..95 */
    reg_value = sts_sw_reg_IPT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & STS_SW_REG_IPT_BIT_IPT_VA_MSK)
                  >> STS_SW_REG_IPT_BIT_IPT_VA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_IPT_BIT_IPT_VA_MSK, STS_SW_REG_IPT_BIT_IPT_VA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_IPT_VA_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void sts_sw_field_EOMF_CNT_set( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_EOMF_CNT_set( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_EOMF_CNT_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_EOMF_CNT_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_EOMF_CNT_set", N, value );

    /* ((0x00001000 + (N) * 0x20) bits 22:16) field EOMF_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                       h_ptr,
                                       N,
                                       STS_SW_REG_HO_CFG_BIT_EOMF_CNT_MSK,
                                       STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF,
                                       value);
}

static INLINE UINT32 sts_sw_field_EOMF_CNT_get( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_EOMF_CNT_get( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_EOMF_CNT_get", N, 95);
    /* ((0x00001000 + (N) * 0x20) bits 22:16) field EOMF_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_HO_CFG_BIT_EOMF_CNT_MSK) >> STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_EOMF_CNT_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_EOMF_CNT_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_EOMF_CNT_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_EOMF_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_EOMF_CNT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_EOMF_CNT_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_EOMF_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x20) bits 22:16) field EOMF_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
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
        /* ((0x00001000 + (N) * 0x20) bits 22:16) field EOMF_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
        sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           subfield_mask << (STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF + subfield_offset),
                                           STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_EOMF_CNT_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_EOMF_CNT_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_EOMF_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_EOMF_CNT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_EOMF_CNT_get", stop_bit, 6 );
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
    /* ((0x00001000 + (N) * 0x20) bits 22:16) field EOMF_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & STS_SW_REG_HO_CFG_BIT_EOMF_CNT_MSK)
                  >> STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CFG_BIT_EOMF_CNT_MSK, STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_EOMF_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_LAST_CNT_set( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_LAST_CNT_set( sts_sw_buffer_t *b_ptr,
                                              sts_sw_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LAST_CNT_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_LAST_CNT_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_LAST_CNT_set", N, value );

    /* ((0x00001000 + (N) * 0x20) bits 14:8) field LAST_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                       h_ptr,
                                       N,
                                       STS_SW_REG_HO_CFG_BIT_LAST_CNT_MSK,
                                       STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF,
                                       value);
}

static INLINE UINT32 sts_sw_field_LAST_CNT_get( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_LAST_CNT_get( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LAST_CNT_get", N, 95);
    /* ((0x00001000 + (N) * 0x20) bits 14:8) field LAST_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_HO_CFG_BIT_LAST_CNT_MSK) >> STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_LAST_CNT_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_LAST_CNT_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_LAST_CNT_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_LAST_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_LAST_CNT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_LAST_CNT_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_LAST_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x20) bits 14:8) field LAST_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
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
        /* ((0x00001000 + (N) * 0x20) bits 14:8) field LAST_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
        sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           subfield_mask << (STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF + subfield_offset),
                                           STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_LAST_CNT_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_LAST_CNT_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_LAST_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_LAST_CNT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_LAST_CNT_get", stop_bit, 6 );
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
    /* ((0x00001000 + (N) * 0x20) bits 14:8) field LAST_CNT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & STS_SW_REG_HO_CFG_BIT_LAST_CNT_MSK)
                  >> STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CFG_BIT_LAST_CNT_MSK, STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_LAST_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_HO_CNT_TOT_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_HO_CNT_TOT_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_HO_CNT_TOT_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_HO_CNT_TOT_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_HO_CNT_TOT_set", N, value );

    /* ((0x00001000 + (N) * 0x20) bits 6:0) field HO_CNT_TOT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                       h_ptr,
                                       N,
                                       STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_MSK,
                                       STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF,
                                       value);
}

static INLINE UINT32 sts_sw_field_HO_CNT_TOT_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_HO_CNT_TOT_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_HO_CNT_TOT_get", N, 95);
    /* ((0x00001000 + (N) * 0x20) bits 6:0) field HO_CNT_TOT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_MSK) >> STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_HO_CNT_TOT_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_HO_CNT_TOT_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_HO_CNT_TOT_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_HO_CNT_TOT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_HO_CNT_TOT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_HO_CNT_TOT_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_HO_CNT_TOT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x20) bits 6:0) field HO_CNT_TOT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
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
        /* ((0x00001000 + (N) * 0x20) bits 6:0) field HO_CNT_TOT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
        sts_sw_reg_HO_CFG_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           subfield_mask << (STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF + subfield_offset),
                                           STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_HO_CNT_TOT_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_HO_CNT_TOT_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_HO_CNT_TOT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_HO_CNT_TOT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_HO_CNT_TOT_get", stop_bit, 6 );
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
    /* ((0x00001000 + (N) * 0x20) bits 6:0) field HO_CNT_TOT of register PMC_STS_SW_REG_HO_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CFG_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_MSK)
                  >> STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_MSK, STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_HO_CNT_TOT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_EOMF_CYCLE_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_EOMF_CYCLE_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_EOMF_CYCLE_set", N, value );

    /* ((0x00001010 + (N) * 0x20) bits 22:16) field EOMF_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_MSK,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF,
                                             value);
}

static INLINE UINT32 sts_sw_field_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_EOMF_CYCLE_get", N, 95);
    /* ((0x00001010 + (N) * 0x20) bits 22:16) field EOMF_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_MSK) >> STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_EOMF_CYCLE_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_EOMF_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_EOMF_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_EOMF_CYCLE_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_EOMF_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001010 + (N) * 0x20) bits 22:16) field EOMF_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
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
        /* ((0x00001010 + (N) * 0x20) bits 22:16) field EOMF_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
        sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF + subfield_offset),
                                                 STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_EOMF_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_EOMF_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_EOMF_CYCLE_get", stop_bit, 6 );
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
    /* ((0x00001010 + (N) * 0x20) bits 22:16) field EOMF_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_MSK)
                  >> STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_MSK, STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_EOMF_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_LAST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_LAST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                sts_sw_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LAST_CYCLE_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_LAST_CYCLE_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_LAST_CYCLE_set", N, value );

    /* ((0x00001010 + (N) * 0x20) bits 14:8) field LAST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_MSK,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF,
                                             value);
}

static INLINE UINT32 sts_sw_field_LAST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_LAST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                  sts_sw_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LAST_CYCLE_get", N, 95);
    /* ((0x00001010 + (N) * 0x20) bits 14:8) field LAST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_MSK) >> STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_LAST_CYCLE_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_LAST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_LAST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_LAST_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_LAST_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_LAST_CYCLE_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_LAST_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001010 + (N) * 0x20) bits 14:8) field LAST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
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
        /* ((0x00001010 + (N) * 0x20) bits 14:8) field LAST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
        sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF + subfield_offset),
                                                 STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_LAST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_LAST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_LAST_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_LAST_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_LAST_CYCLE_get", stop_bit, 6 );
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
    /* ((0x00001010 + (N) * 0x20) bits 14:8) field LAST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_MSK)
                  >> STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_MSK, STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_LAST_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                 sts_sw_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                 sts_sw_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_FIRST_CYCLE_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_FIRST_CYCLE_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_FIRST_CYCLE_set", N, value );

    /* ((0x00001010 + (N) * 0x20) bits 6:0) field FIRST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_MSK,
                                             STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF,
                                             value);
}

static INLINE UINT32 sts_sw_field_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_FIRST_CYCLE_get", N, 95);
    /* ((0x00001010 + (N) * 0x20) bits 6:0) field FIRST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_MSK) >> STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_FIRST_CYCLE_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                       sts_sw_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                       sts_sw_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_FIRST_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_FIRST_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_FIRST_CYCLE_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_FIRST_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001010 + (N) * 0x20) bits 6:0) field FIRST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
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
        /* ((0x00001010 + (N) * 0x20) bits 6:0) field FIRST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
        sts_sw_reg_HO_CYCLE_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF + subfield_offset),
                                                 STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                         sts_sw_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                         sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_FIRST_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_FIRST_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_FIRST_CYCLE_get", stop_bit, 6 );
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
    /* ((0x00001010 + (N) * 0x20) bits 6:0) field FIRST_CYCLE of register PMC_STS_SW_REG_HO_CYCLE_CFG index N=0..95 */
    reg_value = sts_sw_reg_HO_CYCLE_CFG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_MSK)
                  >> STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_MSK, STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_FIRST_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sts_sw_field_MAP_DMSIM_TO_SSF_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_MAP_DMSIM_TO_SSF_set( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_MAP_DMSIM_TO_SSF_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_MAP_DMSIM_TO_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_MAP_DMSIM_TO_SSF_set", N, value );

    /* ((0x00002200 + (N) * 0x4) bits 24) field MAP_DMSIM_TO_SSF of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_MSK,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_OFF,
                                                    value);
}

static INLINE UINT32 sts_sw_field_MAP_DMSIM_TO_SSF_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_MAP_DMSIM_TO_SSF_get( sts_sw_buffer_t *b_ptr,
                                                        sts_sw_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_MAP_DMSIM_TO_SSF_get", N, 95);
    /* ((0x00002200 + (N) * 0x4) bits 24) field MAP_DMSIM_TO_SSF of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    reg_value = sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_MSK) >> STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_MAP_DMSIM_TO_SSF_get", N, value );

    return value;
}
static INLINE void sts_sw_field_LO_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_LO_EOMF_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LO_EOMF_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_LO_EOMF_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_LO_EOMF_CYCLE_set", N, value );

    /* ((0x00002200 + (N) * 0x4) bits 16) field LO_EOMF_CYCLE of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_MSK,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_OFF,
                                                    value);
}

static INLINE UINT32 sts_sw_field_LO_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_LO_EOMF_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LO_EOMF_CYCLE_get", N, 95);
    /* ((0x00002200 + (N) * 0x4) bits 16) field LO_EOMF_CYCLE of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    reg_value = sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_MSK) >> STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_LO_EOMF_CYCLE_get", N, value );

    return value;
}
static INLINE void sts_sw_field_LO_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_LO_FIRST_CYCLE_set( sts_sw_buffer_t *b_ptr,
                                                    sts_sw_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LO_FIRST_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_LO_FIRST_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_LO_FIRST_CYCLE_set", N, value );

    /* ((0x00002200 + (N) * 0x4) bits 8) field LO_FIRST_CYCLE of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_MSK,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_OFF,
                                                    value);
}

static INLINE UINT32 sts_sw_field_LO_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_LO_FIRST_CYCLE_get( sts_sw_buffer_t *b_ptr,
                                                      sts_sw_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_LO_FIRST_CYCLE_get", N, 95);
    /* ((0x00002200 + (N) * 0x4) bits 8) field LO_FIRST_CYCLE of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    reg_value = sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_MSK) >> STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_LO_FIRST_CYCLE_get", N, value );

    return value;
}
static INLINE void sts_sw_field_CHAN_ID_set( sts_sw_buffer_t *b_ptr,
                                             sts_sw_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_CHAN_ID_set( sts_sw_buffer_t *b_ptr,
                                             sts_sw_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_CHAN_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sts_sw_field_CHAN_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "sts_sw_field_CHAN_ID_set", N, value );

    /* ((0x00002200 + (N) * 0x4) bits 6:0) field CHAN_ID of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_MSK,
                                                    STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF,
                                                    value);
}

static INLINE UINT32 sts_sw_field_CHAN_ID_get( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_CHAN_ID_get( sts_sw_buffer_t *b_ptr,
                                               sts_sw_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_CHAN_ID_get", N, 95);
    /* ((0x00002200 + (N) * 0x4) bits 6:0) field CHAN_ID of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    reg_value = sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_MSK) >> STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sts_sw_field_CHAN_ID_get", N, value );

    return value;
}
static INLINE void sts_sw_field_range_CHAN_ID_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sts_sw_field_range_CHAN_ID_set( sts_sw_buffer_t *b_ptr,
                                                   sts_sw_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_CHAN_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_CHAN_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_CHAN_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_CHAN_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002200 + (N) * 0x4) bits 6:0) field CHAN_ID of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
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
        /* ((0x00002200 + (N) * 0x4) bits 6:0) field CHAN_ID of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
        sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF + subfield_offset),
                                                        STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sts_sw_field_range_CHAN_ID_get( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sts_sw_field_range_CHAN_ID_get( sts_sw_buffer_t *b_ptr,
                                                     sts_sw_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sts_sw_field_range_CHAN_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sts_sw_field_range_CHAN_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sts_sw_field_range_CHAN_ID_get", stop_bit, 6 );
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
    /* ((0x00002200 + (N) * 0x4) bits 6:0) field CHAN_ID of register PMC_STS_SW_REG_CALENDAR_CHANNEL_ID index N=0..95 */
    reg_value = sts_sw_reg_CALENDAR_CHANNEL_ID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_MSK)
                  >> STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_MSK, STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sts_sw_field_range_CHAN_ID_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STS_SW_IO_INLINE_H */
