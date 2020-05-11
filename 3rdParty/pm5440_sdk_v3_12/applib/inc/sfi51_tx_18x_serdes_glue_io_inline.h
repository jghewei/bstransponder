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
 *     and register accessor functions for the sfi51_tx_18x_serdes_glue block
 *****************************************************************************/
#ifndef _SFI51_TX_18X_SERDES_GLUE_IO_INLINE_H
#define _SFI51_TX_18X_SERDES_GLUE_IO_INLINE_H

#include "sfi51_d8_tx_18x.h"
#include "sfi51_tx_18x_serdes_glue_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SFI51_TX_18X_SERDES_GLUE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sfi51_tx_18x_serdes_glue
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
    sfi51_d8_tx_18x_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sfi51_tx_18x_serdes_glue_buffer_t;
static INLINE void sfi51_tx_18x_serdes_glue_buffer_init( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_buffer_init( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sfi51_tx_18x_serdes_glue_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sfi51_tx_18x_serdes_glue_buffer_flush( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_buffer_flush( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr )
{
    IOLOG( "sfi51_tx_18x_serdes_glue_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
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
static INLINE void sfi51_tx_18x_serdes_glue_reg_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_tx_18x_serdes_glue_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 unused_mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_tx_18x_serdes_glue_action_on_write_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 mem_type,
                                                                       UINT32 reg,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_action_on_write_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_tx_18x_serdes_glue_burst_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 len,
                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_burst_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void sfi51_tx_18x_serdes_glue_burst_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 len,
                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_burst_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sfi51_tx_18x_serdes_glue_poll( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 mem_type,
                                                                  UINT32 reg,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_tx_18x_serdes_glue_poll( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
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
 *  register access functions for sfi51_tx_18x_serdes_glue
 * ==================================================================================
 */

static INLINE void sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write", value );
    sfi51_tx_18x_serdes_glue_reg_write( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG,
                                        value);
}

static INLINE void sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set", mask, ofs, value );
    sfi51_tx_18x_serdes_glue_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_CONFIG,
                                        PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG,
                                        mask,
                                        PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_UNUSED_MASK,
                                        ofs,
                                        value);

}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_CONFIG,
                                                   PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sfi51_tx_18x_serdes_glue_reg_read( b_ptr,
                                                   h_ptr,
                                                   MEM_TYPE_STATUS,
                                                   PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set", value );

    /* (0x00000000 bits 3) field RXS_CTRL_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_RXS_CTRL_RSTB_MSK,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_RXS_CTRL_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field RXS_CTRL_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_RXS_CTRL_RSTB_MSK) >> SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_RXS_CTRL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set", value );

    /* (0x00000000 bits 2) field LOOPTIME_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_LOOPTIME_RSTB_MSK,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_LOOPTIME_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field LOOPTIME_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_LOOPTIME_RSTB_MSK) >> SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_LOOPTIME_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set", value );

    /* (0x00000000 bits 1) field DIGITAL_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field DIGITAL_RSTB of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK) >> SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_SW_RESET_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_SW_RESET_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_SW_RESET_set", value );

    /* (0x00000000 bits 0) field SW_RESET of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set( b_ptr,
                                                          h_ptr,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_MSK,
                                                          SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_OFF,
                                                          value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SW_RESET_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SW_RESET_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SW_RESET of register PMC_SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL */
    reg_value = sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_MSK) >> SFI51_TX_18X_GLUE_REG_SW_RESET_CTRL_BIT_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_SW_RESET_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_DSM_DENOM_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_DSM_DENOM_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_DENOM_set", value, 16383);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_DENOM_set", value );

    /* (0x00000004 bits 29:16) field DSM_DENOM of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DSM_DENOM_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DSM_DENOM_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 29:16) field DSM_DENOM of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_DENOM_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set", stop_bit, 13 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 29:16) field DSM_DENOM of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
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
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000004 bits 29:16) field DSM_DENOM of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000004 bits 29:16) field DSM_DENOM of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_DENOM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_set", value );

    /* (0x00000004 bits 15:12) field LOOPTIME_FREEZE_CNT of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 15:12) field LOOPTIME_FREEZE_CNT of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 15:12) field LOOPTIME_FREEZE_CNT of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
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
        /* (0x00000004 bits 15:12) field LOOPTIME_FREEZE_CNT of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get", stop_bit, 3 );
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
    /* (0x00000004 bits 15:12) field LOOPTIME_FREEZE_CNT of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_set", value );

    /* (0x00000004 bits 8) field LOOPTIME_FREEZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field LOOPTIME_FREEZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_LOOPTIME_FREEZE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_set", value );

    /* (0x00000004 bits 7:6) field RCLK_WIDTH of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 7:6) field RCLK_WIDTH of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 7:6) field RCLK_WIDTH of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
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
        /* (0x00000004 bits 7:6) field RCLK_WIDTH of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get", stop_bit, 1 );
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
    /* (0x00000004 bits 7:6) field RCLK_WIDTH of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_RCLK_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_DSM_TYPE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_DSM_TYPE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_TYPE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_TYPE_set", value );

    /* (0x00000004 bits 3:2) field DSM_TYPE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DSM_TYPE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DSM_TYPE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3:2) field DSM_TYPE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_DSM_TYPE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 3:2) field DSM_TYPE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
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
        /* (0x00000004 bits 3:2) field DSM_TYPE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get", stop_bit, 1 );
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
    /* (0x00000004 bits 3:2) field DSM_TYPE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_DSM_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set", value );

    /* (0x00000004 bits 0) field PMM_ENABLE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_PMM_ENABLE_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_PMM_ENABLE_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field PMM_ENABLE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_PMM_ENABLE_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_1_BIT_PMM_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set", value );

    /* (0x00000008 bits 31:16) field PKT_PERIOD of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 31:16) field PKT_PERIOD of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 31:16) field PKT_PERIOD of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
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
        /* (0x00000008 bits 31:16) field PKT_PERIOD of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get", stop_bit, 15 );
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
    /* (0x00000008 bits 31:16) field PKT_PERIOD of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_INC_STEP_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_INC_STEP_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_INC_STEP_set", value, 31);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_INC_STEP_set", value );

    /* (0x00000008 bits 13:9) field INC_STEP of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_INC_STEP_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_INC_STEP_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 13:9) field INC_STEP of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_INC_STEP_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 13:9) field INC_STEP of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
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
        /* (0x00000008 bits 13:9) field INC_STEP of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get", stop_bit, 4 );
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
    /* (0x00000008 bits 13:9) field INC_STEP of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_INC_STEP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set", value, 511);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set", value );

    /* (0x00000008 bits 8:0) field PKT_SIZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_MSK,
                                                                 SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF,
                                                                 value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PKT_SIZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_PKT_SIZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 8:0) field PKT_SIZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_MSK) >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_PKT_SIZE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 8:0) field PKT_SIZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
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
        /* (0x00000008 bits 8:0) field PKT_SIZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
        sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF + subfield_offset),
                                                                     SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get", stop_bit, 8 );
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
    /* (0x00000008 bits 8:0) field PKT_SIZE of register PMC_SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_MSK)
                  >> SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_MSK, SFI51_TX_18X_GLUE_REG_LOOPTIMING_CFG_REG_2_BIT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set", value );

    /* (0x0000000c bits 10) field SQUELCH_OVR of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 10) field SQUELCH_OVR of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set", value );

    /* (0x0000000c bits 9:8) field SQUELCH_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 9:8) field SQUELCH_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 9:8) field SQUELCH_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
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
        /* (0x0000000c bits 9:8) field SQUELCH_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
        sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF + subfield_offset),
                                                                  SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get", stop_bit, 1 );
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
    /* (0x0000000c bits 9:8) field SQUELCH_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_MSK)
                  >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_MSK, SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_SQUELCH_OVR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_set", value );

    /* (0x0000000c bits 6) field GEN_FORCE_NOMINAL of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FORCE_NOMINAL_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FORCE_NOMINAL_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 6) field GEN_FORCE_NOMINAL of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FORCE_NOMINAL_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FORCE_NOMINAL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_set", value );

    /* (0x0000000c bits 5) field GEN_HOLDOVER of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_HOLDOVER_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_HOLDOVER_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 5) field GEN_HOLDOVER of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_HOLDOVER_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_HOLDOVER_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_set", value );

    /* (0x0000000c bits 4) field GEN_FREEZE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FREEZE_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FREEZE_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 4) field GEN_FREEZE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FREEZE_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_GEN_FREEZE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_set", value );

    /* (0x0000000c bits 3:2) field RXS_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 3:2) field RXS_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 3:2) field RXS_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
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
        /* (0x0000000c bits 3:2) field RXS_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
        sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF + subfield_offset),
                                                                  SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get", stop_bit, 1 );
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
    /* (0x0000000c bits 3:2) field RXS_OVR_MODE of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_MSK)
                  >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_MSK, SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_OVR_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_OVR_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_set", value );

    /* (0x0000000c bits 1) field RXS_OVR of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 1) field RXS_OVR of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_OVR_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_OVR_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_set", value );

    /* (0x0000000c bits 0) field RXS_CTRL_EN of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_CTRL_EN_MSK,
                                                              SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_CTRL_EN_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 0) field RXS_CTRL_EN of register PMC_SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2 */
    reg_value = sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_CTRL_EN_MSK) >> SFI51_TX_18X_GLUE_REG_RXS_CTRLLER_CFG_2_BIT_RXS_CTRL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_set", value );

    /* (0x00000010 bits 28) field LI_RX_OOF_LOF_POL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_LI_RX_OOF_LOF_POL_MSK,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_LI_RX_OOF_LOF_POL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 28) field LI_RX_OOF_LOF_POL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_LI_RX_OOF_LOF_POL_MSK) >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_LI_RX_OOF_LOF_POL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_POL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_RXS_POL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_POL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_POL_set", value );

    /* (0x00000010 bits 27) field RXS_POL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_RXS_POL_MSK,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_RXS_POL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_POL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_RXS_POL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 27) field RXS_POL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_RXS_POL_MSK) >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_RXS_POL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_RXS_POL_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set", value );

    /* (0x00000010 bits 24:22) field REFCLK_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 24:22) field REFCLK_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK) >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 24:22) field REFCLK_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
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
        /* (0x00000010 bits 24:22) field REFCLK_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
        sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF + subfield_offset),
                                                               SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get", stop_bit, 2 );
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
    /* (0x00000010 bits 24:22) field REFCLK_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK)
                  >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_MSK, SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_REFCLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set", value );

    /* (0x00000010 bits 20) field CKCTL_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_CKCTL_SEL_MSK,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_CKCTL_SEL_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 20) field CKCTL_SEL of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_CKCTL_SEL_MSK) >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_CKCTL_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_set", value );

    /* (0x00000010 bits 19:0) field DCK_DATA_PATT of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_MSK,
                                                           SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF,
                                                           value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 19:0) field DCK_DATA_PATT of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_MSK) >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 19:0) field DCK_DATA_PATT of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000010 bits 19:0) field DCK_DATA_PATT of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
        sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF + subfield_offset),
                                                               SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000010 bits 19:0) field DCK_DATA_PATT of register PMC_SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read( b_ptr,
                                                                  h_ptr);
    field_value = (reg_value & SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_MSK)
                  >> SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_MSK, SFI51_TX_18X_GLUE_REG_GLUE_LOGIC_CFG_BIT_DCK_DATA_PATT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_set", value );

    /* (0x00000014 bits 9) field TX_2X_SLICE8_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 9) field TX_2X_SLICE8_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_set", value );

    /* (0x00000014 bits 8) field TX_2X_SLICE7_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 8) field TX_2X_SLICE7_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_set", value );

    /* (0x00000014 bits 7) field TX_2X_SLICE6_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 7) field TX_2X_SLICE6_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_set", value );

    /* (0x00000014 bits 6) field TX_2X_SLICE5_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 6) field TX_2X_SLICE5_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_set", value );

    /* (0x00000014 bits 5) field TX_2X_SLICE4_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 5) field TX_2X_SLICE4_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_set", value );

    /* (0x00000014 bits 4) field TX_2X_SLICE3_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 4) field TX_2X_SLICE3_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_set", value );

    /* (0x00000014 bits 3) field TX_2X_SLICE2_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 3) field TX_2X_SLICE2_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_set", value );

    /* (0x00000014 bits 2) field TX_2X_SLICE1_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 2) field TX_2X_SLICE1_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_set", value );

    /* (0x00000014 bits 1) field TX_2X_SLICE0_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 1) field TX_2X_SLICE0_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_set", value );

    /* (0x00000014 bits 0) field CSU_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_MSK,
                                                              SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 0) field CSU_INT_DEV_E of register PMC_SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_MSK) >> SFI51_TX_18X_GLUE_REG_DEVICE_INT_EN_REG_BIT_CSU_INT_DEV_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_set", value );

    /* (0x00000018 bits 9) field TX_2X_SLICE8_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE8_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE8_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 9) field TX_2X_SLICE8_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE8_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE8_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_set", value );

    /* (0x00000018 bits 8) field TX_2X_SLICE7_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE7_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE7_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 8) field TX_2X_SLICE7_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE7_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE7_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_set", value );

    /* (0x00000018 bits 7) field TX_2X_SLICE6_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE6_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE6_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 7) field TX_2X_SLICE6_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE6_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE6_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_set", value );

    /* (0x00000018 bits 6) field TX_2X_SLICE5_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE5_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE5_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 6) field TX_2X_SLICE5_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE5_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE5_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_set", value );

    /* (0x00000018 bits 5) field TX_2X_SLICE4_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE4_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE4_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 5) field TX_2X_SLICE4_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE4_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE4_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_set", value );

    /* (0x00000018 bits 4) field TX_2X_SLICE3_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE3_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE3_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 4) field TX_2X_SLICE3_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE3_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE3_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_set", value );

    /* (0x00000018 bits 3) field TX_2X_SLICE2_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE2_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE2_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 3) field TX_2X_SLICE2_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE2_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE2_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_set", value );

    /* (0x00000018 bits 2) field TX_2X_SLICE1_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE1_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE1_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 2) field TX_2X_SLICE1_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE1_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE1_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_set", value );

    /* (0x00000018 bits 1) field TX_2X_SLICE0_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE0_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE0_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 1) field TX_2X_SLICE0_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE0_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_TX_2X_SLICE0_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_set", value );

    /* (0x00000018 bits 0) field CSU_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 0) field CSU_INT_MIP_E of register PMC_SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_MSK) >> SFI51_TX_18X_GLUE_REG_MIPS_INT_EN_REG_BIT_CSU_INT_MIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_set", value );

    /* (0x0000001c bits 9) field TX_2X_SLICE8_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 9) field TX_2X_SLICE8_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE8_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_set", value );

    /* (0x0000001c bits 8) field TX_2X_SLICE7_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 8) field TX_2X_SLICE7_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE7_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_set", value );

    /* (0x0000001c bits 7) field TX_2X_SLICE6_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 7) field TX_2X_SLICE6_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE6_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_set", value );

    /* (0x0000001c bits 6) field TX_2X_SLICE5_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 6) field TX_2X_SLICE5_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE5_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_set", value );

    /* (0x0000001c bits 5) field TX_2X_SLICE4_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 5) field TX_2X_SLICE4_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE4_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_set", value );

    /* (0x0000001c bits 4) field TX_2X_SLICE3_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 4) field TX_2X_SLICE3_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE3_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_set", value );

    /* (0x0000001c bits 3) field TX_2X_SLICE2_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 3) field TX_2X_SLICE2_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE2_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_set", value );

    /* (0x0000001c bits 2) field TX_2X_SLICE1_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 2) field TX_2X_SLICE1_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE1_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_set", value );

    /* (0x0000001c bits 1) field TX_2X_SLICE0_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 1) field TX_2X_SLICE0_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_TX_2X_SLICE0_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_get", value );

    return value;
}
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_set( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_set", value );

    /* (0x0000001c bits 0) field CSU_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_MSK,
                                                            SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_OFF,
                                                            value);
}

static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 0) field CSU_INT_PCI_E of register PMC_SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_MSK) >> SFI51_TX_18X_GLUE_REG_PCIE_INT_EN_REG_BIT_CSU_INT_PCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 9) field TX_2X_SLICE8_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE8_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE8_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 8) field TX_2X_SLICE7_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE7_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE7_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 7) field TX_2X_SLICE6_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE6_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE6_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 6) field TX_2X_SLICE5_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE5_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE5_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 5) field TX_2X_SLICE4_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE4_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE4_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 4) field TX_2X_SLICE3_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE3_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE3_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 3) field TX_2X_SLICE2_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE2_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE2_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 2) field TX_2X_SLICE1_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE1_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE1_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 1) field TX_2X_SLICE0_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE0_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_TX_2X_SLICE0_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_V_get", value );

    return value;
}
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_V_get( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 0) field CSU_INT_V of register PMC_SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG */
    reg_value = sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_CSU_INT_V_MSK) >> SFI51_TX_18X_GLUE_REG_GLOBAL_INT_STAT_REG_BIT_CSU_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "sfi51_tx_18x_serdes_glue_field_CSU_INT_V_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SFI51_TX_18X_SERDES_GLUE_IO_INLINE_H */
