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
 *     and register accessor functions for the di_rcp block
 *****************************************************************************/
#ifndef _DI_RCP_IO_INLINE_H
#define _DI_RCP_IO_INLINE_H

#include "odu_rcp_loc.h"
#include "di_rcp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DI_RCP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for di_rcp
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
    odu_rcp_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} di_rcp_buffer_t;
static INLINE void di_rcp_buffer_init( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void di_rcp_buffer_init( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "di_rcp_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void di_rcp_buffer_flush( di_rcp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void di_rcp_buffer_flush( di_rcp_buffer_t *b_ptr )
{
    IOLOG( "di_rcp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 di_rcp_reg_read( di_rcp_buffer_t *b_ptr,
                                      odu_rcp_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_read( di_rcp_buffer_t *b_ptr,
                                      odu_rcp_handle_t *h_ptr,
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
static INLINE void di_rcp_reg_write( di_rcp_buffer_t *b_ptr,
                                     odu_rcp_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_write( di_rcp_buffer_t *b_ptr,
                                     odu_rcp_handle_t *h_ptr,
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

static INLINE void di_rcp_field_set( di_rcp_buffer_t *b_ptr,
                                     odu_rcp_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_set( di_rcp_buffer_t *b_ptr,
                                     odu_rcp_handle_t *h_ptr,
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

static INLINE void di_rcp_action_on_write_field_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_action_on_write_field_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
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

static INLINE void di_rcp_burst_read( di_rcp_buffer_t *b_ptr,
                                      odu_rcp_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void di_rcp_burst_read( di_rcp_buffer_t *b_ptr,
                                      odu_rcp_handle_t *h_ptr,
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

static INLINE void di_rcp_burst_write( di_rcp_buffer_t *b_ptr,
                                       odu_rcp_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void di_rcp_burst_write( di_rcp_buffer_t *b_ptr,
                                       odu_rcp_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE di_rcp_poll( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_poll( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
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
 *  register access functions for di_rcp
 * ==================================================================================
 */

static INLINE void di_rcp_reg_REG_SW_RESET_write( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_SW_RESET_write( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_SW_RESET_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_SW_RESET,
                      value);
}

static INLINE void di_rcp_reg_REG_SW_RESET_field_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_SW_RESET_field_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_SW_RESET_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_SW_RESET,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_SW_RESET_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_SW_RESET_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_SW_RESET_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_LOWPWR_write( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_LOWPWR_write( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_LOWPWR_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_LOWPWR,
                      value);
}

static INLINE void di_rcp_reg_REG_LOWPWR_field_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_LOWPWR_field_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_LOWPWR_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_LOWPWR,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_LOWPWR_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_LOWPWR_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_LOWPWR_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_LOWPWR);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_LOWPWR_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_RAM_PWR_MGMT_write( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_RAM_PWR_MGMT_write( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_RAM_PWR_MGMT_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT,
                      value);
}

static INLINE void di_rcp_reg_REG_RAM_PWR_MGMT_field_set( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_RAM_PWR_MGMT_field_set( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_RAM_PWR_MGMT_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_RAM_PWR_MGMT_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_RAM_PWR_MGMT_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_RAM_PWR_MGMT_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_GOJF_write( di_rcp_buffer_t *b_ptr,
                                              odu_rcp_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_GOJF_write( di_rcp_buffer_t *b_ptr,
                                              odu_rcp_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_GOJF_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_GOJF,
                      value);
}

static INLINE void di_rcp_reg_REG_GOJF_field_set( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_GOJF_field_set( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_GOJF_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_GOJF,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_GOJF_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_GOJF_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_GOJF_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_GOJF);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_GOJF_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_TX_EN_write( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_TX_EN_write( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_TX_EN_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_TX_EN_field_set( di_rcp_buffer_t *b_ptr,
                                                       odu_rcp_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_TX_EN_field_set( di_rcp_buffer_t *b_ptr,
                                                       odu_rcp_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_TX_EN_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_TX_EN_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_TX_EN_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_TX_EN_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_MAC_DA1_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_DA1_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_MAC_DA1_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_MAC_DA1_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_DA1_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_MAC_DA1_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_DA1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_DA1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_MAC_DA1_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_MAC_DA2_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_DA2_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_MAC_DA2_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_MAC_DA2_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_DA2_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_MAC_DA2_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_DA2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_DA2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_MAC_DA2_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_MAC_SA1_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_SA1_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_MAC_SA1_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_MAC_SA1_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_SA1_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_MAC_SA1_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_SA1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_SA1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_MAC_SA1_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_MAC_SA2_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_SA2_write( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_MAC_SA2_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_MAC_SA2_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_MAC_SA2_field_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_MAC_SA2_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_SA2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_MAC_SA2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_MAC_SA2_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_ETH_ETYPE_BASE_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_ETYPE_BASE_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_ETH_ETYPE_BASE_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE,
                      value);
}

static INLINE void di_rcp_reg_REG_ETH_ETYPE_BASE_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_ETH_ETYPE_BASE_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_ETH_ETYPE_BASE_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_ETH_ETYPE_BASE_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_ETH_ETYPE_BASE_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_ETH_ETYPE_BASE_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_STG_1_N_D_RCOHM_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_1_N_D_RCOHM_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_STG_1_N_D_RCOHM_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM,
                      value);
}

static INLINE void di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_STG_1_N_D_RCOHM_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_STG_1_N_D_RCOHM_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_STG_1_N_D_RCOHM_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM1_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM1_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM1_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1,
                      value);
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM1_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM1_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM1_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM1_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_STG_2_D_RCOHM1_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM2_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM2_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM2_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2,
                      value);
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM2_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM2_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM2_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM2_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_STG_2_D_RCOHM2_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM3_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM3_write( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM3_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3,
                      value);
}

static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM3_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_STG_2_D_RCOHM3_field_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_STG_2_D_RCOHM3_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM3_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_STG_2_D_RCOHM3_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_STG_2_D_RCOHM3_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_REG_PMON_TX_TRIG_write( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_PMON_TX_TRIG_write( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_PMON_TX_TRIG_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG,
                      value);
}

static INLINE void di_rcp_reg_REG_PMON_TX_TRIG_field_set( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_PMON_TX_TRIG_field_set( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_PMON_TX_TRIG_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG,
                      mask,
                      PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_REG_PMON_TX_TRIG_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_PMON_TX_TRIG_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_PMON_TX_TRIG_read", reg_value);
    return reg_value;
}

static INLINE void di_rcp_reg_GEN_IRQ_E_write( di_rcp_buffer_t *b_ptr,
                                               odu_rcp_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_GEN_IRQ_E_write( di_rcp_buffer_t *b_ptr,
                                               odu_rcp_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_GEN_IRQ_E_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_GEN_IRQ_E,
                      value);
}

static INLINE void di_rcp_reg_GEN_IRQ_E_field_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_GEN_IRQ_E_field_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_GEN_IRQ_E_field_set", mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_GEN_IRQ_E,
                      mask,
                      PMC_DI_ODU_RCP_REG_GEN_IRQ_E_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_GEN_IRQ_E_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_GEN_IRQ_E_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_GEN_IRQ_E);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_GEN_IRQ_E_read", reg_value);
    return reg_value;
}

static INLINE UINT32 di_rcp_reg_IRQ_SUMMARY_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_IRQ_SUMMARY_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_IRQ_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_IRQ_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_reg_IRQ_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_reg_IRQ_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "di_rcp_reg_IRQ_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return di_rcp_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DI_ODU_RCP_REG_IRQ_SUMMARY,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void di_rcp_reg_REG_GEN_IRQ_I_write( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_GEN_IRQ_I_write( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_REG_GEN_IRQ_I_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I,
                      value);
}

static INLINE void di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set( di_rcp_buffer_t *b_ptr,
                                                                       odu_rcp_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set( di_rcp_buffer_t *b_ptr,
                                                                       odu_rcp_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set", mask, ofs, value );
    di_rcp_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 di_rcp_reg_REG_GEN_IRQ_I_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_REG_GEN_IRQ_I_read( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I);

    IOLOG( "%s -> 0x%08x;", "di_rcp_reg_REG_GEN_IRQ_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_reg_REG_GEN_IRQ_I_poll( di_rcp_buffer_t *b_ptr,
                                                                  odu_rcp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_reg_REG_GEN_IRQ_I_poll( di_rcp_buffer_t *b_ptr,
                                                                  odu_rcp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "di_rcp_reg_REG_GEN_IRQ_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return di_rcp_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void di_rcp_reg_OTN_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_OTN_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_OTN_PMON_COPY_array_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_OTN_PMON_COPY(N),
                      value);
}

static INLINE void di_rcp_reg_OTN_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_OTN_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_OTN_PMON_COPY_array_field_set", N, mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_OTN_PMON_COPY(N),
                      mask,
                      PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_OTN_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_OTN_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_OTN_PMON_COPY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "di_rcp_reg_OTN_PMON_COPY_array_read", reg_value, N);
    return reg_value;
}

static INLINE void di_rcp_reg_PRBS_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_PRBS_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_PRBS_PMON_COPY_array_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY(N),
                      value);
}

static INLINE void di_rcp_reg_PRBS_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_PRBS_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_PRBS_PMON_COPY_array_field_set", N, mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY(N),
                      mask,
                      PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_PRBS_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_PRBS_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "di_rcp_reg_PRBS_PMON_COPY_array_read", reg_value, N);
    return reg_value;
}

static INLINE void di_rcp_reg_SDH_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_SDH_PMON_COPY_array_write( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "di_rcp_reg_SDH_PMON_COPY_array_write", value );
    di_rcp_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_SDH_PMON_COPY(N),
                      value);
}

static INLINE void di_rcp_reg_SDH_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_reg_SDH_PMON_COPY_array_field_set( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "di_rcp_reg_SDH_PMON_COPY_array_field_set", N, mask, ofs, value );
    di_rcp_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_DI_ODU_RCP_REG_SDH_PMON_COPY(N),
                      mask,
                      PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 di_rcp_reg_SDH_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_reg_SDH_PMON_COPY_array_read( di_rcp_buffer_t *b_ptr,
                                                          odu_rcp_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = di_rcp_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_DI_ODU_RCP_REG_SDH_PMON_COPY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "di_rcp_reg_SDH_PMON_COPY_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void di_rcp_field_REG_SOFT_SW_RESET_set( di_rcp_buffer_t *b_ptr,
                                                       odu_rcp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_SOFT_SW_RESET_set( di_rcp_buffer_t *b_ptr,
                                                       odu_rcp_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_SOFT_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_SOFT_SW_RESET_set", value );

    /* (0x00008000 bits 1) field REG_SOFT_SW_RESET of register PMC_DI_ODU_RCP_REG_REG_SW_RESET */
    di_rcp_reg_REG_SW_RESET_field_set( b_ptr,
                                       h_ptr,
                                       DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_MSK,
                                       DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_OFF,
                                       value);
}

static INLINE UINT32 di_rcp_field_REG_SOFT_SW_RESET_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_SOFT_SW_RESET_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008000 bits 1) field REG_SOFT_SW_RESET of register PMC_DI_ODU_RCP_REG_REG_SW_RESET */
    reg_value = di_rcp_reg_REG_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_MSK) >> DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_SOFT_SW_RESET_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_SW_RESET_set( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_SW_RESET_set( di_rcp_buffer_t *b_ptr,
                                                  odu_rcp_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_SW_RESET_set", value );

    /* (0x00008000 bits 0) field REG_SW_RESET of register PMC_DI_ODU_RCP_REG_REG_SW_RESET */
    di_rcp_reg_REG_SW_RESET_field_set( b_ptr,
                                       h_ptr,
                                       DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_MSK,
                                       DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_OFF,
                                       value);
}

static INLINE UINT32 di_rcp_field_REG_SW_RESET_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_SW_RESET_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008000 bits 0) field REG_SW_RESET of register PMC_DI_ODU_RCP_REG_REG_SW_RESET */
    reg_value = di_rcp_reg_REG_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_MSK) >> DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_SW_RESET_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_LOWPWR_set( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_LOWPWR_set( di_rcp_buffer_t *b_ptr,
                                                odu_rcp_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_LOWPWR_set", value );

    /* (0x00008004 bits 0) field REG_LOWPWR of register PMC_DI_ODU_RCP_REG_REG_LOWPWR */
    di_rcp_reg_REG_LOWPWR_field_set( b_ptr,
                                     h_ptr,
                                     DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_MSK,
                                     DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_OFF,
                                     value);
}

static INLINE UINT32 di_rcp_field_REG_LOWPWR_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_LOWPWR_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008004 bits 0) field REG_LOWPWR of register PMC_DI_ODU_RCP_REG_REG_LOWPWR */
    reg_value = di_rcp_reg_REG_LOWPWR_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_MSK) >> DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_LOWPWR_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_RAM_LOWPWR_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_RAM_LOWPWR_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_RAM_LOWPWR_set", value );

    /* (0x0000800c bits 0) field REG_RAM_LOWPWR of register PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT */
    di_rcp_reg_REG_RAM_PWR_MGMT_field_set( b_ptr,
                                           h_ptr,
                                           DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_MSK,
                                           DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_OFF,
                                           value);
}

static INLINE UINT32 di_rcp_field_REG_RAM_LOWPWR_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_RAM_LOWPWR_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000800c bits 0) field REG_RAM_LOWPWR of register PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT */
    reg_value = di_rcp_reg_REG_RAM_PWR_MGMT_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_MSK) >> DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_FIFO_MIN_SLOT_FREE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_FIFO_MIN_SLOT_FREE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_FIFO_MIN_SLOT_FREE_set", value, 63);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_FIFO_MIN_SLOT_FREE_set", value );

    /* (0x00008014 bits 21:16) field REG_FIFO_MIN_SLOT_FREE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_FIFO_MIN_SLOT_FREE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_FIFO_MIN_SLOT_FREE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 21:16) field REG_FIFO_MIN_SLOT_FREE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_FIFO_MIN_SLOT_FREE_get", value );

    return value;
}
static INLINE void di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set( di_rcp_buffer_t *b_ptr,
                                                                  odu_rcp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set( di_rcp_buffer_t *b_ptr,
                                                                  odu_rcp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008014 bits 21:16) field REG_FIFO_MIN_SLOT_FREE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
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
        /* (0x00008014 bits 21:16) field REG_FIFO_MIN_SLOT_FREE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
        di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF + subfield_offset),
                                       DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get( di_rcp_buffer_t *b_ptr,
                                                                    odu_rcp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get( di_rcp_buffer_t *b_ptr,
                                                                    odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get", stop_bit, 5 );
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
    /* (0x00008014 bits 21:16) field REG_FIFO_MIN_SLOT_FREE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_MSK)
                  >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_MSK, DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_field_REG_FIFO_MIN_LVL_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_FIFO_MIN_LVL_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_FIFO_MIN_LVL_set", value, 63);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_FIFO_MIN_LVL_set", value );

    /* (0x00008014 bits 13:8) field REG_FIFO_MIN_LVL of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_FIFO_MIN_LVL_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_FIFO_MIN_LVL_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 13:8) field REG_FIFO_MIN_LVL of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_FIFO_MIN_LVL_get", value );

    return value;
}
static INLINE void di_rcp_field_range_REG_FIFO_MIN_LVL_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_REG_FIFO_MIN_LVL_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_FIFO_MIN_LVL_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_FIFO_MIN_LVL_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_FIFO_MIN_LVL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008014 bits 13:8) field REG_FIFO_MIN_LVL of register PMC_DI_ODU_RCP_REG_REG_GOJF */
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
        /* (0x00008014 bits 13:8) field REG_FIFO_MIN_LVL of register PMC_DI_ODU_RCP_REG_REG_GOJF */
        di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF + subfield_offset),
                                       DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_REG_FIFO_MIN_LVL_get( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_REG_FIFO_MIN_LVL_get( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_FIFO_MIN_LVL_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_FIFO_MIN_LVL_get", stop_bit, 5 );
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
    /* (0x00008014 bits 13:8) field REG_FIFO_MIN_LVL of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_MSK)
                  >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_MSK, DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_FIFO_MIN_LVL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_TYPE_IN_PDU_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_TYPE_IN_PDU_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_TYPE_IN_PDU_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_TYPE_IN_PDU_set", value );

    /* (0x00008014 bits 4) field REG_ETH_TYPE_IN_PDU of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_TYPE_IN_PDU_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_TYPE_IN_PDU_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 4) field REG_ETH_TYPE_IN_PDU of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_TYPE_IN_PDU_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_RX_10B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_RX_10B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_RX_10B_SWIZZLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_RX_10B_SWIZZLE_set", value );

    /* (0x00008014 bits 2) field REG_ETH_RX_10B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_RX_10B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_RX_10B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 2) field REG_ETH_RX_10B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_RX_10B_SWIZZLE_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set", value );

    /* (0x00008014 bits 1) field REG_ETH_TX_10B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_TX_10B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_TX_10B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 1) field REG_ETH_TX_10B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_TX_10B_SWIZZLE_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_TX_8B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_TX_8B_SWIZZLE_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_TX_8B_SWIZZLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_TX_8B_SWIZZLE_set", value );

    /* (0x00008014 bits 0) field REG_ETH_TX_8B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    di_rcp_reg_REG_GOJF_field_set( b_ptr,
                                   h_ptr,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_MSK,
                                   DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_OFF,
                                   value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_TX_8B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_TX_8B_SWIZZLE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008014 bits 0) field REG_ETH_TX_8B_SWIZZLE of register PMC_DI_ODU_RCP_REG_REG_GOJF */
    reg_value = di_rcp_reg_REG_GOJF_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_MSK) >> DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_TX_8B_SWIZZLE_get", value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_TX_EN_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_TX_EN_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 268435455)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_TX_EN_set", value, 268435455);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_TX_EN_set", value );

    /* (0x00008020 bits 28:1) field REG_ETH_TX_EN of register PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN */
    di_rcp_reg_REG_ETH_TX_EN_field_set( b_ptr,
                                        h_ptr,
                                        DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK,
                                        DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF,
                                        value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_TX_EN_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_TX_EN_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008020 bits 28:1) field REG_ETH_TX_EN of register PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN */
    reg_value = di_rcp_reg_REG_ETH_TX_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK) >> DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_TX_EN_get", value );

    return value;
}
static INLINE void di_rcp_field_range_REG_ETH_TX_EN_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_REG_ETH_TX_EN_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_ETH_TX_EN_set", stop_bit, start_bit );
    if (stop_bit > 27) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_ETH_TX_EN_set", stop_bit, 27 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_ETH_TX_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008020 bits 28:1) field REG_ETH_TX_EN of register PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN */
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
        if (stop_bit < 27) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 27;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00008020 bits 28:1) field REG_ETH_TX_EN of register PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN */
        di_rcp_reg_REG_ETH_TX_EN_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF + subfield_offset),
                                            DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_REG_ETH_TX_EN_get( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_REG_ETH_TX_EN_get( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_ETH_TX_EN_get", stop_bit, start_bit );
    if (stop_bit > 27) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_ETH_TX_EN_get", stop_bit, 27 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 27) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 27;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00008020 bits 28:1) field REG_ETH_TX_EN of register PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN */
    reg_value = di_rcp_reg_REG_ETH_TX_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK)
                  >> DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK, DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_ETH_TX_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_lfield_REG_ETH_MAC_DA_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_ETH_MAC_DA_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "di_rcp_lfield_REG_ETH_MAC_DA_set", value[1] , value[0] );

    /* (0x00008024 bits 31:0) bits 0:31 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1 */
    di_rcp_reg_REG_ETH_MAC_DA1_field_set( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_MSK,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF,
                                          value[0]);

    /* (0x00008028 bits 15:0) bits 32:47 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2 */
    di_rcp_reg_REG_ETH_MAC_DA2_field_set( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_MSK,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF,
                                          value[1]);
}

static INLINE void di_rcp_lfield_REG_ETH_MAC_DA_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_ETH_MAC_DA_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00008024 bits 31:0) bits 0:31 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1 */
    reg_value = di_rcp_reg_REG_ETH_MAC_DA1_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_MSK) >> DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF;
    value[0] |= field_value;

    /* (0x00008028 bits 15:0) bits 32:47 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2 */
    reg_value = di_rcp_reg_REG_ETH_MAC_DA2_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_MSK) >> DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "di_rcp_lfield_REG_ETH_MAC_DA_get", value[1] , value[0] );

}
static INLINE void di_rcp_lfield_range_REG_ETH_MAC_DA_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_range_REG_ETH_MAC_DA_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_ETH_MAC_DA_set", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_ETH_MAC_DA_set", stop_bit, 47 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_ETH_MAC_DA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008024 bits 31:0) bits 0:31 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1 */
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
        /* (0x00008024 bits 31:0) bits 0:31 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1 */
        di_rcp_reg_REG_ETH_MAC_DA1_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF + subfield_offset),
                                              DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF + subfield_offset,
                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008028 bits 15:0) bits 32:47 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2 */
    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00008028 bits 15:0) bits 32:47 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2 */
        di_rcp_reg_REG_ETH_MAC_DA2_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF + subfield_offset),
                                              DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_lfield_range_REG_ETH_MAC_DA_get( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_lfield_range_REG_ETH_MAC_DA_get( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_ETH_MAC_DA_get", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_ETH_MAC_DA_get", stop_bit, 47 );
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
    /* (0x00008024 bits 31:0) bits 0:31 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1 */
        reg_value = di_rcp_reg_REG_ETH_MAC_DA1_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_MSK)
                      >> DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_MSK, DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00008028 bits 15:0) bits 32:47 use field REG_ETH_MAC_DA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2 */
        reg_value = di_rcp_reg_REG_ETH_MAC_DA2_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_MSK)
                      >> DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_MSK, DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_ETH_MAC_DA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_lfield_REG_ETH_MAC_SA_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_ETH_MAC_SA_set( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "di_rcp_lfield_REG_ETH_MAC_SA_set", value[1] , value[0] );

    /* (0x0000802c bits 31:0) bits 0:31 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1 */
    di_rcp_reg_REG_ETH_MAC_SA1_field_set( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_MSK,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF,
                                          value[0]);

    /* (0x00008030 bits 15:0) bits 32:47 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2 */
    di_rcp_reg_REG_ETH_MAC_SA2_field_set( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_MSK,
                                          DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF,
                                          value[1]);
}

static INLINE void di_rcp_lfield_REG_ETH_MAC_SA_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_ETH_MAC_SA_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000802c bits 31:0) bits 0:31 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1 */
    reg_value = di_rcp_reg_REG_ETH_MAC_SA1_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_MSK) >> DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF;
    value[0] |= field_value;

    /* (0x00008030 bits 15:0) bits 32:47 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2 */
    reg_value = di_rcp_reg_REG_ETH_MAC_SA2_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_MSK) >> DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "di_rcp_lfield_REG_ETH_MAC_SA_get", value[1] , value[0] );

}
static INLINE void di_rcp_lfield_range_REG_ETH_MAC_SA_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_range_REG_ETH_MAC_SA_set( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_ETH_MAC_SA_set", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_ETH_MAC_SA_set", stop_bit, 47 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_ETH_MAC_SA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000802c bits 31:0) bits 0:31 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1 */
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
        /* (0x0000802c bits 31:0) bits 0:31 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1 */
        di_rcp_reg_REG_ETH_MAC_SA1_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF + subfield_offset),
                                              DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF + subfield_offset,
                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008030 bits 15:0) bits 32:47 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2 */
    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00008030 bits 15:0) bits 32:47 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2 */
        di_rcp_reg_REG_ETH_MAC_SA2_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF + subfield_offset),
                                              DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_lfield_range_REG_ETH_MAC_SA_get( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_lfield_range_REG_ETH_MAC_SA_get( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_ETH_MAC_SA_get", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_ETH_MAC_SA_get", stop_bit, 47 );
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
    /* (0x0000802c bits 31:0) bits 0:31 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1 */
        reg_value = di_rcp_reg_REG_ETH_MAC_SA1_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_MSK)
                      >> DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_MSK, DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00008030 bits 15:0) bits 32:47 use field REG_ETH_MAC_SA of register PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2 */
        reg_value = di_rcp_reg_REG_ETH_MAC_SA2_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_MSK)
                      >> DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_MSK, DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_ETH_MAC_SA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_field_REG_ETH_ETYPE_BASE_set( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_ETH_ETYPE_BASE_set( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_ETH_ETYPE_BASE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_ETH_ETYPE_BASE_set", value );

    /* (0x00008034 bits 15:0) field REG_ETH_ETYPE_BASE of register PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE */
    di_rcp_reg_REG_ETH_ETYPE_BASE_field_set( b_ptr,
                                             h_ptr,
                                             DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_MSK,
                                             DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF,
                                             value);
}

static INLINE UINT32 di_rcp_field_REG_ETH_ETYPE_BASE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_ETH_ETYPE_BASE_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008034 bits 15:0) field REG_ETH_ETYPE_BASE of register PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE */
    reg_value = di_rcp_reg_REG_ETH_ETYPE_BASE_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_MSK) >> DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_ETH_ETYPE_BASE_get", value );

    return value;
}
static INLINE void di_rcp_field_range_REG_ETH_ETYPE_BASE_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_REG_ETH_ETYPE_BASE_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_ETH_ETYPE_BASE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_ETH_ETYPE_BASE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_ETH_ETYPE_BASE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008034 bits 15:0) field REG_ETH_ETYPE_BASE of register PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE */
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
        /* (0x00008034 bits 15:0) field REG_ETH_ETYPE_BASE of register PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE */
        di_rcp_reg_REG_ETH_ETYPE_BASE_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF + subfield_offset),
                                                 DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_REG_ETH_ETYPE_BASE_get( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_REG_ETH_ETYPE_BASE_get( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_ETH_ETYPE_BASE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_ETH_ETYPE_BASE_get", stop_bit, 15 );
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
    /* (0x00008034 bits 15:0) field REG_ETH_ETYPE_BASE of register PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE */
    reg_value = di_rcp_reg_REG_ETH_ETYPE_BASE_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_MSK)
                  >> DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_MSK, DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_ETH_ETYPE_BASE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_field_REG_STG_1_N_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_STG_1_N_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_STG_1_N_D_RCOHM_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_STG_1_N_D_RCOHM_set", value );

    /* (0x00008040 bits 11:0) field REG_STG_1_N_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM */
    di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set( b_ptr,
                                              h_ptr,
                                              DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_MSK,
                                              DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF,
                                              value);
}

static INLINE UINT32 di_rcp_field_REG_STG_1_N_D_RCOHM_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_STG_1_N_D_RCOHM_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008040 bits 11:0) field REG_STG_1_N_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM */
    reg_value = di_rcp_reg_REG_STG_1_N_D_RCOHM_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_MSK) >> DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_STG_1_N_D_RCOHM_get", value );

    return value;
}
static INLINE void di_rcp_field_range_REG_STG_1_N_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                               odu_rcp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_REG_STG_1_N_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                               odu_rcp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008040 bits 11:0) field REG_STG_1_N_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM */
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
        /* (0x00008040 bits 11:0) field REG_STG_1_N_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM */
        di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF + subfield_offset),
                                                  DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_REG_STG_1_N_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                                 odu_rcp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_REG_STG_1_N_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                                 odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_get", stop_bit, 11 );
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
    /* (0x00008040 bits 11:0) field REG_STG_1_N_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM */
    reg_value = di_rcp_reg_REG_STG_1_N_D_RCOHM_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_MSK)
                  >> DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_MSK, DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_REG_STG_1_N_D_RCOHM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_lfield_REG_STG_2_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_STG_2_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "di_rcp_lfield_REG_STG_2_D_RCOHM_set", value[2] , value[1] , value[0] );

    /* (0x00008044 bits 31:0) bits 0:31 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1 */
    di_rcp_reg_REG_STG_2_D_RCOHM1_field_set( b_ptr,
                                             h_ptr,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_MSK,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF,
                                             value[0]);

    /* (0x00008048 bits 31:0) bits 32:63 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2 */
    di_rcp_reg_REG_STG_2_D_RCOHM2_field_set( b_ptr,
                                             h_ptr,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_MSK,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF,
                                             value[1]);

    /* (0x0000804c bits 31:0) bits 64:95 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3 */
    di_rcp_reg_REG_STG_2_D_RCOHM3_field_set( b_ptr,
                                             h_ptr,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_MSK,
                                             DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF,
                                             value[2]);
}

static INLINE void di_rcp_lfield_REG_STG_2_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_REG_STG_2_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                        odu_rcp_handle_t *h_ptr,
                                                        UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00008044 bits 31:0) bits 0:31 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1 */
    reg_value = di_rcp_reg_REG_STG_2_D_RCOHM1_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_MSK) >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF;
    value[0] |= field_value;

    /* (0x00008048 bits 31:0) bits 32:63 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2 */
    reg_value = di_rcp_reg_REG_STG_2_D_RCOHM2_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_MSK) >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF;
    value[1] |= field_value;

    /* (0x0000804c bits 31:0) bits 64:95 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3 */
    reg_value = di_rcp_reg_REG_STG_2_D_RCOHM3_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_MSK) >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "di_rcp_lfield_REG_STG_2_D_RCOHM_get", value[2] , value[1] , value[0] );

}
static INLINE void di_rcp_lfield_range_REG_STG_2_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_lfield_range_REG_STG_2_D_RCOHM_set( di_rcp_buffer_t *b_ptr,
                                                              odu_rcp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008044 bits 31:0) bits 0:31 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1 */
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
        /* (0x00008044 bits 31:0) bits 0:31 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1 */
        di_rcp_reg_REG_STG_2_D_RCOHM1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset),
                                                 DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008048 bits 31:0) bits 32:63 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00008048 bits 31:0) bits 32:63 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2 */
        di_rcp_reg_REG_STG_2_D_RCOHM2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset),
                                                 DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000804c bits 31:0) bits 64:95 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000804c bits 31:0) bits 64:95 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3 */
        di_rcp_reg_REG_STG_2_D_RCOHM3_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset),
                                                 DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_lfield_range_REG_STG_2_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_lfield_range_REG_STG_2_D_RCOHM_get( di_rcp_buffer_t *b_ptr,
                                                                odu_rcp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_get", stop_bit, 95 );
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
    /* (0x00008044 bits 31:0) bits 0:31 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1 */
        reg_value = di_rcp_reg_REG_STG_2_D_RCOHM1_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_MSK)
                      >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_MSK, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00008048 bits 31:0) bits 32:63 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2 */
        reg_value = di_rcp_reg_REG_STG_2_D_RCOHM2_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_MSK)
                      >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_MSK, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000804c bits 31:0) bits 64:95 use field REG_STG_2_D_RCOHM of register PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3 */
        reg_value = di_rcp_reg_REG_STG_2_D_RCOHM3_read(  b_ptr, h_ptr);
        field_value = (reg_value & DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_MSK)
                      >> DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_MSK, DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_lfield_range_REG_STG_2_D_RCOHM_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void di_rcp_field_REG_PMON_TX_TRIG_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_REG_PMON_TX_TRIG_set( di_rcp_buffer_t *b_ptr,
                                                      odu_rcp_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_REG_PMON_TX_TRIG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_REG_PMON_TX_TRIG_set", value );

    /* (0x00008038 bits 0) field REG_PMON_TX_TRIG of register PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG */
    di_rcp_reg_REG_PMON_TX_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_MSK,
                                           DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_OFF,
                                           value);
}

static INLINE UINT32 di_rcp_field_REG_PMON_TX_TRIG_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_REG_PMON_TX_TRIG_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008038 bits 0) field REG_PMON_TX_TRIG of register PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG */
    reg_value = di_rcp_reg_REG_PMON_TX_TRIG_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_MSK) >> DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_REG_PMON_TX_TRIG_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void di_rcp_field_DFEC_TX_DONE_E_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_DFEC_TX_DONE_E_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_DFEC_TX_DONE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_DFEC_TX_DONE_E_set", value );

    /* (0x00008080 bits 12) field DFEC_TX_DONE_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    di_rcp_reg_GEN_IRQ_E_field_set( b_ptr,
                                    h_ptr,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_MSK,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_OFF,
                                    value);
}

static INLINE UINT32 di_rcp_field_DFEC_TX_DONE_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_DFEC_TX_DONE_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008080 bits 12) field DFEC_TX_DONE_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    reg_value = di_rcp_reg_GEN_IRQ_E_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_MSK) >> DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_DFEC_TX_DONE_E_get", value );

    return value;
}
static INLINE void di_rcp_field_DEFECTS_TIP_E_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_DEFECTS_TIP_E_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_DEFECTS_TIP_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_DEFECTS_TIP_E_set", value );

    /* (0x00008080 bits 4) field DEFECTS_TIP_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    di_rcp_reg_GEN_IRQ_E_field_set( b_ptr,
                                    h_ptr,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_MSK,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_OFF,
                                    value);
}

static INLINE UINT32 di_rcp_field_DEFECTS_TIP_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_DEFECTS_TIP_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008080 bits 4) field DEFECTS_TIP_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    reg_value = di_rcp_reg_GEN_IRQ_E_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_MSK) >> DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_DEFECTS_TIP_E_get", value );

    return value;
}
static INLINE void di_rcp_field_PMON_TX_DONE_E_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_PMON_TX_DONE_E_set( di_rcp_buffer_t *b_ptr,
                                                    odu_rcp_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_PMON_TX_DONE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_PMON_TX_DONE_E_set", value );

    /* (0x00008080 bits 0) field PMON_TX_DONE_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    di_rcp_reg_GEN_IRQ_E_field_set( b_ptr,
                                    h_ptr,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_MSK,
                                    DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_OFF,
                                    value);
}

static INLINE UINT32 di_rcp_field_PMON_TX_DONE_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_PMON_TX_DONE_E_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008080 bits 0) field PMON_TX_DONE_E of register PMC_DI_ODU_RCP_REG_GEN_IRQ_E */
    reg_value = di_rcp_reg_GEN_IRQ_E_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_MSK) >> DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_PMON_TX_DONE_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 di_rcp_field_INTERRUPT_SUMMARY_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_INTERRUPT_SUMMARY_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008010 bits 2:0) bits 0:2 use field INTERRUPT_SUMMARY of register PMC_DI_ODU_RCP_REG_IRQ_SUMMARY */
    reg_value = di_rcp_reg_IRQ_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK) >> DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_INTERRUPT_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 di_rcp_field_range_INTERRUPT_SUMMARY_get( di_rcp_buffer_t *b_ptr,
                                                               odu_rcp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_INTERRUPT_SUMMARY_get( di_rcp_buffer_t *b_ptr,
                                                               odu_rcp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_INTERRUPT_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_INTERRUPT_SUMMARY_get", stop_bit, 2 );
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
    /* (0x00008010 bits 2:0) bits 0:2 use field INTERRUPT_SUMMARY of register PMC_DI_ODU_RCP_REG_IRQ_SUMMARY */
    reg_value = di_rcp_reg_IRQ_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK)
                  >> DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK, DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_INTERRUPT_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_range_INTERRUPT_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                              odu_rcp_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_range_INTERRUPT_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                              odu_rcp_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_INTERRUPT_SUMMARY_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_INTERRUPT_SUMMARY_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_INTERRUPT_SUMMARY_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008010 bits 2:0) bits 0:2 use field INTERRUPT_SUMMARY of register PMC_DI_ODU_RCP_REG_IRQ_SUMMARY */
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
        /* (0x00008010 bits 2:0) bits 0:2 use field INTERRUPT_SUMMARY of register PMC_DI_ODU_RCP_REG_IRQ_SUMMARY */
        return di_rcp_reg_IRQ_SUMMARY_poll( b_ptr,
                                            h_ptr,
                                            subfield_mask << (DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                            value << (DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_INTERRUPT_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                        odu_rcp_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_INTERRUPT_SUMMARY_poll( di_rcp_buffer_t *b_ptr,
                                                                        odu_rcp_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "di_rcp_field_INTERRUPT_SUMMARY_poll", value );

    /* (0x00008010 bits 2:0) bits 0:2 use field INTERRUPT_SUMMARY of register PMC_DI_ODU_RCP_REG_IRQ_SUMMARY */
    return di_rcp_reg_IRQ_SUMMARY_poll( b_ptr,
                                        h_ptr,
                                        DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK,
                                        (value<<DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF),
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
static INLINE void di_rcp_field_DFEC_TX_DONE_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_DFEC_TX_DONE_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_DFEC_TX_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_DFEC_TX_DONE_I_set_to_clear", value );

    /* (0x00008060 bits 12) field DFEC_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_MSK,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_OFF,
                                                        value);
}

static INLINE UINT32 di_rcp_field_DFEC_TX_DONE_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_DFEC_TX_DONE_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008060 bits 12) field DFEC_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    reg_value = di_rcp_reg_REG_GEN_IRQ_I_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_MSK) >> DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_DFEC_TX_DONE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_DFEC_TX_DONE_I_poll( di_rcp_buffer_t *b_ptr,
                                                                     odu_rcp_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_DFEC_TX_DONE_I_poll( di_rcp_buffer_t *b_ptr,
                                                                     odu_rcp_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "di_rcp_field_DFEC_TX_DONE_I_poll", value );

    /* (0x00008060 bits 12) field DFEC_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    return di_rcp_reg_REG_GEN_IRQ_I_poll( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_MSK,
                                          (value<<DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void di_rcp_field_DEFECTS_TIP_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_DEFECTS_TIP_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                            odu_rcp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_DEFECTS_TIP_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_DEFECTS_TIP_I_set_to_clear", value );

    /* (0x00008060 bits 4) field DEFECTS_TIP_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_MSK,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_OFF,
                                                        value);
}

static INLINE UINT32 di_rcp_field_DEFECTS_TIP_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_DEFECTS_TIP_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008060 bits 4) field DEFECTS_TIP_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    reg_value = di_rcp_reg_REG_GEN_IRQ_I_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_MSK) >> DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_DEFECTS_TIP_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_DEFECTS_TIP_I_poll( di_rcp_buffer_t *b_ptr,
                                                                    odu_rcp_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_DEFECTS_TIP_I_poll( di_rcp_buffer_t *b_ptr,
                                                                    odu_rcp_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "di_rcp_field_DEFECTS_TIP_I_poll", value );

    /* (0x00008060 bits 4) field DEFECTS_TIP_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    return di_rcp_reg_REG_GEN_IRQ_I_poll( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_MSK,
                                          (value<<DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void di_rcp_field_PMON_TX_DONE_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_PMON_TX_DONE_I_set_to_clear( di_rcp_buffer_t *b_ptr,
                                                             odu_rcp_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_PMON_TX_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "di_rcp_field_PMON_TX_DONE_I_set_to_clear", value );

    /* (0x00008060 bits 0) field PMON_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_MSK,
                                                        DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_OFF,
                                                        value);
}

static INLINE UINT32 di_rcp_field_PMON_TX_DONE_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_PMON_TX_DONE_I_get( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00008060 bits 0) field PMON_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    reg_value = di_rcp_reg_REG_GEN_IRQ_I_read(  b_ptr, h_ptr);
    value = (reg_value & DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_MSK) >> DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_OFF;
    IOLOG( "%s -> 0x%08x", "di_rcp_field_PMON_TX_DONE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_PMON_TX_DONE_I_poll( di_rcp_buffer_t *b_ptr,
                                                                     odu_rcp_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE di_rcp_field_PMON_TX_DONE_I_poll( di_rcp_buffer_t *b_ptr,
                                                                     odu_rcp_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "di_rcp_field_PMON_TX_DONE_I_poll", value );

    /* (0x00008060 bits 0) field PMON_TX_DONE_I of register PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I */
    return di_rcp_reg_REG_GEN_IRQ_I_poll( b_ptr,
                                          h_ptr,
                                          DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_MSK,
                                          (value<<DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size1600
 * ==================================================================================
 */
static INLINE void di_rcp_field_F_EBC_set( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_F_EBC_set( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_F_EBC_set", N, 1599);
    if (value > 8191)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_F_EBC_set", value, 8191);
    IOLOG( "%s <= N=%d 0x%08x", "di_rcp_field_F_EBC_set", N, value );

    /* ((0x00008200 + (N) * 4) bits 28:16) field F_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    di_rcp_reg_OTN_PMON_COPY_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_MSK,
                                              DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF,
                                              value);
}

static INLINE UINT32 di_rcp_field_F_EBC_get( di_rcp_buffer_t *b_ptr,
                                             odu_rcp_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_F_EBC_get( di_rcp_buffer_t *b_ptr,
                                             odu_rcp_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_F_EBC_get", N, 1599);
    /* ((0x00008200 + (N) * 4) bits 28:16) field F_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    reg_value = di_rcp_reg_OTN_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_MSK) >> DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "di_rcp_field_F_EBC_get", N, value );

    return value;
}
static INLINE void di_rcp_field_range_F_EBC_set( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_F_EBC_set( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_F_EBC_set", N, 1599);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_F_EBC_set", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_F_EBC_set", stop_bit, 12 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_F_EBC_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00008200 + (N) * 4) bits 28:16) field F_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
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
        if (stop_bit < 12) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 12;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00008200 + (N) * 4) bits 28:16) field F_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
        di_rcp_reg_OTN_PMON_COPY_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF + subfield_offset),
                                                  DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_F_EBC_get( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_F_EBC_get( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
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

    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_F_EBC_get", N, 1599);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_F_EBC_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_F_EBC_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00008200 + (N) * 4) bits 28:16) field F_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    reg_value = di_rcp_reg_OTN_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_MSK)
                  >> DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_MSK, DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_F_EBC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void di_rcp_field_N_EBC_set( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_N_EBC_set( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_N_EBC_set", N, 1599);
    if (value > 8191)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_N_EBC_set", value, 8191);
    IOLOG( "%s <= N=%d 0x%08x", "di_rcp_field_N_EBC_set", N, value );

    /* ((0x00008200 + (N) * 4) bits 12:0) field N_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    di_rcp_reg_OTN_PMON_COPY_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_MSK,
                                              DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF,
                                              value);
}

static INLINE UINT32 di_rcp_field_N_EBC_get( di_rcp_buffer_t *b_ptr,
                                             odu_rcp_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_N_EBC_get( di_rcp_buffer_t *b_ptr,
                                             odu_rcp_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_N_EBC_get", N, 1599);
    /* ((0x00008200 + (N) * 4) bits 12:0) field N_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    reg_value = di_rcp_reg_OTN_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_MSK) >> DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "di_rcp_field_N_EBC_get", N, value );

    return value;
}
static INLINE void di_rcp_field_range_N_EBC_set( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_N_EBC_set( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_N_EBC_set", N, 1599);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_N_EBC_set", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_N_EBC_set", stop_bit, 12 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_N_EBC_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00008200 + (N) * 4) bits 12:0) field N_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
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
        if (stop_bit < 12) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 12;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00008200 + (N) * 4) bits 12:0) field N_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
        di_rcp_reg_OTN_PMON_COPY_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF + subfield_offset),
                                                  DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_N_EBC_get( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_N_EBC_get( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
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

    if (N > 1599)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_N_EBC_get", N, 1599);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_N_EBC_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_N_EBC_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00008200 + (N) * 4) bits 12:0) field N_EBC of register PMC_DI_ODU_RCP_REG_OTN_PMON_COPY index N=0..1599 */
    reg_value = di_rcp_reg_OTN_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_MSK)
                  >> DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_MSK, DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_N_EBC_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size48
 * ==================================================================================
 */
static INLINE void di_rcp_field_TSE_set( di_rcp_buffer_t *b_ptr,
                                         odu_rcp_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_TSE_set( di_rcp_buffer_t *b_ptr,
                                         odu_rcp_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_TSE_set", N, 47);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_TSE_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "di_rcp_field_TSE_set", N, value );

    /* ((0x00009b00 + (N) * 4) bits 15:0) field TSE of register PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY index N=0..47 */
    di_rcp_reg_PRBS_PMON_COPY_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_MSK,
                                               DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF,
                                               value);
}

static INLINE UINT32 di_rcp_field_TSE_get( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_TSE_get( di_rcp_buffer_t *b_ptr,
                                           odu_rcp_handle_t *h_ptr,
                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_TSE_get", N, 47);
    /* ((0x00009b00 + (N) * 4) bits 15:0) field TSE of register PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY index N=0..47 */
    reg_value = di_rcp_reg_PRBS_PMON_COPY_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_MSK) >> DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "di_rcp_field_TSE_get", N, value );

    return value;
}
static INLINE void di_rcp_field_range_TSE_set( di_rcp_buffer_t *b_ptr,
                                               odu_rcp_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_TSE_set( di_rcp_buffer_t *b_ptr,
                                               odu_rcp_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_TSE_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_TSE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_TSE_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_TSE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00009b00 + (N) * 4) bits 15:0) field TSE of register PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY index N=0..47 */
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
        /* ((0x00009b00 + (N) * 4) bits 15:0) field TSE of register PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY index N=0..47 */
        di_rcp_reg_PRBS_PMON_COPY_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF + subfield_offset),
                                                   DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_TSE_get( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_TSE_get( di_rcp_buffer_t *b_ptr,
                                                 odu_rcp_handle_t *h_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_TSE_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_TSE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_TSE_get", stop_bit, 15 );
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
    /* ((0x00009b00 + (N) * 4) bits 15:0) field TSE of register PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY index N=0..47 */
    reg_value = di_rcp_reg_PRBS_PMON_COPY_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_MSK)
                  >> DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_MSK, DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_TSE_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size64
 * ==================================================================================
 */
static INLINE void di_rcp_field_SDH_PMON_COPY_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_SDH_PMON_COPY_set( di_rcp_buffer_t *b_ptr,
                                                   odu_rcp_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_SDH_PMON_COPY_set", N, 63);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "di_rcp_field_SDH_PMON_COPY_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "di_rcp_field_SDH_PMON_COPY_set", N, value );

    /* ((0x00008100 + (N) * 4) bits 15:0) field SDH_PMON_COPY of register PMC_DI_ODU_RCP_REG_SDH_PMON_COPY index N=0..63 */
    di_rcp_reg_SDH_PMON_COPY_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_MSK,
                                              DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF,
                                              value);
}

static INLINE UINT32 di_rcp_field_SDH_PMON_COPY_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_SDH_PMON_COPY_get( di_rcp_buffer_t *b_ptr,
                                                     odu_rcp_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_SDH_PMON_COPY_get", N, 63);
    /* ((0x00008100 + (N) * 4) bits 15:0) field SDH_PMON_COPY of register PMC_DI_ODU_RCP_REG_SDH_PMON_COPY index N=0..63 */
    reg_value = di_rcp_reg_SDH_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_MSK) >> DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "di_rcp_field_SDH_PMON_COPY_get", N, value );

    return value;
}
static INLINE void di_rcp_field_range_SDH_PMON_COPY_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void di_rcp_field_range_SDH_PMON_COPY_set( di_rcp_buffer_t *b_ptr,
                                                         odu_rcp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_SDH_PMON_COPY_set", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_SDH_PMON_COPY_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_SDH_PMON_COPY_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_SDH_PMON_COPY_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00008100 + (N) * 4) bits 15:0) field SDH_PMON_COPY of register PMC_DI_ODU_RCP_REG_SDH_PMON_COPY index N=0..63 */
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
        /* ((0x00008100 + (N) * 4) bits 15:0) field SDH_PMON_COPY of register PMC_DI_ODU_RCP_REG_SDH_PMON_COPY index N=0..63 */
        di_rcp_reg_SDH_PMON_COPY_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF + subfield_offset),
                                                  DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 di_rcp_field_range_SDH_PMON_COPY_get( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 di_rcp_field_range_SDH_PMON_COPY_get( di_rcp_buffer_t *b_ptr,
                                                           odu_rcp_handle_t *h_ptr,
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

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "di_rcp_field_range_SDH_PMON_COPY_get", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "di_rcp_field_range_SDH_PMON_COPY_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "di_rcp_field_range_SDH_PMON_COPY_get", stop_bit, 15 );
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
    /* ((0x00008100 + (N) * 4) bits 15:0) field SDH_PMON_COPY of register PMC_DI_ODU_RCP_REG_SDH_PMON_COPY index N=0..63 */
    reg_value = di_rcp_reg_SDH_PMON_COPY_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_MSK)
                  >> DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_MSK, DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "di_rcp_field_range_SDH_PMON_COPY_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DI_RCP_IO_INLINE_H */
