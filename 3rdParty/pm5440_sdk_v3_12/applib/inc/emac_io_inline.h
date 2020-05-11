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
 *     and register accessor functions for the emac block
 *****************************************************************************/
#ifndef _EMAC_IO_INLINE_H
#define _EMAC_IO_INLINE_H

#include "emac_loc.h"
#include "emac_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EMAC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for emac
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
} emac_buffer_t;
static INLINE void emac_buffer_init( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void emac_buffer_init( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "emac_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void emac_buffer_flush( emac_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void emac_buffer_flush( emac_buffer_t *b_ptr )
{
    IOLOG( "emac_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 emac_reg_read( emac_buffer_t *b_ptr,
                                    emac_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 emac_reg_read( emac_buffer_t *b_ptr,
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
static INLINE void emac_reg_write( emac_buffer_t *b_ptr,
                                   emac_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_write( emac_buffer_t *b_ptr,
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

static INLINE void emac_field_set( emac_buffer_t *b_ptr,
                                   emac_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_set( emac_buffer_t *b_ptr,
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

static INLINE void emac_action_on_write_field_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_action_on_write_field_set( emac_buffer_t *b_ptr,
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

static INLINE void emac_burst_read( emac_buffer_t *b_ptr,
                                    emac_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_burst_read( emac_buffer_t *b_ptr,
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

static INLINE void emac_burst_write( emac_buffer_t *b_ptr,
                                     emac_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_burst_write( emac_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE emac_poll( emac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_poll( emac_buffer_t *b_ptr,
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
 *  register access functions for emac
 * ==================================================================================
 */

static INLINE void emac_reg_SW_RESET_write( emac_buffer_t *b_ptr,
                                            emac_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_SW_RESET_write( emac_buffer_t *b_ptr,
                                            emac_handle_t *h_ptr,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_SW_RESET_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_SW_RESET,
                    value);
}

static INLINE void emac_reg_SW_RESET_field_set( emac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_SW_RESET_field_set( emac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_SW_RESET_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_SW_RESET,
                    mask,
                    PMC_EMAC120_TOP_REG_SW_RESET_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_SW_RESET_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_SW_RESET_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "emac_reg_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_ETIME_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_ETIME_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_ETIME_LOW_POWER_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL,
                    value);
}

static INLINE void emac_reg_ETIME_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_ETIME_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_ETIME_LOW_POWER_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_ETIME_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_ETIME_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_ETIME_LOW_POWER_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL,
                    value);
}

static INLINE void emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL,
                    value);
}

static INLINE void emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_write( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL,
                    value);
}

static INLINE void emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_write( emac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_write( emac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL,
                    value);
}

static INLINE void emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_INGRESS_DSI_IF_write( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_INGRESS_DSI_IF_write( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_INGRESS_DSI_IF_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_INGRESS_DSI_IF,
                    value);
}

static INLINE void emac_reg_INGRESS_DSI_IF_field_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_INGRESS_DSI_IF_field_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_INGRESS_DSI_IF_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_INGRESS_DSI_IF,
                    mask,
                    PMC_EMAC120_TOP_REG_INGRESS_DSI_IF_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_INGRESS_DSI_IF_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_INGRESS_DSI_IF_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_INGRESS_DSI_IF);

    IOLOG( "%s -> 0x%08x;", "emac_reg_INGRESS_DSI_IF_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EGRESS_DSI_IF_write( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_DSI_IF_write( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EGRESS_DSI_IF_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF,
                    value);
}

static INLINE void emac_reg_EGRESS_DSI_IF_field_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_DSI_IF_field_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EGRESS_DSI_IF_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF,
                    mask,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EGRESS_DSI_IF_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EGRESS_DSI_IF_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EGRESS_DSI_IF);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EGRESS_DSI_IF_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING,
                    value);
}

static INLINE void emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING,
                    mask,
                    PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_INGRESS_DATA_SINK_write( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_INGRESS_DATA_SINK_write( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_INGRESS_DATA_SINK_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK,
                    value);
}

static INLINE void emac_reg_INGRESS_DATA_SINK_field_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_INGRESS_DATA_SINK_field_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_INGRESS_DATA_SINK_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK,
                    mask,
                    PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_INGRESS_DATA_SINK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_INGRESS_DATA_SINK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK);

    IOLOG( "%s -> 0x%08x;", "emac_reg_INGRESS_DATA_SINK_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_write( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_write( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL,
                    value);
}

static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL,
                    mask,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE,
                    value);
}

static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE,
                    mask,
                    PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_DROP_ERR_MASK_write( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_DROP_ERR_MASK_write( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_DROP_ERR_MASK_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_write( emac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_write( emac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( emac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( emac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EGRESS_CREDIT_CNT_CFG_write( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_CREDIT_CNT_CFG_write( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EGRESS_CREDIT_CNT_CFG_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG,
                    value);
}

static INLINE void emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EGRESS_CREDIT_CNT_CFG_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG,
                    mask,
                    PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EGRESS_CREDIT_CNT_CFG_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EGRESS_CREDIT_CNT_CFG_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EGRESS_CREDIT_CNT_CFG_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_LINK_RESET_write( emac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_LINK_RESET_write( emac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_LINK_RESET_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_LINK_RESET,
                    value);
}

static INLINE void emac_reg_LINK_RESET_field_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_LINK_RESET_field_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_LINK_RESET_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_LINK_RESET,
                    mask,
                    PMC_EMAC120_TOP_REG_LINK_RESET_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_LINK_RESET_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_LINK_RESET_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_LINK_RESET);

    IOLOG( "%s -> 0x%08x;", "emac_reg_LINK_RESET_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__write( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_AGB_OVERFLOW_INT_EN_write( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_AGB_OVERFLOW_INT_EN_write( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_AGB_OVERFLOW_INT_EN_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN,
                    value);
}

static INLINE void emac_reg_AGB_OVERFLOW_INT_EN_field_set( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_AGB_OVERFLOW_INT_EN_field_set( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_AGB_OVERFLOW_INT_EN_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN,
                    mask,
                    PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_AGB_OVERFLOW_INT_EN_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_AGB_OVERFLOW_INT_EN_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN);

    IOLOG( "%s -> 0x%08x;", "emac_reg_AGB_OVERFLOW_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_write( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_write( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN,
                    value);
}

static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set", mask, ofs, value );
    emac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN,
                    mask,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__write( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_,
                    value);
}

static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( emac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void emac_reg_AGB_OVERFLOW_INT_STAT_write( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_AGB_OVERFLOW_INT_STAT_write( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_AGB_OVERFLOW_INT_STAT_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT,
                    value);
}

static INLINE void emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_AGB_OVERFLOW_INT_STAT_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_AGB_OVERFLOW_INT_STAT_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "emac_reg_AGB_OVERFLOW_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_AGB_OVERFLOW_INT_STAT_poll( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_AGB_OVERFLOW_INT_STAT_poll( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_AGB_OVERFLOW_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_write( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_write", value );
    emac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT,
                    value);
}

static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( emac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set", mask, ofs, value );
    emac_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_);

    IOLOG( "%s -> 0x%08x;", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( emac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return emac_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_,
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
static INLINE void emac_field_ETIME_NREGS_SW_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ETIME_NREGS_SW_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ETIME_NREGS_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_field_ETIME_NREGS_SW_RESET_set", value );

    /* (0x00000000 bits 3) field ETIME_NREGS_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    emac_reg_SW_RESET_field_set( b_ptr,
                                 h_ptr,
                                 EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_MSK,
                                 EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_OFF,
                                 value);
}

static INLINE UINT32 emac_field_ETIME_NREGS_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ETIME_NREGS_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field ETIME_NREGS_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    reg_value = emac_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_MSK) >> EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ETIME_NREGS_SW_RESET_get", value );

    return value;
}
static INLINE void emac_field_ETIME_CORE_SW_RESET_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ETIME_CORE_SW_RESET_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ETIME_CORE_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_field_ETIME_CORE_SW_RESET_set", value );

    /* (0x00000000 bits 2) field ETIME_CORE_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    emac_reg_SW_RESET_field_set( b_ptr,
                                 h_ptr,
                                 EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_MSK,
                                 EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_OFF,
                                 value);
}

static INLINE UINT32 emac_field_ETIME_CORE_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ETIME_CORE_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field ETIME_CORE_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    reg_value = emac_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_MSK) >> EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ETIME_CORE_SW_RESET_get", value );

    return value;
}
static INLINE void emac_field_EMAC_NREGS_SW_RESET_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_NREGS_SW_RESET_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_NREGS_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_NREGS_SW_RESET_set", value );

    /* (0x00000000 bits 1) field EMAC_NREGS_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    emac_reg_SW_RESET_field_set( b_ptr,
                                 h_ptr,
                                 EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_MSK,
                                 EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_OFF,
                                 value);
}

static INLINE UINT32 emac_field_EMAC_NREGS_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_NREGS_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field EMAC_NREGS_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    reg_value = emac_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_MSK) >> EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_NREGS_SW_RESET_get", value );

    return value;
}
static INLINE void emac_field_EMAC_CORE_SW_RESET_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_CORE_SW_RESET_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_CORE_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_CORE_SW_RESET_set", value );

    /* (0x00000000 bits 0) field EMAC_CORE_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    emac_reg_SW_RESET_field_set( b_ptr,
                                 h_ptr,
                                 EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_MSK,
                                 EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_OFF,
                                 value);
}

static INLINE UINT32 emac_field_EMAC_CORE_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_CORE_SW_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field EMAC_CORE_SW_RESET of register PMC_EMAC120_TOP_REG_SW_RESET */
    reg_value = emac_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_MSK) >> EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_CORE_SW_RESET_get", value );

    return value;
}
static INLINE void emac_field_ETIME_TS_LOWPWR_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ETIME_TS_LOWPWR_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ETIME_TS_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_field_ETIME_TS_LOWPWR_set", value );

    /* (0x00000010 bits 0) field ETIME_TS_LOWPWR of register PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL */
    emac_reg_ETIME_LOW_POWER_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_MSK,
                                             EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 emac_field_ETIME_TS_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ETIME_TS_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field ETIME_TS_LOWPWR of register PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL */
    reg_value = emac_reg_ETIME_LOW_POWER_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_MSK) >> EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ETIME_TS_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_EMAC_TX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_PMA_CLK_LOWPWR_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_PMA_CLK_LOWPWR_set", value );

    /* (0x00000014 bits 23:12) bits 0:11 use field EMAC_TX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_MSK,
                                                       EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF,
                                                       value);
}

static INLINE UINT32 emac_field_EMAC_TX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 23:12) bits 0:11 use field EMAC_TX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_MSK) >> EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_PMA_CLK_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 23:12) bits 0:11 use field EMAC_TX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
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
        /* (0x00000014 bits 23:12) bits 0:11 use field EMAC_TX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
        emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF + subfield_offset),
                                                           EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get", stop_bit, 11 );
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
    /* (0x00000014 bits 23:12) bits 0:11 use field EMAC_TX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_MSK)
                  >> EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_MSK, EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_RX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_RX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_RX_PMA_CLK_LOWPWR_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_RX_PMA_CLK_LOWPWR_set", value );

    /* (0x00000014 bits 11:0) bits 0:11 use field EMAC_RX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_MSK,
                                                       EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF,
                                                       value);
}

static INLINE UINT32 emac_field_EMAC_RX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_RX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 11:0) bits 0:11 use field EMAC_RX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_MSK) >> EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_RX_PMA_CLK_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 11:0) bits 0:11 use field EMAC_RX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
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
        /* (0x00000014 bits 11:0) bits 0:11 use field EMAC_RX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
        emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF + subfield_offset),
                                                           EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get", stop_bit, 11 );
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
    /* (0x00000014 bits 11:0) bits 0:11 use field EMAC_RX_PMA_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_MSK)
                  >> EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_MSK, EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_set", value, 7);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_set", value );

    /* (0x00000018 bits 18:16) bits 0:2 use field EMAC_LINK_40_100G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_MSK,
                                                        EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF,
                                                        value);
}

static INLINE UINT32 emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 18:16) bits 0:2 use field EMAC_LINK_40_100G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_MSK) >> EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 18:16) bits 0:2 use field EMAC_LINK_40_100G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
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
        /* (0x00000018 bits 18:16) bits 0:2 use field EMAC_LINK_40_100G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
        emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF + subfield_offset),
                                                            EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get", stop_bit, 2 );
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
    /* (0x00000018 bits 18:16) bits 0:2 use field EMAC_LINK_40_100G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_MSK)
                  >> EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_MSK, EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_LINK_10G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_LINK_10G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_LINK_10G_CLK_LOWPWR_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_LINK_10G_CLK_LOWPWR_set", value );

    /* (0x00000018 bits 11:0) bits 0:11 use field EMAC_LINK_10G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_MSK,
                                                        EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF,
                                                        value);
}

static INLINE UINT32 emac_field_EMAC_LINK_10G_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_LINK_10G_CLK_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 11:0) bits 0:11 use field EMAC_LINK_10G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_MSK) >> EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_LINK_10G_CLK_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 11:0) bits 0:11 use field EMAC_LINK_10G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
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
        /* (0x00000018 bits 11:0) bits 0:11 use field EMAC_LINK_10G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
        emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF + subfield_offset),
                                                            EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get", stop_bit, 11 );
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
    /* (0x00000018 bits 11:0) bits 0:11 use field EMAC_LINK_10G_CLK_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_MSK)
                  >> EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_MSK, EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_STAT_RAM_LOWPWR_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_STAT_RAM_LOWPWR_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_STAT_RAM_LOWPWR_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_STAT_RAM_LOWPWR_set", value );

    /* (0x00000020 bits 11:0) bits 0:11 use field EMAC_STAT_RAM_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL */
    emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set( b_ptr,
                                                           h_ptr,
                                                           EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_MSK,
                                                           EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF,
                                                           value);
}

static INLINE UINT32 emac_field_EMAC_STAT_RAM_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_STAT_RAM_LOWPWR_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 11:0) bits 0:11 use field EMAC_STAT_RAM_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_MSK) >> EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_STAT_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_STAT_RAM_LOWPWR_set( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_STAT_RAM_LOWPWR_set( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_STAT_RAM_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_STAT_RAM_LOWPWR_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_STAT_RAM_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 11:0) bits 0:11 use field EMAC_STAT_RAM_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL */
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
        /* (0x00000020 bits 11:0) bits 0:11 use field EMAC_STAT_RAM_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL */
        emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF + subfield_offset),
                                                               EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_STAT_RAM_LOWPWR_get( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_STAT_RAM_LOWPWR_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_STAT_RAM_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_STAT_RAM_LOWPWR_get", stop_bit, 11 );
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
    /* (0x00000020 bits 11:0) bits 0:11 use field EMAC_STAT_RAM_LOWPWR of register PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL */
    reg_value = emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read( b_ptr,
                                                                  h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_MSK)
                  >> EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_MSK, EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_STAT_RAM_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_XOFF_OVERRIDE_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_XOFF_OVERRIDE_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_XOFF_OVERRIDE_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_XOFF_OVERRIDE_set", value );

    /* (0x00000024 bits 11:0) bits 0:11 use field EGR_XOFF_OVERRIDE of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL */
    emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set( b_ptr,
                                                               h_ptr,
                                                               EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_MSK,
                                                               EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF,
                                                               value);
}

static INLINE UINT32 emac_field_EGR_XOFF_OVERRIDE_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_XOFF_OVERRIDE_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 11:0) bits 0:11 use field EGR_XOFF_OVERRIDE of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL */
    reg_value = emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read( b_ptr,
                                                                      h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_MSK) >> EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_XOFF_OVERRIDE_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_XOFF_OVERRIDE_set( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_XOFF_OVERRIDE_set( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_XOFF_OVERRIDE_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_XOFF_OVERRIDE_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_XOFF_OVERRIDE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 11:0) bits 0:11 use field EGR_XOFF_OVERRIDE of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL */
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
        /* (0x00000024 bits 11:0) bits 0:11 use field EGR_XOFF_OVERRIDE of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL */
        emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF + subfield_offset),
                                                                   EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_XOFF_OVERRIDE_get( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_XOFF_OVERRIDE_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_XOFF_OVERRIDE_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_XOFF_OVERRIDE_get", stop_bit, 11 );
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
    /* (0x00000024 bits 11:0) bits 0:11 use field EGR_XOFF_OVERRIDE of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL */
    reg_value = emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read( b_ptr,
                                                                      h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_MSK)
                  >> EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_MSK, EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_XOFF_OVERRIDE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_11_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_11_set", value );

    /* (0x00000080 bits 23:22) bits 0:1 use field ING_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_11_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_11_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 23:22) bits 0:1 use field ING_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_11_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_11_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_11_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_11_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 23:22) bits 0:1 use field ING_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 23:22) bits 0:1 use field ING_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_11_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_11_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_11_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_11_get", stop_bit, 1 );
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
    /* (0x00000080 bits 23:22) bits 0:1 use field ING_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_11_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_10_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_10_set", value );

    /* (0x00000080 bits 21:20) bits 0:1 use field ING_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_10_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_10_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 21:20) bits 0:1 use field ING_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_10_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_10_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_10_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_10_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 21:20) bits 0:1 use field ING_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 21:20) bits 0:1 use field ING_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_10_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_10_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_10_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_10_get", stop_bit, 1 );
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
    /* (0x00000080 bits 21:20) bits 0:1 use field ING_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_10_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_9_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_9_set", value );

    /* (0x00000080 bits 19:18) bits 0:1 use field ING_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_9_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_9_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 19:18) bits 0:1 use field ING_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_9_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_9_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_9_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_9_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 19:18) bits 0:1 use field ING_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 19:18) bits 0:1 use field ING_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_9_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_9_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_9_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_9_get", stop_bit, 1 );
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
    /* (0x00000080 bits 19:18) bits 0:1 use field ING_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_9_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_8_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_8_set", value );

    /* (0x00000080 bits 17:16) bits 0:1 use field ING_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_8_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_8_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 17:16) bits 0:1 use field ING_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_8_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_8_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_8_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 17:16) bits 0:1 use field ING_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 17:16) bits 0:1 use field ING_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_8_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_8_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_8_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_8_get", stop_bit, 1 );
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
    /* (0x00000080 bits 17:16) bits 0:1 use field ING_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_8_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_7_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_7_set", value );

    /* (0x00000080 bits 15:14) bits 0:1 use field ING_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_7_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_7_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 15:14) bits 0:1 use field ING_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_7_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_7_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_7_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 15:14) bits 0:1 use field ING_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 15:14) bits 0:1 use field ING_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_7_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_7_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_7_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_7_get", stop_bit, 1 );
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
    /* (0x00000080 bits 15:14) bits 0:1 use field ING_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_6_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_6_set", value );

    /* (0x00000080 bits 13:12) bits 0:1 use field ING_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_6_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_6_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 13:12) bits 0:1 use field ING_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_6_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_6_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_6_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 13:12) bits 0:1 use field ING_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 13:12) bits 0:1 use field ING_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_6_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_6_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_6_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_6_get", stop_bit, 1 );
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
    /* (0x00000080 bits 13:12) bits 0:1 use field ING_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_5_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_5_set", value );

    /* (0x00000080 bits 11:10) bits 0:1 use field ING_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_5_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_5_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 11:10) bits 0:1 use field ING_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_5_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_5_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_5_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 11:10) bits 0:1 use field ING_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 11:10) bits 0:1 use field ING_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_5_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_5_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_5_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_5_get", stop_bit, 1 );
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
    /* (0x00000080 bits 11:10) bits 0:1 use field ING_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_4_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_4_set", value );

    /* (0x00000080 bits 9:8) bits 0:1 use field ING_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_4_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_4_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 9:8) bits 0:1 use field ING_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_4_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_4_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_4_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 9:8) bits 0:1 use field ING_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 9:8) bits 0:1 use field ING_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_4_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_4_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_4_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_4_get", stop_bit, 1 );
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
    /* (0x00000080 bits 9:8) bits 0:1 use field ING_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_3_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_3_set", value );

    /* (0x00000080 bits 7:6) bits 0:1 use field ING_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_3_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_3_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 7:6) bits 0:1 use field ING_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_3_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_3_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_3_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 7:6) bits 0:1 use field ING_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 7:6) bits 0:1 use field ING_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_3_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_3_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_3_get", stop_bit, 1 );
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
    /* (0x00000080 bits 7:6) bits 0:1 use field ING_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_2_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_2_set", value );

    /* (0x00000080 bits 5:4) bits 0:1 use field ING_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_2_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_2_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 5:4) bits 0:1 use field ING_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_2_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_2_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_2_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 5:4) bits 0:1 use field ING_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 5:4) bits 0:1 use field ING_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_2_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_2_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_2_get", stop_bit, 1 );
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
    /* (0x00000080 bits 5:4) bits 0:1 use field ING_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_1_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_1_set", value );

    /* (0x00000080 bits 3:2) bits 0:1 use field ING_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_1_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_1_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 3:2) bits 0:1 use field ING_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_1_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_1_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_1_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 3:2) bits 0:1 use field ING_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 3:2) bits 0:1 use field ING_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_1_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_1_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_1_get", stop_bit, 1 );
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
    /* (0x00000080 bits 3:2) bits 0:1 use field ING_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DSI_CTRL_0_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DSI_CTRL_0_set", value );

    /* (0x00000080 bits 1:0) bits 0:1 use field ING_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                       h_ptr,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_MSK,
                                       EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF,
                                       value);
}

static INLINE UINT32 emac_field_ING_DSI_CTRL_0_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DSI_CTRL_0_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 1:0) bits 0:1 use field ING_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_MSK) >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DSI_CTRL_0_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_0_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 1:0) bits 0:1 use field ING_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
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
        /* (0x00000080 bits 1:0) bits 0:1 use field ING_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
        emac_reg_INGRESS_DSI_IF_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF + subfield_offset),
                                           EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DSI_CTRL_0_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DSI_CTRL_0_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DSI_CTRL_0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DSI_CTRL_0_get", stop_bit, 1 );
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
    /* (0x00000080 bits 1:0) bits 0:1 use field ING_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_INGRESS_DSI_IF */
    reg_value = emac_reg_INGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_MSK, EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DSI_CTRL_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_11_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_11_set", value );

    /* (0x00000084 bits 23:22) bits 0:1 use field EGR_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_11_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_11_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 23:22) bits 0:1 use field EGR_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_11_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_11_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_11_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_11_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_11_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 23:22) bits 0:1 use field EGR_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 23:22) bits 0:1 use field EGR_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_11_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_11_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_11_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_11_get", stop_bit, 1 );
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
    /* (0x00000084 bits 23:22) bits 0:1 use field EGR_DSI_CTRL_11 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_11_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_10_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_10_set", value );

    /* (0x00000084 bits 21:20) bits 0:1 use field EGR_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_10_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_10_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 21:20) bits 0:1 use field EGR_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_10_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_10_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_10_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_10_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_10_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 21:20) bits 0:1 use field EGR_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 21:20) bits 0:1 use field EGR_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_10_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_10_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_10_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_10_get", stop_bit, 1 );
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
    /* (0x00000084 bits 21:20) bits 0:1 use field EGR_DSI_CTRL_10 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_10_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_9_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_9_set", value );

    /* (0x00000084 bits 19:18) bits 0:1 use field EGR_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_9_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_9_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 19:18) bits 0:1 use field EGR_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_9_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_9_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_9_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_9_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_9_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 19:18) bits 0:1 use field EGR_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 19:18) bits 0:1 use field EGR_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_9_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_9_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_9_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_9_get", stop_bit, 1 );
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
    /* (0x00000084 bits 19:18) bits 0:1 use field EGR_DSI_CTRL_9 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_9_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_8_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_8_set", value );

    /* (0x00000084 bits 17:16) bits 0:1 use field EGR_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_8_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_8_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 17:16) bits 0:1 use field EGR_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_8_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_8_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_8_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_8_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 17:16) bits 0:1 use field EGR_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 17:16) bits 0:1 use field EGR_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_8_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_8_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_8_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_8_get", stop_bit, 1 );
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
    /* (0x00000084 bits 17:16) bits 0:1 use field EGR_DSI_CTRL_8 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_8_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_7_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_7_set", value );

    /* (0x00000084 bits 15:14) bits 0:1 use field EGR_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_7_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_7_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 15:14) bits 0:1 use field EGR_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_7_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_7_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_7_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_7_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 15:14) bits 0:1 use field EGR_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 15:14) bits 0:1 use field EGR_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_7_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_7_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_7_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_7_get", stop_bit, 1 );
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
    /* (0x00000084 bits 15:14) bits 0:1 use field EGR_DSI_CTRL_7 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_6_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_6_set", value );

    /* (0x00000084 bits 13:12) bits 0:1 use field EGR_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_6_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_6_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 13:12) bits 0:1 use field EGR_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_6_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_6_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_6_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_6_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 13:12) bits 0:1 use field EGR_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 13:12) bits 0:1 use field EGR_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_6_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_6_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_6_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_6_get", stop_bit, 1 );
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
    /* (0x00000084 bits 13:12) bits 0:1 use field EGR_DSI_CTRL_6 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_5_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_5_set", value );

    /* (0x00000084 bits 11:10) bits 0:1 use field EGR_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_5_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_5_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 11:10) bits 0:1 use field EGR_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_5_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_5_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_5_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_5_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 11:10) bits 0:1 use field EGR_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 11:10) bits 0:1 use field EGR_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_5_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_5_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_5_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_5_get", stop_bit, 1 );
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
    /* (0x00000084 bits 11:10) bits 0:1 use field EGR_DSI_CTRL_5 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_4_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_4_set", value );

    /* (0x00000084 bits 9:8) bits 0:1 use field EGR_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_4_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_4_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 9:8) bits 0:1 use field EGR_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_4_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_4_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_4_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_4_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 9:8) bits 0:1 use field EGR_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 9:8) bits 0:1 use field EGR_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_4_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_4_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_4_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_4_get", stop_bit, 1 );
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
    /* (0x00000084 bits 9:8) bits 0:1 use field EGR_DSI_CTRL_4 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_3_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_3_set", value );

    /* (0x00000084 bits 7:6) bits 0:1 use field EGR_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_3_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_3_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 7:6) bits 0:1 use field EGR_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_3_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_3_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_3_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_3_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 7:6) bits 0:1 use field EGR_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 7:6) bits 0:1 use field EGR_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_3_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_3_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_3_get", stop_bit, 1 );
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
    /* (0x00000084 bits 7:6) bits 0:1 use field EGR_DSI_CTRL_3 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_2_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_2_set", value );

    /* (0x00000084 bits 5:4) bits 0:1 use field EGR_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_2_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_2_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 5:4) bits 0:1 use field EGR_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_2_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_2_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_2_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_2_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 5:4) bits 0:1 use field EGR_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 5:4) bits 0:1 use field EGR_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_2_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_2_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_2_get", stop_bit, 1 );
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
    /* (0x00000084 bits 5:4) bits 0:1 use field EGR_DSI_CTRL_2 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_1_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_1_set", value );

    /* (0x00000084 bits 3:2) bits 0:1 use field EGR_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_1_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_1_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 3:2) bits 0:1 use field EGR_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_1_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_1_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_1_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_1_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 3:2) bits 0:1 use field EGR_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 3:2) bits 0:1 use field EGR_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_1_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_1_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_1_get", stop_bit, 1 );
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
    /* (0x00000084 bits 3:2) bits 0:1 use field EGR_DSI_CTRL_1 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_CTRL_0_set", value, 3);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_CTRL_0_set", value );

    /* (0x00000084 bits 1:0) bits 0:1 use field EGR_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                      h_ptr,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_MSK,
                                      EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF,
                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_CTRL_0_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_CTRL_0_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 1:0) bits 0:1 use field EGR_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_CTRL_0_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_CTRL_0_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_0_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000084 bits 1:0) bits 0:1 use field EGR_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
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
        /* (0x00000084 bits 1:0) bits 0:1 use field EGR_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
        emac_reg_EGRESS_DSI_IF_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF + subfield_offset),
                                          EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_0_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_CTRL_0_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_CTRL_0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_CTRL_0_get", stop_bit, 1 );
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
    /* (0x00000084 bits 1:0) bits 0:1 use field EGR_DSI_CTRL_0 of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF */
    reg_value = emac_reg_EGRESS_DSI_IF_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_CTRL_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_OVR_EN_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_OVR_EN_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_OVR_EN_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_OVR_EN_set", value );

    /* (0x00000088 bits 27:16) bits 0:11 use field EGR_DSI_OVR_EN of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( b_ptr,
                                                      h_ptr,
                                                      EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_MSK,
                                                      EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF,
                                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_OVR_EN_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_OVR_EN_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 27:16) bits 0:11 use field EGR_DSI_OVR_EN of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    reg_value = emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_OVR_EN_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_OVR_EN_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_OVR_EN_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_OVR_EN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_OVR_EN_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_OVR_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000088 bits 27:16) bits 0:11 use field EGR_DSI_OVR_EN of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
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
        /* (0x00000088 bits 27:16) bits 0:11 use field EGR_DSI_OVR_EN of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
        emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF + subfield_offset),
                                                          EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_OVR_EN_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_OVR_EN_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_OVR_EN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_OVR_EN_get", stop_bit, 11 );
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
    /* (0x00000088 bits 27:16) bits 0:11 use field EGR_DSI_OVR_EN of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    reg_value = emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_OVR_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_DSI_OVR_PAT_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_DSI_OVR_PAT_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_DSI_OVR_PAT_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_DSI_OVR_PAT_set", value );

    /* (0x00000088 bits 15:0) bits 0:15 use field EGR_DSI_OVR_PAT of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( b_ptr,
                                                      h_ptr,
                                                      EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_MSK,
                                                      EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF,
                                                      value);
}

static INLINE UINT32 emac_field_EGR_DSI_OVR_PAT_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_DSI_OVR_PAT_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 15:0) bits 0:15 use field EGR_DSI_OVR_PAT of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    reg_value = emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_MSK) >> EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_DSI_OVR_PAT_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_DSI_OVR_PAT_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_DSI_OVR_PAT_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_OVR_PAT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_OVR_PAT_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_OVR_PAT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000088 bits 15:0) bits 0:15 use field EGR_DSI_OVR_PAT of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
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
        /* (0x00000088 bits 15:0) bits 0:15 use field EGR_DSI_OVR_PAT of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
        emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF + subfield_offset),
                                                          EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_DSI_OVR_PAT_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_DSI_OVR_PAT_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_DSI_OVR_PAT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_DSI_OVR_PAT_get", stop_bit, 15 );
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
    /* (0x00000088 bits 15:0) bits 0:15 use field EGR_DSI_OVR_PAT of register PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING */
    reg_value = emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_MSK)
                  >> EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_MSK, EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_DSI_OVR_PAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_DATA_SINK_set( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_DATA_SINK_set( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_DATA_SINK_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_DATA_SINK_set", value );

    /* (0x0000008c bits 11:0) bits 0:11 use field ING_DATA_SINK of register PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK */
    emac_reg_INGRESS_DATA_SINK_field_set( b_ptr,
                                          h_ptr,
                                          EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_MSK,
                                          EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF,
                                          value);
}

static INLINE UINT32 emac_field_ING_DATA_SINK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_DATA_SINK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 11:0) bits 0:11 use field ING_DATA_SINK of register PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK */
    reg_value = emac_reg_INGRESS_DATA_SINK_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_MSK) >> EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_DATA_SINK_get", value );

    return value;
}
static INLINE void emac_field_range_ING_DATA_SINK_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_DATA_SINK_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DATA_SINK_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DATA_SINK_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DATA_SINK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000008c bits 11:0) bits 0:11 use field ING_DATA_SINK of register PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK */
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
        /* (0x0000008c bits 11:0) bits 0:11 use field ING_DATA_SINK of register PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK */
        emac_reg_INGRESS_DATA_SINK_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF + subfield_offset),
                                              EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_DATA_SINK_get( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_DATA_SINK_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_DATA_SINK_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_DATA_SINK_get", stop_bit, 11 );
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
    /* (0x0000008c bits 11:0) bits 0:11 use field ING_DATA_SINK of register PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK */
    reg_value = emac_reg_INGRESS_DATA_SINK_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_MSK)
                  >> EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_MSK, EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_DATA_SINK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_TS_MASK_END_set( emac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_TS_MASK_END_set( emac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_TS_MASK_END_set", value, 32767);
    IOLOG( "%s <= 0x%08x", "emac_field_TS_MASK_END_set", value );

    /* (0x00000090 bits 30:16) bits 0:14 use field TS_MASK_END of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( b_ptr,
                                                   h_ptr,
                                                   EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_MSK,
                                                   EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF,
                                                   value);
}

static INLINE UINT32 emac_field_TS_MASK_END_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_TS_MASK_END_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 30:16) bits 0:14 use field TS_MASK_END of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_MSK) >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_TS_MASK_END_get", value );

    return value;
}
static INLINE void emac_field_range_TS_MASK_END_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_TS_MASK_END_set( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_END_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_END_set", stop_bit, 14 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_END_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 30:16) bits 0:14 use field TS_MASK_END of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
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
        /* (0x00000090 bits 30:16) bits 0:14 use field TS_MASK_END of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
        emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF + subfield_offset),
                                                       EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_TS_MASK_END_get( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_TS_MASK_END_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_END_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_END_get", stop_bit, 14 );
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
    /* (0x00000090 bits 30:16) bits 0:14 use field TS_MASK_END of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_MSK)
                  >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_MSK, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_END_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_TS_MASK_START_set( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_TS_MASK_START_set( emac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_TS_MASK_START_set", value, 32767);
    IOLOG( "%s <= 0x%08x", "emac_field_TS_MASK_START_set", value );

    /* (0x00000090 bits 14:0) bits 0:14 use field TS_MASK_START of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( b_ptr,
                                                   h_ptr,
                                                   EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_MSK,
                                                   EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF,
                                                   value);
}

static INLINE UINT32 emac_field_TS_MASK_START_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_TS_MASK_START_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 14:0) bits 0:14 use field TS_MASK_START of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_MSK) >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_TS_MASK_START_get", value );

    return value;
}
static INLINE void emac_field_range_TS_MASK_START_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_TS_MASK_START_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_START_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_START_set", stop_bit, 14 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_START_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 14:0) bits 0:14 use field TS_MASK_START of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
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
        /* (0x00000090 bits 14:0) bits 0:14 use field TS_MASK_START of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
        emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF + subfield_offset),
                                                       EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_TS_MASK_START_get( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_TS_MASK_START_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_START_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_START_get", stop_bit, 14 );
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
    /* (0x00000090 bits 14:0) bits 0:14 use field TS_MASK_START of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_MSK)
                  >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_MSK, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_START_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_TS_MASK_DISABLE_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_TS_MASK_DISABLE_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_TS_MASK_DISABLE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "emac_field_TS_MASK_DISABLE_set", value );

    /* (0x00000094 bits 2:0) bits 0:2 use field TS_MASK_DISABLE of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE */
    emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set( b_ptr,
                                                      h_ptr,
                                                      EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_MSK,
                                                      EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF,
                                                      value);
}

static INLINE UINT32 emac_field_TS_MASK_DISABLE_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_TS_MASK_DISABLE_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 2:0) bits 0:2 use field TS_MASK_DISABLE of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_MSK) >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_TS_MASK_DISABLE_get", value );

    return value;
}
static INLINE void emac_field_range_TS_MASK_DISABLE_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_TS_MASK_DISABLE_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_DISABLE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_DISABLE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_DISABLE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 2:0) bits 0:2 use field TS_MASK_DISABLE of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE */
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
        /* (0x00000094 bits 2:0) bits 0:2 use field TS_MASK_DISABLE of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE */
        emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF + subfield_offset),
                                                          EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_TS_MASK_DISABLE_get( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_TS_MASK_DISABLE_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_TS_MASK_DISABLE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_TS_MASK_DISABLE_get", stop_bit, 2 );
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
    /* (0x00000094 bits 2:0) bits 0:2 use field TS_MASK_DISABLE of register PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE */
    reg_value = emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_MSK)
                  >> EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_MSK, EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_TS_MASK_DISABLE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_ING_DPI_DROP_MASK_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_ING_DPI_DROP_MASK_set( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_ING_DPI_DROP_MASK_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_ING_DPI_DROP_MASK_set", value );

    /* (0x00000098 bits 15:8) bits 0:7 use field EMAC_ING_DPI_DROP_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( b_ptr,
                                                   h_ptr,
                                                   EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_MSK,
                                                   EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF,
                                                   value);
}

static INLINE UINT32 emac_field_EMAC_ING_DPI_DROP_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_ING_DPI_DROP_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 15:8) bits 0:7 use field EMAC_ING_DPI_DROP_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_ING_DPI_DROP_MASK_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_ING_DPI_DROP_MASK_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_ING_DPI_DROP_MASK_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_DROP_MASK_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_DROP_MASK_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_DROP_MASK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 15:8) bits 0:7 use field EMAC_ING_DPI_DROP_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
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
        /* (0x00000098 bits 15:8) bits 0:7 use field EMAC_ING_DPI_DROP_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
        emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF + subfield_offset),
                                                       EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_ING_DPI_DROP_MASK_get( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_ING_DPI_DROP_MASK_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_DROP_MASK_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_DROP_MASK_get", stop_bit, 7 );
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
    /* (0x00000098 bits 15:8) bits 0:7 use field EMAC_ING_DPI_DROP_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_MSK, EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_DROP_MASK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_ING_DPI_ERR_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_ING_DPI_ERR_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_ING_DPI_ERR_MASK_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_ING_DPI_ERR_MASK_set", value );

    /* (0x00000098 bits 7:0) bits 0:7 use field EMAC_ING_DPI_ERR_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( b_ptr,
                                                   h_ptr,
                                                   EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_MSK,
                                                   EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF,
                                                   value);
}

static INLINE UINT32 emac_field_EMAC_ING_DPI_ERR_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_ING_DPI_ERR_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 7:0) bits 0:7 use field EMAC_ING_DPI_ERR_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_ING_DPI_ERR_MASK_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_ING_DPI_ERR_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_ING_DPI_ERR_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_ERR_MASK_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_ERR_MASK_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_ERR_MASK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 7:0) bits 0:7 use field EMAC_ING_DPI_ERR_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
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
        /* (0x00000098 bits 7:0) bits 0:7 use field EMAC_ING_DPI_ERR_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
        emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF + subfield_offset),
                                                       EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_ING_DPI_ERR_MASK_get( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_ING_DPI_ERR_MASK_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_ERR_MASK_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_ERR_MASK_get", stop_bit, 7 );
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
    /* (0x00000098 bits 7:0) bits 0:7 use field EMAC_ING_DPI_ERR_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_MSK, EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_ERR_MASK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_ING_DPI_FCS_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_ING_DPI_FCS_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_ING_DPI_FCS_MASK_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_ING_DPI_FCS_MASK_set", value );

    /* (0x0000009c bits 23:12) bits 0:11 use field EMAC_ING_DPI_FCS_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( b_ptr,
                                                                    h_ptr,
                                                                    EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_MSK,
                                                                    EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF,
                                                                    value);
}

static INLINE UINT32 emac_field_EMAC_ING_DPI_FCS_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_ING_DPI_FCS_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000009c bits 23:12) bits 0:11 use field EMAC_ING_DPI_FCS_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( b_ptr,
                                                                           h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_ING_DPI_FCS_MASK_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_ING_DPI_FCS_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_ING_DPI_FCS_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_FCS_MASK_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_FCS_MASK_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_FCS_MASK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000009c bits 23:12) bits 0:11 use field EMAC_ING_DPI_FCS_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
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
        /* (0x0000009c bits 23:12) bits 0:11 use field EMAC_ING_DPI_FCS_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
        emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF + subfield_offset),
                                                                        EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_ING_DPI_FCS_MASK_get( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_ING_DPI_FCS_MASK_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_FCS_MASK_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_FCS_MASK_get", stop_bit, 11 );
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
    /* (0x0000009c bits 23:12) bits 0:11 use field EMAC_ING_DPI_FCS_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( b_ptr,
                                                                           h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_FCS_MASK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_ING_DPI_LEN_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_ING_DPI_LEN_MASK_set( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_ING_DPI_LEN_MASK_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_ING_DPI_LEN_MASK_set", value );

    /* (0x0000009c bits 11:0) bits 0:11 use field EMAC_ING_DPI_LEN_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( b_ptr,
                                                                    h_ptr,
                                                                    EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_MSK,
                                                                    EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF,
                                                                    value);
}

static INLINE UINT32 emac_field_EMAC_ING_DPI_LEN_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_ING_DPI_LEN_MASK_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000009c bits 11:0) bits 0:11 use field EMAC_ING_DPI_LEN_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( b_ptr,
                                                                           h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_ING_DPI_LEN_MASK_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_ING_DPI_LEN_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_ING_DPI_LEN_MASK_set( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_LEN_MASK_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_LEN_MASK_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_LEN_MASK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000009c bits 11:0) bits 0:11 use field EMAC_ING_DPI_LEN_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
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
        /* (0x0000009c bits 11:0) bits 0:11 use field EMAC_ING_DPI_LEN_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
        emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF + subfield_offset),
                                                                        EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_ING_DPI_LEN_MASK_get( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_ING_DPI_LEN_MASK_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_ING_DPI_LEN_MASK_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_ING_DPI_LEN_MASK_get", stop_bit, 11 );
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
    /* (0x0000009c bits 11:0) bits 0:11 use field EMAC_ING_DPI_LEN_MASK of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK */
    reg_value = emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read( b_ptr,
                                                                           h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_ING_DPI_LEN_MASK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set", value, 63);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set", value );

    /* (0x000000a0 bits 29:24) bits 0:5 use field EMAC_TX_CREDIT_COUNT_MAX_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 29:24) bits 0:5 use field EMAC_TX_CREDIT_COUNT_MAX_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 29:24) bits 0:5 use field EMAC_TX_CREDIT_COUNT_MAX_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 29:24) bits 0:5 use field EMAC_TX_CREDIT_COUNT_MAX_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get", stop_bit, 5 );
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
    /* (0x000000a0 bits 29:24) bits 0:5 use field EMAC_TX_CREDIT_COUNT_MAX_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_set", value, 15);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_set", value );

    /* (0x000000a0 bits 23:20) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 23:20) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 23:20) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 23:20) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get", stop_bit, 3 );
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
    /* (0x000000a0 bits 23:20) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_set( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_set", value, 15);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_set", value );

    /* (0x000000a0 bits 19:16) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 19:16) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 19:16) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 19:16) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get", stop_bit, 3 );
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
    /* (0x000000a0 bits 19:16) bits 0:3 use field EMAC_TX_CREDIT_COUNT_MAX_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set( emac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set", value, 63);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set", value );

    /* (0x000000a0 bits 13:8) bits 0:5 use field EMAC_TX_CREDIT_COUNT_INIT_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 13:8) bits 0:5 use field EMAC_TX_CREDIT_COUNT_INIT_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 13:8) bits 0:5 use field EMAC_TX_CREDIT_COUNT_INIT_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 13:8) bits 0:5 use field EMAC_TX_CREDIT_COUNT_INIT_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get( emac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get", stop_bit, 5 );
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
    /* (0x000000a0 bits 13:8) bits 0:5 use field EMAC_TX_CREDIT_COUNT_INIT_100G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_set", value, 15);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_set", value );

    /* (0x000000a0 bits 7:4) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 7:4) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 7:4) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 7:4) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get", stop_bit, 3 );
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
    /* (0x000000a0 bits 7:4) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_40G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_set( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_set", value, 15);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_set", value );

    /* (0x000000a0 bits 3:0) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                              h_ptr,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_MSK,
                                              EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF,
                                              value);
}

static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 3:0) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_MSK) >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 3:0) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
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
        /* (0x000000a0 bits 3:0) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
        emac_reg_EGRESS_CREDIT_CNT_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF + subfield_offset),
                                                  EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get", stop_bit, 3 );
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
    /* (0x000000a0 bits 3:0) bits 0:3 use field EMAC_TX_CREDIT_COUNT_INIT_10G of register PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG */
    reg_value = emac_reg_EGRESS_CREDIT_CNT_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_MSK)
                  >> EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_MSK, EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void emac_field_ING_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_LINK_RESET_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_LINK_RESET_set", value );

    /* (0x00000004 bits 23:12) bits 0:11 use field ING_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    emac_reg_LINK_RESET_field_set( b_ptr,
                                   h_ptr,
                                   EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_MSK,
                                   EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF,
                                   value);
}

static INLINE UINT32 emac_field_ING_LINK_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_LINK_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 23:12) bits 0:11 use field ING_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    reg_value = emac_reg_LINK_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_MSK) >> EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_LINK_RESET_get", value );

    return value;
}
static INLINE void emac_field_range_ING_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_LINK_RESET_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_LINK_RESET_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_LINK_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 23:12) bits 0:11 use field ING_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
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
        /* (0x00000004 bits 23:12) bits 0:11 use field ING_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
        emac_reg_LINK_RESET_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF + subfield_offset),
                                       EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_LINK_RESET_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_LINK_RESET_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_LINK_RESET_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_LINK_RESET_get", stop_bit, 11 );
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
    /* (0x00000004 bits 23:12) bits 0:11 use field ING_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    reg_value = emac_reg_LINK_RESET_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_MSK)
                  >> EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_MSK, EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_LINK_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_LINK_RESET_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_LINK_RESET_set", value );

    /* (0x00000004 bits 11:0) bits 0:11 use field EGR_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    emac_reg_LINK_RESET_field_set( b_ptr,
                                   h_ptr,
                                   EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_MSK,
                                   EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF,
                                   value);
}

static INLINE UINT32 emac_field_EGR_LINK_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_LINK_RESET_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 11:0) bits 0:11 use field EGR_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    reg_value = emac_reg_LINK_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_MSK) >> EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_LINK_RESET_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_LINK_RESET_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_LINK_RESET_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_LINK_RESET_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_LINK_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 11:0) bits 0:11 use field EGR_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
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
        /* (0x00000004 bits 11:0) bits 0:11 use field EGR_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
        emac_reg_LINK_RESET_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF + subfield_offset),
                                       EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_LINK_RESET_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_LINK_RESET_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_LINK_RESET_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_LINK_RESET_get", stop_bit, 11 );
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
    /* (0x00000004 bits 11:0) bits 0:11 use field EGR_LINK_RESET of register PMC_EMAC120_TOP_REG_LINK_RESET */
    reg_value = emac_reg_LINK_RESET_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_MSK)
                  >> EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_MSK, EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_LINK_RESET_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void emac_field_RX_REM_FAULT_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_RX_REM_FAULT_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_RX_REM_FAULT_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_RX_REM_FAULT_E_set", value );

    /* (0x00000060 bits 23:12) bits 0:11 use field RX_REM_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_RX_REM_FAULT_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_REM_FAULT_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 23:12) bits 0:11 use field RX_REM_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_REM_FAULT_E_get", value );

    return value;
}
static INLINE void emac_field_range_RX_REM_FAULT_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_RX_REM_FAULT_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000060 bits 23:12) bits 0:11 use field RX_REM_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
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
        /* (0x00000060 bits 23:12) bits 0:11 use field RX_REM_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_RX_REM_FAULT_E_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_REM_FAULT_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_E_get", stop_bit, 11 );
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
    /* (0x00000060 bits 23:12) bits 0:11 use field RX_REM_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_RX_LOC_FAULT_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_RX_LOC_FAULT_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_RX_LOC_FAULT_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_RX_LOC_FAULT_E_set", value );

    /* (0x00000060 bits 11:0) bits 0:11 use field RX_LOC_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_RX_LOC_FAULT_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_LOC_FAULT_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 11:0) bits 0:11 use field RX_LOC_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_LOC_FAULT_E_get", value );

    return value;
}
static INLINE void emac_field_range_RX_LOC_FAULT_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_RX_LOC_FAULT_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000060 bits 11:0) bits 0:11 use field RX_LOC_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
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
        /* (0x00000060 bits 11:0) bits 0:11 use field RX_LOC_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_RX_LOC_FAULT_E_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_LOC_FAULT_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_E_get", stop_bit, 11 );
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
    /* (0x00000060 bits 11:0) bits 0:11 use field RX_LOC_FAULT_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_PHY_LOS_E_set( emac_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_PHY_LOS_E_set( emac_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_PHY_LOS_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_PHY_LOS_E_set", value );

    /* (0x00000064 bits 11:0) bits 0:11 use field PHY_LOS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_PHY_LOS_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_PHY_LOS_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000064 bits 11:0) bits 0:11 use field PHY_LOS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_PHY_LOS_E_get", value );

    return value;
}
static INLINE void emac_field_range_PHY_LOS_E_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_PHY_LOS_E_set( emac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000064 bits 11:0) bits 0:11 use field PHY_LOS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_ */
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
        /* (0x00000064 bits 11:0) bits 0:11 use field PHY_LOS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_PHY_LOS_E_get( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_PHY_LOS_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_E_get", stop_bit, 11 );
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
    /* (0x00000064 bits 11:0) bits 0:11 use field PHY_LOS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_HI_BER_E_set( emac_buffer_t *b_ptr,
                                            emac_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_HI_BER_E_set( emac_buffer_t *b_ptr,
                                            emac_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_HI_BER_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_HI_BER_E_set", value );

    /* (0x00000068 bits 11:0) bits 0:11 use field HI_BER_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_HI_BER_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_HI_BER_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 11:0) bits 0:11 use field HI_BER_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_HI_BER_E_get", value );

    return value;
}
static INLINE void emac_field_range_HI_BER_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_HI_BER_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000068 bits 11:0) bits 0:11 use field HI_BER_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_ */
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
        /* (0x00000068 bits 11:0) bits 0:11 use field HI_BER_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_HI_BER_E_get( emac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_HI_BER_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_E_get", stop_bit, 11 );
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
    /* (0x00000068 bits 11:0) bits 0:11 use field HI_BER_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ALIGN_STATUS_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ALIGN_STATUS_E_set( emac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ALIGN_STATUS_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "emac_field_ALIGN_STATUS_E_set", value );

    /* (0x0000006c bits 18:16) bits 0:2 use field ALIGN_STATUS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_ALIGN_STATUS_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ALIGN_STATUS_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000006c bits 18:16) bits 0:2 use field ALIGN_STATUS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ALIGN_STATUS_E_get", value );

    return value;
}
static INLINE void emac_field_range_ALIGN_STATUS_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ALIGN_STATUS_E_set( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000006c bits 18:16) bits 0:2 use field ALIGN_STATUS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
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
        /* (0x0000006c bits 18:16) bits 0:2 use field ALIGN_STATUS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ALIGN_STATUS_E_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ALIGN_STATUS_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_E_get", stop_bit, 2 );
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
    /* (0x0000006c bits 18:16) bits 0:2 use field ALIGN_STATUS_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_BLOCK_LOCK_E_set( emac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_BLOCK_LOCK_E_set( emac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_BLOCK_LOCK_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_BLOCK_LOCK_E_set", value );

    /* (0x0000006c bits 11:0) bits 0:11 use field BLOCK_LOCK_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( b_ptr,
                                                     h_ptr,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_MSK,
                                                     EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF,
                                                     value);
}

static INLINE UINT32 emac_field_BLOCK_LOCK_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_BLOCK_LOCK_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000006c bits 11:0) bits 0:11 use field BLOCK_LOCK_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_BLOCK_LOCK_E_get", value );

    return value;
}
static INLINE void emac_field_range_BLOCK_LOCK_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_BLOCK_LOCK_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000006c bits 11:0) bits 0:11 use field BLOCK_LOCK_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
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
        /* (0x0000006c bits 11:0) bits 0:11 use field BLOCK_LOCK_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF + subfield_offset),
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_BLOCK_LOCK_E_get( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_BLOCK_LOCK_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_E_get", stop_bit, 11 );
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
    /* (0x0000006c bits 11:0) bits 0:11 use field BLOCK_LOCK_E of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_ING_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_AGB_OVERFLOW_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_AGB_OVERFLOW_E_set", value );

    /* (0x00000070 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    emac_reg_AGB_OVERFLOW_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_MSK,
                                            EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF,
                                            value);
}

static INLINE UINT32 emac_field_ING_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    reg_value = emac_reg_AGB_OVERFLOW_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_MSK) >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_AGB_OVERFLOW_E_get", value );

    return value;
}
static INLINE void emac_field_range_ING_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_AGB_OVERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_AGB_OVERFLOW_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_AGB_OVERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000070 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
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
        /* (0x00000070 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
        emac_reg_AGB_OVERFLOW_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF + subfield_offset),
                                                EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_AGB_OVERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_AGB_OVERFLOW_E_get", stop_bit, 11 );
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
    /* (0x00000070 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    reg_value = emac_reg_AGB_OVERFLOW_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_MSK)
                  >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_MSK, EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_AGB_OVERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EGR_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_AGB_OVERFLOW_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_AGB_OVERFLOW_E_set", value );

    /* (0x00000070 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    emac_reg_AGB_OVERFLOW_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_MSK,
                                            EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF,
                                            value);
}

static INLINE UINT32 emac_field_EGR_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    reg_value = emac_reg_AGB_OVERFLOW_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_MSK) >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_AGB_OVERFLOW_E_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_AGB_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_AGB_OVERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_AGB_OVERFLOW_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_AGB_OVERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000070 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
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
        /* (0x00000070 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
        emac_reg_AGB_OVERFLOW_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF + subfield_offset),
                                                EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_AGB_OVERFLOW_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_AGB_OVERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_AGB_OVERFLOW_E_get", stop_bit, 11 );
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
    /* (0x00000070 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_E of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN */
    reg_value = emac_reg_AGB_OVERFLOW_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_MSK)
                  >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_MSK, EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_AGB_OVERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_UNDERFLOW_E_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_UNDERFLOW_E_set( emac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_E_set", value );

    /* (0x00000078 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( b_ptr,
                                                    h_ptr,
                                                    EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_MSK,
                                                    EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF,
                                                    value);
}

static INLINE UINT32 emac_field_EMAC_TX_UNDERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_UNDERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000078 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_MSK) >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_E_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_UNDERFLOW_E_set( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_UNDERFLOW_E_set( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_UNDERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_UNDERFLOW_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_UNDERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000078 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
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
        /* (0x00000078 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
        emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF + subfield_offset),
                                                        EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_UNDERFLOW_E_get( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_UNDERFLOW_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_UNDERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_UNDERFLOW_E_get", stop_bit, 11 );
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
    /* (0x00000078 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_MSK, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_UNDERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_field_EMAC_TX_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_OVERFLOW_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_OVERFLOW_E_set", value );

    /* (0x00000078 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( b_ptr,
                                                    h_ptr,
                                                    EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_MSK,
                                                    EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF,
                                                    value);
}

static INLINE UINT32 emac_field_EMAC_TX_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_OVERFLOW_E_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000078 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_MSK) >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_OVERFLOW_E_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_OVERFLOW_E_set( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_OVERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_OVERFLOW_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_OVERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000078 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
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
        /* (0x00000078 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
        emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF + subfield_offset),
                                                        EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_OVERFLOW_E_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_OVERFLOW_E_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_OVERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_OVERFLOW_E_get", stop_bit, 11 );
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
    /* (0x00000078 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_E of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_MSK)
                  >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_MSK, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_OVERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void emac_field_PHY_LOS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_PHY_LOS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_PHY_LOS_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_PHY_LOS_I_set_to_clear", value );

    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_PHY_LOS_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_PHY_LOS_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_PHY_LOS_I_get", value );

    return value;
}
static INLINE void emac_field_range_PHY_LOS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_PHY_LOS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
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
        /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_PHY_LOS_I_get( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_PHY_LOS_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_I_get", stop_bit, 11 );
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
    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_I_poll( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_I_poll( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_I_poll( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_I_poll( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_PHY_LOS_I_poll", value );

    /* (0x00000040 bits 11:0) bits 0:11 use field PHY_LOS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_RX_REM_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_RX_REM_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_RX_REM_FAULT_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_RX_REM_FAULT_I_set_to_clear", value );

    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_RX_REM_FAULT_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_REM_FAULT_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_REM_FAULT_I_get", value );

    return value;
}
static INLINE void emac_field_range_RX_REM_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_RX_REM_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
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
        /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_RX_REM_FAULT_I_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_REM_FAULT_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_I_get", stop_bit, 11 );
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
    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_RX_REM_FAULT_I_poll", value );

    /* (0x00000044 bits 23:12) bits 0:11 use field RX_REM_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_RX_LOC_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_RX_LOC_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_RX_LOC_FAULT_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_RX_LOC_FAULT_I_set_to_clear", value );

    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_RX_LOC_FAULT_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_LOC_FAULT_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_LOC_FAULT_I_get", value );

    return value;
}
static INLINE void emac_field_range_RX_LOC_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_RX_LOC_FAULT_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
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
        /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_RX_LOC_FAULT_I_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_LOC_FAULT_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_I_get", stop_bit, 11 );
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
    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_RX_LOC_FAULT_I_poll", value );

    /* (0x00000044 bits 11:0) bits 0:11 use field RX_LOC_FAULT_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_HI_BER_I_set_to_clear( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_HI_BER_I_set_to_clear( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_HI_BER_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_HI_BER_I_set_to_clear", value );

    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_HI_BER_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_HI_BER_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_HI_BER_I_get", value );

    return value;
}
static INLINE void emac_field_range_HI_BER_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_HI_BER_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
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
        /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_HI_BER_I_get( emac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_HI_BER_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_I_get", stop_bit, 11 );
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
    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_HI_BER_I_poll( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_HI_BER_I_poll( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_HI_BER_I_poll", value );

    /* (0x00000048 bits 11:0) bits 0:11 use field HI_BER_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_ALIGN_STATUS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ALIGN_STATUS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ALIGN_STATUS_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "emac_field_ALIGN_STATUS_I_set_to_clear", value );

    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_ALIGN_STATUS_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ALIGN_STATUS_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ALIGN_STATUS_I_get", value );

    return value;
}
static INLINE void emac_field_range_ALIGN_STATUS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ALIGN_STATUS_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
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
        /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ALIGN_STATUS_I_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ALIGN_STATUS_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_I_get", stop_bit, 2 );
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
    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_I_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_I_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_ALIGN_STATUS_I_poll", value );

    /* (0x0000004c bits 18:16) bits 0:2 use field ALIGN_STATUS_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_BLOCK_LOCK_I_set_to_clear( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_BLOCK_LOCK_I_set_to_clear( emac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_BLOCK_LOCK_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_BLOCK_LOCK_I_set_to_clear", value );

    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK,
                                                                       EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF,
                                                                       value);
}

static INLINE UINT32 emac_field_BLOCK_LOCK_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_BLOCK_LOCK_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( b_ptr,
                                                              h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_BLOCK_LOCK_I_get", value );

    return value;
}
static INLINE void emac_field_range_BLOCK_LOCK_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_BLOCK_LOCK_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
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
        /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
        emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF + subfield_offset),
                                                                           EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_BLOCK_LOCK_I_get( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_BLOCK_LOCK_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_I_get", stop_bit, 11 );
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
    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF + subfield_offset),
                                                             value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_I_poll( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_I_poll( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_BLOCK_LOCK_I_poll", value );

    /* (0x0000004c bits 11:0) bits 0:11 use field BLOCK_LOCK_I of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll( b_ptr,
                                                         h_ptr,
                                                         EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK,
                                                         (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void emac_field_ING_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_ING_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_ING_AGB_OVERFLOW_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_ING_AGB_OVERFLOW_I_set_to_clear", value );

    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK,
                                                              EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF,
                                                              value);
}

static INLINE UINT32 emac_field_ING_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ING_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    reg_value = emac_reg_AGB_OVERFLOW_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK) >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ING_AGB_OVERFLOW_I_get", value );

    return value;
}
static INLINE void emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
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
        /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
        emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                                  EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_ING_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ING_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_AGB_OVERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_AGB_OVERFLOW_I_get", stop_bit, 11 );
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
    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    reg_value = emac_reg_AGB_OVERFLOW_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK)
                  >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK, EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_AGB_OVERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ING_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ING_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ING_AGB_OVERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ING_AGB_OVERFLOW_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ING_AGB_OVERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
        return emac_reg_AGB_OVERFLOW_INT_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                    value << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_ING_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_ING_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_ING_AGB_OVERFLOW_I_poll", value );

    /* (0x00000050 bits 23:12) bits 0:11 use field ING_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    return emac_reg_AGB_OVERFLOW_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK,
                                                (value<<EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void emac_field_EGR_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EGR_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EGR_AGB_OVERFLOW_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EGR_AGB_OVERFLOW_I_set_to_clear", value );

    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK,
                                                              EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF,
                                                              value);
}

static INLINE UINT32 emac_field_EGR_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EGR_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    reg_value = emac_reg_AGB_OVERFLOW_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK) >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EGR_AGB_OVERFLOW_I_get", value );

    return value;
}
static INLINE void emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
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
        /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
        emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                                  EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EGR_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EGR_AGB_OVERFLOW_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_AGB_OVERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_AGB_OVERFLOW_I_get", stop_bit, 11 );
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
    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    reg_value = emac_reg_AGB_OVERFLOW_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK)
                  >> EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK, EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_AGB_OVERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EGR_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EGR_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EGR_AGB_OVERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EGR_AGB_OVERFLOW_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EGR_AGB_OVERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
        return emac_reg_AGB_OVERFLOW_INT_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                    value << (EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_EGR_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_EGR_AGB_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_EGR_AGB_OVERFLOW_I_poll", value );

    /* (0x00000050 bits 11:0) bits 0:11 use field EGR_AGB_OVERFLOW_I of register PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT */
    return emac_reg_AGB_OVERFLOW_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK,
                                                (value<<EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void emac_field_EMAC_TX_UNDERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_UNDERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_I_set_to_clear", value );

    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK,
                                                                      EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF,
                                                                      value);
}

static INLINE UINT32 emac_field_EMAC_TX_UNDERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_UNDERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK) >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_UNDERFLOW_I_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
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
        /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
        emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF + subfield_offset),
                                                                          EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_UNDERFLOW_I_get( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_UNDERFLOW_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_UNDERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_UNDERFLOW_I_get", stop_bit, 11 );
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
    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_UNDERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_UNDERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_UNDERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_UNDERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_UNDERFLOW_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_UNDERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
        return emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_UNDERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_UNDERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_EMAC_TX_UNDERFLOW_I_poll", value );

    /* (0x00000058 bits 23:12) bits 0:11 use field EMAC_TX_UNDERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    return emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE void emac_field_EMAC_TX_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_EMAC_TX_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_field_EMAC_TX_OVERFLOW_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_field_EMAC_TX_OVERFLOW_I_set_to_clear", value );

    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK,
                                                                      EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF,
                                                                      value);
}

static INLINE UINT32 emac_field_EMAC_TX_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_EMAC_TX_OVERFLOW_I_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK) >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_EMAC_TX_OVERFLOW_I_get", value );

    return value;
}
static INLINE void emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear( emac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
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
        /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
        emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF + subfield_offset),
                                                                          EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_field_range_EMAC_TX_OVERFLOW_I_get( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_EMAC_TX_OVERFLOW_I_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_OVERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_OVERFLOW_I_get", stop_bit, 11 );
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
    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    reg_value = emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK)
                  >> EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK, EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_OVERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_EMAC_TX_OVERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_EMAC_TX_OVERFLOW_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_EMAC_TX_OVERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
        return emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_OVERFLOW_I_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_EMAC_TX_OVERFLOW_I_poll", value );

    /* (0x00000058 bits 11:0) bits 0:11 use field EMAC_TX_OVERFLOW_I of register PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT */
    return emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF),
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
static INLINE UINT32 emac_field_PHY_LOS_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_PHY_LOS_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 11:0) bits 0:11 use field PHY_LOS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_PHY_LOS_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_PHY_LOS_V_get( emac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_PHY_LOS_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_V_get", stop_bit, 11 );
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
    /* (0x00000030 bits 11:0) bits 0:11 use field PHY_LOS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_V_poll( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_V_poll( emac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_PHY_LOS_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_PHY_LOS_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_PHY_LOS_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000030 bits 11:0) bits 0:11 use field PHY_LOS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000030 bits 11:0) bits 0:11 use field PHY_LOS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_V_poll( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_V_poll( emac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_PHY_LOS_V_poll", value );

    /* (0x00000030 bits 11:0) bits 0:11 use field PHY_LOS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 emac_field_RX_REM_FAULT_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_REM_FAULT_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 23:12) bits 0:11 use field RX_REM_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_REM_FAULT_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_RX_REM_FAULT_V_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_REM_FAULT_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_V_get", stop_bit, 11 );
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
    /* (0x00000034 bits 23:12) bits 0:11 use field RX_REM_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_REM_FAULT_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_REM_FAULT_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_REM_FAULT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 23:12) bits 0:11 use field RX_REM_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000034 bits 23:12) bits 0:11 use field RX_REM_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_RX_REM_FAULT_V_poll", value );

    /* (0x00000034 bits 23:12) bits 0:11 use field RX_REM_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 emac_field_RX_LOC_FAULT_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_RX_LOC_FAULT_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 11:0) bits 0:11 use field RX_LOC_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_RX_LOC_FAULT_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_RX_LOC_FAULT_V_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_RX_LOC_FAULT_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_V_get", stop_bit, 11 );
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
    /* (0x00000034 bits 11:0) bits 0:11 use field RX_LOC_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_RX_LOC_FAULT_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_RX_LOC_FAULT_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_RX_LOC_FAULT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 11:0) bits 0:11 use field RX_LOC_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000034 bits 11:0) bits 0:11 use field RX_LOC_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_RX_LOC_FAULT_V_poll", value );

    /* (0x00000034 bits 11:0) bits 0:11 use field RX_LOC_FAULT_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 emac_field_HI_BER_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_HI_BER_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 11:0) bits 0:11 use field HI_BER_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_HI_BER_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_HI_BER_V_get( emac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_HI_BER_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_V_get", stop_bit, 11 );
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
    /* (0x00000038 bits 11:0) bits 0:11 use field HI_BER_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_HI_BER_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_HI_BER_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_HI_BER_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000038 bits 11:0) bits 0:11 use field HI_BER_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000038 bits 11:0) bits 0:11 use field HI_BER_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_HI_BER_V_poll( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_HI_BER_V_poll( emac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_HI_BER_V_poll", value );

    /* (0x00000038 bits 11:0) bits 0:11 use field HI_BER_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 emac_field_ALIGN_STATUS_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_ALIGN_STATUS_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 18:16) bits 0:2 use field ALIGN_STATUS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_ALIGN_STATUS_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_ALIGN_STATUS_V_get( emac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_ALIGN_STATUS_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_V_get", stop_bit, 2 );
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
    /* (0x0000003c bits 18:16) bits 0:2 use field ALIGN_STATUS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_V_poll( emac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_ALIGN_STATUS_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_ALIGN_STATUS_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_ALIGN_STATUS_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 18:16) bits 0:2 use field ALIGN_STATUS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000003c bits 18:16) bits 0:2 use field ALIGN_STATUS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_V_poll( emac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_ALIGN_STATUS_V_poll", value );

    /* (0x0000003c bits 18:16) bits 0:2 use field ALIGN_STATUS_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 emac_field_BLOCK_LOCK_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_BLOCK_LOCK_V_get( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 11:0) bits 0:11 use field BLOCK_LOCK_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( b_ptr,
                                                             h_ptr);
    value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_MSK) >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF;
    IOLOG( "%s -> 0x%08x", "emac_field_BLOCK_LOCK_V_get", value );

    return value;
}
static INLINE UINT32 emac_field_range_BLOCK_LOCK_V_get( emac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_field_range_BLOCK_LOCK_V_get( emac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_V_get", stop_bit, 11 );
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
    /* (0x0000003c bits 11:0) bits 0:11 use field BLOCK_LOCK_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    reg_value = emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_MSK)
                  >> EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_MSK, EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_V_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_V_poll( emac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_field_range_BLOCK_LOCK_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_field_range_BLOCK_LOCK_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_field_range_BLOCK_LOCK_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 11:0) bits 0:11 use field BLOCK_LOCK_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000003c bits 11:0) bits 0:11 use field BLOCK_LOCK_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
        return emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF + subfield_offset),
                                                            value << (EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_V_poll( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_V_poll( emac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "emac_field_BLOCK_LOCK_V_poll", value );

    /* (0x0000003c bits 11:0) bits 0:11 use field BLOCK_LOCK_V of register PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_ */
    return emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll( b_ptr,
                                                        h_ptr,
                                                        EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_MSK,
                                                        (value<<EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_IO_INLINE_H */
