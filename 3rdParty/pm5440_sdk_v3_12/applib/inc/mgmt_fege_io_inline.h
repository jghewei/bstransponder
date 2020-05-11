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
 *     and register accessor functions for the mgmt_fege block
 *****************************************************************************/
#ifndef _MGMT_FEGE_IO_INLINE_H
#define _MGMT_FEGE_IO_INLINE_H

#include "mgmt_fege_loc.h"
#include "mgmt_fege_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MGMT_FEGE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mgmt_fege
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
    mgmt_fege_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mgmt_fege_buffer_t;
static INLINE void mgmt_fege_buffer_init( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mgmt_fege_buffer_init( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mgmt_fege_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mgmt_fege_buffer_flush( mgmt_fege_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mgmt_fege_buffer_flush( mgmt_fege_buffer_t *b_ptr )
{
    IOLOG( "mgmt_fege_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mgmt_fege_reg_read( mgmt_fege_buffer_t *b_ptr,
                                         mgmt_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_read( mgmt_fege_buffer_t *b_ptr,
                                         mgmt_fege_handle_t *h_ptr,
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
static INLINE void mgmt_fege_reg_write( mgmt_fege_buffer_t *b_ptr,
                                        mgmt_fege_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_write( mgmt_fege_buffer_t *b_ptr,
                                        mgmt_fege_handle_t *h_ptr,
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

static INLINE void mgmt_fege_field_set( mgmt_fege_buffer_t *b_ptr,
                                        mgmt_fege_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_set( mgmt_fege_buffer_t *b_ptr,
                                        mgmt_fege_handle_t *h_ptr,
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

static INLINE void mgmt_fege_action_on_write_field_set( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_action_on_write_field_set( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
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

static INLINE void mgmt_fege_burst_read( mgmt_fege_buffer_t *b_ptr,
                                         mgmt_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_burst_read( mgmt_fege_buffer_t *b_ptr,
                                         mgmt_fege_handle_t *h_ptr,
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

static INLINE void mgmt_fege_burst_write( mgmt_fege_buffer_t *b_ptr,
                                          mgmt_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_burst_write( mgmt_fege_buffer_t *b_ptr,
                                          mgmt_fege_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_poll( mgmt_fege_buffer_t *b_ptr,
                                                   mgmt_fege_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_poll( mgmt_fege_buffer_t *b_ptr,
                                                   mgmt_fege_handle_t *h_ptr,
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
 *  register access functions for mgmt_fege
 * ==================================================================================
 */

static INLINE void mgmt_fege_reg_SW_RESET_write( mgmt_fege_buffer_t *b_ptr,
                                                 mgmt_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_SW_RESET_write( mgmt_fege_buffer_t *b_ptr,
                                                 mgmt_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_SW_RESET_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_SW_RESET,
                         value);
}

static INLINE void mgmt_fege_reg_SW_RESET_field_set( mgmt_fege_buffer_t *b_ptr,
                                                     mgmt_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_SW_RESET_field_set( mgmt_fege_buffer_t *b_ptr,
                                                     mgmt_fege_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_SW_RESET_field_set", mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_SW_RESET,
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_SW_RESET_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_SW_RESET_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_SW_RESET_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MGMT_FEGE_TOP_REG_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void mgmt_fege_reg_TMVP_ETHERNET_TYPE_write( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_TMVP_ETHERNET_TYPE_write( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_TMVP_ETHERNET_TYPE_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE,
                         value);
}

static INLINE void mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set", mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE,
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_TMVP_ETHERNET_TYPE_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_TMVP_ETHERNET_TYPE_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_TMVP_ETHERNET_TYPE_read", reg_value);
    return reg_value;
}

static INLINE void mgmt_fege_reg_RMVP_ETHERNET_TYPE_write( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_RMVP_ETHERNET_TYPE_write( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_RMVP_ETHERNET_TYPE_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE,
                         value);
}

static INLINE void mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set", mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE,
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_RMVP_ETHERNET_TYPE_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_RMVP_ETHERNET_TYPE_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_RMVP_ETHERNET_TYPE_read", reg_value);
    return reg_value;
}

static INLINE void mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_write( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_write( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL,
                         value);
}

static INLINE void mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set", mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL,
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void mgmt_fege_reg_RX_VLAN_LUT_array_write( mgmt_fege_buffer_t *b_ptr,
                                                          mgmt_fege_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_RX_VLAN_LUT_array_write( mgmt_fege_buffer_t *b_ptr,
                                                          mgmt_fege_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_RX_VLAN_LUT_array_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT(N),
                         value);
}

static INLINE void mgmt_fege_reg_RX_VLAN_LUT_array_field_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_RX_VLAN_LUT_array_field_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_RX_VLAN_LUT_array_field_set", N, mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT(N),
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_RX_VLAN_LUT_array_read( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_RX_VLAN_LUT_array_read( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mgmt_fege_reg_RX_VLAN_LUT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mgmt_fege_reg_INT_EN_write( mgmt_fege_buffer_t *b_ptr,
                                               mgmt_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_INT_EN_write( mgmt_fege_buffer_t *b_ptr,
                                               mgmt_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_INT_EN_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MGMT_FEGE_TOP_REG_INT_EN,
                         value);
}

static INLINE void mgmt_fege_reg_INT_EN_field_set( mgmt_fege_buffer_t *b_ptr,
                                                   mgmt_fege_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_INT_EN_field_set( mgmt_fege_buffer_t *b_ptr,
                                                   mgmt_fege_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_INT_EN_field_set", mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MGMT_FEGE_TOP_REG_INT_EN,
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_INT_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_INT_EN_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_INT_EN_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MGMT_FEGE_TOP_REG_INT_EN);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void mgmt_fege_reg_INT_STAT_write( mgmt_fege_buffer_t *b_ptr,
                                                 mgmt_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_INT_STAT_write( mgmt_fege_buffer_t *b_ptr,
                                                 mgmt_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_INT_STAT_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MGMT_FEGE_TOP_REG_INT_STAT,
                         value);
}

static INLINE void mgmt_fege_reg_INT_STAT_action_on_write_field_set( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_INT_STAT_action_on_write_field_set( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_INT_STAT_action_on_write_field_set", mask, ofs, value );
    mgmt_fege_action_on_write_field_set( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_MGMT_FEGE_TOP_REG_INT_STAT,
                                         mask,
                                         ofs,
                                         value);

}

static INLINE UINT32 mgmt_fege_reg_INT_STAT_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_INT_STAT_read( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MGMT_FEGE_TOP_REG_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "mgmt_fege_reg_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_reg_INT_STAT_poll( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_reg_INT_STAT_poll( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mgmt_fege_reg_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mgmt_fege_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_MGMT_FEGE_TOP_REG_INT_STAT,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void mgmt_fege_reg_TX_VLAN_LUT_array_write( mgmt_fege_buffer_t *b_ptr,
                                                          mgmt_fege_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_TX_VLAN_LUT_array_write( mgmt_fege_buffer_t *b_ptr,
                                                          mgmt_fege_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mgmt_fege_reg_TX_VLAN_LUT_array_write", value );
    mgmt_fege_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT(N),
                         value);
}

static INLINE void mgmt_fege_reg_TX_VLAN_LUT_array_field_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_reg_TX_VLAN_LUT_array_field_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mgmt_fege_reg_TX_VLAN_LUT_array_field_set", N, mask, ofs, value );
    mgmt_fege_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT(N),
                         mask,
                         PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mgmt_fege_reg_TX_VLAN_LUT_array_read( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_reg_TX_VLAN_LUT_array_read( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mgmt_fege_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mgmt_fege_reg_TX_VLAN_LUT_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mgmt_fege_field_MGMT_FEGE_SW_RESET_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_MGMT_FEGE_SW_RESET_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_MGMT_FEGE_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_MGMT_FEGE_SW_RESET_set", value );

    /* (0x00000000 bits 0) field MGMT_FEGE_SW_RESET of register PMC_MGMT_FEGE_TOP_REG_SW_RESET */
    mgmt_fege_reg_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_MSK,
                                      MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_OFF,
                                      value);
}

static INLINE UINT32 mgmt_fege_field_MGMT_FEGE_SW_RESET_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_MGMT_FEGE_SW_RESET_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MGMT_FEGE_SW_RESET of register PMC_MGMT_FEGE_TOP_REG_SW_RESET */
    reg_value = mgmt_fege_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_MSK) >> MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_MGMT_FEGE_SW_RESET_get", value );

    return value;
}
static INLINE void mgmt_fege_field_TMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_ETHERNET_TYPE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_ETHERNET_TYPE_set", value );

    /* (0x00000020 bits 15:0) bits 0:15 use field TMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE */
    mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set( b_ptr,
                                                h_ptr,
                                                MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_MSK,
                                                MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF,
                                                value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 15:0) bits 0:15 use field TMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE */
    reg_value = mgmt_fege_reg_TMVP_ETHERNET_TYPE_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_MSK) >> MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_ETHERNET_TYPE_get", value );

    return value;
}
static INLINE void mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 15:0) bits 0:15 use field TMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE */
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
        /* (0x00000020 bits 15:0) bits 0:15 use field TMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE */
        mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF + subfield_offset),
                                                    MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr,
                                                                   mgmt_fege_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr,
                                                                   mgmt_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get", stop_bit, 15 );
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
    /* (0x00000020 bits 15:0) bits 0:15 use field TMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE */
    reg_value = mgmt_fege_reg_TMVP_ETHERNET_TYPE_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_MSK)
                  >> MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_MSK, MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mgmt_fege_field_RMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_ETHERNET_TYPE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_ETHERNET_TYPE_set", value );

    /* (0x00000024 bits 15:0) bits 0:15 use field RMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE */
    mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set( b_ptr,
                                                h_ptr,
                                                MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_MSK,
                                                MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF,
                                                value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 15:0) bits 0:15 use field RMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE */
    reg_value = mgmt_fege_reg_RMVP_ETHERNET_TYPE_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_MSK) >> MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_ETHERNET_TYPE_get", value );

    return value;
}
static INLINE void mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 15:0) bits 0:15 use field RMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE */
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
        /* (0x00000024 bits 15:0) bits 0:15 use field RMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE */
        mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF + subfield_offset),
                                                    MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr,
                                                                   mgmt_fege_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get( mgmt_fege_buffer_t *b_ptr,
                                                                   mgmt_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get", stop_bit, 15 );
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
    /* (0x00000024 bits 15:0) bits 0:15 use field RMVP_ETHERNET_TYPE of register PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE */
    reg_value = mgmt_fege_reg_RMVP_ETHERNET_TYPE_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_MSK)
                  >> MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_MSK, MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mgmt_fege_field_RDATA_BIT_SWAP_set( mgmt_fege_buffer_t *b_ptr,
                                                       mgmt_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RDATA_BIT_SWAP_set( mgmt_fege_buffer_t *b_ptr,
                                                       mgmt_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RDATA_BIT_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RDATA_BIT_SWAP_set", value );

    /* (0x0000002c bits 1) field RDATA_BIT_SWAP of register PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL */
    mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_MSK,
                                                      MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_RDATA_BIT_SWAP_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RDATA_BIT_SWAP_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 1) field RDATA_BIT_SWAP of register PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL */
    reg_value = mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_MSK) >> MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RDATA_BIT_SWAP_get", value );

    return value;
}
static INLINE void mgmt_fege_field_TDATA_BIT_SWAP_set( mgmt_fege_buffer_t *b_ptr,
                                                       mgmt_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TDATA_BIT_SWAP_set( mgmt_fege_buffer_t *b_ptr,
                                                       mgmt_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TDATA_BIT_SWAP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TDATA_BIT_SWAP_set", value );

    /* (0x0000002c bits 0) field TDATA_BIT_SWAP of register PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL */
    mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_MSK,
                                                      MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_TDATA_BIT_SWAP_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TDATA_BIT_SWAP_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 0) field TDATA_BIT_SWAP of register PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL */
    reg_value = mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_MSK) >> MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TDATA_BIT_SWAP_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size64
 * ==================================================================================
 */
static INLINE void mgmt_fege_field_RXLUT_VALID_set( mgmt_fege_buffer_t *b_ptr,
                                                    mgmt_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RXLUT_VALID_set( mgmt_fege_buffer_t *b_ptr,
                                                    mgmt_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_VALID_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RXLUT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_RXLUT_VALID_set", N, value );

    /* ((0x00000200 + (N) * 0x4) bits 15) field RXLUT_VALID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_MSK,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_RXLUT_VALID_get( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RXLUT_VALID_get( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_VALID_get", N, 63);
    /* ((0x00000200 + (N) * 0x4) bits 15) field RXLUT_VALID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_MSK) >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_RXLUT_VALID_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set", N, value );

    /* ((0x00000200 + (N) * 0x4) bits 14) field RXLUT_PRIORITY_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_MSK,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get", N, 63);
    /* ((0x00000200 + (N) * 0x4) bits 14) field RXLUT_PRIORITY_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_MSK) >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set", N, value );

    /* ((0x00000200 + (N) * 0x4) bits 13) field RXLUT_FIFO_ZONE_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_MSK,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                                 mgmt_fege_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_get", N, 63);
    /* ((0x00000200 + (N) * 0x4) bits 13) field RXLUT_FIFO_ZONE_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_MSK) >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_RXLUT_FCS_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                         mgmt_fege_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RXLUT_FCS_ENABLE_set( mgmt_fege_buffer_t *b_ptr,
                                                         mgmt_fege_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_FCS_ENABLE_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RXLUT_FCS_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_RXLUT_FCS_ENABLE_set", N, value );

    /* ((0x00000200 + (N) * 0x4) bits 12) field RXLUT_FCS_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_MSK,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_RXLUT_FCS_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RXLUT_FCS_ENABLE_get( mgmt_fege_buffer_t *b_ptr,
                                                           mgmt_fege_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_FCS_ENABLE_get", N, 63);
    /* ((0x00000200 + (N) * 0x4) bits 12) field RXLUT_FCS_ENABLE of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_MSK) >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_RXLUT_FCS_ENABLE_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_RXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_VLAN_ID_set", N, 63);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RXLUT_VLAN_ID_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_RXLUT_VLAN_ID_set", N, value );

    /* ((0x00000200 + (N) * 0x4) bits 11:0) bits 0:11 use field RXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_MSK,
                                               MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_RXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_RXLUT_VLAN_ID_get", N, 63);
    /* ((0x00000200 + (N) * 0x4) bits 11:0) bits 0:11 use field RXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_MSK) >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_RXLUT_VLAN_ID_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_range_RXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_range_RXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_range_RXLUT_VLAN_ID_set", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_RXLUT_VLAN_ID_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_RXLUT_VLAN_ID_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_RXLUT_VLAN_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000200 + (N) * 0x4) bits 11:0) bits 0:11 use field RXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
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
        /* ((0x00000200 + (N) * 0x4) bits 11:0) bits 0:11 use field RXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
        mgmt_fege_reg_RX_VLAN_LUT_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF + subfield_offset),
                                                   MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mgmt_fege_field_range_RXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_range_RXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_range_RXLUT_VLAN_ID_get", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_RXLUT_VLAN_ID_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_RXLUT_VLAN_ID_get", stop_bit, 11 );
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
    /* ((0x00000200 + (N) * 0x4) bits 11:0) bits 0:11 use field RXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_RX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_MSK)
                  >> MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_MSK, MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_RXLUT_VLAN_ID_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mgmt_fege_field_RMVP_INVALID_TYPE_E_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_INVALID_TYPE_E_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_E_set", value );

    /* (0x00000010 bits 5) field RMVP_INVALID_TYPE_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_TYPE_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_TYPE_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 5) field RMVP_INVALID_TYPE_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_E_get", value );

    return value;
}
static INLINE void mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_set( mgmt_fege_buffer_t *b_ptr,
                                                               mgmt_fege_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_set", value );

    /* (0x00000010 bits 4) field RMVP_INVALID_VLAN_ID_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 4) field RMVP_INVALID_VLAN_ID_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_get", value );

    return value;
}
static INLINE void mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_set( mgmt_fege_buffer_t *b_ptr,
                                                             mgmt_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_set( mgmt_fege_buffer_t *b_ptr,
                                                             mgmt_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_set", value );

    /* (0x00000010 bits 3) field RMVP_FIFO_OVERFLOW_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 3) field RMVP_FIFO_OVERFLOW_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_get", value );

    return value;
}
static INLINE void mgmt_fege_field_TMVP_INVALID_TYPE_E_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_INVALID_TYPE_E_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_E_set", value );

    /* (0x00000010 bits 2) field TMVP_INVALID_TYPE_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_TYPE_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_TYPE_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 2) field TMVP_INVALID_TYPE_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_E_get", value );

    return value;
}
static INLINE void mgmt_fege_field_TMVP_INVALID_DPI_ID_E_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_INVALID_DPI_ID_E_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_E_set", value );

    /* (0x00000010 bits 1) field TMVP_INVALID_DPI_ID_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_DPI_ID_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_DPI_ID_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 1) field TMVP_INVALID_DPI_ID_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_E_get", value );

    return value;
}
static INLINE void mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_set( mgmt_fege_buffer_t *b_ptr,
                                                             mgmt_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_set( mgmt_fege_buffer_t *b_ptr,
                                                             mgmt_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_set", value );

    /* (0x00000010 bits 0) field TMVP_FIFO_OVERFLOW_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    mgmt_fege_reg_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_MSK,
                                    MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_OFF,
                                    value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field TMVP_FIFO_OVERFLOW_E of register PMC_MGMT_FEGE_TOP_REG_INT_EN */
    reg_value = mgmt_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_MSK) >> MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mgmt_fege_field_RMVP_INVALID_TYPE_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_INVALID_TYPE_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_I_set_to_clear", value );

    /* (0x00000014 bits 5) field RMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_TYPE_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_TYPE_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 5) field RMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_TYPE_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                             mgmt_fege_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_TYPE_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                             mgmt_fege_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_RMVP_INVALID_TYPE_I_poll", value );

    /* (0x00000014 bits 5) field RMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                        mgmt_fege_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                        mgmt_fege_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_set_to_clear", value );

    /* (0x00000014 bits 4) field RMVP_INVALID_VLAN_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 4) field RMVP_INVALID_VLAN_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                                mgmt_fege_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                                mgmt_fege_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_poll", value );

    /* (0x00000014 bits 4) field RMVP_INVALID_VLAN_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                      mgmt_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                      mgmt_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_set_to_clear", value );

    /* (0x00000014 bits 3) field RMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 3) field RMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                              mgmt_fege_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                              mgmt_fege_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_poll", value );

    /* (0x00000014 bits 3) field RMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void mgmt_fege_field_TMVP_INVALID_TYPE_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_INVALID_TYPE_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                     mgmt_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_I_set_to_clear", value );

    /* (0x00000014 bits 2) field TMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_TYPE_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_TYPE_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 2) field TMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_TYPE_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                             mgmt_fege_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_TYPE_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                             mgmt_fege_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_TMVP_INVALID_TYPE_I_poll", value );

    /* (0x00000014 bits 2) field TMVP_INVALID_TYPE_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void mgmt_fege_field_TMVP_INVALID_DPI_ID_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                       mgmt_fege_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_INVALID_DPI_ID_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                       mgmt_fege_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_I_set_to_clear", value );

    /* (0x00000014 bits 1) field TMVP_INVALID_DPI_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_DPI_ID_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_INVALID_DPI_ID_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 1) field TMVP_INVALID_DPI_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_DPI_ID_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                               mgmt_fege_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_DPI_ID_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                               mgmt_fege_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_TMVP_INVALID_DPI_ID_I_poll", value );

    /* (0x00000014 bits 1) field TMVP_INVALID_DPI_ID_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                      mgmt_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_set_to_clear( mgmt_fege_buffer_t *b_ptr,
                                                                      mgmt_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_set_to_clear", value );

    /* (0x00000014 bits 0) field TMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    mgmt_fege_reg_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_MSK,
                                                      MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_OFF,
                                                      value);
}

static INLINE UINT32 mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_get( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 0) field TMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    reg_value = mgmt_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_MSK) >> MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                              mgmt_fege_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_poll( mgmt_fege_buffer_t *b_ptr,
                                                                              mgmt_fege_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_poll", value );

    /* (0x00000014 bits 0) field TMVP_FIFO_OVERFLOW_I of register PMC_MGMT_FEGE_TOP_REG_INT_STAT */
    return mgmt_fege_reg_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_MSK,
                                        (value<<MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size64
 * ==================================================================================
 */
static INLINE void mgmt_fege_field_TXLUT_VALID_set( mgmt_fege_buffer_t *b_ptr,
                                                    mgmt_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TXLUT_VALID_set( mgmt_fege_buffer_t *b_ptr,
                                                    mgmt_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VALID_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TXLUT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_TXLUT_VALID_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 13) field TXLUT_VALID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_TX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_MSK,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_TXLUT_VALID_get( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TXLUT_VALID_get( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VALID_get", N, 63);
    /* ((0x00000100 + (N) * 0x4) bits 13) field TXLUT_VALID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_TX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_MSK) >> MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_TXLUT_VALID_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 12) field TXLUT_VLAN_ID_REPLACE of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_TX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_MSK,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get( mgmt_fege_buffer_t *b_ptr,
                                                                mgmt_fege_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get", N, 63);
    /* ((0x00000100 + (N) * 0x4) bits 12) field TXLUT_VLAN_ID_REPLACE of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_TX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_MSK) >> MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_TXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_TXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                      mgmt_fege_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VLAN_ID_set", N, 63);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 11:0) field TXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    mgmt_fege_reg_TX_VLAN_LUT_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_MSK,
                                               MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF,
                                               value);
}

static INLINE UINT32 mgmt_fege_field_TXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_TXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                        mgmt_fege_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_TXLUT_VLAN_ID_get", N, 63);
    /* ((0x00000100 + (N) * 0x4) bits 11:0) field TXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_TX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_MSK) >> MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mgmt_fege_field_TXLUT_VLAN_ID_get", N, value );

    return value;
}
static INLINE void mgmt_fege_field_range_TXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mgmt_fege_field_range_TXLUT_VLAN_ID_set( mgmt_fege_buffer_t *b_ptr,
                                                            mgmt_fege_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_range_TXLUT_VLAN_ID_set", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_TXLUT_VLAN_ID_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_TXLUT_VLAN_ID_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_TXLUT_VLAN_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x4) bits 11:0) field TXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
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
        /* ((0x00000100 + (N) * 0x4) bits 11:0) field TXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
        mgmt_fege_reg_TX_VLAN_LUT_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF + subfield_offset),
                                                   MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mgmt_fege_field_range_TXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mgmt_fege_field_range_TXLUT_VLAN_ID_get( mgmt_fege_buffer_t *b_ptr,
                                                              mgmt_fege_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mgmt_fege_field_range_TXLUT_VLAN_ID_get", N, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mgmt_fege_field_range_TXLUT_VLAN_ID_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mgmt_fege_field_range_TXLUT_VLAN_ID_get", stop_bit, 11 );
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
    /* ((0x00000100 + (N) * 0x4) bits 11:0) field TXLUT_VLAN_ID of register PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT index N=0..63 */
    reg_value = mgmt_fege_reg_TX_VLAN_LUT_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_MSK)
                  >> MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_MSK, MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mgmt_fege_field_range_TXLUT_VLAN_ID_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGMT_FEGE_IO_INLINE_H */
