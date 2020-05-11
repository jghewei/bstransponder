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
 *     and register accessor functions for the enet block
 *****************************************************************************/
#ifndef _ENET_IO_INLINE_H
#define _ENET_IO_INLINE_H

#include "enet_loc.h"
#include "enet_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ENET_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for enet
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
    enet_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} enet_buffer_t;
static INLINE void enet_buffer_init( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void enet_buffer_init( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "enet_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void enet_buffer_flush( enet_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void enet_buffer_flush( enet_buffer_t *b_ptr )
{
    IOLOG( "enet_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 enet_reg_read( enet_buffer_t *b_ptr,
                                    enet_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 enet_reg_read( enet_buffer_t *b_ptr,
                                    enet_handle_t *h_ptr,
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
static INLINE void enet_reg_write( enet_buffer_t *b_ptr,
                                   enet_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_write( enet_buffer_t *b_ptr,
                                   enet_handle_t *h_ptr,
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

static INLINE void enet_field_set( enet_buffer_t *b_ptr,
                                   enet_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_set( enet_buffer_t *b_ptr,
                                   enet_handle_t *h_ptr,
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

static INLINE void enet_action_on_write_field_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_action_on_write_field_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
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

static INLINE void enet_burst_read( enet_buffer_t *b_ptr,
                                    enet_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void enet_burst_read( enet_buffer_t *b_ptr,
                                    enet_handle_t *h_ptr,
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

static INLINE void enet_burst_write( enet_buffer_t *b_ptr,
                                     enet_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void enet_burst_write( enet_buffer_t *b_ptr,
                                     enet_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE enet_poll( enet_buffer_t *b_ptr,
                                              enet_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enet_poll( enet_buffer_t *b_ptr,
                                              enet_handle_t *h_ptr,
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
 *  register access functions for enet
 * ==================================================================================
 */

static INLINE void enet_reg_MTSB_SW_RESET_write( enet_buffer_t *b_ptr,
                                                 enet_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_MTSB_SW_RESET_write( enet_buffer_t *b_ptr,
                                                 enet_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_MTSB_SW_RESET_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_MTSB_SW_RESET,
                    value);
}

static INLINE void enet_reg_MTSB_SW_RESET_field_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_MTSB_SW_RESET_field_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_reg_MTSB_SW_RESET_field_set", mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_MTSB_SW_RESET,
                    mask,
                    PMC_ENET120_TOP_REG_MTSB_SW_RESET_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_MTSB_SW_RESET_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_MTSB_SW_RESET_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_MTSB_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "enet_reg_MTSB_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void enet_reg_MTSB_LOW_POWER_MODE_write( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_MTSB_LOW_POWER_MODE_write( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_MTSB_LOW_POWER_MODE_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE,
                    value);
}

static INLINE void enet_reg_MTSB_LOW_POWER_MODE_field_set( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_MTSB_LOW_POWER_MODE_field_set( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_reg_MTSB_LOW_POWER_MODE_field_set", mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE,
                    mask,
                    PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_MTSB_LOW_POWER_MODE_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_MTSB_LOW_POWER_MODE_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE);

    IOLOG( "%s -> 0x%08x;", "enet_reg_MTSB_LOW_POWER_MODE_read", reg_value);
    return reg_value;
}

static INLINE void enet_reg_SUBSYSTEM_CTRL_write( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSYSTEM_CTRL_write( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_SUBSYSTEM_CTRL_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL,
                    value);
}

static INLINE void enet_reg_SUBSYSTEM_CTRL_field_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSYSTEM_CTRL_field_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_reg_SUBSYSTEM_CTRL_field_set", mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL,
                    mask,
                    PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_SUBSYSTEM_CTRL_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_SUBSYSTEM_CTRL_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL);

    IOLOG( "%s -> 0x%08x;", "enet_reg_SUBSYSTEM_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void enet_reg_ENET_DATAPATH_CFG_write( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_ENET_DATAPATH_CFG_write( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_ENET_DATAPATH_CFG_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG,
                    value);
}

static INLINE void enet_reg_ENET_DATAPATH_CFG_field_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_ENET_DATAPATH_CFG_field_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enet_reg_ENET_DATAPATH_CFG_field_set", mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG,
                    mask,
                    PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_ENET_DATAPATH_CFG_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_ENET_DATAPATH_CFG_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG);

    IOLOG( "%s -> 0x%08x;", "enet_reg_ENET_DATAPATH_CFG_read", reg_value);
    return reg_value;
}

static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0(N),
                    value);
}

static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( enet_buffer_t *b_ptr,
                                                                    enet_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( enet_buffer_t *b_ptr,
                                                                    enet_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set", N, mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0(N),
                    mask,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( enet_buffer_t *b_ptr,
                                                                 enet_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( enet_buffer_t *b_ptr,
                                                                 enet_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write", value );
    enet_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1(N),
                    value);
}

static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( enet_buffer_t *b_ptr,
                                                                    enet_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( enet_buffer_t *b_ptr,
                                                                    enet_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set", N, mask, ofs, value );
    enet_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1(N),
                    mask,
                    PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( enet_buffer_t *b_ptr,
                                                                 enet_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( enet_buffer_t *b_ptr,
                                                                 enet_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 enet_reg_ENET_SS_INT_SMRY_PGE_0_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_ENET_SS_INT_SMRY_PGE_0_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0);

    IOLOG( "%s -> 0x%08x;", "enet_reg_ENET_SS_INT_SMRY_PGE_0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enet_reg_ENET_SS_INT_SMRY_PG_1_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_reg_ENET_SS_INT_SMRY_PG_1_read( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enet_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1);

    IOLOG( "%s -> 0x%08x;", "enet_reg_ENET_SS_INT_SMRY_PG_1_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void enet_field_ETRANS120_SW_RST_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS120_SW_RST_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS120_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_ETRANS120_SW_RST_set", value );

    /* (0x00000000 bits 4) field ETRANS120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    enet_reg_MTSB_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_ETRANS120_SW_RST_MSK,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_ETRANS120_SW_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_field_ETRANS120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field ETRANS120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    reg_value = enet_reg_MTSB_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_SW_RESET_BIT_ETRANS120_SW_RST_MSK) >> ENET120_TOP_REG_MTSB_SW_RESET_BIT_ETRANS120_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS120_SW_RST_get", value );

    return value;
}
static INLINE void enet_field_EPMM120_SW_RST_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM120_SW_RST_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM120_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EPMM120_SW_RST_set", value );

    /* (0x00000000 bits 3) field EPMM120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    enet_reg_MTSB_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EPMM120_SW_RST_MSK,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EPMM120_SW_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_field_EPMM120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field EPMM120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    reg_value = enet_reg_MTSB_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_SW_RESET_BIT_EPMM120_SW_RST_MSK) >> ENET120_TOP_REG_MTSB_SW_RESET_BIT_EPMM120_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM120_SW_RST_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_E_SW_RST_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_E_SW_RST_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_E_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_E_SW_RST_set", value );

    /* (0x00000000 bits 2) field EMPEI120_E_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    enet_reg_MTSB_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_E_SW_RST_MSK,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_E_SW_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_field_EMPEI120_E_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_E_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field EMPEI120_E_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    reg_value = enet_reg_MTSB_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_E_SW_RST_MSK) >> ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_E_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_E_SW_RST_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_I_SW_RST_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_I_SW_RST_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_I_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_I_SW_RST_set", value );

    /* (0x00000000 bits 1) field EMPEI120_I_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    enet_reg_MTSB_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_I_SW_RST_MSK,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_I_SW_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_field_EMPEI120_I_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_I_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field EMPEI120_I_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    reg_value = enet_reg_MTSB_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_I_SW_RST_MSK) >> ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMPEI120_I_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_I_SW_RST_get", value );

    return value;
}
static INLINE void enet_field_EMAC120_SW_RST_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMAC120_SW_RST_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMAC120_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMAC120_SW_RST_set", value );

    /* (0x00000000 bits 0) field EMAC120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    enet_reg_MTSB_SW_RESET_field_set( b_ptr,
                                      h_ptr,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMAC120_SW_RST_MSK,
                                      ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMAC120_SW_RST_OFF,
                                      value);
}

static INLINE UINT32 enet_field_EMAC120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMAC120_SW_RST_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field EMAC120_SW_RST of register PMC_ENET120_TOP_REG_MTSB_SW_RESET */
    reg_value = enet_reg_MTSB_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMAC120_SW_RST_MSK) >> ENET120_TOP_REG_MTSB_SW_RESET_BIT_EMAC120_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMAC120_SW_RST_get", value );

    return value;
}
static INLINE void enet_field_ETRANS120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS120_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_ETRANS120_RAM_LOWPWR_set", value );

    /* (0x00000004 bits 12) field ETRANS120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_RAM_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_RAM_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_ETRANS120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 12) field ETRANS120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_RAM_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS120_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EPMM120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM120_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EPMM120_RAM_LOWPWR_set", value );

    /* (0x00000004 bits 11) field EPMM120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_RAM_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_RAM_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EPMM120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 11) field EPMM120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_RAM_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM120_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_E_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_E_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_E_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_E_RAM_LOWPWR_set", value );

    /* (0x00000004 bits 10) field EMPEI120_E_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_RAM_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_RAM_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMPEI120_E_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_E_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 10) field EMPEI120_E_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_RAM_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_E_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_I_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_I_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_I_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_I_RAM_LOWPWR_set", value );

    /* (0x00000004 bits 9) field EMPEI120_I_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_RAM_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_RAM_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMPEI120_I_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_I_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 9) field EMPEI120_I_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_RAM_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_I_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMAC120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMAC120_RAM_LOWPWR_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMAC120_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMAC120_RAM_LOWPWR_set", value );

    /* (0x00000004 bits 8) field EMAC120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_RAM_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_RAM_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMAC120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMAC120_RAM_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field EMAC120_RAM_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_RAM_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMAC120_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_ETRANS120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS120_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_ETRANS120_LOWPWR_set", value );

    /* (0x00000004 bits 4) field ETRANS120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_ETRANS120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4) field ETRANS120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS120_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EPMM120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM120_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EPMM120_LOWPWR_set", value );

    /* (0x00000004 bits 3) field EPMM120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EPMM120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3) field EPMM120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EPMM120_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM120_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_E_LOWPWR_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_E_LOWPWR_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_E_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_E_LOWPWR_set", value );

    /* (0x00000004 bits 2) field EMPEI120_E_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMPEI120_E_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_E_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2) field EMPEI120_E_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_E_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_E_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMPEI120_I_LOWPWR_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI120_I_LOWPWR_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI120_I_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMPEI120_I_LOWPWR_set", value );

    /* (0x00000004 bits 1) field EMPEI120_I_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMPEI120_I_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI120_I_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field EMPEI120_I_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMPEI120_I_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI120_I_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_EMAC120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMAC120_LOWPWR_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMAC120_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_EMAC120_LOWPWR_set", value );

    /* (0x00000004 bits 0) field EMAC120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    enet_reg_MTSB_LOW_POWER_MODE_field_set( b_ptr,
                                            h_ptr,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_LOWPWR_MSK,
                                            ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_LOWPWR_OFF,
                                            value);
}

static INLINE UINT32 enet_field_EMAC120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMAC120_LOWPWR_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field EMAC120_LOWPWR of register PMC_ENET120_TOP_REG_MTSB_LOW_POWER_MODE */
    reg_value = enet_reg_MTSB_LOW_POWER_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_LOWPWR_MSK) >> ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_EMAC120_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMAC120_LOWPWR_get", value );

    return value;
}
static INLINE void enet_field_SS_PG_OVERRIDE_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_SS_PG_OVERRIDE_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_SS_PG_OVERRIDE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_SS_PG_OVERRIDE_set", value );

    /* (0x00000008 bits 2) field SS_PG_OVERRIDE of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    enet_reg_SUBSYSTEM_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_PG_OVERRIDE_MSK,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_PG_OVERRIDE_OFF,
                                       value);
}

static INLINE UINT32 enet_field_SS_PG_OVERRIDE_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_SS_PG_OVERRIDE_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 2) field SS_PG_OVERRIDE of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    reg_value = enet_reg_SUBSYSTEM_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_PG_OVERRIDE_MSK) >> ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_PG_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_SS_PG_OVERRIDE_get", value );

    return value;
}
static INLINE void enet_field_SS_CLR_INT_set( enet_buffer_t *b_ptr,
                                              enet_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_SS_CLR_INT_set( enet_buffer_t *b_ptr,
                                              enet_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_SS_CLR_INT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_SS_CLR_INT_set", value );

    /* (0x00000008 bits 1) field SS_CLR_INT of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    enet_reg_SUBSYSTEM_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_CLR_INT_MSK,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_CLR_INT_OFF,
                                       value);
}

static INLINE UINT32 enet_field_SS_CLR_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_SS_CLR_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field SS_CLR_INT of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    reg_value = enet_reg_SUBSYSTEM_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_CLR_INT_MSK) >> ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_CLR_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_SS_CLR_INT_get", value );

    return value;
}
static INLINE void enet_field_SS_WCI_MODE_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_SS_WCI_MODE_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_SS_WCI_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_SS_WCI_MODE_set", value );

    /* (0x00000008 bits 0) field SS_WCI_MODE of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    enet_reg_SUBSYSTEM_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_WCI_MODE_MSK,
                                       ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_WCI_MODE_OFF,
                                       value);
}

static INLINE UINT32 enet_field_SS_WCI_MODE_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_SS_WCI_MODE_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field SS_WCI_MODE of register PMC_ENET120_TOP_REG_SUBSYSTEM_CTRL */
    reg_value = enet_reg_SUBSYSTEM_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_WCI_MODE_MSK) >> ENET120_TOP_REG_SUBSYSTEM_CTRL_BIT_SS_WCI_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_SS_WCI_MODE_get", value );

    return value;
}
static INLINE void enet_field_LINK_100G_EN_set( enet_buffer_t *b_ptr,
                                                enet_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_LINK_100G_EN_set( enet_buffer_t *b_ptr,
                                                enet_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_LINK_100G_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enet_field_LINK_100G_EN_set", value );

    /* (0x0000000c bits 15) field LINK_100G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    enet_reg_ENET_DATAPATH_CFG_field_set( b_ptr,
                                          h_ptr,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_100G_EN_MSK,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_100G_EN_OFF,
                                          value);
}

static INLINE UINT32 enet_field_LINK_100G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_LINK_100G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 15) field LINK_100G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    reg_value = enet_reg_ENET_DATAPATH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_100G_EN_MSK) >> ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_100G_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_LINK_100G_EN_get", value );

    return value;
}
static INLINE void enet_field_LINK_40G_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_LINK_40G_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_LINK_40G_EN_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enet_field_LINK_40G_EN_set", value );

    /* (0x0000000c bits 14:12) bits 0:2 use field LINK_40G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    enet_reg_ENET_DATAPATH_CFG_field_set( b_ptr,
                                          h_ptr,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_MSK,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF,
                                          value);
}

static INLINE UINT32 enet_field_LINK_40G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_LINK_40G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 14:12) bits 0:2 use field LINK_40G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    reg_value = enet_reg_ENET_DATAPATH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_MSK) >> ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_LINK_40G_EN_get", value );

    return value;
}
static INLINE void enet_field_range_LINK_40G_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_LINK_40G_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_LINK_40G_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_LINK_40G_EN_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_LINK_40G_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 14:12) bits 0:2 use field LINK_40G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
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
        /* (0x0000000c bits 14:12) bits 0:2 use field LINK_40G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
        enet_reg_ENET_DATAPATH_CFG_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF + subfield_offset),
                                              ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_LINK_40G_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_LINK_40G_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_LINK_40G_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_LINK_40G_EN_get", stop_bit, 2 );
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
    /* (0x0000000c bits 14:12) bits 0:2 use field LINK_40G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    reg_value = enet_reg_ENET_DATAPATH_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_MSK)
                  >> ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_MSK, ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_40G_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_LINK_40G_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_field_LINK_10G_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_LINK_10G_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_LINK_10G_EN_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "enet_field_LINK_10G_EN_set", value );

    /* (0x0000000c bits 11:0) bits 0:11 use field LINK_10G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    enet_reg_ENET_DATAPATH_CFG_field_set( b_ptr,
                                          h_ptr,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_MSK,
                                          ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF,
                                          value);
}

static INLINE UINT32 enet_field_LINK_10G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_LINK_10G_EN_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 11:0) bits 0:11 use field LINK_10G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    reg_value = enet_reg_ENET_DATAPATH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_MSK) >> ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_LINK_10G_EN_get", value );

    return value;
}
static INLINE void enet_field_range_LINK_10G_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_LINK_10G_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_LINK_10G_EN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_LINK_10G_EN_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_LINK_10G_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 11:0) bits 0:11 use field LINK_10G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
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
        /* (0x0000000c bits 11:0) bits 0:11 use field LINK_10G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
        enet_reg_ENET_DATAPATH_CFG_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF + subfield_offset),
                                              ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_LINK_10G_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_LINK_10G_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_LINK_10G_EN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_LINK_10G_EN_get", stop_bit, 11 );
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
    /* (0x0000000c bits 11:0) bits 0:11 use field LINK_10G_EN of register PMC_ENET120_TOP_REG_ENET_DATAPATH_CFG */
    reg_value = enet_reg_ENET_DATAPATH_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_MSK)
                  >> ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_MSK, ENET120_TOP_REG_ENET_DATAPATH_CFG_BIT_LINK_10G_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_LINK_10G_EN_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void enet_field_EPMM_MPMO_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_MPMO_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_MPMO_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_MPMO_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_MPMO_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 24) field EPMM_MPMO_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMO_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMO_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_MPMO_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_MPMO_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_MPMO_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 24) field EPMM_MPMO_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMO_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMO_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_MPMO_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EPMM_MPMA_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_MPMA_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_MPMA_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_MPMA_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_MPMA_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 23) field EPMM_MPMA_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMA_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMA_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_MPMA_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_MPMA_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_MPMA_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 23) field EPMM_MPMA_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMA_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_MPMA_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_MPMA_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EPMM_EXCO4M_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_EXCO4M_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_EXCO4M_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_EXCO4M_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_EXCO4M_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 22) field EPMM_EXCO4M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO4M_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO4M_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_EXCO4M_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_EXCO4M_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_EXCO4M_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 22) field EPMM_EXCO4M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO4M_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO4M_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_EXCO4M_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EPMM_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_EXCO3M_INT_EN_set", N, 3);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_EXCO3M_INT_EN_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_EXCO3M_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 21:19) field EPMM_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_EXCO3M_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 21:19) field EPMM_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_EXCO3M_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_range_EPMM_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_EPMM_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_EPMM_EXCO3M_INT_EN_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_EXCO3M_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_EXCO3M_INT_EN_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_EXCO3M_INT_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000020 + (N) * 8) bits 21:19) field EPMM_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
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
        /* ((0x00000020 + (N) * 8) bits 21:19) field EPMM_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF + subfield_offset),
                                                         ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_EPMM_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_EPMM_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_EPMM_EXCO3M_INT_EN_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_EXCO3M_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_EXCO3M_INT_EN_get", stop_bit, 2 );
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
    /* ((0x00000020 + (N) * 8) bits 21:19) field EPMM_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_MSK)
                  >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_MSK, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_EXCO3M_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_EXCO3M_INT_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_field_EPMM_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_GSUP_INT_EN_set", N, 3);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_GSUP_INT_EN_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_GSUP_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 18:7) field EPMM_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_GSUP_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 18:7) field EPMM_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_GSUP_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_range_EPMM_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_EPMM_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_EPMM_GSUP_INT_EN_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_GSUP_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_GSUP_INT_EN_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_GSUP_INT_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000020 + (N) * 8) bits 18:7) field EPMM_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
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
        /* ((0x00000020 + (N) * 8) bits 18:7) field EPMM_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF + subfield_offset),
                                                         ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_EPMM_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_EPMM_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_EPMM_GSUP_INT_EN_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_GSUP_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_GSUP_INT_EN_get", stop_bit, 11 );
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
    /* ((0x00000020 + (N) * 8) bits 18:7) field EPMM_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_MSK)
                  >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_MSK, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_GSUP_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_GSUP_INT_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_field_EPMM_TOP_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EPMM_TOP_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_TOP_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EPMM_TOP_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EPMM_TOP_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 6) field EPMM_TOP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_TOP_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_TOP_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EPMM_TOP_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_TOP_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EPMM_TOP_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 6) field EPMM_TOP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_TOP_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EPMM_TOP_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EPMM_TOP_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_ECLASS_E_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ECLASS_E_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ECLASS_E_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ECLASS_E_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ECLASS_E_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 5) field ECLASS_E_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_E_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_E_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ECLASS_E_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ECLASS_E_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ECLASS_E_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 5) field ECLASS_E_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_E_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_E_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ECLASS_E_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EMPEI_E_INT_EN_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI_E_INT_EN_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMPEI_E_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI_E_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EMPEI_E_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 4) field EMPEI_E_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_E_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_E_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EMPEI_E_INT_EN_get( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI_E_INT_EN_get( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMPEI_E_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 4) field EMPEI_E_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_E_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_E_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EMPEI_E_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_ECLASS_I_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ECLASS_I_INT_EN_set( enet_buffer_t *b_ptr,
                                                   enet_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ECLASS_I_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ECLASS_I_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ECLASS_I_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 3) field ECLASS_I_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_I_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_I_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ECLASS_I_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ECLASS_I_INT_EN_get( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ECLASS_I_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 3) field ECLASS_I_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_I_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ECLASS_I_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ECLASS_I_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EMPEI_I_INT_EN_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMPEI_I_INT_EN_set( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMPEI_I_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMPEI_I_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EMPEI_I_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 2) field EMPEI_I_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_I_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_I_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EMPEI_I_INT_EN_get( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI_I_INT_EN_get( enet_buffer_t *b_ptr,
                                                    enet_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMPEI_I_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 2) field EMPEI_I_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_I_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMPEI_I_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EMPEI_I_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_ETIME_INT_EN_set( enet_buffer_t *b_ptr,
                                                enet_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETIME_INT_EN_set( enet_buffer_t *b_ptr,
                                                enet_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETIME_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETIME_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ETIME_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 1) field ETIME_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ETIME_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ETIME_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ETIME_INT_EN_get( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETIME_INT_EN_get( enet_buffer_t *b_ptr,
                                                  enet_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETIME_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 1) field ETIME_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ETIME_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_ETIME_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ETIME_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_EMAC_INT_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_EMAC_INT_EN_set( enet_buffer_t *b_ptr,
                                               enet_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMAC_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_EMAC_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_EMAC_INT_EN_set", N, value );

    /* ((0x00000020 + (N) * 8) bits 0) field EMAC_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMAC_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMAC_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_EMAC_INT_EN_get( enet_buffer_t *b_ptr,
                                                 enet_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMAC_INT_EN_get( enet_buffer_t *b_ptr,
                                                 enet_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_EMAC_INT_EN_get", N, 3);
    /* ((0x00000020 + (N) * 8) bits 0) field EMAC_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMAC_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_0_BIT_EMAC_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_EMAC_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_ETRANS_EXCO4M_INT_EN_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS_EXCO4M_INT_EN_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_EXCO4M_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS_EXCO4M_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ETRANS_EXCO4M_INT_EN_set", N, value );

    /* ((0x00000024 + (N) * 8) bits 16) field ETRANS_EXCO4M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO4M_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO4M_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ETRANS_EXCO4M_INT_EN_get( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_EXCO4M_INT_EN_get( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_EXCO4M_INT_EN_get", N, 3);
    /* ((0x00000024 + (N) * 8) bits 16) field ETRANS_EXCO4M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO4M_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO4M_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ETRANS_EXCO4M_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_ETRANS_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_EXCO3M_INT_EN_set", N, 3);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS_EXCO3M_INT_EN_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ETRANS_EXCO3M_INT_EN_set", N, value );

    /* ((0x00000024 + (N) * 8) bits 15:13) field ETRANS_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ETRANS_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                          enet_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_EXCO3M_INT_EN_get", N, 3);
    /* ((0x00000024 + (N) * 8) bits 15:13) field ETRANS_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ETRANS_EXCO3M_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_range_ETRANS_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_ETRANS_EXCO3M_INT_EN_set( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_ETRANS_EXCO3M_INT_EN_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_EXCO3M_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_EXCO3M_INT_EN_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_EXCO3M_INT_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000024 + (N) * 8) bits 15:13) field ETRANS_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
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
        /* ((0x00000024 + (N) * 8) bits 15:13) field ETRANS_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF + subfield_offset),
                                                         ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_ETRANS_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_ETRANS_EXCO3M_INT_EN_get( enet_buffer_t *b_ptr,
                                                                enet_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_ETRANS_EXCO3M_INT_EN_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_EXCO3M_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_EXCO3M_INT_EN_get", stop_bit, 2 );
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
    /* ((0x00000024 + (N) * 8) bits 15:13) field ETRANS_EXCO3M_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_MSK)
                  >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_MSK, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_EXCO3M_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_EXCO3M_INT_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_field_ETRANS_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                      enet_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_GSUP_INT_EN_set", N, 3);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS_GSUP_INT_EN_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ETRANS_GSUP_INT_EN_set", N, value );

    /* ((0x00000024 + (N) * 8) bits 12:1) field ETRANS_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ETRANS_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                        enet_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_GSUP_INT_EN_get", N, 3);
    /* ((0x00000024 + (N) * 8) bits 12:1) field ETRANS_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ETRANS_GSUP_INT_EN_get", N, value );

    return value;
}
static INLINE void enet_field_range_ETRANS_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_range_ETRANS_GSUP_INT_EN_set( enet_buffer_t *b_ptr,
                                                            enet_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_ETRANS_GSUP_INT_EN_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_GSUP_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_GSUP_INT_EN_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_GSUP_INT_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000024 + (N) * 8) bits 12:1) field ETRANS_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
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
        /* ((0x00000024 + (N) * 8) bits 12:1) field ETRANS_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF + subfield_offset),
                                                         ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enet_field_range_ETRANS_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_ETRANS_GSUP_INT_EN_get( enet_buffer_t *b_ptr,
                                                              enet_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_range_ETRANS_GSUP_INT_EN_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_GSUP_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_GSUP_INT_EN_get", stop_bit, 11 );
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
    /* ((0x00000024 + (N) * 8) bits 12:1) field ETRANS_GSUP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_MSK)
                  >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_MSK, ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_GSUP_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_GSUP_INT_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void enet_field_ETRANS_TOP_INT_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enet_field_ETRANS_TOP_INT_EN_set( enet_buffer_t *b_ptr,
                                                     enet_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_TOP_INT_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enet_field_ETRANS_TOP_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "enet_field_ETRANS_TOP_INT_EN_set", N, value );

    /* ((0x00000024 + (N) * 8) bits 0) field ETRANS_TOP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_TOP_INT_EN_MSK,
                                                     ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_TOP_INT_EN_OFF,
                                                     value);
}

static INLINE UINT32 enet_field_ETRANS_TOP_INT_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_TOP_INT_EN_get( enet_buffer_t *b_ptr,
                                                       enet_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "enet_field_ETRANS_TOP_INT_EN_get", N, 3);
    /* ((0x00000024 + (N) * 8) bits 0) field ETRANS_TOP_INT_EN of register PMC_ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1 index N=0..3 */
    reg_value = enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_TOP_INT_EN_MSK) >> ENET120_TOP_REG_SUBSTSEM_INT_EN_PAGE_1_BIT_ETRANS_TOP_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "enet_field_ETRANS_TOP_INT_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 enet_field_EPMM_MPMO_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_MPMO_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 24) field EPMM_MPMO_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_MPMO_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_MPMO_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_MPMO_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EPMM_MPMA_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_MPMA_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 23) field EPMM_MPMA_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_MPMA_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_MPMA_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_MPMA_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EPMM_EXCO4M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_EXCO4M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 22) field EPMM_EXCO4M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO4M_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO4M_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_EXCO4M_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EPMM_EXCO3M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_EXCO3M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 21:19) bits 0:2 use field EPMM_EXCO3M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_EXCO3M_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_range_EPMM_EXCO3M_INT_get( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_EPMM_EXCO3M_INT_get( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_EXCO3M_INT_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_EXCO3M_INT_get", stop_bit, 2 );
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
    /* (0x00000010 bits 21:19) bits 0:2 use field EPMM_EXCO3M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_MSK)
                  >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_MSK, ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_EXCO3M_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_EXCO3M_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_field_EPMM_GSUP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_GSUP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 18:7) bits 0:11 use field EPMM_GSUP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_GSUP_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_range_EPMM_GSUP_INT_get( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_EPMM_GSUP_INT_get( enet_buffer_t *b_ptr,
                                                         enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_EPMM_GSUP_INT_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_EPMM_GSUP_INT_get", stop_bit, 11 );
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
    /* (0x00000010 bits 18:7) bits 0:11 use field EPMM_GSUP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_MSK)
                  >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_MSK, ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_GSUP_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_EPMM_GSUP_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_field_EPMM_TOP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EPMM_TOP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 6) field EPMM_TOP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_TOP_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EPMM_TOP_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EPMM_TOP_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_ECLASS_E_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ECLASS_E_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 5) field ECLASS_E_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ECLASS_E_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ECLASS_E_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ECLASS_E_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EMPEI_E_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI_E_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 4) field EMPEI_E_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMPEI_E_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMPEI_E_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI_E_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_ECLASS_I_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ECLASS_I_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 3) field ECLASS_I_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ECLASS_I_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ECLASS_I_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ECLASS_I_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EMPEI_I_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMPEI_I_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 2) field EMPEI_I_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMPEI_I_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMPEI_I_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMPEI_I_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_ETIME_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETIME_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 1) field ETIME_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ETIME_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_ETIME_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETIME_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_EMAC_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_EMAC_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field EMAC_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PGE_0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMAC_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PGE_0_BIT_EMAC_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_EMAC_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_ETRANS_EXCO4M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_EXCO4M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 22) field ETRANS_EXCO4M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO4M_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO4M_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS_EXCO4M_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_ETRANS_EXCO3M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_EXCO3M_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 21:19) bits 0:2 use field ETRANS_EXCO3M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS_EXCO3M_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_range_ETRANS_EXCO3M_INT_get( enet_buffer_t *b_ptr,
                                                             enet_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_ETRANS_EXCO3M_INT_get( enet_buffer_t *b_ptr,
                                                             enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_EXCO3M_INT_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_EXCO3M_INT_get", stop_bit, 2 );
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
    /* (0x00000014 bits 21:19) bits 0:2 use field ETRANS_EXCO3M_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_MSK)
                  >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_MSK, ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_EXCO3M_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_EXCO3M_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_field_ETRANS_GSUP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_GSUP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 12:1) bits 0:11 use field ETRANS_GSUP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS_GSUP_INT_get", value );

    return value;
}
static INLINE UINT32 enet_field_range_ETRANS_GSUP_INT_get( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_range_ETRANS_GSUP_INT_get( enet_buffer_t *b_ptr,
                                                           enet_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enet_field_range_ETRANS_GSUP_INT_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enet_field_range_ETRANS_GSUP_INT_get", stop_bit, 11 );
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
    /* (0x00000014 bits 12:1) bits 0:11 use field ETRANS_GSUP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_MSK)
                  >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_MSK, ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_GSUP_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enet_field_range_ETRANS_GSUP_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enet_field_ETRANS_TOP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enet_field_ETRANS_TOP_INT_get( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 0) field ETRANS_TOP_INT of register PMC_ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1 */
    reg_value = enet_reg_ENET_SS_INT_SMRY_PG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_TOP_INT_MSK) >> ENET120_TOP_REG_ENET_SS_INT_SMRY_PG_1_BIT_ETRANS_TOP_INT_OFF;
    IOLOG( "%s -> 0x%08x", "enet_field_ETRANS_TOP_INT_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ENET_IO_INLINE_H */
