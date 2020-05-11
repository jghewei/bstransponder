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
 *     and register accessor functions for the sysotn block
 *****************************************************************************/
#ifndef _SYSOTN_IO_INLINE_H
#define _SYSOTN_IO_INLINE_H

#include "lineotn_loc.h"
#include "sysotn_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SYSOTN_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sysotn
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
} sysotn_buffer_t;
static INLINE void sysotn_buffer_init( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sysotn_buffer_init( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sysotn_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sysotn_buffer_flush( sysotn_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sysotn_buffer_flush( sysotn_buffer_t *b_ptr )
{
    IOLOG( "sysotn_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sysotn_reg_read( sysotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sysotn_reg_read( sysotn_buffer_t *b_ptr,
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
static INLINE void sysotn_reg_write( sysotn_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_write( sysotn_buffer_t *b_ptr,
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

static INLINE void sysotn_field_set( sysotn_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_set( sysotn_buffer_t *b_ptr,
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

static INLINE void sysotn_action_on_write_field_set( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_action_on_write_field_set( sysotn_buffer_t *b_ptr,
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

static INLINE void sysotn_burst_read( sysotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void sysotn_burst_read( sysotn_buffer_t *b_ptr,
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

static INLINE void sysotn_burst_write( sysotn_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void sysotn_burst_write( sysotn_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sysotn_poll( sysotn_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sysotn_poll( sysotn_buffer_t *b_ptr,
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
 *  register access functions for sysotn
 * ==================================================================================
 */

static INLINE void sysotn_reg_MASTER_CONFIG_write( sysotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_MASTER_CONFIG_write( sysotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_reg_MASTER_CONFIG_write", value );
    sysotn_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_SYSOTN_REG_MASTER_CONFIG,
                      value);
}

static INLINE void sysotn_reg_MASTER_CONFIG_field_set( sysotn_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_MASTER_CONFIG_field_set( sysotn_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_reg_MASTER_CONFIG_field_set", mask, ofs, value );
    sysotn_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_SYSOTN_REG_MASTER_CONFIG,
                      mask,
                      PMC_SYSOTN_REG_MASTER_CONFIG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sysotn_reg_MASTER_CONFIG_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_reg_MASTER_CONFIG_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_SYSOTN_REG_MASTER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "sysotn_reg_MASTER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_reg_TOP_INT_EN_write( sysotn_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_TOP_INT_EN_write( sysotn_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_reg_TOP_INT_EN_write", value );
    sysotn_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_SYSOTN_REG_TOP_INT_EN,
                      value);
}

static INLINE void sysotn_reg_TOP_INT_EN_field_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_TOP_INT_EN_field_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_reg_TOP_INT_EN_field_set", mask, ofs, value );
    sysotn_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_SYSOTN_REG_TOP_INT_EN,
                      mask,
                      PMC_SYSOTN_REG_TOP_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sysotn_reg_TOP_INT_EN_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_reg_TOP_INT_EN_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_SYSOTN_REG_TOP_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sysotn_reg_TOP_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sysotn_reg_FEC_STAT_UPDATE_write( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_FEC_STAT_UPDATE_write( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sysotn_reg_FEC_STAT_UPDATE_write", value );
    sysotn_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_SYSOTN_REG_FEC_STAT_UPDATE,
                      value);
}

static INLINE void sysotn_reg_FEC_STAT_UPDATE_field_set( sysotn_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_reg_FEC_STAT_UPDATE_field_set( sysotn_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sysotn_reg_FEC_STAT_UPDATE_field_set", mask, ofs, value );
    sysotn_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_SYSOTN_REG_FEC_STAT_UPDATE,
                      mask,
                      PMC_SYSOTN_REG_FEC_STAT_UPDATE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 sysotn_reg_FEC_STAT_UPDATE_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_reg_FEC_STAT_UPDATE_read( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sysotn_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_SYSOTN_REG_FEC_STAT_UPDATE);

    IOLOG( "%s -> 0x%08x;", "sysotn_reg_FEC_STAT_UPDATE_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void sysotn_field_FEC_FAIL_DLOM_ENABLE_set( sysotn_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_FEC_FAIL_DLOM_ENABLE_set( sysotn_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_FEC_FAIL_DLOM_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_FEC_FAIL_DLOM_ENABLE_set", value );

    /* (0x00000000 bits 10) field FEC_FAIL_DLOM_ENABLE of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_FEC_FAIL_DLOM_ENABLE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_FEC_FAIL_DLOM_ENABLE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 10) field FEC_FAIL_DLOM_ENABLE of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_FEC_FAIL_DLOM_ENABLE_get", value );

    return value;
}
static INLINE void sysotn_field_BYPASS_GFEC_TX_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_BYPASS_GFEC_TX_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_BYPASS_GFEC_TX_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_BYPASS_GFEC_TX_set", value );

    /* (0x00000000 bits 9) field BYPASS_GFEC_TX of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_BYPASS_GFEC_TX_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_BYPASS_GFEC_TX_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field BYPASS_GFEC_TX of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_BYPASS_GFEC_TX_get", value );

    return value;
}
static INLINE void sysotn_field_BYPASS_GFEC_RX_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_BYPASS_GFEC_RX_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_BYPASS_GFEC_RX_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_BYPASS_GFEC_RX_set", value );

    /* (0x00000000 bits 8) field BYPASS_GFEC_RX of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_BYPASS_GFEC_RX_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_BYPASS_GFEC_RX_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field BYPASS_GFEC_RX of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_BYPASS_GFEC_RX_get", value );

    return value;
}
static INLINE void sysotn_field_OTL_MODE_set( sysotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_OTL_MODE_set( sysotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_OTL_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_OTL_MODE_set", value );

    /* (0x00000000 bits 7) field OTL_MODE of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_OTL_MODE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_OTL_MODE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 7) field OTL_MODE of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_OTL_MODE_get", value );

    return value;
}
static INLINE void sysotn_field_OTN_OTU_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_OTN_OTU_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_OTN_OTU_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_OTN_OTU_LOWPWR_set", value );

    /* (0x00000000 bits 6) field OTN_OTU_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_OTN_OTU_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_OTN_OTU_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 6) field OTN_OTU_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_OTN_OTU_LOWPWR_get", value );

    return value;
}
static INLINE void sysotn_field_SFIS_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_SFIS_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_SFIS_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_SFIS_LOWPWR_set", value );

    /* (0x00000000 bits 5) field SFIS_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_SFIS_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_SFIS_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field SFIS_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_SFIS_LOWPWR_get", value );

    return value;
}
static INLINE void sysotn_field_OPSM4_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_OPSM4_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_OPSM4_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_OPSM4_LOWPWR_set", value );

    /* (0x00000000 bits 4) field OPSM4_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_OPSM4_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_OPSM4_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field OPSM4_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_OPSM4_LOWPWR_get", value );

    return value;
}
static INLINE void sysotn_field_GFEC_234_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_GFEC_234_LOWPWR_set( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_GFEC_234_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_GFEC_234_LOWPWR_set", value );

    /* (0x00000000 bits 3) field GFEC_234_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_GFEC_234_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_GFEC_234_LOWPWR_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field GFEC_234_LOWPWR of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_GFEC_234_LOWPWR_get", value );

    return value;
}
static INLINE void sysotn_field_PCBI_LCLK_MASK_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_PCBI_LCLK_MASK_set( sysotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_PCBI_LCLK_MASK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_PCBI_LCLK_MASK_set", value );

    /* (0x00000000 bits 2) field PCBI_LCLK_MASK of register PMC_SYSOTN_REG_MASTER_CONFIG */
    sysotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK,
                                        SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF,
                                        value);
}

static INLINE UINT32 sysotn_field_PCBI_LCLK_MASK_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_PCBI_LCLK_MASK_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field PCBI_LCLK_MASK of register PMC_SYSOTN_REG_MASTER_CONFIG */
    reg_value = sysotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK) >> SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_PCBI_LCLK_MASK_get", value );

    return value;
}
static INLINE void sysotn_field_TOP_INT_E_set( sysotn_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_TOP_INT_E_set( sysotn_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "sysotn_field_TOP_INT_E_set", value );

    /* (0x00000004 bits 31:0) bits 0:31 use field TOP_INT_E of register PMC_SYSOTN_REG_TOP_INT_EN */
    sysotn_reg_TOP_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_MSK,
                                     SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF,
                                     value);
}

static INLINE UINT32 sysotn_field_TOP_INT_E_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_TOP_INT_E_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 31:0) bits 0:31 use field TOP_INT_E of register PMC_SYSOTN_REG_TOP_INT_EN */
    reg_value = sysotn_reg_TOP_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_MSK) >> SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_TOP_INT_E_get", value );

    return value;
}
static INLINE void sysotn_field_range_TOP_INT_E_set( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_range_TOP_INT_E_set( sysotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_field_range_TOP_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_field_range_TOP_INT_E_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sysotn_field_range_TOP_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 31:0) bits 0:31 use field TOP_INT_E of register PMC_SYSOTN_REG_TOP_INT_EN */
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
        /* (0x00000004 bits 31:0) bits 0:31 use field TOP_INT_E of register PMC_SYSOTN_REG_TOP_INT_EN */
        sysotn_reg_TOP_INT_EN_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF + subfield_offset),
                                         SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 sysotn_field_range_TOP_INT_E_get( sysotn_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_range_TOP_INT_E_get( sysotn_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sysotn_field_range_TOP_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sysotn_field_range_TOP_INT_E_get", stop_bit, 31 );
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
    /* (0x00000004 bits 31:0) bits 0:31 use field TOP_INT_E of register PMC_SYSOTN_REG_TOP_INT_EN */
    reg_value = sysotn_reg_TOP_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_MSK)
                  >> SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_MSK, SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sysotn_field_range_TOP_INT_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void sysotn_field_RS_FEC_STATS_UPDATE_set( sysotn_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sysotn_field_RS_FEC_STATS_UPDATE_set( sysotn_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sysotn_field_RS_FEC_STATS_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sysotn_field_RS_FEC_STATS_UPDATE_set", value );

    /* (0x00000010 bits 1) field RS_FEC_STATS_UPDATE of register PMC_SYSOTN_REG_FEC_STAT_UPDATE */
    sysotn_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK,
                                          SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF,
                                          value);
}

static INLINE UINT32 sysotn_field_RS_FEC_STATS_UPDATE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sysotn_field_RS_FEC_STATS_UPDATE_get( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 1) field RS_FEC_STATS_UPDATE of register PMC_SYSOTN_REG_FEC_STAT_UPDATE */
    reg_value = sysotn_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK) >> SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "sysotn_field_RS_FEC_STATS_UPDATE_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSOTN_IO_INLINE_H */
