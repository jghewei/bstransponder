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
 *     and register accessor functions for the enet_fege block
 *****************************************************************************/
#ifndef _ENET_FEGE_IO_INLINE_H
#define _ENET_FEGE_IO_INLINE_H

#include "enet_fege_loc.h"
#include "enet_fege_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ENET_FEGE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for enet_fege
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
    enet_fege_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} enet_fege_buffer_t;
static INLINE void enet_fege_buffer_init( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void enet_fege_buffer_init( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "enet_fege_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void enet_fege_buffer_flush( enet_fege_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void enet_fege_buffer_flush( enet_fege_buffer_t *b_ptr )
{
    IOLOG( "enet_fege_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 enet_fege_reg_read( enet_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_read( enet_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
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
static INLINE void enet_fege_reg_write( enet_fege_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_write( enet_fege_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
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

static INLINE void enet_fege_field_set( enet_fege_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_set( enet_fege_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
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

static INLINE void enet_fege_action_on_write_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_action_on_write_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
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

static INLINE void enet_fege_burst_read( enet_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void enet_fege_burst_read( enet_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
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

static INLINE void enet_fege_burst_write( enet_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void enet_fege_burst_write( enet_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE enet_fege_poll( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_poll( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
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
 *  register access functions for enet_fege
 * ==================================================================================
 */

static INLINE void enet_fege_reg_MODE_CFG_write( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_MODE_CFG_write( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_MODE_CFG_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_MODE_CFG,
                         value);
}

static INLINE void enet_fege_reg_MODE_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_MODE_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_MODE_CFG_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_MODE_CFG,
                         mask,
                         PMC_ENET_FEGE_REG_MODE_CFG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_MODE_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_MODE_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_MODE_CFG);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_MODE_CFG_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_SW_RESET_write( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_SW_RESET_write( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_SW_RESET_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_SW_RESET,
                         value);
}

static INLINE void enet_fege_reg_SW_RESET_field_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_SW_RESET_field_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_SW_RESET_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_SW_RESET,
                         mask,
                         PMC_ENET_FEGE_REG_SW_RESET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_SW_RESET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_SW_RESET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_PTP_NTP_CFG_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_CFG_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_PTP_NTP_CFG_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_CFG,
                         value);
}

static INLINE void enet_fege_reg_PTP_NTP_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_PTP_NTP_CFG_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_CFG,
                         mask,
                         PMC_ENET_FEGE_REG_PTP_NTP_CFG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_PTP_NTP_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_PTP_NTP_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_PTP_NTP_CFG);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_PTP_NTP_CFG_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                       enet_fege_handle_t *h_ptr,
                                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                       enet_fege_handle_t *h_ptr,
                                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET,
                         value);
}

static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                           enet_fege_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                           enet_fege_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET,
                         mask,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                      enet_fege_handle_t *h_ptr,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                      enet_fege_handle_t *h_ptr,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET,
                         value);
}

static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                          enet_fege_handle_t *h_ptr,
                                                                                          UINT32 mask,
                                                                                          UINT32 ofs,
                                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                          enet_fege_handle_t *h_ptr,
                                                                                          UINT32 mask,
                                                                                          UINT32 ofs,
                                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET,
                         mask,
                         PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                       enet_fege_handle_t *h_ptr,
                                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                       enet_fege_handle_t *h_ptr,
                                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET,
                         value);
}

static INLINE void enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                           enet_fege_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                           enet_fege_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET,
                         mask,
                         PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                            enet_fege_handle_t *h_ptr,
                                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write( enet_fege_buffer_t *b_ptr,
                                                                                            enet_fege_handle_t *h_ptr,
                                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET,
                         value);
}

static INLINE void enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                                enet_fege_handle_t *h_ptr,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( enet_fege_buffer_t *b_ptr,
                                                                                                enet_fege_handle_t *h_ptr,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET,
                         mask,
                         PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_write( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_write( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT,
                         value);
}

static INLINE void enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_field_set( enet_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_field_set( enet_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT,
                         mask,
                         PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_TX_TSTMP_FIFO_CFG_write( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_TX_TSTMP_FIFO_CFG_write( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_TX_TSTMP_FIFO_CFG_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG,
                         value);
}

static INLINE void enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG,
                         mask,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_TX_TSTMP_FIFO_CFG_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_INT_INTP_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTP_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_INT_INTP_EN_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTP_EN,
                         value);
}

static INLINE void enet_fege_reg_INT_INTP_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTP_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_INT_INTP_EN_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTP_EN,
                         mask,
                         PMC_ENET_FEGE_REG_INT_INTP_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_INT_INTP_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_INTP_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_INT_INTP_EN);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_INTP_EN_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_INT_INTM_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTM_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_INT_INTM_EN_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTM_EN,
                         value);
}

static INLINE void enet_fege_reg_INT_INTM_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTM_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_INT_INTM_EN_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTM_EN,
                         mask,
                         PMC_ENET_FEGE_REG_INT_INTM_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_INT_INTM_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_INTM_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_INT_INTM_EN);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_INTM_EN_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_INT_INTD_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTD_EN_write( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_INT_INTD_EN_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTD_EN,
                         value);
}

static INLINE void enet_fege_reg_INT_INTD_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_INTD_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_INT_INTD_EN_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_INT_INTD_EN,
                         mask,
                         PMC_ENET_FEGE_REG_INT_INTD_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_INT_INTD_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_INTD_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_INT_INTD_EN);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_INTD_EN_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_write( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_write( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG,
                         value);
}

static INLINE void enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( enet_fege_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG,
                         mask,
                         PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read", reg_value);
    return reg_value;
}

static INLINE void enet_fege_reg_TX_TSTMP_FIFO_ACCESS_write( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_TX_TSTMP_FIFO_ACCESS_write( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_TX_TSTMP_FIFO_ACCESS_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS,
                         value);
}

static INLINE void enet_fege_reg_TX_TSTMP_FIFO_ACCESS_field_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_TX_TSTMP_FIFO_ACCESS_field_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_TX_TSTMP_FIFO_ACCESS_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS,
                         mask,
                         PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_TX_TSTMP_FIFO_ACCESS_poll( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_TX_TSTMP_FIFO_ACCESS_poll( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enet_fege_reg_TX_TSTMP_FIFO_ACCESS_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enet_fege_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void enet_fege_reg_INT_write( enet_fege_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_write( enet_fege_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_INT_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ENET_FEGE_REG_INT,
                         value);
}

static INLINE void enet_fege_reg_INT_action_on_write_field_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_action_on_write_field_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_INT_action_on_write_field_set", mask, ofs, value );
    enet_fege_action_on_write_field_set( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_ENET_FEGE_REG_INT,
                                         mask,
                                         ofs,
                                         value);

}

static INLINE UINT32 enet_fege_reg_INT_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_INT);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_INT_poll( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_INT_poll( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enet_fege_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enet_fege_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_ENET_FEGE_REG_INT,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void enet_fege_reg_INT_EN_write( enet_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_EN_write( enet_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_fege_reg_INT_EN_write", value );
    enet_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ENET_FEGE_REG_INT_EN,
                         value);
}

static INLINE void enet_fege_reg_INT_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_reg_INT_EN_field_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_fege_reg_INT_EN_field_set", mask, ofs, value );
    enet_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ENET_FEGE_REG_INT_EN,
                         mask,
                         PMC_ENET_FEGE_REG_INT_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 enet_fege_reg_INT_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_EN_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_INT_EN);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enet_fege_reg_INT_VAL_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_VAL_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_INT_VAL_poll( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_INT_VAL_poll( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enet_fege_reg_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enet_fege_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_ENET_FEGE_REG_INT_VAL,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enet_fege_reg_INT_SUMMARY_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_INT_SUMMARY_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_INT_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enet_fege_reg_TIP_SUMMARY_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_reg_TIP_SUMMARY_read( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_ENET_FEGE_REG_TIP_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "enet_fege_reg_TIP_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_TIP_SUMMARY_poll( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_reg_TIP_SUMMARY_poll( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enet_fege_reg_TIP_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enet_fege_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_ENET_FEGE_REG_TIP_SUMMARY,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void enet_fege_field_TSTMP_CORR_UPD_BYPASS_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TSTMP_CORR_UPD_BYPASS_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_BYPASS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_BYPASS_set", value );

    /* (0x00000000 bits 31) field TSTMP_CORR_UPD_BYPASS of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_TSTMP_CORR_UPD_BYPASS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TSTMP_CORR_UPD_BYPASS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 31) field TSTMP_CORR_UPD_BYPASS of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_BYPASS_get", value );

    return value;
}
static INLINE void enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set", value, 31);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set", value );

    /* (0x00000000 bits 29:25) bits 0:4 use field TSTMP_CORR_UPD_SKIP_BYTES of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 29:25) bits 0:4 use field TSTMP_CORR_UPD_SKIP_BYTES of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 29:25) bits 0:4 use field TSTMP_CORR_UPD_SKIP_BYTES of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000000 bits 29:25) bits 0:4 use field TSTMP_CORR_UPD_SKIP_BYTES of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000000 bits 29:25) bits 0:4 use field TSTMP_CORR_UPD_SKIP_BYTES of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_MSK, ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_AUTO_ERR_CLEAR_EN_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_AUTO_ERR_CLEAR_EN_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_AUTO_ERR_CLEAR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_AUTO_ERR_CLEAR_EN_set", value );

    /* (0x00000000 bits 20) field AUTO_ERR_CLEAR_EN of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_AUTO_ERR_CLEAR_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_AUTO_ERR_CLEAR_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 20) field AUTO_ERR_CLEAR_EN of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_AUTO_ERR_CLEAR_EN_get", value );

    return value;
}
static INLINE void enet_fege_field_IPG_RED_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_IPG_RED_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_IPG_RED_ON_THRESH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_IPG_RED_ON_THRESH_set", value );

    /* (0x00000000 bits 17:16) bits 0:1 use field IPG_RED_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_IPG_RED_ON_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_IPG_RED_ON_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 17:16) bits 0:1 use field IPG_RED_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_IPG_RED_ON_THRESH_get", value );

    return value;
}
static INLINE void enet_fege_field_range_IPG_RED_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_IPG_RED_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_IPG_RED_ON_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_IPG_RED_ON_THRESH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_IPG_RED_ON_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 17:16) bits 0:1 use field IPG_RED_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        /* (0x00000000 bits 17:16) bits 0:1 use field IPG_RED_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_IPG_RED_ON_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_IPG_RED_ON_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_IPG_RED_ON_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_IPG_RED_ON_THRESH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 17:16) bits 0:1 use field IPG_RED_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_MSK, ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_IPG_RED_ON_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_IPG_RED_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_IPG_RED_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_IPG_RED_OFF_THRESH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_IPG_RED_OFF_THRESH_set", value );

    /* (0x00000000 bits 15:14) bits 0:1 use field IPG_RED_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_IPG_RED_OFF_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_IPG_RED_OFF_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 15:14) bits 0:1 use field IPG_RED_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_IPG_RED_OFF_THRESH_get", value );

    return value;
}
static INLINE void enet_fege_field_range_IPG_RED_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_IPG_RED_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_IPG_RED_OFF_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_IPG_RED_OFF_THRESH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_IPG_RED_OFF_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 15:14) bits 0:1 use field IPG_RED_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        /* (0x00000000 bits 15:14) bits 0:1 use field IPG_RED_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_IPG_RED_OFF_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_IPG_RED_OFF_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_IPG_RED_OFF_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_IPG_RED_OFF_THRESH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 15:14) bits 0:1 use field IPG_RED_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_MSK, ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_IPG_RED_OFF_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_PAUSE_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_PAUSE_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_PAUSE_ON_THRESH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_PAUSE_ON_THRESH_set", value );

    /* (0x00000000 bits 13:12) bits 0:1 use field PAUSE_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_PAUSE_ON_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_PAUSE_ON_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 13:12) bits 0:1 use field PAUSE_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_PAUSE_ON_THRESH_get", value );

    return value;
}
static INLINE void enet_fege_field_range_PAUSE_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_PAUSE_ON_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_PAUSE_ON_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_PAUSE_ON_THRESH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_PAUSE_ON_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 13:12) bits 0:1 use field PAUSE_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        /* (0x00000000 bits 13:12) bits 0:1 use field PAUSE_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_PAUSE_ON_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_PAUSE_ON_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_PAUSE_ON_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_PAUSE_ON_THRESH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 13:12) bits 0:1 use field PAUSE_ON_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_MSK, ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_PAUSE_ON_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_PAUSE_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_PAUSE_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_PAUSE_OFF_THRESH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_PAUSE_OFF_THRESH_set", value );

    /* (0x00000000 bits 11:10) bits 0:1 use field PAUSE_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_PAUSE_OFF_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_PAUSE_OFF_THRESH_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 11:10) bits 0:1 use field PAUSE_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_PAUSE_OFF_THRESH_get", value );

    return value;
}
static INLINE void enet_fege_field_range_PAUSE_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_PAUSE_OFF_THRESH_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_PAUSE_OFF_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_PAUSE_OFF_THRESH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_PAUSE_OFF_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 11:10) bits 0:1 use field PAUSE_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        /* (0x00000000 bits 11:10) bits 0:1 use field PAUSE_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_PAUSE_OFF_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_PAUSE_OFF_THRESH_get( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_PAUSE_OFF_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_PAUSE_OFF_THRESH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 11:10) bits 0:1 use field PAUSE_OFF_THRESH of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_MSK, ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_PAUSE_OFF_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_MAC_LOWPWR_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_MAC_LOWPWR_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_MAC_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_MAC_LOWPWR_set", value );

    /* (0x00000000 bits 9) field MAC_LOWPWR of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_MAC_LOWPWR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MAC_LOWPWR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field MAC_LOWPWR of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MAC_LOWPWR_get", value );

    return value;
}
static INLINE void enet_fege_field_GE_GFPT_LOWPWR_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_GE_GFPT_LOWPWR_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_GE_GFPT_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_GE_GFPT_LOWPWR_set", value );

    /* (0x00000000 bits 8) field GE_GFPT_LOWPWR of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_GE_GFPT_LOWPWR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_GE_GFPT_LOWPWR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field GE_GFPT_LOWPWR of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_GE_GFPT_LOWPWR_get", value );

    return value;
}
static INLINE void enet_fege_field_UNIDIRECTIONAL_MODE_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_UNIDIRECTIONAL_MODE_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_UNIDIRECTIONAL_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_UNIDIRECTIONAL_MODE_set", value );

    /* (0x00000000 bits 3) field UNIDIRECTIONAL_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_UNIDIRECTIONAL_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_UNIDIRECTIONAL_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field UNIDIRECTIONAL_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_UNIDIRECTIONAL_MODE_get", value );

    return value;
}
static INLINE void enet_fege_field_FEGE_MODE_set( enet_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_FEGE_MODE_set( enet_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_FEGE_MODE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_FEGE_MODE_set", value );

    /* (0x00000000 bits 2:0) bits 0:2 use field FEGE_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_FEGE_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FEGE_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2:0) bits 0:2 use field FEGE_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FEGE_MODE_get", value );

    return value;
}
static INLINE void enet_fege_field_range_FEGE_MODE_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_FEGE_MODE_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_FEGE_MODE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_FEGE_MODE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_FEGE_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 2:0) bits 0:2 use field FEGE_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
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
        /* (0x00000000 bits 2:0) bits 0:2 use field FEGE_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
        enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF + subfield_offset),
                                          ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_FEGE_MODE_get( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_FEGE_MODE_get( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_FEGE_MODE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_FEGE_MODE_get", stop_bit, 2 );
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
    /* (0x00000000 bits 2:0) bits 0:2 use field FEGE_MODE of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_MSK)
                  >> ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_MSK, ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_FEGE_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FIFO_RST_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FIFO_RST_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_RST_set", value );

    /* (0x00000004 bits 10) field TX_TSTMP_FIFO_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 10) field TX_TSTMP_FIFO_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_PTP_NTP_RST_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_PTP_NTP_RST_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_PTP_NTP_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_PTP_NTP_RST_set", value );

    /* (0x00000004 bits 9) field PTP_NTP_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_PTP_NTP_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_PTP_NTP_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 9) field PTP_NTP_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_PTP_NTP_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_GE_GFPT_RST_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_GE_GFPT_RST_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_GE_GFPT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_GE_GFPT_RST_set", value );

    /* (0x00000004 bits 8) field GE_GFPT_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_GE_GFPT_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_GE_GFPT_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field GE_GFPT_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_GE_GFPT_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_MSTATX_RST_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_MSTATX_RST_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_MSTATX_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_MSTATX_RST_set", value );

    /* (0x00000004 bits 7) field MSTATX_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_MSTATX_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 7) field MSTATX_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_L1RPP_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_L1RPP_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_L1RPP_FEGE_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_L1RPP_FEGE_RST_set", value );

    /* (0x00000004 bits 6) field L1RPP_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_L1RPP_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 6) field L1RPP_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_RMAC_RST_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RMAC_RST_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RMAC_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RMAC_RST_set", value );

    /* (0x00000004 bits 5) field RMAC_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_RMAC_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RMAC_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 5) field RMAC_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RMAC_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_ECLASS_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_ECLASS_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_ECLASS_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_ECLASS_RST_set", value );

    /* (0x00000004 bits 4) field RX_ECLASS_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_RX_ECLASS_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_ECLASS_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4) field RX_ECLASS_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_ECLASS_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_L1TPP_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_L1TPP_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_L1TPP_FEGE_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_L1TPP_FEGE_RST_set", value );

    /* (0x00000004 bits 3) field L1TPP_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_L1TPP_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1TPP_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3) field L1TPP_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1TPP_FEGE_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_TMAC_RST_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TMAC_RST_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TMAC_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TMAC_RST_set", value );

    /* (0x00000004 bits 2) field TMAC_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_TMAC_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TMAC_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2) field TMAC_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TMAC_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_ECLASS_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_ECLASS_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_ECLASS_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_ECLASS_RST_set", value );

    /* (0x00000004 bits 1) field TX_ECLASS_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_TX_ECLASS_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_ECLASS_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field TX_ECLASS_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_ECLASS_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_ENET_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_ENET_FEGE_RST_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_ENET_FEGE_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_ENET_FEGE_RST_set", value );

    /* (0x00000004 bits 0) field ENET_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    enet_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_MSK,
                                      ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_ENET_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_ENET_FEGE_RST_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field ENET_FEGE_RST of register PMC_ENET_FEGE_REG_SW_RESET */
    reg_value = enet_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_MSK) >> ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_ENET_FEGE_RST_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMP_OFFSET_UPD_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMP_OFFSET_UPD_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_TSTMP_OFFSET_UPD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMP_OFFSET_UPD_set", value );

    /* (0x00000008 bits 31) field RX_TSTMP_OFFSET_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMP_OFFSET_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMP_OFFSET_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 31) field RX_TSTMP_OFFSET_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMP_OFFSET_UPD_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_OFFSET_UPD_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_OFFSET_UPD_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_OFFSET_UPD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_OFFSET_UPD_set", value );

    /* (0x00000008 bits 30) field TX_TSTMP_OFFSET_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_OFFSET_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_OFFSET_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 30) field TX_TSTMP_OFFSET_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_OFFSET_UPD_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set", value );

    /* (0x00000008 bits 29) field RX_TSTMP_FRAC_STAT_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 29) field RX_TSTMP_FRAC_STAT_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set", value );

    /* (0x00000008 bits 28) field RX_TSTMP_FRAC_DYN_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 28) field RX_TSTMP_FRAC_DYN_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set", value );

    /* (0x00000008 bits 27) field TX_TSTMP_FRAC_STAT_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 27) field TX_TSTMP_FRAC_STAT_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set", value );

    /* (0x00000008 bits 26) field TX_TSTMP_FRAC_DYN_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 26) field TX_TSTMP_FRAC_DYN_SIGN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_get", value );

    return value;
}
static INLINE void enet_fege_field_CORR_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_CORR_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_CORR_POSITION_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_CORR_POSITION_set", value );

    /* (0x00000008 bits 23:16) bits 0:7 use field CORR_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_CORR_POSITION_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_CORR_POSITION_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 23:16) bits 0:7 use field CORR_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_CORR_POSITION_get", value );

    return value;
}
static INLINE void enet_fege_field_range_CORR_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_CORR_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_CORR_POSITION_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_CORR_POSITION_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_CORR_POSITION_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 23:16) bits 0:7 use field CORR_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
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
        /* (0x00000008 bits 23:16) bits 0:7 use field CORR_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
        enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF + subfield_offset),
                                             ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_CORR_POSITION_get( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_CORR_POSITION_get( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_CORR_POSITION_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_CORR_POSITION_get", stop_bit, 7 );
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
    /* (0x00000008 bits 23:16) bits 0:7 use field CORR_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_MSK, ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_CORR_POSITION_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_TSTMP_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TSTMP_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TSTMP_POSITION_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TSTMP_POSITION_set", value );

    /* (0x00000008 bits 15:8) bits 0:7 use field TSTMP_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TSTMP_POSITION_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TSTMP_POSITION_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 15:8) bits 0:7 use field TSTMP_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TSTMP_POSITION_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TSTMP_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TSTMP_POSITION_set( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_POSITION_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_POSITION_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_POSITION_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 15:8) bits 0:7 use field TSTMP_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
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
        /* (0x00000008 bits 15:8) bits 0:7 use field TSTMP_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
        enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF + subfield_offset),
                                             ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TSTMP_POSITION_get( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TSTMP_POSITION_get( enet_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_POSITION_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_POSITION_get", stop_bit, 7 );
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
    /* (0x00000008 bits 15:8) bits 0:7 use field TSTMP_POSITION of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_MSK, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_POSITION_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_TAG_FOR_UPD_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TAG_FOR_UPD_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TAG_FOR_UPD_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TAG_FOR_UPD_set", value );

    /* (0x00000008 bits 7:5) bits 0:2 use field TAG_FOR_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TAG_FOR_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TAG_FOR_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 7:5) bits 0:2 use field TAG_FOR_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TAG_FOR_UPD_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TAG_FOR_UPD_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TAG_FOR_UPD_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TAG_FOR_UPD_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TAG_FOR_UPD_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TAG_FOR_UPD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 7:5) bits 0:2 use field TAG_FOR_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
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
        /* (0x00000008 bits 7:5) bits 0:2 use field TAG_FOR_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
        enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF + subfield_offset),
                                             ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TAG_FOR_UPD_get( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TAG_FOR_UPD_get( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TAG_FOR_UPD_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TAG_FOR_UPD_get", stop_bit, 2 );
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
    /* (0x00000008 bits 7:5) bits 0:2 use field TAG_FOR_UPD of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_MSK, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TAG_FOR_UPD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_ONE_STEP_CLK_FCS_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_ONE_STEP_CLK_FCS_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_ONE_STEP_CLK_FCS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_ONE_STEP_CLK_FCS_set", value );

    /* (0x00000008 bits 4) field ONE_STEP_CLK_FCS of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_ONE_STEP_CLK_FCS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_ONE_STEP_CLK_FCS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 4) field ONE_STEP_CLK_FCS of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_ONE_STEP_CLK_FCS_get", value );

    return value;
}
static INLINE void enet_fege_field_TSTMP_MODE_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TSTMP_MODE_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TSTMP_MODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TSTMP_MODE_set", value );

    /* (0x00000008 bits 2:1) bits 0:1 use field TSTMP_MODE of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_TSTMP_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TSTMP_MODE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 2:1) bits 0:1 use field TSTMP_MODE of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TSTMP_MODE_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TSTMP_MODE_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TSTMP_MODE_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 2:1) bits 0:1 use field TSTMP_MODE of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
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
        /* (0x00000008 bits 2:1) bits 0:1 use field TSTMP_MODE of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
        enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF + subfield_offset),
                                             ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TSTMP_MODE_get( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TSTMP_MODE_get( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TSTMP_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TSTMP_MODE_get", stop_bit, 1 );
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
    /* (0x00000008 bits 2:1) bits 0:1 use field TSTMP_MODE of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_MSK, ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TSTMP_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_PTP_NTP_EN_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_PTP_NTP_EN_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_PTP_NTP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_PTP_NTP_EN_set", value );

    /* (0x00000008 bits 0) field PTP_NTP_EN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    enet_fege_reg_PTP_NTP_CFG_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_MSK,
                                         ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_PTP_NTP_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_PTP_NTP_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field PTP_NTP_EN of register PMC_ENET_FEGE_REG_PTP_NTP_CFG */
    reg_value = enet_fege_reg_PTP_NTP_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_MSK) >> ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_PTP_NTP_EN_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_set", value );

    /* (0x0000000c bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( b_ptr,
                                                                            h_ptr,
                                                                            ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_MSK,
                                                                            ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF,
                                                                            value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( b_ptr,
                                                                                   h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_MSK) >> ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_get", value );

    return value;
}
static INLINE void enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000000c bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
        enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( b_ptr,
                                                                                h_ptr,
                                                                                subfield_mask << (ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF + subfield_offset),
                                                                                ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF + subfield_offset,
                                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get", stop_bit, 31 );
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
    /* (0x0000000c bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( b_ptr,
                                                                                   h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_MSK, ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_set", value );

    /* (0x00000010 bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( b_ptr,
                                                                           h_ptr,
                                                                           ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_MSK,
                                                                           ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF,
                                                                           value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( b_ptr,
                                                                                  h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_MSK) >> ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_get", value );

    return value;
}
static INLINE void enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000010 bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
        enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( b_ptr,
                                                                               h_ptr,
                                                                               subfield_mask << (ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF + subfield_offset),
                                                                               ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF + subfield_offset,
                                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get", stop_bit, 31 );
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
    /* (0x00000010 bits 31:0) bits 0:31 use field RX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( b_ptr,
                                                                                  h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_MSK, ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_set", value );

    /* (0x00000014 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( b_ptr,
                                                                            h_ptr,
                                                                            ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_MSK,
                                                                            ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF,
                                                                            value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( b_ptr,
                                                                                   h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_MSK) >> ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000014 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
        enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set( b_ptr,
                                                                                h_ptr,
                                                                                subfield_mask << (ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF + subfield_offset),
                                                                                ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF + subfield_offset,
                                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get", stop_bit, 31 );
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
    /* (0x00000014 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_DYN_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read( b_ptr,
                                                                                   h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_MSK, ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_set", value );

    /* (0x00000018 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_MSK,
                                                                                 ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF,
                                                                                 value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( b_ptr,
                                                                                        h_ptr);
    value = (reg_value & ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_MSK) >> ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_get", value );

    return value;
}
static INLINE void enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000018 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
        enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set( b_ptr,
                                                                                     h_ptr,
                                                                                     subfield_mask << (ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF + subfield_offset),
                                                                                     ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF + subfield_offset,
                                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get( enet_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get", stop_bit, 31 );
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
    /* (0x00000018 bits 31:0) bits 0:31 use field TX_TSTMP_FRAC_STAT_OFFSET of register PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET */
    reg_value = enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read( b_ptr,
                                                                                        h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_MSK)
                  >> ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_MSK, ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set( enet_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set", value );

    /* (0x0000001c bits 4) field AUTO_BIT_ALIGN_OFFSET of register PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT */
    enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_field_set( b_ptr,
                                                      h_ptr,
                                                      ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_MSK,
                                                      ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_OFF,
                                                      value);
}

static INLINE UINT32 enet_fege_field_AUTO_BIT_ALIGN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_AUTO_BIT_ALIGN_OFFSET_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 4) field AUTO_BIT_ALIGN_OFFSET of register PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT */
    reg_value = enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_MSK) >> ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_AUTO_BIT_ALIGN_OFFSET_get", value );

    return value;
}
static INLINE void enet_fege_field_ERR_PKT_DROP_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_ERR_PKT_DROP_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_ERR_PKT_DROP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_ERR_PKT_DROP_set", value );

    /* (0x00000020 bits 8) field ERR_PKT_DROP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
    enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set( b_ptr,
                                               h_ptr,
                                               ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_MSK,
                                               ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_OFF,
                                               value);
}

static INLINE UINT32 enet_fege_field_ERR_PKT_DROP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_ERR_PKT_DROP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 8) field ERR_PKT_DROP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_ERR_PKT_DROP_get", value );

    return value;
}
static INLINE void enet_fege_field_EXTRACT_TAG_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_EXTRACT_TAG_set( enet_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_EXTRACT_TAG_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_EXTRACT_TAG_set", value );

    /* (0x00000020 bits 7:0) bits 0:7 use field EXTRACT_TAG of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
    enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set( b_ptr,
                                               h_ptr,
                                               ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_MSK,
                                               ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF,
                                               value);
}

static INLINE UINT32 enet_fege_field_EXTRACT_TAG_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_EXTRACT_TAG_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 7:0) bits 0:7 use field EXTRACT_TAG of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_EXTRACT_TAG_get", value );

    return value;
}
static INLINE void enet_fege_field_range_EXTRACT_TAG_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_EXTRACT_TAG_set( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_EXTRACT_TAG_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_EXTRACT_TAG_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_EXTRACT_TAG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 7:0) bits 0:7 use field EXTRACT_TAG of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
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
        /* (0x00000020 bits 7:0) bits 0:7 use field EXTRACT_TAG of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
        enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF + subfield_offset),
                                                   ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_EXTRACT_TAG_get( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_EXTRACT_TAG_get( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_EXTRACT_TAG_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_EXTRACT_TAG_get", stop_bit, 7 );
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
    /* (0x00000020 bits 7:0) bits 0:7 use field EXTRACT_TAG of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_CFG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_MSK)
                  >> ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_MSK, ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_EXTRACT_TAG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_fege_field_FEGE_P_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_FEGE_P_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_FEGE_P_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_FEGE_P_E_set", value );

    /* (0x0000003c bits 3) field FEGE_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    enet_fege_reg_INT_INTP_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_MSK,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_FEGE_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FEGE_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 3) field FEGE_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    reg_value = enet_fege_reg_INT_INTP_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_MSK) >> ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FEGE_P_E_get", value );

    return value;
}
static INLINE void enet_fege_field_MSTATX_P_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_MSTATX_P_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_MSTATX_P_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_MSTATX_P_E_set", value );

    /* (0x0000003c bits 2) field MSTATX_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    enet_fege_reg_INT_INTP_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_MSK,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_MSTATX_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 2) field MSTATX_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    reg_value = enet_fege_reg_INT_INTP_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_MSK) >> ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_P_E_get", value );

    return value;
}
static INLINE void enet_fege_field_L1RPP_FEGE_P_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_L1RPP_FEGE_P_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_L1RPP_FEGE_P_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_L1RPP_FEGE_P_E_set", value );

    /* (0x0000003c bits 0) field L1RPP_FEGE_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    enet_fege_reg_INT_INTP_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_MSK,
                                         ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_L1RPP_FEGE_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_P_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 0) field L1RPP_FEGE_P_E of register PMC_ENET_FEGE_REG_INT_INTP_EN */
    reg_value = enet_fege_reg_INT_INTP_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_MSK) >> ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_P_E_get", value );

    return value;
}
static INLINE void enet_fege_field_FEGE_M_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_FEGE_M_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_FEGE_M_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_FEGE_M_E_set", value );

    /* (0x00000040 bits 3) field FEGE_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    enet_fege_reg_INT_INTM_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_MSK,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_FEGE_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FEGE_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 3) field FEGE_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    reg_value = enet_fege_reg_INT_INTM_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_MSK) >> ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FEGE_M_E_get", value );

    return value;
}
static INLINE void enet_fege_field_MSTATX_M_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_MSTATX_M_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_MSTATX_M_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_MSTATX_M_E_set", value );

    /* (0x00000040 bits 2) field MSTATX_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    enet_fege_reg_INT_INTM_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_MSK,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_MSTATX_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 2) field MSTATX_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    reg_value = enet_fege_reg_INT_INTM_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_MSK) >> ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_M_E_get", value );

    return value;
}
static INLINE void enet_fege_field_L1RPP_FEGE_M_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_L1RPP_FEGE_M_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_L1RPP_FEGE_M_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_L1RPP_FEGE_M_E_set", value );

    /* (0x00000040 bits 0) field L1RPP_FEGE_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    enet_fege_reg_INT_INTM_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_MSK,
                                         ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_L1RPP_FEGE_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_M_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 0) field L1RPP_FEGE_M_E of register PMC_ENET_FEGE_REG_INT_INTM_EN */
    reg_value = enet_fege_reg_INT_INTM_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_MSK) >> ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_M_E_get", value );

    return value;
}
static INLINE void enet_fege_field_FEGE_D_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_FEGE_D_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_FEGE_D_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_FEGE_D_E_set", value );

    /* (0x00000044 bits 3) field FEGE_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    enet_fege_reg_INT_INTD_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_MSK,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_FEGE_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FEGE_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 3) field FEGE_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    reg_value = enet_fege_reg_INT_INTD_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_MSK) >> ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FEGE_D_E_get", value );

    return value;
}
static INLINE void enet_fege_field_MSTATX_D_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_MSTATX_D_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_MSTATX_D_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_MSTATX_D_E_set", value );

    /* (0x00000044 bits 2) field MSTATX_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    enet_fege_reg_INT_INTD_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_MSK,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_MSTATX_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 2) field MSTATX_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    reg_value = enet_fege_reg_INT_INTD_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_MSK) >> ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_D_E_get", value );

    return value;
}
static INLINE void enet_fege_field_L1RPP_FEGE_D_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_L1RPP_FEGE_D_E_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_L1RPP_FEGE_D_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_L1RPP_FEGE_D_E_set", value );

    /* (0x00000044 bits 0) field L1RPP_FEGE_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    enet_fege_reg_INT_INTD_EN_field_set( b_ptr,
                                         h_ptr,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_MSK,
                                         ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_OFF,
                                         value);
}

static INLINE UINT32 enet_fege_field_L1RPP_FEGE_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_D_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 0) field L1RPP_FEGE_D_E of register PMC_ENET_FEGE_REG_INT_INTD_EN */
    reg_value = enet_fege_reg_INT_INTD_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_MSK) >> ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_D_E_get", value );

    return value;
}
static INLINE void enet_fege_field_UDP_EN_set( enet_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_UDP_EN_set( enet_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_UDP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_UDP_EN_set", value );

    /* (0x00000058 bits 11) field UDP_EN of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( b_ptr,
                                                       h_ptr,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_MSK,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_OFF,
                                                       value);
}

static INLINE UINT32 enet_fege_field_UDP_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_UDP_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 11) field UDP_EN of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    reg_value = enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_MSK) >> ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_UDP_EN_get", value );

    return value;
}
static INLINE void enet_fege_field_UDP_CORR_POS_EN_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_UDP_CORR_POS_EN_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_UDP_CORR_POS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_UDP_CORR_POS_EN_set", value );

    /* (0x00000058 bits 10) field UDP_CORR_POS_EN of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( b_ptr,
                                                       h_ptr,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_MSK,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_OFF,
                                                       value);
}

static INLINE UINT32 enet_fege_field_UDP_CORR_POS_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_UDP_CORR_POS_EN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 10) field UDP_CORR_POS_EN of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    reg_value = enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_MSK) >> ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_UDP_CORR_POS_EN_get", value );

    return value;
}
static INLINE void enet_fege_field_UDP_CORR_POS_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_UDP_CORR_POS_set( enet_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_UDP_CORR_POS_set", value, 511);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_UDP_CORR_POS_set", value );

    /* (0x00000058 bits 8:0) bits 0:8 use field UDP_CORR_POS of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( b_ptr,
                                                       h_ptr,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_MSK,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF,
                                                       value);
}

static INLINE UINT32 enet_fege_field_UDP_CORR_POS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_UDP_CORR_POS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 8:0) bits 0:8 use field UDP_CORR_POS of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    reg_value = enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_MSK) >> ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_UDP_CORR_POS_get", value );

    return value;
}
static INLINE void enet_fege_field_range_UDP_CORR_POS_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_range_UDP_CORR_POS_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_UDP_CORR_POS_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_UDP_CORR_POS_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_UDP_CORR_POS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 8:0) bits 0:8 use field UDP_CORR_POS of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
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
        if (stop_bit < 8) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 8;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000058 bits 8:0) bits 0:8 use field UDP_CORR_POS of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
        enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF + subfield_offset),
                                                           ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 enet_fege_field_range_UDP_CORR_POS_get( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_UDP_CORR_POS_get( enet_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_UDP_CORR_POS_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_UDP_CORR_POS_get", stop_bit, 8 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 8) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 8;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000058 bits 8:0) bits 0:8 use field UDP_CORR_POS of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    reg_value = enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_MSK)
                  >> ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_MSK, ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_UDP_CORR_POS_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void enet_fege_field_FORCE_ERR_PATT_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_FORCE_ERR_PATT_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_FORCE_ERR_PATT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_FORCE_ERR_PATT_set", value );

    /* (0x00000000 bits 19) field FORCE_ERR_PATT of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_FORCE_ERR_PATT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FORCE_ERR_PATT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 19) field FORCE_ERR_PATT of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FORCE_ERR_PATT_get", value );

    return value;
}
static INLINE void enet_fege_field_CLEAR_ERR_PATT_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_CLEAR_ERR_PATT_set( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_CLEAR_ERR_PATT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_CLEAR_ERR_PATT_set", value );

    /* (0x00000000 bits 18) field CLEAR_ERR_PATT of register PMC_ENET_FEGE_REG_MODE_CFG */
    enet_fege_reg_MODE_CFG_field_set( b_ptr,
                                      h_ptr,
                                      ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_MSK,
                                      ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_OFF,
                                      value);
}

static INLINE UINT32 enet_fege_field_CLEAR_ERR_PATT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_CLEAR_ERR_PATT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 18) field CLEAR_ERR_PATT of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_CLEAR_ERR_PATT_get", value );

    return value;
}
static INLINE void enet_fege_field_Q_BUSY_RD_set( enet_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_Q_BUSY_RD_set( enet_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_Q_BUSY_RD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_Q_BUSY_RD_set", value );

    /* (0x00000024 bits 0) field Q_BUSY_RD of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS */
    enet_fege_reg_TX_TSTMP_FIFO_ACCESS_field_set( b_ptr,
                                                  h_ptr,
                                                  ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_MSK,
                                                  ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_OFF,
                                                  value);
}

static INLINE UINT32 enet_fege_field_Q_BUSY_RD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_Q_BUSY_RD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 0) field Q_BUSY_RD of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_Q_BUSY_RD_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_Q_BUSY_RD_poll( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_Q_BUSY_RD_poll( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_Q_BUSY_RD_poll", value );

    /* (0x00000024 bits 0) field Q_BUSY_RD of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS */
    return enet_fege_reg_TX_TSTMP_FIFO_ACCESS_poll( b_ptr,
                                                    h_ptr,
                                                    ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_MSK,
                                                    (value<<ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_UDP_CORR_POS_UPD_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_UDP_CORR_POS_UPD_set( enet_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_UDP_CORR_POS_UPD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_UDP_CORR_POS_UPD_set", value );

    /* (0x00000058 bits 9) field UDP_CORR_POS_UPD of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set( b_ptr,
                                                       h_ptr,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_MSK,
                                                       ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_OFF,
                                                       value);
}

static INLINE UINT32 enet_fege_field_UDP_CORR_POS_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_UDP_CORR_POS_UPD_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 9) field UDP_CORR_POS_UPD of register PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG */
    reg_value = enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_MSK) >> ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_UDP_CORR_POS_UPD_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear", value );

    /* (0x00000038 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_MSK) >> ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_poll", value );

    /* (0x00000038 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear", value );

    /* (0x00000038 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_MSK) >> ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_poll", value );

    /* (0x00000038 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void enet_fege_field_OVERFLOW_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_OVERFLOW_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_OVERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_OVERFLOW_I_set_to_clear", value );

    /* (0x00000038 bits 4) field OVERFLOW_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_OVERFLOW_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_OVERFLOW_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_OVERFLOW_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_OVERFLOW_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 4) field OVERFLOW_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_OVERFLOW_I_MSK) >> ENET_FEGE_REG_INT_BIT_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_OVERFLOW_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_OVERFLOW_I_poll( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_OVERFLOW_I_poll( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_OVERFLOW_I_poll", value );

    /* (0x00000038 bits 4) field OVERFLOW_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_OVERFLOW_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_OVERFLOW_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_RX_COMMA_DEL_CHG_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_COMMA_DEL_CHG_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_I_set_to_clear", value );

    /* (0x00000038 bits 3) field RX_COMMA_DEL_CHG_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_RX_COMMA_DEL_CHG_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_COMMA_DEL_CHG_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 3) field RX_COMMA_DEL_CHG_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_MSK) >> ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_COMMA_DEL_CHG_I_poll( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_COMMA_DEL_CHG_I_poll( enet_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_I_poll", value );

    /* (0x00000038 bits 3) field RX_COMMA_DEL_CHG_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_TX_TSTMP_FIFO_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FIFO_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_I_set_to_clear", value );

    /* (0x00000038 bits 2) field TX_TSTMP_FIFO_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 2) field TX_TSTMP_FIFO_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_MSK) >> ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMP_FIFO_I_poll( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMP_FIFO_I_poll( enet_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_TX_TSTMP_FIFO_I_poll", value );

    /* (0x00000038 bits 2) field TX_TSTMP_FIFO_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_TX_SOF_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_SOF_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_SOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_SOF_I_set_to_clear", value );

    /* (0x00000038 bits 1) field TX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_TX_SOF_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_TX_SOF_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_TX_SOF_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_SOF_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 1) field TX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_TX_SOF_I_MSK) >> ENET_FEGE_REG_INT_BIT_TX_SOF_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_SOF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_SOF_I_poll( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_SOF_I_poll( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_TX_SOF_I_poll", value );

    /* (0x00000038 bits 1) field TX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_TX_SOF_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_TX_SOF_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}

static INLINE void enet_fege_field_RX_SOF_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_SOF_I_set_to_clear( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_SOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_SOF_I_set_to_clear", value );

    /* (0x00000038 bits 0) field RX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    enet_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 ENET_FEGE_REG_INT_BIT_RX_SOF_I_MSK,
                                                 ENET_FEGE_REG_INT_BIT_RX_SOF_I_OFF,
                                                 value);
}

static INLINE UINT32 enet_fege_field_RX_SOF_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_SOF_I_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 0) field RX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    reg_value = enet_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_BIT_RX_SOF_I_MSK) >> ENET_FEGE_REG_INT_BIT_RX_SOF_I_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_SOF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_SOF_I_poll( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_SOF_I_poll( enet_fege_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_RX_SOF_I_poll", value );

    /* (0x00000038 bits 0) field RX_SOF_I of register PMC_ENET_FEGE_REG_INT */
    return enet_fege_reg_INT_poll( b_ptr,
                                   h_ptr,
                                   ENET_FEGE_REG_INT_BIT_RX_SOF_I_MSK,
                                   (value<<ENET_FEGE_REG_INT_BIT_RX_SOF_I_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_set( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_set( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_set", value );

    /* (0x00000030 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_set( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_set( enet_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_set", value );

    /* (0x00000030 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_get", value );

    return value;
}
static INLINE void enet_fege_field_OVERFLOW_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_OVERFLOW_E_set( enet_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_OVERFLOW_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_OVERFLOW_E_set", value );

    /* (0x00000030 bits 4) field OVERFLOW_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_OVERFLOW_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_OVERFLOW_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 4) field OVERFLOW_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_OVERFLOW_E_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_COMMA_DEL_CHG_E_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_COMMA_DEL_CHG_E_set( enet_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_E_set", value );

    /* (0x00000030 bits 3) field RX_COMMA_DEL_CHG_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_RX_COMMA_DEL_CHG_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_COMMA_DEL_CHG_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 3) field RX_COMMA_DEL_CHG_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_COMMA_DEL_CHG_E_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_TSTMP_FIFO_E_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_TSTMP_FIFO_E_set( enet_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_E_set", value );

    /* (0x00000030 bits 2) field TX_TSTMP_FIFO_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_FIFO_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 2) field TX_TSTMP_FIFO_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_FIFO_E_get", value );

    return value;
}
static INLINE void enet_fege_field_TX_SOF_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_TX_SOF_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_TX_SOF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_TX_SOF_E_set", value );

    /* (0x00000030 bits 1) field TX_SOF_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_TX_SOF_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_SOF_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 1) field TX_SOF_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_SOF_E_get", value );

    return value;
}
static INLINE void enet_fege_field_RX_SOF_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_fege_field_RX_SOF_E_set( enet_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_fege_field_RX_SOF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_fege_field_RX_SOF_E_set", value );

    /* (0x00000030 bits 0) field RX_SOF_E of register PMC_ENET_FEGE_REG_INT_EN */
    enet_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_MSK,
                                    ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_OFF,
                                    value);
}

static INLINE UINT32 enet_fege_field_RX_SOF_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_SOF_E_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 0) field RX_SOF_E of register PMC_ENET_FEGE_REG_INT_EN */
    reg_value = enet_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_MSK) >> ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_SOF_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_V of register PMC_ENET_FEGE_REG_INT_VAL */
    reg_value = enet_fege_reg_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_MSK) >> ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_poll", value );

    /* (0x00000034 bits 6) field TX_TSTMPR_TOD_SIGN_OVRFL_V of register PMC_ENET_FEGE_REG_INT_VAL */
    return enet_fege_reg_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_MSK,
                                       (value<<ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_V of register PMC_ENET_FEGE_REG_INT_VAL */
    reg_value = enet_fege_reg_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_MSK) >> ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_poll( enet_fege_buffer_t *b_ptr,
                                                                                    enet_fege_handle_t *h_ptr,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_poll", value );

    /* (0x00000034 bits 5) field RX_TSTMPR_TOD_SIGN_OVRFL_V of register PMC_ENET_FEGE_REG_INT_VAL */
    return enet_fege_reg_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_MSK,
                                       (value<<ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_CARRIER_SENSE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_CARRIER_SENSE_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 7) field L1RPP_FEGE_CARRIER_SENSE of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_CARRIER_SENSE_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_CARRIER_SENSE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_CARRIER_SENSE_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_RXERROR_STATUS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_RXERROR_STATUS_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field L1RPP_FEGE_RXERROR_STATUS of register PMC_ENET_FEGE_REG_MODE_CFG */
    reg_value = enet_fege_reg_MODE_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_RXERROR_STATUS_MSK) >> ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_RXERROR_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_RXERROR_STATUS_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_BIT_ALIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_BIT_ALIGN_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 3:0) bits 0:3 use field L1RPP_FEGE_BIT_ALIGN of register PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT */
    reg_value = enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_MSK) >> ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_BIT_ALIGN_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get( enet_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get( enet_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get", stop_bit, 3 );
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
    /* (0x0000001c bits 3:0) bits 0:3 use field L1RPP_FEGE_BIT_ALIGN of register PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT */
    reg_value = enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_MSK)
                  >> ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_MSK, ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_fege_field_TX_TSTMP_PKT_AVAIL_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TX_TSTMP_PKT_AVAIL_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 1) field TX_TSTMP_PKT_AVAIL of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_TX_TSTMP_PKT_AVAIL_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_TX_TSTMP_PKT_AVAIL_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TX_TSTMP_PKT_AVAIL_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_TOD_TSTMP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_TOD_TSTMP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 31:24) bits 0:7 use field TOD_TSTMP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_TOD_TSTMP_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_range_TOD_TSTMP_get( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_TOD_TSTMP_get( enet_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_TOD_TSTMP_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_TOD_TSTMP_get", stop_bit, 7 );
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
    /* (0x00000028 bits 31:24) bits 0:7 use field TOD_TSTMP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_MSK)
                  >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_MSK, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_TOD_TSTMP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_fege_field_Q_SOP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_Q_SOP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 5) field Q_SOP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_SOP_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_SOP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_Q_SOP_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_Q_EOP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_Q_EOP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 4:1) bits 0:3 use field Q_EOP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_Q_EOP_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_range_Q_EOP_get( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_Q_EOP_get( enet_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_Q_EOP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_Q_EOP_get", stop_bit, 3 );
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
    /* (0x00000028 bits 4:1) bits 0:3 use field Q_EOP of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_MSK)
                  >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_MSK, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_Q_EOP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_fege_field_Q_ERR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_Q_ERR_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 0) field Q_ERR of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read( b_ptr,
                                                                                h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_ERR_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_ERR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_Q_ERR_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_Q_DATA_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_Q_DATA_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 31:0) bits 0:31 use field Q_DATA of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_MSK) >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_Q_DATA_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_range_Q_DATA_get( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_range_Q_DATA_get( enet_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_fege_field_range_Q_DATA_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_fege_field_range_Q_DATA_get", stop_bit, 31 );
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
    /* (0x0000002c bits 31:0) bits 0:31 use field Q_DATA of register PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA */
    reg_value = enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_MSK)
                  >> ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_MSK, ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_fege_field_range_Q_DATA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_fege_field_FEGE_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_FEGE_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 3) field FEGE_INT of register PMC_ENET_FEGE_REG_INT_SUMMARY */
    reg_value = enet_fege_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_SUMMARY_BIT_FEGE_INT_MSK) >> ENET_FEGE_REG_INT_SUMMARY_BIT_FEGE_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_FEGE_INT_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_MSTATX_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 2) field MSTATX_INT of register PMC_ENET_FEGE_REG_INT_SUMMARY */
    reg_value = enet_fege_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_SUMMARY_BIT_MSTATX_INT_MSK) >> ENET_FEGE_REG_INT_SUMMARY_BIT_MSTATX_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_INT_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_INT_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 0) field L1RPP_FEGE_INT of register PMC_ENET_FEGE_REG_INT_SUMMARY */
    reg_value = enet_fege_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_INT_SUMMARY_BIT_L1RPP_FEGE_INT_MSK) >> ENET_FEGE_REG_INT_SUMMARY_BIT_L1RPP_FEGE_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_INT_get", value );

    return value;
}
static INLINE UINT32 enet_fege_field_MSTATX_TIP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_MSTATX_TIP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000004c bits 2) field MSTATX_TIP of register PMC_ENET_FEGE_REG_TIP_SUMMARY */
    reg_value = enet_fege_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_MSK) >> ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_MSTATX_TIP_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_MSTATX_TIP_poll( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_MSTATX_TIP_poll( enet_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_MSTATX_TIP_poll", value );

    /* (0x0000004c bits 2) field MSTATX_TIP of register PMC_ENET_FEGE_REG_TIP_SUMMARY */
    return enet_fege_reg_TIP_SUMMARY_poll( b_ptr,
                                           h_ptr,
                                           ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_MSK,
                                           (value<<ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 enet_fege_field_L1RPP_FEGE_TIP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_fege_field_L1RPP_FEGE_TIP_get( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000004c bits 0) field L1RPP_FEGE_TIP of register PMC_ENET_FEGE_REG_TIP_SUMMARY */
    reg_value = enet_fege_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_MSK) >> ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "enet_fege_field_L1RPP_FEGE_TIP_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_L1RPP_FEGE_TIP_poll( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_fege_field_L1RPP_FEGE_TIP_poll( enet_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enet_fege_field_L1RPP_FEGE_TIP_poll", value );

    /* (0x0000004c bits 0) field L1RPP_FEGE_TIP of register PMC_ENET_FEGE_REG_TIP_SUMMARY */
    return enet_fege_reg_TIP_SUMMARY_poll( b_ptr,
                                           h_ptr,
                                           ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_MSK,
                                           (value<<ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ENET_FEGE_IO_INLINE_H */
