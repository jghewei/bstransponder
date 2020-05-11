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
 *     and register accessor functions for the sysotn_sfis_rx block
 *****************************************************************************/
#ifndef _SYSOTN_SFIS_RX_IO_INLINE_H
#define _SYSOTN_SFIS_RX_IO_INLINE_H

#include "lineotn_loc.h"
#include "sysotn_sfis_rx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SYSOTN_SFIS_RX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sysotn_sfis_rx
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
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sysotn_sfis_rx_buffer_t;
static INLINE void sysotn_sfis_rx_buffer_init( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_buffer_init( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sysotn_sfis_rx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sysotn_sfis_rx_buffer_flush( sysotn_sfis_rx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_buffer_flush( sysotn_sfis_rx_buffer_t *b_ptr )
{
    IOLOG( "sysotn_sfis_rx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sysotn_sfis_rx_reg_read( sysotn_sfis_rx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_read( sysotn_sfis_rx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
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
static INLINE void sysotn_sfis_rx_reg_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
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

static INLINE void sysotn_sfis_rx_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 mask,
                                             UINT32 unused_mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
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

static INLINE void sysotn_sfis_rx_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mem_type,
                                                             UINT32 reg,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

static INLINE void sysotn_sfis_rx_burst_read( sysotn_sfis_rx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 len,
                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_burst_read( sysotn_sfis_rx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
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

static INLINE void sysotn_sfis_rx_burst_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 len,
                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_burst_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
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
 *  register access functions for sysotn_sfis_rx
 * ==================================================================================
 */

static INLINE void sysotn_sfis_rx_reg_RX_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_CFG_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_CFG,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_CFG_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_CFG,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_CFG_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_CONFIG,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_CFG);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_BUFFER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_BUFFER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_BUFFER_CFG_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_BUFFER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_BUFFER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_CONFIG,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_BUFFER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_CONFIG,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_CONFIG,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_CONFIG,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_0_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_0_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_0_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_0_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_EN_0_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_1_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_1_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_1_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_1_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_1_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_1_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_EN_1_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_2_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_2_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_2_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_2_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_EN_2_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_EN_2_field_set", mask, ofs, value );
    sysotn_sfis_rx_field_set( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2,
                              mask,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2_UNUSED_MASK,
                              ofs,
                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_2_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_EN_2_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_EN_2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_SUMMARY_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_SUMMARY_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sysotn_sfis_rx_poll( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY,
                                mask,
                                value,
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);

}


static INLINE void sysotn_sfis_rx_reg_RX_INT_0_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_0_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_0_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_0,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set", mask, ofs, value );
    sysotn_sfis_rx_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              MEM_TYPE_STATUS,
                                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_0,
                                              mask,
                                              ofs,
                                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_0);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_0_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_0_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sysotn_sfis_rx_reg_RX_INT_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sysotn_sfis_rx_poll( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_SYSOTN_SFIS_RX_REG_RX_INT_0,
                                mask,
                                value,
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);

}


static INLINE void sysotn_sfis_rx_reg_RX_INT_1_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_1_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_1_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_1,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set", mask, ofs, value );
    sysotn_sfis_rx_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              MEM_TYPE_STATUS,
                                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_1,
                                              mask,
                                              ofs,
                                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_1_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_1_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_1);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_1_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_1_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sysotn_sfis_rx_reg_RX_INT_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sysotn_sfis_rx_poll( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_SYSOTN_SFIS_RX_REG_RX_INT_1,
                                mask,
                                value,
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);

}


static INLINE void sysotn_sfis_rx_reg_RX_INT_2_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_2_write( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_sfis_rx_reg_RX_INT_2_write", value );
    sysotn_sfis_rx_reg_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_2,
                              value);
}

static INLINE void sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set", mask, ofs, value );
    sysotn_sfis_rx_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              MEM_TYPE_STATUS,
                                              PMC_SYSOTN_SFIS_RX_REG_RX_INT_2,
                                              mask,
                                              ofs,
                                              value);

}

static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_2_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_2_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_2);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_2_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_2_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sysotn_sfis_rx_reg_RX_INT_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sysotn_sfis_rx_poll( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_SYSOTN_SFIS_RX_REG_RX_INT_2,
                                mask,
                                value,
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);

}


static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_VAL_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_reg_RX_INT_VAL_0_read( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_sfis_rx_reg_read( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0);

    IOLOG( "%s -> 0x%08x;", "sysotn_sfis_rx_reg_RX_INT_VAL_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sysotn_sfis_rx_reg_RX_INT_VAL_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sysotn_sfis_rx_poll( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0,
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
static INLINE void sysotn_sfis_rx_field_RX_ODD_INV_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_ODD_INV_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_ODD_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_ODD_INV_set", value );

    /* (0x00004000 bits 5) field RX_ODD_INV of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    sysotn_sfis_rx_reg_RX_CFG_field_set( b_ptr,
                                         h_ptr,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_MSK,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_OFF,
                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_ODD_INV_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_ODD_INV_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004000 bits 5) field RX_ODD_INV of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_MSK) >> SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_ODD_INV_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_NUM_LANES_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_NUM_LANES_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_NUM_LANES_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_NUM_LANES_set", value );

    /* (0x00004000 bits 3:0) bits 0:3 use field RX_NUM_LANES of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    sysotn_sfis_rx_reg_RX_CFG_field_set( b_ptr,
                                         h_ptr,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_MSK,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF,
                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_NUM_LANES_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_NUM_LANES_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004000 bits 3:0) bits 0:3 use field RX_NUM_LANES of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_MSK) >> SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_NUM_LANES_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_NUM_LANES_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_NUM_LANES_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_NUM_LANES_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_NUM_LANES_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_NUM_LANES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004000 bits 3:0) bits 0:3 use field RX_NUM_LANES of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
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
        /* (0x00004000 bits 3:0) bits 0:3 use field RX_NUM_LANES of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
        sysotn_sfis_rx_reg_RX_CFG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF + subfield_offset),
                                             SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_NUM_LANES_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_NUM_LANES_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_NUM_LANES_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_NUM_LANES_get", stop_bit, 3 );
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
    /* (0x00004000 bits 3:0) bits 0:3 use field RX_NUM_LANES of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_MSK, SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_NUM_LANES_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_set", value );

    /* (0x0000402c bits 6) field RX_FIFO_AUTO_CENTER of register PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG */
    sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set( b_ptr,
                                                h_ptr,
                                                SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_MSK,
                                                SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_OFF,
                                                value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000402c bits 6) field RX_FIFO_AUTO_CENTER of register PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_BUFFER_CFG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_MSK) >> SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_set", value, 255);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_set", value );

    /* (0x00004030 bits 15:8) bits 0:7 use field RX_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                         h_ptr,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_MSK,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF,
                                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004030 bits 15:8) bits 0:7 use field RX_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004030 bits 15:8) bits 0:7 use field RX_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
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
        /* (0x00004030 bits 15:8) bits 0:7 use field RX_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF + subfield_offset),
                                                             SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get", stop_bit, 7 );
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
    /* (0x00004030 bits 15:8) bits 0:7 use field RX_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_THRESH_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_THRESH_set", value );

    /* (0x00004030 bits 7:4) bits 0:3 use field RX_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                         h_ptr,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_MSK,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF,
                                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004030 bits 7:4) bits 0:3 use field RX_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_MATCH_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004030 bits 7:4) bits 0:3 use field RX_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
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
        /* (0x00004030 bits 7:4) bits 0:3 use field RX_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF + subfield_offset),
                                                             SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get", stop_bit, 3 );
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
    /* (0x00004030 bits 7:4) bits 0:3 use field RX_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_BAD_THRESH_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_BAD_THRESH_set", value );

    /* (0x00004030 bits 3:0) bits 0:3 use field RX_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                         h_ptr,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_MSK,
                                                         SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF,
                                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004030 bits 3:0) bits 0:3 use field RX_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_BAD_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004030 bits 3:0) bits 0:3 use field RX_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
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
        /* (0x00004030 bits 3:0) bits 0:3 use field RX_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF + subfield_offset),
                                                             SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_get", stop_bit, 3 );
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
    /* (0x00004030 bits 3:0) bits 0:3 use field RX_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BAD_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_set", value, 255);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_set", value );

    /* (0x00004034 bits 15:8) bits 0:7 use field RX_ALIGN_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_MSK,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF,
                                                        value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004034 bits 15:8) bits 0:7 use field RX_ALIGN_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004034 bits 15:8) bits 0:7 use field RX_ALIGN_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
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
        /* (0x00004034 bits 15:8) bits 0:7 use field RX_ALIGN_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF + subfield_offset),
                                                            SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get", stop_bit, 7 );
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
    /* (0x00004034 bits 15:8) bits 0:7 use field RX_ALIGN_MATCH_TIMEOUT_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_set", value );

    /* (0x00004034 bits 7:4) bits 0:3 use field RX_ALIGN_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_MSK,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF,
                                                        value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004034 bits 7:4) bits 0:3 use field RX_ALIGN_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004034 bits 7:4) bits 0:3 use field RX_ALIGN_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
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
        /* (0x00004034 bits 7:4) bits 0:3 use field RX_ALIGN_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF + subfield_offset),
                                                            SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get", stop_bit, 3 );
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
    /* (0x00004034 bits 7:4) bits 0:3 use field RX_ALIGN_MATCH_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_set", value );

    /* (0x00004034 bits 3:0) bits 0:3 use field RX_ALIGN_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_MSK,
                                                        SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF,
                                                        value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004034 bits 3:0) bits 0:3 use field RX_ALIGN_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_MSK) >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004034 bits 3:0) bits 0:3 use field RX_ALIGN_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
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
        /* (0x00004034 bits 3:0) bits 0:3 use field RX_ALIGN_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
        sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF + subfield_offset),
                                                            SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get", stop_bit, 3 );
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
    /* (0x00004034 bits 3:0) bits 0:3 use field RX_ALIGN_BAD_THRESH of register PMC_SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_MSK, SYSOTN_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOOA_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOA_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOA_ALARM_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOA_ALARM_EN_set", value );

    /* (0x0000403c bits 3) field RXOOA_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( b_ptr,
                                                       h_ptr,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_MSK,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_OFF,
                                                       value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000403c bits 3) field RXOOA_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    reg_value = sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_MSK) >> SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOA_ALARM_EN_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOOF_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOF_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOF_ALARM_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOF_ALARM_EN_set", value );

    /* (0x0000403c bits 2) field RXOOF_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( b_ptr,
                                                       h_ptr,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_MSK,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_OFF,
                                                       value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000403c bits 2) field RXOOF_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    reg_value = sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_MSK) >> SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOF_ALARM_EN_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXLOS_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXLOS_ALARM_EN_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXLOS_ALARM_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXLOS_ALARM_EN_set", value );

    /* (0x0000403c bits 1) field RXLOS_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( b_ptr,
                                                       h_ptr,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_MSK,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_OFF,
                                                       value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_ALARM_EN_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000403c bits 1) field RXLOS_ALARM_EN of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    reg_value = sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_MSK) >> SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_ALARM_EN_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_FORCE_LOS_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_FORCE_LOS_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_FORCE_LOS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_FORCE_LOS_set", value );

    /* (0x0000403c bits 0) field RX_FORCE_LOS of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set( b_ptr,
                                                       h_ptr,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_MSK,
                                                       SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_OFF,
                                                       value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_FORCE_LOS_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_FORCE_LOS_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000403c bits 0) field RX_FORCE_LOS of register PMC_SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION */
    reg_value = sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_MSK) >> SYSOTN_SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_FORCE_LOS_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void sysotn_sfis_rx_field_RX_FIFO_CENTER_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_FIFO_CENTER_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_CENTER_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_CENTER_set", value );

    /* (0x0000402c bits 7) field RX_FIFO_CENTER of register PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG */
    sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set( b_ptr,
                                                h_ptr,
                                                SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_MSK,
                                                SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_OFF,
                                                value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_FIFO_CENTER_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_FIFO_CENTER_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000402c bits 7) field RX_FIFO_CENTER of register PMC_SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_BUFFER_CFG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_MSK) >> SYSOTN_SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_FIFO_CENTER_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void sysotn_sfis_rx_field_RXOOA_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOA_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOA_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOA_E_set", value );

    /* (0x00004018 bits 25:16) bits 0:9 use field RXOOA_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 25:16) bits 0:9 use field RXOOA_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOA_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXOOA_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXOOA_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 25:16) bits 0:9 use field RXOOA_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004018 bits 25:16) bits 0:9 use field RXOOA_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
        sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF + subfield_offset),
                                                  SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_E_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004018 bits 25:16) bits 0:9 use field RXOOA_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_MSK, SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOOF_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOF_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOF_E_set", value );

    /* (0x00004018 bits 11) field RXOOF_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 11) field RXOOF_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOF_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXLOS_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXLOS_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_E_set", value );

    /* (0x00004018 bits 10) field RXLOS_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 10) field RXLOS_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXLOS_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXLOS_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXLOS_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXLOS_E_set", value );

    /* (0x00004018 bits 9:0) bits 0:9 use field RXLOS_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 9:0) bits 0:9 use field RXLOS_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXLOS_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXLOS_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 9:0) bits 0:9 use field RXLOS_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004018 bits 9:0) bits 0:9 use field RXLOS_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
        sysotn_sfis_rx_reg_RX_INT_EN_0_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF + subfield_offset),
                                                  SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_E_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004018 bits 9:0) bits 0:9 use field RXLOS_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_MSK, SYSOTN_SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_E_set", value );

    /* (0x0000401c bits 26) field RXOVR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 26) field RXOVR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR9_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR9_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR9_E_set", value );

    /* (0x0000401c bits 25) field RXOVR9_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR9_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR9_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 25) field RXOVR9_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR9_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR8_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR8_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR8_E_set", value );

    /* (0x0000401c bits 24) field RXOVR8_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR8_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR8_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 24) field RXOVR8_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR8_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR7_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR7_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR7_E_set", value );

    /* (0x0000401c bits 23) field RXOVR7_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR7_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR7_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 23) field RXOVR7_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR7_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR6_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR6_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR6_E_set", value );

    /* (0x0000401c bits 22) field RXOVR6_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR6_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR6_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 22) field RXOVR6_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR6_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR5_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR5_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR5_E_set", value );

    /* (0x0000401c bits 21) field RXOVR5_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR5_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR5_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 21) field RXOVR5_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR5_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR4_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR4_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR4_E_set", value );

    /* (0x0000401c bits 20) field RXOVR4_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR4_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR4_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 20) field RXOVR4_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR4_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR3_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR3_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR3_E_set", value );

    /* (0x0000401c bits 19) field RXOVR3_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR3_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR3_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 19) field RXOVR3_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR3_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR2_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR2_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR2_E_set", value );

    /* (0x0000401c bits 18) field RXOVR2_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR2_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR2_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 18) field RXOVR2_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR2_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR1_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR1_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR1_E_set", value );

    /* (0x0000401c bits 17) field RXOVR1_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR1_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR1_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 17) field RXOVR1_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR1_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXOVR0_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR0_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR0_E_set", value );

    /* (0x0000401c bits 16) field RXOVR0_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR0_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR0_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 16) field RXOVR0_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR0_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXUDR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXUDR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_E_set", value );

    /* (0x0000401c bits 10) field RXUDR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 10) field RXUDR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RXUDR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXUDR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXUDR_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXUDR_E_set", value );

    /* (0x0000401c bits 9:0) bits 0:9 use field RXUDR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 9:0) bits 0:9 use field RXUDR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXUDR_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXUDR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXUDR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXUDR_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXUDR_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXUDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000401c bits 9:0) bits 0:9 use field RXUDR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000401c bits 9:0) bits 0:9 use field RXUDR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
        sysotn_sfis_rx_reg_RX_INT_EN_1_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF + subfield_offset),
                                                  SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXUDR_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXUDR_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXUDR_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXUDR_E_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000401c bits 9:0) bits 0:9 use field RXUDR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_MSK, SYSOTN_SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXUDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_BITERR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_BITERR_DSC_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_E_set", value );

    /* (0x00004020 bits 10) field RX_BITERR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
    sysotn_sfis_rx_reg_RX_INT_EN_2_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_DSC_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 10) field RX_BITERR_DSC_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_RX_BITERR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_BITERR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_E_set", value );

    /* (0x00004020 bits 9:0) bits 0:9 use field RX_BITERR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
    sysotn_sfis_rx_reg_RX_INT_EN_2_field_set( b_ptr,
                                              h_ptr,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_MSK,
                                              SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF,
                                              value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_E_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 9:0) bits 0:9 use field RX_BITERR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_E_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_BITERR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_BITERR_E_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BITERR_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BITERR_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BITERR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004020 bits 9:0) bits 0:9 use field RX_BITERR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004020 bits 9:0) bits 0:9 use field RX_BITERR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
        sysotn_sfis_rx_reg_RX_INT_EN_2_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF + subfield_offset),
                                                  SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BITERR_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BITERR_E_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BITERR_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BITERR_E_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004020 bits 9:0) bits 0:9 use field RX_BITERR_E of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_EN_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_EN_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_MSK, SYSOTN_SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BITERR_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 sysotn_sfis_rx_field_RX_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004004 bits 3:0) bits 0:3 use field RX_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY */
    reg_value = sysotn_sfis_rx_reg_RX_INT_SUMMARY_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_I_get", value );

    return value;
}
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_I_get", stop_bit, 3 );
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
    /* (0x00004004 bits 3:0) bits 0:3 use field RX_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY */
    reg_value = sysotn_sfis_rx_reg_RX_INT_SUMMARY_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_MSK, SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_I_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004004 bits 3:0) bits 0:3 use field RX_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004004 bits 3:0) bits 0:3 use field RX_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY */
        return sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF + subfield_offset),
                                                       value << (SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RX_I_poll", value );

    /* (0x00004004 bits 3:0) bits 0:3 use field RX_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY */
    return sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll( b_ptr,
                                                   h_ptr,
                                                   SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_MSK,
                                                   (value<<SYSOTN_SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void sysotn_sfis_rx_field_RXOOA_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOA_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOA_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOA_I_set_to_clear", value );

    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOA_I_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
        sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF + subfield_offset),
                                                               SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_I_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK, SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
        return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF + subfield_offset),
                                                 value << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOOA_I_poll", value );

    /* (0x00004008 bits 25:16) bits 0:9 use field RXOOA_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOOF_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOOF_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOOF_I_set_to_clear", value );

    /* (0x00004008 bits 11) field RXOOF_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004008 bits 11) field RXOOF_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOOF_I_poll", value );

    /* (0x00004008 bits 11) field RXOOF_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXLOS_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXLOS_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_I_set_to_clear", value );

    /* (0x00004008 bits 10) field RXLOS_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004008 bits 10) field RXLOS_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_I_poll", value );

    /* (0x00004008 bits 10) field RXLOS_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXLOS_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXLOS_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXLOS_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXLOS_I_set_to_clear", value );

    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_I_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
        sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF + subfield_offset),
                                                               SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_I_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK, SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
        return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF + subfield_offset),
                                                 value << (SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXLOS_I_poll", value );

    /* (0x00004008 bits 9:0) bits 0:9 use field RXLOS_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_0 */
    return sysotn_sfis_rx_reg_RX_INT_0_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_I_set_to_clear", value );

    /* (0x0000400c bits 26) field RXOVR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 26) field RXOVR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR_DSC_I_poll", value );

    /* (0x0000400c bits 26) field RXOVR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR9_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR9_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR9_I_set_to_clear", value );

    /* (0x0000400c bits 25) field RXOVR9_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR9_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR9_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 25) field RXOVR9_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR9_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR9_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR9_I_poll", value );

    /* (0x0000400c bits 25) field RXOVR9_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR8_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR8_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR8_I_set_to_clear", value );

    /* (0x0000400c bits 24) field RXOVR8_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR8_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR8_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 24) field RXOVR8_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR8_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR8_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR8_I_poll", value );

    /* (0x0000400c bits 24) field RXOVR8_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR7_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR7_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR7_I_set_to_clear", value );

    /* (0x0000400c bits 23) field RXOVR7_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR7_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR7_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 23) field RXOVR7_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR7_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR7_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR7_I_poll", value );

    /* (0x0000400c bits 23) field RXOVR7_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR6_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR6_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR6_I_set_to_clear", value );

    /* (0x0000400c bits 22) field RXOVR6_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR6_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR6_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 22) field RXOVR6_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR6_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR6_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR6_I_poll", value );

    /* (0x0000400c bits 22) field RXOVR6_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR5_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR5_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR5_I_set_to_clear", value );

    /* (0x0000400c bits 21) field RXOVR5_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR5_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR5_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 21) field RXOVR5_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR5_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR5_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR5_I_poll", value );

    /* (0x0000400c bits 21) field RXOVR5_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR4_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR4_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR4_I_set_to_clear", value );

    /* (0x0000400c bits 20) field RXOVR4_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR4_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR4_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 20) field RXOVR4_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR4_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR4_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR4_I_poll", value );

    /* (0x0000400c bits 20) field RXOVR4_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR3_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR3_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR3_I_set_to_clear", value );

    /* (0x0000400c bits 19) field RXOVR3_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR3_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR3_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 19) field RXOVR3_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR3_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR3_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR3_I_poll", value );

    /* (0x0000400c bits 19) field RXOVR3_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR2_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR2_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR2_I_set_to_clear", value );

    /* (0x0000400c bits 18) field RXOVR2_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR2_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR2_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 18) field RXOVR2_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR2_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR2_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR2_I_poll", value );

    /* (0x0000400c bits 18) field RXOVR2_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR1_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR1_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR1_I_set_to_clear", value );

    /* (0x0000400c bits 17) field RXOVR1_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR1_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR1_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 17) field RXOVR1_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR1_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR1_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR1_I_poll", value );

    /* (0x0000400c bits 17) field RXOVR1_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXOVR0_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXOVR0_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXOVR0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXOVR0_I_set_to_clear", value );

    /* (0x0000400c bits 16) field RXOVR0_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOVR0_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOVR0_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 16) field RXOVR0_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOVR0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR0_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR0_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOVR0_I_poll", value );

    /* (0x0000400c bits 16) field RXOVR0_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXUDR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXUDR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_I_set_to_clear", value );

    /* (0x0000400c bits 10) field RXUDR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 10) field RXUDR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXUDR_DSC_I_poll", value );

    /* (0x0000400c bits 10) field RXUDR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RXUDR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RXUDR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RXUDR_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RXUDR_I_set_to_clear", value );

    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXUDR_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXUDR_I_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
        sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF + subfield_offset),
                                                               SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RXUDR_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXUDR_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXUDR_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXUDR_I_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK, SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXUDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXUDR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXUDR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXUDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXUDR_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXUDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
        return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF + subfield_offset),
                                                 value << (SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXUDR_I_poll", value );

    /* (0x0000400c bits 9:0) bits 0:9 use field RXUDR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_1 */
    return sysotn_sfis_rx_reg_RX_INT_1_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RX_BITERR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_BITERR_DSC_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_I_set_to_clear", value );

    /* (0x00004010 bits 10) field RX_BITERR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_DSC_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 10) field RX_BITERR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_DSC_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RX_BITERR_DSC_I_poll", value );

    /* (0x00004010 bits 10) field RX_BITERR_DSC_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    return sysotn_sfis_rx_reg_RX_INT_2_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sysotn_sfis_rx_field_RX_BITERR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_BITERR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_I_set_to_clear", value );

    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK,
                                                           SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF,
                                                           value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_BITERR_I_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_BITERR_I_get", value );

    return value;
}
static INLINE void sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
        sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF + subfield_offset),
                                                               SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BITERR_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RX_BITERR_I_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BITERR_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BITERR_I_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK, SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BITERR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_BITERR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_BITERR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RX_BITERR_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RX_BITERR_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RX_BITERR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
        return sysotn_sfis_rx_reg_RX_INT_2_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF + subfield_offset),
                                                 value << (SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_I_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RX_BITERR_I_poll", value );

    /* (0x00004010 bits 9:0) bits 0:9 use field RX_BITERR_I of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_2 */
    return sysotn_sfis_rx_reg_RX_INT_2_poll( b_ptr,
                                             h_ptr,
                                             SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK,
                                             (value<<SYSOTN_SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOA_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004028 bits 25:16) bits 0:9 use field RXOOA_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOA_V_get", value );

    return value;
}
static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_V_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXOOA_V_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_V_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_V_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004028 bits 25:16) bits 0:9 use field RXOOA_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_MSK, SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXOOA_V_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXOOA_V_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXOOA_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004028 bits 25:16) bits 0:9 use field RXOOA_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004028 bits 25:16) bits 0:9 use field RXOOA_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
        return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF + subfield_offset),
                                                     value << (SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOOA_V_poll", value );

    /* (0x00004028 bits 25:16) bits 0:9 use field RXOOA_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                 h_ptr,
                                                 SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_MSK,
                                                 (value<<SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXOOF_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004028 bits 11) field RXOOF_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXOOF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXOOF_V_poll", value );

    /* (0x00004028 bits 11) field RXOOF_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                 h_ptr,
                                                 SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_MSK,
                                                 (value<<SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_DSC_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004028 bits 10) field RXLOS_DSC_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXLOS_DSC_V_poll", value );

    /* (0x00004028 bits 10) field RXLOS_DSC_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                 h_ptr,
                                                 SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_MSK,
                                                 (value<<SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RXLOS_V_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004028 bits 9:0) bits 0:9 use field RXLOS_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_MSK) >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RXLOS_V_get", value );

    return value;
}
static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_V_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_range_RXLOS_V_get( sysotn_sfis_rx_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_V_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_V_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004028 bits 9:0) bits 0:9 use field RXLOS_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    reg_value = sysotn_sfis_rx_reg_RX_INT_VAL_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_MSK)
                  >> SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_MSK, SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_sfis_rx_field_range_RXLOS_V_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_sfis_rx_field_range_RXLOS_V_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_sfis_rx_field_range_RXLOS_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004028 bits 9:0) bits 0:9 use field RXLOS_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004028 bits 9:0) bits 0:9 use field RXLOS_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
        return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF + subfield_offset),
                                                     value << (SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_V_poll( sysotn_sfis_rx_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sysotn_sfis_rx_field_RXLOS_V_poll", value );

    /* (0x00004028 bits 9:0) bits 0:9 use field RXLOS_V of register PMC_SYSOTN_SFIS_RX_REG_RX_INT_VAL_0 */
    return sysotn_sfis_rx_reg_RX_INT_VAL_0_poll( b_ptr,
                                                 h_ptr,
                                                 SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_MSK,
                                                 (value<<SYSOTN_SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void sysotn_sfis_rx_field_RX_SW_RST_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_sfis_rx_field_RX_SW_RST_set( sysotn_sfis_rx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_sfis_rx_field_RX_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_sfis_rx_field_RX_SW_RST_set", value );

    /* (0x00004000 bits 4) field RX_SW_RST of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    sysotn_sfis_rx_reg_RX_CFG_field_set( b_ptr,
                                         h_ptr,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_MSK,
                                         SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_OFF,
                                         value);
}

static INLINE UINT32 sysotn_sfis_rx_field_RX_SW_RST_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_sfis_rx_field_RX_SW_RST_get( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004000 bits 4) field RX_SW_RST of register PMC_SYSOTN_SFIS_RX_REG_RX_CFG */
    reg_value = sysotn_sfis_rx_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_MSK) >> SYSOTN_SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_sfis_rx_field_RX_SW_RST_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSOTN_SFIS_RX_IO_INLINE_H */
