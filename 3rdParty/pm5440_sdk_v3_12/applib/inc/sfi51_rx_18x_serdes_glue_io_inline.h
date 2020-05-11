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
 *     and register accessor functions for the sfi51_rx_18x_serdes_glue block
 *****************************************************************************/
#ifndef _SFI51_RX_18X_SERDES_GLUE_IO_INLINE_H
#define _SFI51_RX_18X_SERDES_GLUE_IO_INLINE_H

#include "sfi51_d8_rx_18x.h"
#include "sfi51_rx_18x_serdes_glue_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SFI51_RX_18X_SERDES_GLUE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sfi51_rx_18x_serdes_glue
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
    sfi51_d8_rx_18x_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sfi51_rx_18x_serdes_glue_buffer_t;
static INLINE void sfi51_rx_18x_serdes_glue_buffer_init( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_buffer_init( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sfi51_rx_18x_serdes_glue_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sfi51_rx_18x_serdes_glue_buffer_flush( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_buffer_flush( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr )
{
    IOLOG( "sfi51_rx_18x_serdes_glue_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
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
static INLINE void sfi51_rx_18x_serdes_glue_reg_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_rx_18x_serdes_glue_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 unused_mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_rx_18x_serdes_glue_action_on_write_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 mem_type,
                                                                       UINT32 reg,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_action_on_write_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_rx_18x_serdes_glue_burst_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 len,
                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_burst_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_rx_18x_serdes_glue_burst_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 len,
                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_burst_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 mem_type,
                                                                  UINT32 reg,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
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
 *  register access functions for sfi51_rx_18x_serdes_glue
 * ==================================================================================
 */

static INLINE void sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_INT_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SFI51_RX_18X_GLUE_REG_INT_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        MEM_TYPE_STATUS,
                                                        PMC_SFI51_RX_18X_GLUE_REG_INT_REG,
                                                        mask,
                                                        ofs,
                                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_RX_18X_GLUE_REG_INT_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_REG_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_REG_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_rx_18x_serdes_glue_reg_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_rx_18x_serdes_glue_poll( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_SFI51_RX_18X_GLUE_REG_INT_REG,
                                          mask,
                                          value,
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);

}


static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_write( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_write", value );
    sfi51_rx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_rx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_rx_18x_serdes_glue_poll( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG,
                                          mask,
                                          value,
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_rx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_set", value );

    /* (0x00000000 bits 5) field EFUSE_INT_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_INT_RSTB_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_INT_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field EFUSE_INT_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_INT_RSTB_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_INT_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_set", value );

    /* (0x00000000 bits 4) field REFCLK_DIVIDER_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_REFCLK_DIVIDER_RSTB_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_REFCLK_DIVIDER_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field REFCLK_DIVIDER_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_REFCLK_DIVIDER_RSTB_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_REFCLK_DIVIDER_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_set", value );

    /* (0x00000000 bits 3) field EFUSE_CTRL_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_CTRL_RSTB_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_CTRL_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field EFUSE_CTRL_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_CTRL_RSTB_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_EFUSE_CTRL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_set", value );

    /* (0x00000000 bits 2) field HRCLKDIV4_DIVIDER_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_HRCLKDIV4_DIVIDER_RSTB_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_HRCLKDIV4_DIVIDER_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field HRCLKDIV4_DIVIDER_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_HRCLKDIV4_DIVIDER_RSTB_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_HRCLKDIV4_DIVIDER_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_set", value );

    /* (0x00000000 bits 1) field DIGITAL_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field DIGITAL_RSTB of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_SW_RESET_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_SW_RESET_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_SW_RESET_set", value );

    /* (0x00000000 bits 0) field SW_RESET of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_MSK,
                                                          SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_SW_RESET_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_SW_RESET_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SW_RESET of register PMC_SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_MSK) >> SFI51_RX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_SW_RESET_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_set", value );

    /* (0x00000010 bits 0) field HRCLKDIV4_DIV_EN of register PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG */
    sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG_BIT_HRCLKDIV4_DIV_EN_MSK,
                                                              SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG_BIT_HRCLKDIV4_DIV_EN_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field HRCLKDIV4_DIV_EN of register PMC_SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG_BIT_HRCLKDIV4_DIV_EN_MSK) >> SFI51_RX_18X_GLUE_REG_HRCLKDIV4_DIV_CFG_BIT_HRCLKDIV4_DIV_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_set", value );

    /* (0x00000014 bits 0) field EFUSE_REFCLK_DIV_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG_BIT_EFUSE_REFCLK_DIV_EN_MSK,
                                                             SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG_BIT_EFUSE_REFCLK_DIV_EN_OFF,
                                                             value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 0) field EFUSE_REFCLK_DIV_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG_BIT_EFUSE_REFCLK_DIV_EN_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_REFCLK_CFG_BIT_EFUSE_REFCLK_DIV_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_set", value, 63);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_set", value );

    /* (0x00000018 bits 29:24) field RAMS_ADDR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 29:24) field RAMS_ADDR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 29:24) field RAMS_ADDR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
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
        /* (0x00000018 bits 29:24) field RAMS_ADDR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
        sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF + subfield_offset),
                                                                  SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_rx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get", stop_bit, 5 );
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
    /* (0x00000018 bits 29:24) field RAMS_ADDR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_MSK)
                  >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_MSK, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_set", value, 255);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_set", value );

    /* (0x00000018 bits 15:8) field RAMS_DATA_WR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 15:8) field RAMS_DATA_WR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 15:8) field RAMS_DATA_WR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
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
        /* (0x00000018 bits 15:8) field RAMS_DATA_WR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
        sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF + subfield_offset),
                                                                  SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get", stop_bit, 7 );
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
    /* (0x00000018 bits 15:8) field RAMS_DATA_WR of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_MSK)
                  >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_MSK, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_WR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_set", value );

    /* (0x00000018 bits 4) field RAMS_RD_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_RD_EN_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_RD_EN_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 4) field RAMS_RD_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_RD_EN_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_RD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_set", value );

    /* (0x00000018 bits 3) field RAMS_WR_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_WR_EN_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_WR_EN_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 3) field RAMS_WR_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_WR_EN_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_WR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_PCLK_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_PCLK_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_PCLK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_PCLK_EN_set", value );

    /* (0x00000018 bits 2) field PCLK_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_PCLK_EN_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_PCLK_EN_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_PCLK_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_PCLK_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 2) field PCLK_EN of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_PCLK_EN_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_PCLK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_PCLK_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_set", value );

    /* (0x00000018 bits 1:0) field RAM_EXT_INT_SEL of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_MSK,
                                                              SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 1:0) field RAM_EXT_INT_SEL of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 1:0) field RAM_EXT_INT_SEL of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
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
        /* (0x00000018 bits 1:0) field RAM_EXT_INT_SEL of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
        sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF + subfield_offset),
                                                                  SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                               sfi51_d8_rx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get", stop_bit, 1 );
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
    /* (0x00000018 bits 1:0) field RAM_EXT_INT_SEL of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_MSK)
                  >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_MSK, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAM_EXT_INT_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_set", value );

    /* (0x0000001c bits 7) field TX_SFI_LOS_POL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_LOS_POL_MSK,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_LOS_POL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 7) field TX_SFI_LOS_POL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_LOS_POL_MSK) >> SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_LOS_POL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_set", value );

    /* (0x0000001c bits 6) field TX_SFI_OOA_LOF_POL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_OOA_LOF_POL_MSK,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_OOA_LOF_POL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 6) field TX_SFI_OOA_LOF_POL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_OOA_LOF_POL_MSK) >> SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TX_SFI_OOA_LOF_POL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set", value );

    /* (0x0000001c bits 5) field TXDCK_SLICE_EN of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TXDCK_SLICE_EN_MSK,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TXDCK_SLICE_EN_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 5) field TXDCK_SLICE_EN of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TXDCK_SLICE_EN_MSK) >> SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_TXDCK_SLICE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_set", value );

    /* (0x0000001c bits 2:0) field REFCLK_SEL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK,
                                                           SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 2:0) field REFCLK_SEL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK) >> SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 2:0) field REFCLK_SEL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
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
        /* (0x0000001c bits 2:0) field REFCLK_SEL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
        sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF + subfield_offset),
                                                               SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_rx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get", stop_bit, 2 );
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
    /* (0x0000001c bits 2:0) field REFCLK_SEL of register PMC_SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    field_value = (reg_value & SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK)
                  >> SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK, SFI51_RX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_set", value );

    /* (0x00000024 bits 10) field RX_18X_GLUE_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_18X_GLUE_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_18X_GLUE_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 10) field RX_18X_GLUE_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_18X_GLUE_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_18X_GLUE_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_set", value );

    /* (0x00000024 bits 9) field RX_2X_SLICE8_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 9) field RX_2X_SLICE8_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_set", value );

    /* (0x00000024 bits 8) field RX_2X_SLICE7_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 8) field RX_2X_SLICE7_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_set", value );

    /* (0x00000024 bits 7) field RX_2X_SLICE6_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 7) field RX_2X_SLICE6_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_set", value );

    /* (0x00000024 bits 6) field RX_2X_SLICE5_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 6) field RX_2X_SLICE5_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_set", value );

    /* (0x00000024 bits 5) field RX_2X_SLICE4_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 5) field RX_2X_SLICE4_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_set", value );

    /* (0x00000024 bits 4) field RX_2X_SLICE3_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 4) field RX_2X_SLICE3_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_set", value );

    /* (0x00000024 bits 3) field RX_2X_SLICE2_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 3) field RX_2X_SLICE2_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_set", value );

    /* (0x00000024 bits 2) field RX_2X_SLICE1_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 2) field RX_2X_SLICE1_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_set", value );

    /* (0x00000024 bits 1) field RX_2X_SLICE0_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 1) field RX_2X_SLICE0_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_set", value );

    /* (0x00000024 bits 0) field CSU_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_MSK,
                                                              SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 0) field CSU_INT_DEV_E of register PMC_SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_MSK) >> SFI51_RX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_set", value );

    /* (0x00000028 bits 10) field RX_18X_GLUE_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_18X_GLUE_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_18X_GLUE_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 10) field RX_18X_GLUE_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_18X_GLUE_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_18X_GLUE_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_set", value );

    /* (0x00000028 bits 9) field RX_2X_SLICE8_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE8_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE8_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 9) field RX_2X_SLICE8_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE8_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE8_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_set", value );

    /* (0x00000028 bits 8) field RX_2X_SLICE7_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE7_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE7_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 8) field RX_2X_SLICE7_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE7_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE7_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_set", value );

    /* (0x00000028 bits 7) field RX_2X_SLICE6_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE6_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE6_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 7) field RX_2X_SLICE6_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE6_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE6_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_set", value );

    /* (0x00000028 bits 6) field RX_2X_SLICE5_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE5_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE5_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 6) field RX_2X_SLICE5_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE5_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE5_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_set", value );

    /* (0x00000028 bits 5) field RX_2X_SLICE4_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE4_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE4_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 5) field RX_2X_SLICE4_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE4_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE4_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_set", value );

    /* (0x00000028 bits 4) field RX_2X_SLICE3_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE3_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE3_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 4) field RX_2X_SLICE3_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE3_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE3_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_set", value );

    /* (0x00000028 bits 3) field RX_2X_SLICE2_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE2_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE2_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 3) field RX_2X_SLICE2_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE2_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE2_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_set", value );

    /* (0x00000028 bits 2) field RX_2X_SLICE1_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE1_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE1_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 2) field RX_2X_SLICE1_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE1_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE1_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_set", value );

    /* (0x00000028 bits 1) field RX_2X_SLICE0_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE0_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE0_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 1) field RX_2X_SLICE0_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE0_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_RX_2X_SLICE0_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_set", value );

    /* (0x00000028 bits 0) field CSU_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 0) field CSU_INT_MIP_E of register PMC_SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_MSK) >> SFI51_RX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_set", value );

    /* (0x0000002c bits 10) field RX_18X_GLUE_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_18X_GLUE_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_18X_GLUE_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 10) field RX_18X_GLUE_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_18X_GLUE_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_18X_GLUE_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_set", value );

    /* (0x0000002c bits 9) field RX_2X_SLICE8_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 9) field RX_2X_SLICE8_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE8_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_set", value );

    /* (0x0000002c bits 8) field RX_2X_SLICE7_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 8) field RX_2X_SLICE7_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE7_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_set", value );

    /* (0x0000002c bits 7) field RX_2X_SLICE6_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 7) field RX_2X_SLICE6_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE6_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_set", value );

    /* (0x0000002c bits 6) field RX_2X_SLICE5_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 6) field RX_2X_SLICE5_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE5_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_set", value );

    /* (0x0000002c bits 5) field RX_2X_SLICE4_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 5) field RX_2X_SLICE4_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE4_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_set", value );

    /* (0x0000002c bits 4) field RX_2X_SLICE3_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 4) field RX_2X_SLICE3_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE3_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_set", value );

    /* (0x0000002c bits 3) field RX_2X_SLICE2_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 3) field RX_2X_SLICE2_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE2_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_set", value );

    /* (0x0000002c bits 2) field RX_2X_SLICE1_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 2) field RX_2X_SLICE1_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE1_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_set", value );

    /* (0x0000002c bits 1) field RX_2X_SLICE0_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 1) field RX_2X_SLICE0_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_RX_2X_SLICE0_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_set", value );

    /* (0x0000002c bits 0) field CSU_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_MSK,
                                                            SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 0) field CSU_INT_PCI_E of register PMC_SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_MSK) >> SFI51_RX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_set_to_clear( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_set_to_clear( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_set_to_clear", value );

    /* (0x00000004 bits 1) field TX_SFI_LOS_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_MSK,
                                                                    SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_OFF,
                                                                    value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field TX_SFI_LOS_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_MSK) >> SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_poll", value );

    /* (0x00000004 bits 1) field TX_SFI_LOS_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    return sfi51_rx_18x_serdes_glue_reg_INT_REG_poll( b_ptr,
                                                      h_ptr,
                                                      SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_MSK,
                                                      (value<<SFI51_RX_18X_GLUE_REG_INT_REG_BIT_TX_SFI_LOS_I_OFF),
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
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_set", value );

    /* (0x00000008 bits 1) field TX_SFI_LOS_E of register PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set( b_ptr,
                                                       h_ptr,
                                                       SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_TX_SFI_LOS_E_MSK,
                                                       SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_TX_SFI_LOS_E_OFF,
                                                       value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field TX_SFI_LOS_E of register PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_TX_SFI_LOS_E_MSK) >> SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_TX_SFI_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_get", value );

    return value;
}
static INLINE void sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_set( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_set", value );

    /* (0x00000008 bits 0) field FUSE_FAIL_OUT_E of register PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG */
    sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set( b_ptr,
                                                       h_ptr,
                                                       SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_FUSE_FAIL_OUT_E_MSK,
                                                       SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_FUSE_FAIL_OUT_E_OFF,
                                                       value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field FUSE_FAIL_OUT_E of register PMC_SFI51_RX_18X_GLUE_REG_INT_EN_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_FUSE_FAIL_OUT_E_MSK) >> SFI51_RX_18X_GLUE_REG_INT_EN_REG_BIT_FUSE_FAIL_OUT_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_set_to_clear( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_set_to_clear( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_set_to_clear", value );

    /* (0x00000004 bits 0) field FUSE_FAIL_OUT_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_MSK,
                                                                    SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_OFF,
                                                                    value);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field FUSE_FAIL_OUT_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_MSK) >> SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_poll", value );

    /* (0x00000004 bits 0) field FUSE_FAIL_OUT_I of register PMC_SFI51_RX_18X_GLUE_REG_INT_REG */
    return sfi51_rx_18x_serdes_glue_reg_INT_REG_poll( b_ptr,
                                                      h_ptr,
                                                      SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_MSK,
                                                      (value<<SFI51_RX_18X_GLUE_REG_INT_REG_BIT_FUSE_FAIL_OUT_I_OFF),
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
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 1) field TX_SFI_LOS_V of register PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_TX_SFI_LOS_V_MSK) >> SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_TX_SFI_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                     sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_poll", value );

    /* (0x0000000c bits 1) field TX_SFI_LOS_V of register PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG */
    return sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_TX_SFI_LOS_V_MSK,
                                                           (value<<SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_TX_SFI_LOS_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 0) field FUSE_FAIL_OUT_V of register PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_FUSE_FAIL_OUT_V_MSK) >> SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_FUSE_FAIL_OUT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_poll( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                        sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_poll", value );

    /* (0x0000000c bits 0) field FUSE_FAIL_OUT_V of register PMC_SFI51_RX_18X_GLUE_REG_INT_STAT_REG */
    return sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll( b_ptr,
                                                           h_ptr,
                                                           SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_FUSE_FAIL_OUT_V_MSK,
                                                           (value<<SFI51_RX_18X_GLUE_REG_INT_STAT_REG_BIT_FUSE_FAIL_OUT_V_OFF),
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
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_RD_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_RD_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 23:16) field RAMS_DATA_RD of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_MSK) >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RAMS_DATA_RD_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_rx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get", stop_bit, 7 );
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
    /* (0x00000018 bits 23:16) field RAMS_DATA_RD of register PMC_SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_MSK)
                  >> SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_MSK, SFI51_RX_18X_GLUE_REG_EFUSE_INT_CFG_REG_BIT_RAMS_DATA_RD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_17_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_17_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 17) field LOS_17 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_17_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_17_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_17_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_16_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_16_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 16) field LOS_16 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_16_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_16_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_16_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_15_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_15_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 15) field LOS_15 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_15_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_15_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_15_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_14_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_14_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 14) field LOS_14 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_14_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_14_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_14_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_13_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_13_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 13) field LOS_13 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_13_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_13_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_13_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_12_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_12_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 12) field LOS_12 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_12_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_12_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_12_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_11_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_11_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 11) field LOS_11 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_11_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_11_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_11_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_10_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_10_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 10) field LOS_10 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_10_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_10_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_10_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_9_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_9_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 9) field LOS_9 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_9_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_9_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_9_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_8_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_8_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 8) field LOS_8 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_8_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_8_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_8_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_7_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_7_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 7) field LOS_7 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_7_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_7_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_7_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_6_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_6_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 6) field LOS_6 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_6_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_6_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_6_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_5_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_5_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 5) field LOS_5 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_5_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_5_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_5_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_4_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_4_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 4) field LOS_4 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_4_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_4_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_4_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_3_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_3_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 3) field LOS_3 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_3_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_3_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_3_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_2_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_2_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 2) field LOS_2 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_2_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_2_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_2_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_1_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_1_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 1) field LOS_1 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_1_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_1_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_1_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_0_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_LOS_0_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 0) field LOS_0 of register PMC_SFI51_RX_18X_GLUE_REG_LOS_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_0_MSK) >> SFI51_RX_18X_GLUE_REG_LOS_STAT_REG_BIT_LOS_0_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_LOS_0_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 10) field RX_18X_GLUE_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_18X_GLUE_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_18X_GLUE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 9) field RX_2X_SLICE8_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE8_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE8_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 8) field RX_2X_SLICE7_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE7_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE7_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 7) field RX_2X_SLICE6_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE6_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE6_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 6) field RX_2X_SLICE5_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE5_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE5_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 5) field RX_2X_SLICE4_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE4_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE4_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 4) field RX_2X_SLICE3_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE3_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE3_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 3) field RX_2X_SLICE2_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE2_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE2_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 2) field RX_2X_SLICE1_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE1_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE1_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 1) field RX_2X_SLICE0_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE0_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_RX_2X_SLICE0_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_V_get( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 0) field CSU_INT_V of register PMC_SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_CSU_INT_V_MSK) >> SFI51_RX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_CSU_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_rx_18x_serdes_glue_field_CSU_INT_V_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SFI51_RX_18X_SERDES_GLUE_IO_INLINE_H */
