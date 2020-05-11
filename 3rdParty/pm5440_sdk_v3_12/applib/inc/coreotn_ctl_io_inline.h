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
 *     and register accessor functions for the coreotn_ctl block
 *****************************************************************************/
#ifndef _COREOTN_CTL_IO_INLINE_H
#define _COREOTN_CTL_IO_INLINE_H

#include "coreotn_loc.h"
#include "coreotn_ctl_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define COREOTN_CTL_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for coreotn_ctl
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
    coreotn_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} coreotn_ctl_buffer_t;
static INLINE void coreotn_ctl_buffer_init( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_buffer_init( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "coreotn_ctl_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void coreotn_ctl_buffer_flush( coreotn_ctl_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_buffer_flush( coreotn_ctl_buffer_t *b_ptr )
{
    IOLOG( "coreotn_ctl_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 coreotn_ctl_reg_read( coreotn_ctl_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_read( coreotn_ctl_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
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
static INLINE void coreotn_ctl_reg_write( coreotn_ctl_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_write( coreotn_ctl_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
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

static INLINE void coreotn_ctl_field_set( coreotn_ctl_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 mask,
                                          UINT32 unused_mask,
                                          UINT32 ofs,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_set( coreotn_ctl_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
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

static INLINE void coreotn_ctl_action_on_write_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_action_on_write_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
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

static INLINE void coreotn_ctl_burst_read( coreotn_ctl_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_burst_read( coreotn_ctl_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
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

static INLINE void coreotn_ctl_burst_write( coreotn_ctl_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_burst_write( coreotn_ctl_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE coreotn_ctl_poll( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 value,
                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                     UINT32 max_count,
                                                     UINT32 *num_failed_polls,
                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_ctl_poll( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
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
 *  register access functions for coreotn_ctl
 * ==================================================================================
 */

static INLINE void coreotn_ctl_reg_SW_RESET_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_SW_RESET_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_SW_RESET_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_SW_RESET_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_SW_RESET_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_SW_RESET_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_SW_RESET_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_SW_RESET_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_SW_RESET_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_SW_RESET_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_SW_RESET_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_SW_RESET_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_SW_RESET_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_CLK_GATING_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_CLK_GATING_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_CLK_GATING_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_CLK_GATING_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_CLK_GATING_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_CLK_GATING_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_CLK_GATING_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_CLK_GATING_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_CLK_GATING_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_CLK_GATING_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_CLK_GATING_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_CLK_GATING_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_CLK_GATING_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_RAM_LOW_POWER_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_RAM_LOW_POWER_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_RAM_LOW_POWER_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_RAM_LOW_POWER_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_RAM_LOW_POWER_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_RAM_LOW_POWER_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_1_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_1_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_1_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1,
                           value);
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1,
                           mask,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_LOWER_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_1_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_1_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_1_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1,
                           value);
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1,
                           mask,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_UPPER_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_2_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_2_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_2_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2,
                           value);
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2,
                           mask,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_LOWER_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_2_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_2_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_2_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2,
                           value);
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2,
                           mask,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_UPPER_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_3_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_3_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_3_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3,
                           value);
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3,
                           mask,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_3_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_3_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_LOWER_INT_EN_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_3_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_3_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_3_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3,
                           value);
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3,
                           mask,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_3_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_3_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_UPPER_INT_EN_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_4_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_4_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_4_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4,
                           value);
}

static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4,
                           mask,
                           PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_4_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_4_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_LOWER_INT_EN_REG_4_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_4_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_4_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_4_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4,
                           value);
}

static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4,
                           mask,
                           PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_4_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_4_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_UPPER_INT_EN_REG_4_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_DPI_BUS_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_DPI_BUS_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_DPI_BUS_CONFIG_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_CKCTL_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_CKCTL_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_CKCTL_CONFIG_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_CKCTL_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_CKCTL_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_CKCTL_CONFIG_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_CKCTL_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_CKCTL_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_CKCTL_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_OHP_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_OHP_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_OHP_CONFIG_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_OHP_CONFIG_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_OHP_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_OHP_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_OHP_CONFIG_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_OHP_CONFIG_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_OHP_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_OHP_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_OHP_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_OHP_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_OHP_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_write( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_write", value );
    coreotn_ctl_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG,
                           value);
}

static INLINE void coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set( coreotn_ctl_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set", mask, ofs, value );
    coreotn_ctl_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG,
                           mask,
                           PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 coreotn_ctl_reg_INT_STAT_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_INT_STAT_REG_1_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_CTL_REG_INT_STAT_REG_1);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_INT_STAT_REG_1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 coreotn_ctl_reg_INT_STAT_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_INT_STAT_REG_2_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_CTL_REG_INT_STAT_REG_2);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_INT_STAT_REG_2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = coreotn_ctl_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_set( coreotn_ctl_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_set( coreotn_ctl_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_set", value );

    /* (0x002ac000 bits 7) field ECC_FRM_MICROPROC_RESETN of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 7) field ECC_FRM_MICROPROC_RESETN of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_RSTB_set", value );

    /* (0x002ac000 bits 6) field ECC_FRM_RSTB of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 6) field ECC_FRM_RSTB of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_RSTB_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_S_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_S_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_S_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_S_RSTB_set", value );

    /* (0x002ac000 bits 5) field ECC_FRM_S_RSTB of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_S_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_S_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 5) field ECC_FRM_S_RSTB of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_S_RSTB_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMO_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMO_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMO_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMO_RST_set", value );

    /* (0x002ac000 bits 4) field MPMO_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_MPMO_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMO_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 4) field MPMO_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMO_RST_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMA_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMA_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMA_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMA_RST_set", value );

    /* (0x002ac000 bits 3) field MPMA_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_MPMA_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMA_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 3) field MPMA_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMA_RST_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP3_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP3_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP3_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP3_RST_set", value );

    /* (0x002ac000 bits 2) field OHP3_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_OHP3_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP3_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 2) field OHP3_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP3_RST_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP2_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP2_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP2_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP2_RST_set", value );

    /* (0x002ac000 bits 1) field OHP2_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_OHP2_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP2_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 1) field OHP2_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP2_RST_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP1_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP1_RST_set( coreotn_ctl_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP1_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP1_RST_set", value );

    /* (0x002ac000 bits 0) field OHP1_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    coreotn_ctl_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_MSK,
                                            COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_ctl_field_OHP1_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP1_RST_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac000 bits 0) field OHP1_RST of register PMC_COREOTN_CTL_REG_SW_RESET_REG */
    reg_value = coreotn_ctl_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_MSK) >> COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP1_RST_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_LOWPWR_set", value );

    /* (0x002ac004 bits 5) field ECC_FRM_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 5) field ECC_FRM_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMO_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMO_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMO_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMO_LOWPWR_set", value );

    /* (0x002ac004 bits 4) field MPMO_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_MPMO_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMO_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 4) field MPMO_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMO_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMA_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMA_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMA_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMA_LOWPWR_set", value );

    /* (0x002ac004 bits 3) field MPMA_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_MPMA_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMA_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 3) field MPMA_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMA_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP3_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP3_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP3_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP3_LOWPWR_set", value );

    /* (0x002ac004 bits 2) field OHP3_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP3_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP3_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 2) field OHP3_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP3_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP2_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP2_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP2_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP2_LOWPWR_set", value );

    /* (0x002ac004 bits 1) field OHP2_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP2_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP2_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 1) field OHP2_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP2_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP1_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP1_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP1_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP1_LOWPWR_set", value );

    /* (0x002ac004 bits 0) field OHP1_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    coreotn_ctl_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_MSK,
                                              COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP1_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP1_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac004 bits 0) field OHP1_LOWPWR of register PMC_COREOTN_CTL_REG_CLK_GATING_REG */
    reg_value = coreotn_ctl_reg_CLK_GATING_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_MSK) >> COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP1_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 5) field ECC_FRM_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 5) field ECC_FRM_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMO_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMO_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMO_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMO_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 4) field MPMO_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_MPMO_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMO_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 4) field MPMO_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMO_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_MPMA_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MPMA_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MPMA_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MPMA_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 3) field MPMA_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_MPMA_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MPMA_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 3) field MPMA_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MPMA_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP3_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP3_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP3_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP3_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 2) field OHP3_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_OHP3_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP3_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 2) field OHP3_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP3_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP2_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP2_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP2_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP2_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 1) field OHP2_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_OHP2_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP2_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 1) field OHP2_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP2_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP1_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP1_RAM_LOWPWR_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP1_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP1_RAM_LOWPWR_set", value );

    /* (0x002ac008 bits 0) field OHP1_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_MSK,
                                                 COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_ctl_field_OHP1_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP1_RAM_LOWPWR_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac008 bits 0) field OHP1_RAM_LOWPWR of register PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_ctl_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_MSK) >> COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP1_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void coreotn_ctl_lfield_INT_EN_1_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_1_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_1_set", value[1] , value[0] );

    /* (0x002ac028 bits 31:0) bits 0:31 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1 */
    coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_MSK,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF,
                                                  value[0]);

    /* (0x002ac02c bits 7:0) bits 32:39 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1 */
    coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_MSK,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF,
                                                  value[1]);
}

static INLINE void coreotn_ctl_lfield_INT_EN_1_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_1_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x002ac028 bits 31:0) bits 0:31 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1 */
    reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_MSK) >> COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF;
    value[0] |= field_value;

    /* (0x002ac02c bits 7:0) bits 32:39 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1 */
    reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_MSK) >> COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_1_get", value[1] , value[0] );

}
static INLINE void coreotn_ctl_lfield_range_INT_EN_1_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_range_INT_EN_1_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_1_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_1_set", stop_bit, 39 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac028 bits 31:0) bits 0:31 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1 */
    if (start_bit <= 31) {
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
        /* (0x002ac028 bits 31:0) bits 0:31 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1 */
        coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac02c bits 7:0) bits 32:39 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1 */
    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x002ac02c bits 7:0) bits 32:39 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1 */
        coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_1_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_1_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_1_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_1_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x002ac028 bits 31:0) bits 0:31 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1 */
        reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_MSK)
                      >> COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_MSK, COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x002ac02c bits 7:0) bits 32:39 use field INT_EN_1 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1 */
        reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_MSK)
                      >> COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_MSK, COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_lfield_INT_EN_2_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_2_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_2_set", value[1] , value[0] );

    /* (0x002ac030 bits 31:0) bits 0:31 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2 */
    coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_MSK,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF,
                                                  value[0]);

    /* (0x002ac034 bits 7:0) bits 32:39 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2 */
    coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_MSK,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF,
                                                  value[1]);
}

static INLINE void coreotn_ctl_lfield_INT_EN_2_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_2_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x002ac030 bits 31:0) bits 0:31 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2 */
    reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_MSK) >> COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF;
    value[0] |= field_value;

    /* (0x002ac034 bits 7:0) bits 32:39 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2 */
    reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_MSK) >> COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_2_get", value[1] , value[0] );

}
static INLINE void coreotn_ctl_lfield_range_INT_EN_2_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_range_INT_EN_2_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_2_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_2_set", stop_bit, 39 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac030 bits 31:0) bits 0:31 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2 */
    if (start_bit <= 31) {
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
        /* (0x002ac030 bits 31:0) bits 0:31 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2 */
        coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac034 bits 7:0) bits 32:39 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2 */
    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x002ac034 bits 7:0) bits 32:39 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2 */
        coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_2_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_2_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_2_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_2_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x002ac030 bits 31:0) bits 0:31 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2 */
        reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_MSK)
                      >> COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_MSK, COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x002ac034 bits 7:0) bits 32:39 use field INT_EN_2 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2 */
        reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_MSK)
                      >> COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_MSK, COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_lfield_INT_EN_3_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_3_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_3_set", value[1] , value[0] );

    /* (0x002ac038 bits 31:0) bits 0:31 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3 */
    coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_MSK,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF,
                                                  value[0]);

    /* (0x002ac03c bits 7:0) bits 32:39 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3 */
    coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_MSK,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF,
                                                  value[1]);
}

static INLINE void coreotn_ctl_lfield_INT_EN_3_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_3_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x002ac038 bits 31:0) bits 0:31 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3 */
    reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_3_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_MSK) >> COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF;
    value[0] |= field_value;

    /* (0x002ac03c bits 7:0) bits 32:39 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3 */
    reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_3_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_MSK) >> COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_3_get", value[1] , value[0] );

}
static INLINE void coreotn_ctl_lfield_range_INT_EN_3_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_range_INT_EN_3_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_3_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_3_set", stop_bit, 39 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac038 bits 31:0) bits 0:31 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3 */
    if (start_bit <= 31) {
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
        /* (0x002ac038 bits 31:0) bits 0:31 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3 */
        coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac03c bits 7:0) bits 32:39 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3 */
    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x002ac03c bits 7:0) bits 32:39 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3 */
        coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_3_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_3_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_3_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_3_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x002ac038 bits 31:0) bits 0:31 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3 */
        reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_MSK)
                      >> COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_MSK, COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x002ac03c bits 7:0) bits 32:39 use field INT_EN_3 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3 */
        reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_MSK)
                      >> COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_MSK, COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_lfield_INT_EN_4_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_4_set( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_4_set", value[1] , value[0] );

    /* (0x002ac040 bits 31:0) bits 0:31 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4 */
    coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_MSK,
                                                  COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF,
                                                  value[0]);

    /* (0x002ac044 bits 7:0) bits 32:39 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4 */
    coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_MSK,
                                                  COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF,
                                                  value[1]);
}

static INLINE void coreotn_ctl_lfield_INT_EN_4_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_EN_4_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x002ac040 bits 31:0) bits 0:31 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4 */
    reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_4_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_MSK) >> COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF;
    value[0] |= field_value;

    /* (0x002ac044 bits 7:0) bits 32:39 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4 */
    reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_4_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_MSK) >> COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_EN_4_get", value[1] , value[0] );

}
static INLINE void coreotn_ctl_lfield_range_INT_EN_4_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_range_INT_EN_4_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_4_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_4_set", stop_bit, 39 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac040 bits 31:0) bits 0:31 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4 */
    if (start_bit <= 31) {
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
        /* (0x002ac040 bits 31:0) bits 0:31 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4 */
        coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac044 bits 7:0) bits 32:39 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4 */
    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x002ac044 bits 7:0) bits 32:39 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4 */
        coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF + subfield_offset),
                                                      COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_4_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_lfield_range_INT_EN_4_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_EN_4_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_EN_4_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x002ac040 bits 31:0) bits 0:31 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4 */
        reg_value = coreotn_ctl_reg_LOWER_INT_EN_REG_4_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_MSK)
                      >> COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_MSK, COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x002ac044 bits 7:0) bits 32:39 use field INT_EN_4 of register PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4 */
        reg_value = coreotn_ctl_reg_UPPER_INT_EN_REG_4_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_MSK)
                      >> COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_MSK, COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_EN_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_SIFD1_SEL_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_SIFD1_SEL_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_SIFD1_SEL_CTRL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_SIFD1_SEL_CTRL_set", value );

    /* (0x002ac048 bits 4) field SIFD1_SEL_CTRL of register PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG */
    coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_MSK,
                                                  COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_OFF,
                                                  value);
}

static INLINE UINT32 coreotn_ctl_field_SIFD1_SEL_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_SIFD1_SEL_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac048 bits 4) field SIFD1_SEL_CTRL of register PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG */
    reg_value = coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_MSK) >> COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_SIFD1_SEL_CTRL_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_SIFD1_MUX_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_SIFD1_MUX_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_SIFD1_MUX_CTRL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_SIFD1_MUX_CTRL_set", value );

    /* (0x002ac048 bits 0) field SIFD1_MUX_CTRL of register PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG */
    coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_MSK,
                                                  COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_OFF,
                                                  value);
}

static INLINE UINT32 coreotn_ctl_field_SIFD1_MUX_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_SIFD1_MUX_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac048 bits 0) field SIFD1_MUX_CTRL of register PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG */
    reg_value = coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_MSK) >> COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_SIFD1_MUX_CTRL_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_CKCTL_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_CKCTL_CTRL_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_CKCTL_CTRL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_CKCTL_CTRL_set", value );

    /* (0x002ac04c bits 0) field CKCTL_CTRL of register PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG */
    coreotn_ctl_reg_CKCTL_CONFIG_REG_field_set( b_ptr,
                                                h_ptr,
                                                COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_MSK,
                                                COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_OFF,
                                                value);
}

static INLINE UINT32 coreotn_ctl_field_CKCTL_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_CKCTL_CTRL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac04c bits 0) field CKCTL_CTRL of register PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG */
    reg_value = coreotn_ctl_reg_CKCTL_CONFIG_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_MSK) >> COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_CKCTL_CTRL_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP3_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP3_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP3_SEL_B_set", value, 255);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP3_SEL_B_set", value );

    /* (0x002ac050 bits 31:24) field OHP3_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_MSK,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP3_SEL_B_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP3_SEL_B_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac050 bits 31:24) field OHP3_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_MSK) >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP3_SEL_B_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_OHP3_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_OHP3_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP3_SEL_B_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP3_SEL_B_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP3_SEL_B_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac050 bits 31:24) field OHP3_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
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
        /* (0x002ac050 bits 31:24) field OHP3_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
        coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF + subfield_offset),
                                                  COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_OHP3_SEL_B_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_OHP3_SEL_B_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP3_SEL_B_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP3_SEL_B_get", stop_bit, 7 );
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
    /* (0x002ac050 bits 31:24) field OHP3_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_MSK)
                  >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_MSK, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP3_SEL_B_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP3_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP3_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP3_SEL_A_set", value, 255);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP3_SEL_A_set", value );

    /* (0x002ac050 bits 23:16) field OHP3_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_MSK,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP3_SEL_A_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP3_SEL_A_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac050 bits 23:16) field OHP3_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_MSK) >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP3_SEL_A_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_OHP3_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_OHP3_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP3_SEL_A_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP3_SEL_A_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP3_SEL_A_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac050 bits 23:16) field OHP3_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
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
        /* (0x002ac050 bits 23:16) field OHP3_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
        coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF + subfield_offset),
                                                  COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_OHP3_SEL_A_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_OHP3_SEL_A_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP3_SEL_A_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP3_SEL_A_get", stop_bit, 7 );
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
    /* (0x002ac050 bits 23:16) field OHP3_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_MSK)
                  >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_MSK, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP3_SEL_A_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP2_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP2_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP2_SEL_B_set", value, 255);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP2_SEL_B_set", value );

    /* (0x002ac050 bits 15:8) field OHP2_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_MSK,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP2_SEL_B_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP2_SEL_B_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac050 bits 15:8) field OHP2_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_MSK) >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP2_SEL_B_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_OHP2_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_OHP2_SEL_B_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP2_SEL_B_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP2_SEL_B_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP2_SEL_B_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac050 bits 15:8) field OHP2_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
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
        /* (0x002ac050 bits 15:8) field OHP2_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
        coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF + subfield_offset),
                                                  COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_OHP2_SEL_B_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_OHP2_SEL_B_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP2_SEL_B_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP2_SEL_B_get", stop_bit, 7 );
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
    /* (0x002ac050 bits 15:8) field OHP2_SEL_B of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_MSK)
                  >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_MSK, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP2_SEL_B_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_OHP2_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_OHP2_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_OHP2_SEL_A_set", value, 255);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_OHP2_SEL_A_set", value );

    /* (0x002ac050 bits 7:0) field OHP2_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_MSK,
                                              COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF,
                                              value);
}

static INLINE UINT32 coreotn_ctl_field_OHP2_SEL_A_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_OHP2_SEL_A_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac050 bits 7:0) field OHP2_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_MSK) >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_OHP2_SEL_A_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_OHP2_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_OHP2_SEL_A_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP2_SEL_A_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP2_SEL_A_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP2_SEL_A_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac050 bits 7:0) field OHP2_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
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
        /* (0x002ac050 bits 7:0) field OHP2_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
        coreotn_ctl_reg_OHP_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF + subfield_offset),
                                                  COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_OHP2_SEL_A_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_OHP2_SEL_A_get( coreotn_ctl_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_OHP2_SEL_A_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_OHP2_SEL_A_get", stop_bit, 7 );
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
    /* (0x002ac050 bits 7:0) field OHP2_SEL_A of register PMC_COREOTN_CTL_REG_OHP_CONFIG_REG */
    reg_value = coreotn_ctl_reg_OHP_CONFIG_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_MSK)
                  >> COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_MSK, COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_OHP2_SEL_A_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_SLV_OP_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_SLV_OP_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_SLV_OP_MAX_CNT_set", value, 127);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_SLV_OP_MAX_CNT_set", value );

    /* (0x002ac054 bits 22:16) field SLV_OP_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_MSK,
                                                            COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF,
                                                            value);
}

static INLINE UINT32 coreotn_ctl_field_SLV_OP_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_SLV_OP_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac054 bits 22:16) field SLV_OP_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_MSK) >> COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_SLV_OP_MAX_CNT_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_SLV_OP_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_SLV_OP_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac054 bits 22:16) field SLV_OP_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
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
        /* (0x002ac054 bits 22:16) field SLV_OP_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
        coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF + subfield_offset),
                                                                COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_SLV_OP_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_SLV_OP_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_get", stop_bit, 6 );
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
    /* (0x002ac054 bits 22:16) field SLV_OP_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_MSK, COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_SLV_OP_MAX_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_MP_BLOCK_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_MP_BLOCK_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_MP_BLOCK_MAX_CNT_set", value, 32767);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_MP_BLOCK_MAX_CNT_set", value );

    /* (0x002ac054 bits 14:0) field MP_BLOCK_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_MSK,
                                                            COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF,
                                                            value);
}

static INLINE UINT32 coreotn_ctl_field_MP_BLOCK_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_MP_BLOCK_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac054 bits 14:0) field MP_BLOCK_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_MSK) >> COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_MP_BLOCK_MAX_CNT_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set( coreotn_ctl_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set", stop_bit, 14 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x002ac054 bits 14:0) field MP_BLOCK_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
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
        /* (0x002ac054 bits 14:0) field MP_BLOCK_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
        coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF + subfield_offset),
                                                                COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get( coreotn_ctl_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get", stop_bit, 14 );
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
    /* (0x002ac054 bits 14:0) field MP_BLOCK_MAX_CNT of register PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_MSK, COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_ctl_field_TX_GCC_BYTE_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_TX_GCC_BYTE_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_TX_GCC_BYTE_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_TX_GCC_BYTE_SWAP_set", value );

    /* (0x002ac058 bits 12) field TX_GCC_BYTE_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_TX_GCC_BYTE_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_TX_GCC_BYTE_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac058 bits 12) field TX_GCC_BYTE_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_MSK) >> COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_TX_GCC_BYTE_SWAP_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_TX_GCC_BIT_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_TX_GCC_BIT_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_TX_GCC_BIT_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_TX_GCC_BIT_SWAP_set", value );

    /* (0x002ac058 bits 8) field TX_GCC_BIT_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_TX_GCC_BIT_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_TX_GCC_BIT_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac058 bits 8) field TX_GCC_BIT_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_MSK) >> COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_TX_GCC_BIT_SWAP_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_RX_GCC_BYTE_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_RX_GCC_BYTE_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_RX_GCC_BYTE_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_RX_GCC_BYTE_SWAP_set", value );

    /* (0x002ac058 bits 4) field RX_GCC_BYTE_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_RX_GCC_BYTE_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_RX_GCC_BYTE_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac058 bits 4) field RX_GCC_BYTE_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_MSK) >> COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_RX_GCC_BYTE_SWAP_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_RX_GCC_BIT_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_RX_GCC_BIT_SWAP_set( coreotn_ctl_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_RX_GCC_BIT_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_RX_GCC_BIT_SWAP_set", value );

    /* (0x002ac058 bits 0) field RX_GCC_BIT_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_RX_GCC_BIT_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_RX_GCC_BIT_SWAP_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac058 bits 0) field RX_GCC_BIT_SWAP of register PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_MSK) >> COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_RX_GCC_BIT_SWAP_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_set", value );

    /* (0x002ac05c bits 4) field ECC_FRM_FIFO_TX_RSTB of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG */
    coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac05c bits 4) field ECC_FRM_FIFO_TX_RSTB of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_get", value );

    return value;
}
static INLINE void coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_set( coreotn_ctl_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_set", value );

    /* (0x002ac05c bits 0) field ECC_FRM_FIFO_RX_RSTB of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG */
    coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_MSK,
                                                      COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac05c bits 0) field ECC_FRM_FIFO_RX_RSTB of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE void coreotn_ctl_lfield_INT_STAT_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_ctl_lfield_INT_STAT_get( coreotn_ctl_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x002ac020 bits 31:0) bits 0:31 use field INT_STAT of register PMC_COREOTN_CTL_REG_INT_STAT_REG_1 */
    reg_value = coreotn_ctl_reg_INT_STAT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_MSK) >> COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_OFF;
    value[0] |= field_value;

    /* (0x002ac024 bits 7:0) bits 32:39 use field INT_STAT of register PMC_COREOTN_CTL_REG_INT_STAT_REG_2 */
    reg_value = coreotn_ctl_reg_INT_STAT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_MSK) >> COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "coreotn_ctl_lfield_INT_STAT_get", value[1] , value[0] );

}
static INLINE UINT32 coreotn_ctl_lfield_range_INT_STAT_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_lfield_range_INT_STAT_get( coreotn_ctl_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_lfield_range_INT_STAT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_lfield_range_INT_STAT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x002ac020 bits 31:0) bits 0:31 use field INT_STAT of register PMC_COREOTN_CTL_REG_INT_STAT_REG_1 */
        reg_value = coreotn_ctl_reg_INT_STAT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_MSK)
                      >> COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_MSK, COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x002ac024 bits 7:0) bits 32:39 use field INT_STAT of register PMC_COREOTN_CTL_REG_INT_STAT_REG_2 */
        reg_value = coreotn_ctl_reg_INT_STAT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_MSK)
                      >> COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_MSK, COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_lfield_range_INT_STAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac060 bits 29:24) field ECC_FRM_FIFO_RX_EMPTY of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_EMPTY_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get", stop_bit, 5 );
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
    /* (0x002ac060 bits 29:24) field ECC_FRM_FIFO_RX_EMPTY of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_FULL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_FULL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac060 bits 21:16) field ECC_FRM_FIFO_RX_FULL of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_FULL_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get", stop_bit, 5 );
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
    /* (0x002ac060 bits 21:16) field ECC_FRM_FIFO_RX_FULL of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac060 bits 13:8) field ECC_FRM_FIFO_RX_UNDERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_UNDERFLOW_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                            coreotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                            coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get", stop_bit, 5 );
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
    /* (0x002ac060 bits 13:8) field ECC_FRM_FIFO_RX_UNDERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac060 bits 5:0) field ECC_FRM_FIFO_RX_OVERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_RX_OVERFLOW_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get", stop_bit, 5 );
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
    /* (0x002ac060 bits 5:0) field ECC_FRM_FIFO_RX_OVERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac064 bits 29:24) field ECC_FRM_FIFO_TX_EMPTY of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_EMPTY_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get( coreotn_ctl_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get", stop_bit, 5 );
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
    /* (0x002ac064 bits 29:24) field ECC_FRM_FIFO_TX_EMPTY of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_FULL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_FULL_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac064 bits 21:16) field ECC_FRM_FIFO_TX_FULL of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_FULL_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get( coreotn_ctl_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get", stop_bit, 5 );
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
    /* (0x002ac064 bits 21:16) field ECC_FRM_FIFO_TX_FULL of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac064 bits 13:8) field ECC_FRM_FIFO_TX_UNDERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_UNDERFLOW_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                            coreotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                            coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get", stop_bit, 5 );
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
    /* (0x002ac064 bits 13:8) field ECC_FRM_FIFO_TX_UNDERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x002ac064 bits 5:0) field ECC_FRM_FIFO_TX_OVERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_MSK) >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_OFF;
    IOLOG( "%s -> 0x%08x", "coreotn_ctl_field_ECC_FRM_FIFO_TX_OVERFLOW_get", value );

    return value;
}
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get( coreotn_ctl_buffer_t *b_ptr,
                                                                           coreotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get", stop_bit, 5 );
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
    /* (0x002ac064 bits 5:0) field ECC_FRM_FIFO_TX_OVERFLOW of register PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG */
    reg_value = coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_MSK)
                  >> COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_MSK, COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COREOTN_CTL_IO_INLINE_H */
