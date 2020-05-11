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
 *     and register accessor functions for the odu_ohp_core block
 *****************************************************************************/
#ifndef _ODU_OHP_CORE_IO_INLINE_H
#define _ODU_OHP_CORE_IO_INLINE_H

#include "odu_ohp_loc.h"
#include "odu_ohp_core_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODU_OHP_CORE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for odu_ohp_core
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
    odu_ohp_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} odu_ohp_core_buffer_t;
static INLINE void odu_ohp_core_buffer_init( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_buffer_init( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "odu_ohp_core_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void odu_ohp_core_buffer_flush( odu_ohp_core_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_buffer_flush( odu_ohp_core_buffer_t *b_ptr )
{
    IOLOG( "odu_ohp_core_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 odu_ohp_core_reg_read( odu_ohp_core_buffer_t *b_ptr,
                                            odu_ohp_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_read( odu_ohp_core_buffer_t *b_ptr,
                                            odu_ohp_handle_t *h_ptr,
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
static INLINE void odu_ohp_core_reg_write( odu_ohp_core_buffer_t *b_ptr,
                                           odu_ohp_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_write( odu_ohp_core_buffer_t *b_ptr,
                                           odu_ohp_handle_t *h_ptr,
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

static INLINE void odu_ohp_core_field_set( odu_ohp_core_buffer_t *b_ptr,
                                           odu_ohp_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_set( odu_ohp_core_buffer_t *b_ptr,
                                           odu_ohp_handle_t *h_ptr,
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

static INLINE void odu_ohp_core_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
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

static INLINE void odu_ohp_core_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                            odu_ohp_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                            odu_ohp_handle_t *h_ptr,
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

static INLINE void odu_ohp_core_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                             odu_ohp_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                             odu_ohp_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_poll( odu_ohp_core_buffer_t *b_ptr,
                                                      odu_ohp_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_poll( odu_ohp_core_buffer_t *b_ptr,
                                                      odu_ohp_handle_t *h_ptr,
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
 *  register access functions for odu_ohp_core
 * ==================================================================================
 */

static INLINE void odu_ohp_core_reg_OHP_CFG_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_OHP_CFG_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_OHP_CFG_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_OHP_CFG_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_OHP_CFG_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_OHP_CFG_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_OHP_CFG_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_OHP_CFG_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_OHP_CFG_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_OHP_CFG_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_OHP_CFG_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_OHP_CFG_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_OHP_CFG_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGA_SA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_SA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_STGA_SA_UID_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGA_SA_UID,
                            value);
}

static INLINE void odu_ohp_core_reg_STGA_SA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_SA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGA_SA_UID_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGA_SA_UID,
                            mask,
                            PMC_OHP_CORE_REG_STGA_SA_UID_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_STGA_SA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGA_SA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGA_SA_UID);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGA_SA_UID_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGB_SA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_SA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_STGB_SA_UID_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGB_SA_UID,
                            value);
}

static INLINE void odu_ohp_core_reg_STGB_SA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_SA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGB_SA_UID_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGB_SA_UID,
                            mask,
                            PMC_OHP_CORE_REG_STGB_SA_UID_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_STGB_SA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGB_SA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGB_SA_UID);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGB_SA_UID_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGA_DA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_DA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_STGA_DA_UID_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGA_DA_UID,
                            value);
}

static INLINE void odu_ohp_core_reg_STGA_DA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_DA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGA_DA_UID_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGA_DA_UID,
                            mask,
                            PMC_OHP_CORE_REG_STGA_DA_UID_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_STGA_DA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGA_DA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGA_DA_UID);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGA_DA_UID_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGB_DA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_DA_UID_write( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_STGB_DA_UID_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGB_DA_UID,
                            value);
}

static INLINE void odu_ohp_core_reg_STGB_DA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_DA_UID_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGB_DA_UID_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGB_DA_UID,
                            mask,
                            PMC_OHP_CORE_REG_STGB_DA_UID_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_STGB_DA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGB_DA_UID_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGB_DA_UID);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGB_DA_UID_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_PWR_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_RXFA_PWR_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_PWR_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_RXFA_PWR_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_PWR_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_RXFA_PWR_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_PWR_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_PWR_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_RXFB_PWR_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_PWR_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_RXFB_PWR_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_PWR_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_RXFB_PWR_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_PWR_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_TXFA_PWR_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_PWR_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_TXFA_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_PWR_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_PWR_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_TXFA_PWR_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_TXFA_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFA_PWR_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFA_PWR_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_PWR_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_TXFB_PWR_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_PWR_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_TXFB_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_PWR_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_PWR_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_PWR_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_TXFB_PWR_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_TXFB_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_PWR_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFB_PWR_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFB_PWR_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_OHP_ENET_IO_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_OHP_ENET_IO_CTRL_write( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_OHP_ENET_IO_CTRL_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL,
                            value);
}

static INLINE void odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL,
                            mask,
                            PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_OHP_ENET_IO_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFA_GCC0_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_GCC0_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFA_GCC0_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFA_GCC0_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC0_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC0_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFA_GCC0_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFA_GCC0_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFA_GCC1_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_GCC1_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFA_GCC1_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFA_GCC1_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC1_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC1_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFA_GCC1_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFA_GCC1_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFA_GCC2_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_GCC2_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFA_GCC2_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFA_GCC2_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC2_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_GCC2_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFA_GCC2_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFA_GCC2_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFB_GCC0_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_GCC0_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFB_GCC0_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFB_GCC0_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC0_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC0_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFB_GCC0_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFB_GCC0_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFB_GCC1_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_GCC1_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFB_GCC1_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFB_GCC1_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC1_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC1_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFB_GCC1_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFB_GCC1_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write", 3, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFB_GCC2_MASK(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_GCC2_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFB_GCC2_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read", ofs, len, 3 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFB_GCC2_MASK(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC2_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_GCC2_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFB_GCC2_MASK(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFB_GCC2_MASK_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_STGA_CH_EN_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_STGA_CH_EN_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_STGA_CH_EN(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGA_CH_EN_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGA_CH_EN(N),
                            mask,
                            PMC_OHP_CORE_REG_STGA_CH_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGA_CH_EN_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_STGA_CH_EN_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_STGA_CH_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_STGA_CH_EN_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_STGA_CH_EN_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGA_CH_EN_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGA_CH_EN(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGA_CH_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_STGB_CH_EN_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_STGB_CH_EN_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_STGB_CH_EN(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_STGB_CH_EN_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_STGB_CH_EN(N),
                            mask,
                            PMC_OHP_CORE_REG_STGB_CH_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_STGB_CH_EN_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_STGB_CH_EN_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_STGB_CH_EN(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_STGB_CH_EN_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_STGB_CH_EN_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_STGB_CH_EN_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_STGB_CH_EN(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_STGB_CH_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_OFFSET_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_OFFSET_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFA_OFFSET(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_OFFSET_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFA_OFFSET(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFA_OFFSET_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_OFFSET_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFA_OFFSET_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFA_OFFSET(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFA_OFFSET_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFA_OFFSET_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_OFFSET_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFA_OFFSET(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFA_OFFSET_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_OFFSET_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_OFFSET_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_CONFIG,
                              PMC_OHP_CORE_REG_TXFB_OFFSET(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_OFFSET_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_OHP_CORE_REG_TXFB_OFFSET(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFB_OFFSET_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_OFFSET_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_TXFB_OFFSET_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_OHP_CORE_REG_TXFB_OFFSET(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_TXFB_OFFSET_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_TXFB_OFFSET_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_OFFSET_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_OHP_CORE_REG_TXFB_OFFSET(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_TXFB_OFFSET_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_SCH_RXFA_CFG_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_SCH_RXFA_CFG_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_SCH_RXFA_CFG_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_SCH_RXFA_CFG,
                            value);
}

static INLINE void odu_ohp_core_reg_SCH_RXFA_CFG_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_SCH_RXFA_CFG_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_SCH_RXFA_CFG_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_SCH_RXFA_CFG,
                            mask,
                            PMC_OHP_CORE_REG_SCH_RXFA_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_SCH_RXFA_CFG_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_SCH_RXFA_CFG_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_SCH_RXFA_CFG);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_SCH_RXFA_CFG_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_SCH_RXFB_CFG_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_SCH_RXFB_CFG_write( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_SCH_RXFB_CFG_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_SCH_RXFB_CFG,
                            value);
}

static INLINE void odu_ohp_core_reg_SCH_RXFB_CFG_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_SCH_RXFB_CFG_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_SCH_RXFB_CFG_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_SCH_RXFB_CFG,
                            mask,
                            PMC_OHP_CORE_REG_SCH_RXFB_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_SCH_RXFB_CFG_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_SCH_RXFB_CFG_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_SCH_RXFB_CFG);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_SCH_RXFB_CFG_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2,
                            mask,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2,
                            mask,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_UID_FAIL_EN_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_UID_FAIL_EN_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN,
                            mask,
                            PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_UID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_UID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_UID_FAIL_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN,
                            mask,
                            PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN,
                            mask,
                            PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN,
                            mask,
                            PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN,
                            mask,
                            PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT0,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_TXDLY_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_TXDLY_INT0,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT1,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_TXDLY_INT1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_TXDLY_INT1,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFA_TXDLY_INT2,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_TXDLY_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_TXDLY_INT2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_TXDLY_INT2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFA_TXDLY_INT2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_TXDLY_INT2,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT0_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT0_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT0,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT0,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT0_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT0);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT0_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_TXDLY_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_TXDLY_INT0,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT1_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT1_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT1,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT1,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT1_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT1);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT1_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_TXDLY_INT1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_TXDLY_INT1,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT2_write( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT2_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT2,
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXFB_TXDLY_INT2,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_TXDLY_INT2_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_TXDLY_INT2);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_TXDLY_INT2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT2_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXFB_TXDLY_INT2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_TXDLY_INT2,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_UID_FAIL_INT_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_UID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                  odu_ohp_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_UID_FAIL_INT_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_UID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_UID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_UID_FAIL_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_UID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_UID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXMAC_UID_FAIL_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                     odu_ohp_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                     odu_ohp_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                              odu_ohp_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                              odu_ohp_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_PKTERR_INT_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_PKTERR_INT,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_PKTERR_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_PKTERR_INT_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXMAC_PKTERR_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_PKTERR_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_PKTERR_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_PKTERR_INT);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_PKTERR_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_PKTERR_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_PKTERR_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXMAC_PKTERR_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXMAC_PKTERR_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_write( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT,
                            value);
}

static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                    odu_ohp_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_action_on_write_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                                    odu_ohp_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_action_on_write_field_set", mask, ofs, value );
    odu_ohp_core_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_read( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT);

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                               odu_ohp_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_ohp_core_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFA_CH_SCHEDULE(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_CH_SCHEDULE(N),
                            mask,
                            PMC_OHP_CORE_REG_RXFA_CH_SCHEDULE_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_OHP_CORE_REG_RXFA_CH_SCHEDULE(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_CH_SCHEDULE(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write", ofs, len, 96 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write", 96, ofs, len);
    odu_ohp_core_burst_write( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_OHP_CORE_REG_RXFB_CH_SCHEDULE(ofs),
                              len,
                              value);

}

static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_field_set", mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_CH_SCHEDULE(N),
                            mask,
                            PMC_OHP_CORE_REG_RXFB_CH_SCHEDULE_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 96)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read", ofs, len, 96 );
    odu_ohp_core_burst_read( b_ptr,
                             h_ptr,
                             MEM_TYPE_STATUS,
                             PMC_OHP_CORE_REG_RXFB_CH_SCHEDULE(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read", 96, ofs, len);
}

static INLINE UINT32 odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_CH_SCHEDULE(N));

    IOLOG( "%s -> 0x%08x;", "odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFA_EXT_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_EXT_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFA_EXT_MASK_array_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_EXT_MASK(N),
                            value);
}

static INLINE void odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set", N, mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFA_EXT_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_RXFA_EXT_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFA_EXT_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFA_EXT_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFA_EXT_MASK(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_ohp_core_reg_RXFA_EXT_MASK_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_RXFB_EXT_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_EXT_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_RXFB_EXT_MASK_array_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_EXT_MASK(N),
                            value);
}

static INLINE void odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set", N, mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_RXFB_EXT_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_RXFB_EXT_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_RXFB_EXT_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_RXFB_EXT_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_RXFB_EXT_MASK(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_ohp_core_reg_RXFB_EXT_MASK_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFA_INS_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_INS_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_TXFA_INS_MASK_array_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_TXFA_INS_MASK(N),
                            value);
}

static INLINE void odu_ohp_core_reg_TXFA_INS_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFA_INS_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFA_INS_MASK_array_field_set", N, mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_TXFA_INS_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFA_INS_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_TXFA_INS_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFA_INS_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_TXFA_INS_MASK(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_ohp_core_reg_TXFA_INS_MASK_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_ohp_core_reg_TXFB_INS_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_INS_MASK_array_write( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_ohp_core_reg_TXFB_INS_MASK_array_write", value );
    odu_ohp_core_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_TXFB_INS_MASK(N),
                            value);
}

static INLINE void odu_ohp_core_reg_TXFB_INS_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_reg_TXFB_INS_MASK_array_field_set( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_ohp_core_reg_TXFB_INS_MASK_array_field_set", N, mask, ofs, value );
    odu_ohp_core_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OHP_CORE_REG_TXFB_INS_MASK(N),
                            mask,
                            PMC_OHP_CORE_REG_TXFB_INS_MASK_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 odu_ohp_core_reg_TXFB_INS_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_reg_TXFB_INS_MASK_array_read( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_ohp_core_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OHP_CORE_REG_TXFB_INS_MASK(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_ohp_core_reg_TXFB_INS_MASK_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void odu_ohp_core_field_STGB_GCC_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGB_GCC_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_STGB_GCC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGB_GCC_EN_set", value );

    /* (0x00000000 bits 8) field STGB_GCC_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    odu_ohp_core_reg_OHP_CFG_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGB_GCC_EN_MSK,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGB_GCC_EN_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_STGB_GCC_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGB_GCC_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field STGB_GCC_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    reg_value = odu_ohp_core_reg_OHP_CFG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGB_GCC_EN_MSK) >> OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGB_GCC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGB_GCC_EN_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_STGA_GCC_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGA_GCC_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_STGA_GCC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGA_GCC_EN_set", value );

    /* (0x00000000 bits 4) field STGA_GCC_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    odu_ohp_core_reg_OHP_CFG_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGA_GCC_EN_MSK,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGA_GCC_EN_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_STGA_GCC_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGA_GCC_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field STGA_GCC_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    reg_value = odu_ohp_core_reg_OHP_CFG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGA_GCC_EN_MSK) >> OHP_CORE_REG_OHP_CFG_CTRL_BIT_STGA_GCC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGA_GCC_EN_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_OHP_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                  odu_ohp_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_OHP_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                  odu_ohp_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_OHP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_OHP_EN_set", value );

    /* (0x00000000 bits 0) field OHP_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    odu_ohp_core_reg_OHP_CFG_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_OHP_EN_MSK,
                                             OHP_CORE_REG_OHP_CFG_CTRL_BIT_OHP_EN_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_OHP_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_OHP_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field OHP_EN of register PMC_OHP_CORE_REG_OHP_CFG_CTRL */
    reg_value = odu_ohp_core_reg_OHP_CFG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_CFG_CTRL_BIT_OHP_EN_MSK) >> OHP_CORE_REG_OHP_CFG_CTRL_BIT_OHP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_OHP_EN_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_STGA_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGA_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGA_SADDR_UID_set", value );

    /* (0x0000000c bits 31:0) field STGA_SADDR_UID of register PMC_OHP_CORE_REG_STGA_SA_UID */
    odu_ohp_core_reg_STGA_SA_UID_field_set( b_ptr,
                                            h_ptr,
                                            OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_MSK,
                                            OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF,
                                            value);
}

static INLINE UINT32 odu_ohp_core_field_STGA_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGA_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 31:0) field STGA_SADDR_UID of register PMC_OHP_CORE_REG_STGA_SA_UID */
    reg_value = odu_ohp_core_reg_STGA_SA_UID_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_MSK) >> OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGA_SADDR_UID_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_STGA_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_STGA_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGA_SADDR_UID_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGA_SADDR_UID_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGA_SADDR_UID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 31:0) field STGA_SADDR_UID of register PMC_OHP_CORE_REG_STGA_SA_UID */
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
        /* (0x0000000c bits 31:0) field STGA_SADDR_UID of register PMC_OHP_CORE_REG_STGA_SA_UID */
        odu_ohp_core_reg_STGA_SA_UID_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF + subfield_offset),
                                                OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_STGA_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_STGA_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGA_SADDR_UID_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGA_SADDR_UID_get", stop_bit, 31 );
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
    /* (0x0000000c bits 31:0) field STGA_SADDR_UID of register PMC_OHP_CORE_REG_STGA_SA_UID */
    reg_value = odu_ohp_core_reg_STGA_SA_UID_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_MSK)
                  >> OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_MSK, OHP_CORE_REG_STGA_SA_UID_BIT_STGA_SADDR_UID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGA_SADDR_UID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_STGB_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGB_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGB_SADDR_UID_set", value );

    /* (0x00000010 bits 31:0) field STGB_SADDR_UID of register PMC_OHP_CORE_REG_STGB_SA_UID */
    odu_ohp_core_reg_STGB_SA_UID_field_set( b_ptr,
                                            h_ptr,
                                            OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_MSK,
                                            OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF,
                                            value);
}

static INLINE UINT32 odu_ohp_core_field_STGB_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGB_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 31:0) field STGB_SADDR_UID of register PMC_OHP_CORE_REG_STGB_SA_UID */
    reg_value = odu_ohp_core_reg_STGB_SA_UID_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_MSK) >> OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGB_SADDR_UID_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_STGB_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_STGB_SADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGB_SADDR_UID_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGB_SADDR_UID_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGB_SADDR_UID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 31:0) field STGB_SADDR_UID of register PMC_OHP_CORE_REG_STGB_SA_UID */
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
        /* (0x00000010 bits 31:0) field STGB_SADDR_UID of register PMC_OHP_CORE_REG_STGB_SA_UID */
        odu_ohp_core_reg_STGB_SA_UID_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF + subfield_offset),
                                                OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_STGB_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_STGB_SADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGB_SADDR_UID_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGB_SADDR_UID_get", stop_bit, 31 );
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
    /* (0x00000010 bits 31:0) field STGB_SADDR_UID of register PMC_OHP_CORE_REG_STGB_SA_UID */
    reg_value = odu_ohp_core_reg_STGB_SA_UID_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_MSK)
                  >> OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_MSK, OHP_CORE_REG_STGB_SA_UID_BIT_STGB_SADDR_UID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGB_SADDR_UID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_STGA_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGA_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGA_DADDR_UID_set", value );

    /* (0x00000014 bits 31:0) field STGA_DADDR_UID of register PMC_OHP_CORE_REG_STGA_DA_UID */
    odu_ohp_core_reg_STGA_DA_UID_field_set( b_ptr,
                                            h_ptr,
                                            OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_MSK,
                                            OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF,
                                            value);
}

static INLINE UINT32 odu_ohp_core_field_STGA_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGA_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 31:0) field STGA_DADDR_UID of register PMC_OHP_CORE_REG_STGA_DA_UID */
    reg_value = odu_ohp_core_reg_STGA_DA_UID_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_MSK) >> OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGA_DADDR_UID_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_STGA_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_STGA_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGA_DADDR_UID_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGA_DADDR_UID_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGA_DADDR_UID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 31:0) field STGA_DADDR_UID of register PMC_OHP_CORE_REG_STGA_DA_UID */
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
        /* (0x00000014 bits 31:0) field STGA_DADDR_UID of register PMC_OHP_CORE_REG_STGA_DA_UID */
        odu_ohp_core_reg_STGA_DA_UID_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF + subfield_offset),
                                                OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_STGA_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_STGA_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGA_DADDR_UID_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGA_DADDR_UID_get", stop_bit, 31 );
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
    /* (0x00000014 bits 31:0) field STGA_DADDR_UID of register PMC_OHP_CORE_REG_STGA_DA_UID */
    reg_value = odu_ohp_core_reg_STGA_DA_UID_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_MSK)
                  >> OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_MSK, OHP_CORE_REG_STGA_DA_UID_BIT_STGA_DADDR_UID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGA_DADDR_UID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_STGB_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_STGB_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_STGB_DADDR_UID_set", value );

    /* (0x00000018 bits 31:0) field STGB_DADDR_UID of register PMC_OHP_CORE_REG_STGB_DA_UID */
    odu_ohp_core_reg_STGB_DA_UID_field_set( b_ptr,
                                            h_ptr,
                                            OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_MSK,
                                            OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF,
                                            value);
}

static INLINE UINT32 odu_ohp_core_field_STGB_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_STGB_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 31:0) field STGB_DADDR_UID of register PMC_OHP_CORE_REG_STGB_DA_UID */
    reg_value = odu_ohp_core_reg_STGB_DA_UID_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_MSK) >> OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_STGB_DADDR_UID_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_STGB_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_STGB_DADDR_UID_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGB_DADDR_UID_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGB_DADDR_UID_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGB_DADDR_UID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 31:0) field STGB_DADDR_UID of register PMC_OHP_CORE_REG_STGB_DA_UID */
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
        /* (0x00000018 bits 31:0) field STGB_DADDR_UID of register PMC_OHP_CORE_REG_STGB_DA_UID */
        odu_ohp_core_reg_STGB_DA_UID_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF + subfield_offset),
                                                OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_STGB_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_STGB_DADDR_UID_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_STGB_DADDR_UID_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_STGB_DADDR_UID_get", stop_bit, 31 );
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
    /* (0x00000018 bits 31:0) field STGB_DADDR_UID of register PMC_OHP_CORE_REG_STGB_DA_UID */
    reg_value = odu_ohp_core_reg_STGB_DA_UID_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_MSK)
                  >> OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_MSK, OHP_CORE_REG_STGB_DA_UID_BIT_STGB_DADDR_UID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_STGB_DADDR_UID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFA_SD_CONFIG_set", value, 63);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFA_SD_CONFIG_set", value );

    /* (0x0000001c bits 5:0) field RXFA_SD_CONFIG of register PMC_OHP_CORE_REG_RXFA_PWR_CTRL */
    odu_ohp_core_reg_RXFA_PWR_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_MSK,
                                              OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF,
                                              value);
}

static INLINE UINT32 odu_ohp_core_field_RXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 5:0) field RXFA_SD_CONFIG of register PMC_OHP_CORE_REG_RXFA_PWR_CTRL */
    reg_value = odu_ohp_core_reg_RXFA_PWR_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_MSK) >> OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFA_SD_CONFIG_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_RXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_RXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFA_SD_CONFIG_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFA_SD_CONFIG_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFA_SD_CONFIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 5:0) field RXFA_SD_CONFIG of register PMC_OHP_CORE_REG_RXFA_PWR_CTRL */
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
        /* (0x0000001c bits 5:0) field RXFA_SD_CONFIG of register PMC_OHP_CORE_REG_RXFA_PWR_CTRL */
        odu_ohp_core_reg_RXFA_PWR_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF + subfield_offset),
                                                  OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_RXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_RXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFA_SD_CONFIG_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFA_SD_CONFIG_get", stop_bit, 5 );
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
    /* (0x0000001c bits 5:0) field RXFA_SD_CONFIG of register PMC_OHP_CORE_REG_RXFA_PWR_CTRL */
    reg_value = odu_ohp_core_reg_RXFA_PWR_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_MSK)
                  >> OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_MSK, OHP_CORE_REG_RXFA_PWR_CTRL_BIT_RXFA_SD_CONFIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFA_SD_CONFIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFB_SD_CONFIG_set", value, 63);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFB_SD_CONFIG_set", value );

    /* (0x00000020 bits 5:0) field RXFB_SD_CONFIG of register PMC_OHP_CORE_REG_RXFB_PWR_CTRL */
    odu_ohp_core_reg_RXFB_PWR_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_MSK,
                                              OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF,
                                              value);
}

static INLINE UINT32 odu_ohp_core_field_RXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 5:0) field RXFB_SD_CONFIG of register PMC_OHP_CORE_REG_RXFB_PWR_CTRL */
    reg_value = odu_ohp_core_reg_RXFB_PWR_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_MSK) >> OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFB_SD_CONFIG_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_RXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_RXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFB_SD_CONFIG_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFB_SD_CONFIG_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFB_SD_CONFIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 5:0) field RXFB_SD_CONFIG of register PMC_OHP_CORE_REG_RXFB_PWR_CTRL */
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
        /* (0x00000020 bits 5:0) field RXFB_SD_CONFIG of register PMC_OHP_CORE_REG_RXFB_PWR_CTRL */
        odu_ohp_core_reg_RXFB_PWR_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF + subfield_offset),
                                                  OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_RXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_RXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFB_SD_CONFIG_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFB_SD_CONFIG_get", stop_bit, 5 );
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
    /* (0x00000020 bits 5:0) field RXFB_SD_CONFIG of register PMC_OHP_CORE_REG_RXFB_PWR_CTRL */
    reg_value = odu_ohp_core_reg_RXFB_PWR_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_MSK)
                  >> OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_MSK, OHP_CORE_REG_RXFB_PWR_CTRL_BIT_RXFB_SD_CONFIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFB_SD_CONFIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_TXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_TXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_TXFA_SD_CONFIG_set", value, 63);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_TXFA_SD_CONFIG_set", value );

    /* (0x00000024 bits 5:0) field TXFA_SD_CONFIG of register PMC_OHP_CORE_REG_TXFA_PWR_CTRL */
    odu_ohp_core_reg_TXFA_PWR_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_MSK,
                                              OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF,
                                              value);
}

static INLINE UINT32 odu_ohp_core_field_TXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_TXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 5:0) field TXFA_SD_CONFIG of register PMC_OHP_CORE_REG_TXFA_PWR_CTRL */
    reg_value = odu_ohp_core_reg_TXFA_PWR_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_MSK) >> OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_TXFA_SD_CONFIG_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_TXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_TXFA_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFA_SD_CONFIG_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFA_SD_CONFIG_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFA_SD_CONFIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 5:0) field TXFA_SD_CONFIG of register PMC_OHP_CORE_REG_TXFA_PWR_CTRL */
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
        /* (0x00000024 bits 5:0) field TXFA_SD_CONFIG of register PMC_OHP_CORE_REG_TXFA_PWR_CTRL */
        odu_ohp_core_reg_TXFA_PWR_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF + subfield_offset),
                                                  OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_TXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_TXFA_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFA_SD_CONFIG_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFA_SD_CONFIG_get", stop_bit, 5 );
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
    /* (0x00000024 bits 5:0) field TXFA_SD_CONFIG of register PMC_OHP_CORE_REG_TXFA_PWR_CTRL */
    reg_value = odu_ohp_core_reg_TXFA_PWR_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_MSK)
                  >> OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_MSK, OHP_CORE_REG_TXFA_PWR_CTRL_BIT_TXFA_SD_CONFIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFA_SD_CONFIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_TXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_TXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_TXFB_SD_CONFIG_set", value, 63);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_TXFB_SD_CONFIG_set", value );

    /* (0x00000028 bits 5:0) field TXFB_SD_CONFIG of register PMC_OHP_CORE_REG_TXFB_PWR_CTRL */
    odu_ohp_core_reg_TXFB_PWR_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_MSK,
                                              OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF,
                                              value);
}

static INLINE UINT32 odu_ohp_core_field_TXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_TXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 5:0) field TXFB_SD_CONFIG of register PMC_OHP_CORE_REG_TXFB_PWR_CTRL */
    reg_value = odu_ohp_core_reg_TXFB_PWR_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_MSK) >> OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_TXFB_SD_CONFIG_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_range_TXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_TXFB_SD_CONFIG_set( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFB_SD_CONFIG_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFB_SD_CONFIG_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFB_SD_CONFIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 5:0) field TXFB_SD_CONFIG of register PMC_OHP_CORE_REG_TXFB_PWR_CTRL */
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
        /* (0x00000028 bits 5:0) field TXFB_SD_CONFIG of register PMC_OHP_CORE_REG_TXFB_PWR_CTRL */
        odu_ohp_core_reg_TXFB_PWR_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF + subfield_offset),
                                                  OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_TXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_TXFB_SD_CONFIG_get( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFB_SD_CONFIG_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFB_SD_CONFIG_get", stop_bit, 5 );
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
    /* (0x00000028 bits 5:0) field TXFB_SD_CONFIG of register PMC_OHP_CORE_REG_TXFB_PWR_CTRL */
    reg_value = odu_ohp_core_reg_TXFB_PWR_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_MSK)
                  >> OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_MSK, OHP_CORE_REG_TXFB_PWR_CTRL_BIT_TXFB_SD_CONFIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFB_SD_CONFIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_FEGE_RX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_FEGE_RX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_FEGE_RX_FLIP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_FEGE_RX_FLIP_set", value );

    /* (0x00001390 bits 24) field FEGE_RX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_RX_FLIP_MSK,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_RX_FLIP_OFF,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_field_FEGE_RX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_FEGE_RX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001390 bits 24) field FEGE_RX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    reg_value = odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_RX_FLIP_MSK) >> OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_RX_FLIP_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_FEGE_RX_FLIP_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_FEGE_TX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_FEGE_TX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_FEGE_TX_FLIP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_FEGE_TX_FLIP_set", value );

    /* (0x00001390 bits 16) field FEGE_TX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_TX_FLIP_MSK,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_TX_FLIP_OFF,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_field_FEGE_TX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_FEGE_TX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001390 bits 16) field FEGE_TX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    reg_value = odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_TX_FLIP_MSK) >> OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_FEGE_TX_FLIP_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_FEGE_TX_FLIP_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_SGMII_RX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_SGMII_RX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_SGMII_RX_FLIP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_SGMII_RX_FLIP_set", value );

    /* (0x00001390 bits 8) field SGMII_RX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_RX_FLIP_MSK,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_RX_FLIP_OFF,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_field_SGMII_RX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_SGMII_RX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001390 bits 8) field SGMII_RX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    reg_value = odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_RX_FLIP_MSK) >> OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_RX_FLIP_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_SGMII_RX_FLIP_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_SGMII_TX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_SGMII_TX_FLIP_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_SGMII_TX_FLIP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_SGMII_TX_FLIP_set", value );

    /* (0x00001390 bits 0) field SGMII_TX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_TX_FLIP_MSK,
                                                 OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_TX_FLIP_OFF,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_field_SGMII_TX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_SGMII_TX_FLIP_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001390 bits 0) field SGMII_TX_FLIP of register PMC_OHP_CORE_REG_OHP_ENET_IO_CTRL */
    reg_value = odu_ohp_core_reg_OHP_ENET_IO_CTRL_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_TX_FLIP_MSK) >> OHP_CORE_REG_OHP_ENET_IO_CTRL_BIT_SGMII_TX_FLIP_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_SGMII_TX_FLIP_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize3
 * ==================================================================================
 */
static INLINE void odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_set");
    odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_get");
    odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFA_GCC0_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFA_GCC0_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_set");
    odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_get");
    odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFA_GCC1_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFA_GCC1_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_set");
    odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_get");
    odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFA_GCC2_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFA_GCC2_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_set");
    odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_get");
    odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFB_GCC0_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFB_GCC0_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_set");
    odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_get");
    odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFB_GCC1_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFB_GCC1_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_set");
    odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_get");
    odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFB_GCC2_MASK_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                       odu_ohp_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFB_GCC2_MASK_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize96
 * ==================================================================================
 */
static INLINE void odu_ohp_core_lfield_STGA_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_STGA_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_STGA_CH_EN_CTRL_set");
    odu_ohp_core_reg_STGA_CH_EN_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   96,
                                                   value);
}

static INLINE void odu_ohp_core_lfield_STGA_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_STGA_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_STGA_CH_EN_CTRL_get");
    odu_ohp_core_reg_STGA_CH_EN_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  96,
                                                  (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_STGA_CH_EN_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_STGA_CH_EN_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_STGB_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_STGB_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_STGB_CH_EN_CTRL_set");
    odu_ohp_core_reg_STGB_CH_EN_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   96,
                                                   value);
}

static INLINE void odu_ohp_core_lfield_STGB_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_STGB_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_STGB_CH_EN_CTRL_get");
    odu_ohp_core_reg_STGB_CH_EN_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  96,
                                                  (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_STGB_CH_EN_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_STGB_CH_EN_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFA_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_INS_OFFSET_set");
    odu_ohp_core_reg_TXFA_OFFSET_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    96,
                                                    value);
}

static INLINE void odu_ohp_core_lfield_TXFA_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFA_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFA_INS_OFFSET_get");
    odu_ohp_core_reg_TXFA_OFFSET_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   96,
                                                   (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFA_OFFSET_array_field_set( b_ptr,
                                                  h_ptr,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFA_OFFSET_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_TXFB_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_INS_OFFSET_set");
    odu_ohp_core_reg_TXFB_OFFSET_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    96,
                                                    value);
}

static INLINE void odu_ohp_core_lfield_TXFB_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_TXFB_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_TXFB_INS_OFFSET_get");
    odu_ohp_core_reg_TXFB_OFFSET_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   96,
                                                   (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_TXFB_OFFSET_array_field_set( b_ptr,
                                                  h_ptr,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_TXFB_OFFSET_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void odu_ohp_core_field_RXFA_SCH_FORCE_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFA_SCH_FORCE_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFA_SCH_FORCE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFA_SCH_FORCE_set", value );

    /* (0x00000004 bits 16) field RXFA_SCH_FORCE of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    odu_ohp_core_reg_SCH_RXFA_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_FORCE_MSK,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_FORCE_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_FORCE_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_FORCE_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 16) field RXFA_SCH_FORCE of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFA_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_FORCE_MSK) >> OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_FORCE_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFA_SCH_FORCE_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFA_SCH_REQ_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFA_SCH_REQ_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFA_SCH_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFA_SCH_REQ_set", value );

    /* (0x00000004 bits 8) field RXFA_SCH_REQ of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    odu_ohp_core_reg_SCH_RXFA_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_REQ_MSK,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_REQ_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_REQ_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_REQ_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field RXFA_SCH_REQ of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFA_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_REQ_MSK) >> OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFA_SCH_REQ_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFA_SCH_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFA_SCH_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFA_SCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFA_SCH_EN_set", value );

    /* (0x00000004 bits 0) field RXFA_SCH_EN of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    odu_ohp_core_reg_SCH_RXFA_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_EN_MSK,
                                             OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_EN_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFA_SCH_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field RXFA_SCH_EN of register PMC_OHP_CORE_REG_SCH_RXFA_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFA_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_EN_MSK) >> OHP_CORE_REG_SCH_RXFA_CFG_BIT_RXFA_SCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFA_SCH_EN_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFB_SCH_FORCE_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFB_SCH_FORCE_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFB_SCH_FORCE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFB_SCH_FORCE_set", value );

    /* (0x00000008 bits 16) field RXFB_SCH_FORCE of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    odu_ohp_core_reg_SCH_RXFB_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_FORCE_MSK,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_FORCE_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_FORCE_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_FORCE_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 16) field RXFB_SCH_FORCE of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFB_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_FORCE_MSK) >> OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_FORCE_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFB_SCH_FORCE_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFB_SCH_REQ_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFB_SCH_REQ_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFB_SCH_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFB_SCH_REQ_set", value );

    /* (0x00000008 bits 8) field RXFB_SCH_REQ of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    odu_ohp_core_reg_SCH_RXFB_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_REQ_MSK,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_REQ_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_REQ_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_REQ_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 8) field RXFB_SCH_REQ of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFB_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_REQ_MSK) >> OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFB_SCH_REQ_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFB_SCH_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFB_SCH_EN_set( odu_ohp_core_buffer_t *b_ptr,
                                                       odu_ohp_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFB_SCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXFB_SCH_EN_set", value );

    /* (0x00000008 bits 0) field RXFB_SCH_EN of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    odu_ohp_core_reg_SCH_RXFB_CFG_field_set( b_ptr,
                                             h_ptr,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_EN_MSK,
                                             OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_EN_OFF,
                                             value);
}

static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFB_SCH_EN_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field RXFB_SCH_EN of register PMC_OHP_CORE_REG_SCH_RXFB_CFG */
    reg_value = odu_ohp_core_reg_SCH_RXFB_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_EN_MSK) >> OHP_CORE_REG_SCH_RXFB_CFG_BIT_RXFB_SCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXFB_SCH_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_CFC_FULL_E_set", value[2] , value[1] , value[0] );

    /* (0x00000674 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF,
                                                      value[0]);

    /* (0x00000678 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF,
                                                      value[1]);

    /* (0x0000067c bits 31:0) bits 64:95 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF,
                                                      value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00000674 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF;
    value[0] |= field_value;

    /* (0x00000678 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF;
    value[1] |= field_value;

    /* (0x0000067c bits 31:0) bits 64:95 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_CFC_FULL_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000674 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0 */
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
        /* (0x00000674 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000678 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1 */
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
        /* (0x00000678 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000067c bits 31:0) bits 64:95 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2 */
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
        /* (0x0000067c bits 31:0) bits 64:95 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get", stop_bit, 95 );
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
    /* (0x00000674 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN0_BIT_RXFA_CFC_FULL_E_OFF, field_value );
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
    /* (0x00000678 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN1_BIT_RXFA_CFC_FULL_E_OFF, field_value );
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
    /* (0x0000067c bits 31:0) bits 64:95 use field RXFA_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT_EN2_BIT_RXFA_CFC_FULL_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_CFC_FULL_E_set", value[2] , value[1] , value[0] );

    /* (0x0000068c bits 31:0) bits 0:31 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF,
                                                      value[0]);

    /* (0x00000690 bits 31:0) bits 32:63 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF,
                                                      value[1]);

    /* (0x00000694 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_MSK,
                                                      OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF,
                                                      value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000068c bits 31:0) bits 0:31 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF;
    value[0] |= field_value;

    /* (0x00000690 bits 31:0) bits 32:63 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF;
    value[1] |= field_value;

    /* (0x00000694 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_CFC_FULL_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000068c bits 31:0) bits 0:31 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0 */
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
        /* (0x0000068c bits 31:0) bits 0:31 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000690 bits 31:0) bits 32:63 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1 */
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
        /* (0x00000690 bits 31:0) bits 32:63 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000694 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2 */
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
        /* (0x00000694 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset),
                                                          OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get", stop_bit, 95 );
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
    /* (0x0000068c bits 31:0) bits 0:31 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN0_BIT_RXFB_CFC_FULL_E_OFF, field_value );
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
    /* (0x00000690 bits 31:0) bits 32:63 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN1_BIT_RXFB_CFC_FULL_E_OFF, field_value );
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
    /* (0x00000694 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_E of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT_EN2_BIT_RXFB_CFC_FULL_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_TXDLY_E_set", value[2] , value[1] , value[0] );

    /* (0x000006a4 bits 31:0) bits 0:31 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0 */
    odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF,
                                                   value[0]);

    /* (0x000006a8 bits 31:0) bits 32:63 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1 */
    odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF,
                                                   value[1]);

    /* (0x000006ac bits 31:0) bits 64:95 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2 */
    odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF,
                                                   value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000006a4 bits 31:0) bits 0:31 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF;
    value[0] |= field_value;

    /* (0x000006a8 bits 31:0) bits 32:63 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF;
    value[1] |= field_value;

    /* (0x000006ac bits 31:0) bits 64:95 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_TXDLY_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFA_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFA_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006a4 bits 31:0) bits 0:31 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0 */
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
        /* (0x000006a4 bits 31:0) bits 0:31 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0 */
        odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006a8 bits 31:0) bits 32:63 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1 */
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
        /* (0x000006a8 bits 31:0) bits 32:63 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1 */
        odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006ac bits 31:0) bits 64:95 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2 */
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
        /* (0x000006ac bits 31:0) bits 64:95 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2 */
        odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_get", stop_bit, 95 );
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
    /* (0x000006a4 bits 31:0) bits 0:31 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN0 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_MSK, OHP_CORE_REG_RXFA_TXDLY_INT_EN0_BIT_RXFA_TXDLY_E_OFF, field_value );
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
    /* (0x000006a8 bits 31:0) bits 32:63 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN1 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_MSK, OHP_CORE_REG_RXFA_TXDLY_INT_EN1_BIT_RXFA_TXDLY_E_OFF, field_value );
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
    /* (0x000006ac bits 31:0) bits 64:95 use field RXFA_TXDLY_E of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT_EN2 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_MSK, OHP_CORE_REG_RXFA_TXDLY_INT_EN2_BIT_RXFA_TXDLY_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_TXDLY_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_TXDLY_E_set", value[2] , value[1] , value[0] );

    /* (0x000006bc bits 31:0) bits 0:31 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0 */
    odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF,
                                                   value[0]);

    /* (0x000006c0 bits 31:0) bits 32:63 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1 */
    odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF,
                                                   value[1]);

    /* (0x000006c4 bits 31:0) bits 64:95 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2 */
    odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_MSK,
                                                   OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF,
                                                   value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000006bc bits 31:0) bits 0:31 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF;
    value[0] |= field_value;

    /* (0x000006c0 bits 31:0) bits 32:63 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF;
    value[1] |= field_value;

    /* (0x000006c4 bits 31:0) bits 64:95 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_TXDLY_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFB_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFB_TXDLY_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006bc bits 31:0) bits 0:31 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0 */
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
        /* (0x000006bc bits 31:0) bits 0:31 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0 */
        odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006c0 bits 31:0) bits 32:63 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1 */
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
        /* (0x000006c0 bits 31:0) bits 32:63 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1 */
        odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006c4 bits 31:0) bits 64:95 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2 */
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
        /* (0x000006c4 bits 31:0) bits 64:95 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2 */
        odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF + subfield_offset),
                                                       OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_TXDLY_E_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_get", stop_bit, 95 );
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
    /* (0x000006bc bits 31:0) bits 0:31 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN0 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_MSK, OHP_CORE_REG_RXFB_TXDLY_INT_EN0_BIT_RXFB_TXDLY_E_OFF, field_value );
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
    /* (0x000006c0 bits 31:0) bits 32:63 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN1 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_MSK, OHP_CORE_REG_RXFB_TXDLY_INT_EN1_BIT_RXFB_TXDLY_E_OFF, field_value );
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
    /* (0x000006c4 bits 31:0) bits 64:95 use field RXFB_TXDLY_E of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT_EN2 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_MSK, OHP_CORE_REG_RXFB_TXDLY_INT_EN2_BIT_RXFB_TXDLY_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_TXDLY_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_RXMAC_UID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_UID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_E_set", value );

    /* (0x000006d4 bits 0) field RXMAC_UID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN */
    odu_ohp_core_reg_RXMAC_UID_FAIL_EN_field_set( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXMAC_UID_FAIL_EN_BIT_RXMAC_UID_FAIL_E_MSK,
                                                  OHP_CORE_REG_RXMAC_UID_FAIL_EN_BIT_RXMAC_UID_FAIL_E_OFF,
                                                  value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_UID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_UID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006d4 bits 0) field RXMAC_UID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_UID_FAIL_EN */
    reg_value = odu_ohp_core_reg_RXMAC_UID_FAIL_EN_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_UID_FAIL_EN_BIT_RXMAC_UID_FAIL_E_MSK) >> OHP_CORE_REG_RXMAC_UID_FAIL_EN_BIT_RXMAC_UID_FAIL_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_E_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_set", value );

    /* (0x000006d8 bits 0) field RXMAC_FRMRID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN */
    odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_field_set( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN_BIT_RXMAC_FRMRID_FAIL_E_MSK,
                                                     OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN_BIT_RXMAC_FRMRID_FAIL_E_OFF,
                                                     value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006d8 bits 0) field RXMAC_FRMRID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN */
    reg_value = odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN_BIT_RXMAC_FRMRID_FAIL_E_MSK) >> OHP_CORE_REG_RXMAC_FRMRID_FAIL_EN_BIT_RXMAC_FRMRID_FAIL_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXMAC_CHID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_CHID_FAIL_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                             odu_ohp_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_E_set", value );

    /* (0x000006dc bits 0) field RXMAC_CHID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN */
    odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXMAC_CHID_FAIL_EN_BIT_RXMAC_CHID_FAIL_E_MSK,
                                                   OHP_CORE_REG_RXMAC_CHID_FAIL_EN_BIT_RXMAC_CHID_FAIL_E_OFF,
                                                   value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_CHID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_CHID_FAIL_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006dc bits 0) field RXMAC_CHID_FAIL_E of register PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_EN */
    reg_value = odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_CHID_FAIL_EN_BIT_RXMAC_CHID_FAIL_E_MSK) >> OHP_CORE_REG_RXMAC_CHID_FAIL_EN_BIT_RXMAC_CHID_FAIL_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_E_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXMAC_PKTERR_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_PKTERR_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                          odu_ohp_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_E_set", value );

    /* (0x000006e0 bits 0) field RXMAC_PKTERR_E of register PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN */
    odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_field_set( b_ptr,
                                                    h_ptr,
                                                    OHP_CORE_REG_RXMAC_PKTERR_INT_EN_BIT_RXMAC_PKTERR_E_MSK,
                                                    OHP_CORE_REG_RXMAC_PKTERR_INT_EN_BIT_RXMAC_PKTERR_E_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_PKTERR_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_PKTERR_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006e0 bits 0) field RXMAC_PKTERR_E of register PMC_OHP_CORE_REG_RXMAC_PKTERR_INT_EN */
    reg_value = odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_PKTERR_INT_EN_BIT_RXMAC_PKTERR_E_MSK) >> OHP_CORE_REG_RXMAC_PKTERR_INT_EN_BIT_RXMAC_PKTERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_E_get", value );

    return value;
}
static INLINE void odu_ohp_core_field_RXMAC_FIFO_ERR_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_FIFO_ERR_E_set( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_E_set", value );

    /* (0x000006e4 bits 0) field RXMAC_FIFO_ERR_E of register PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN */
    odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_field_set( b_ptr,
                                                    h_ptr,
                                                    OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN_BIT_RXMAC_FIFO_ERR_E_MSK,
                                                    OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN_BIT_RXMAC_FIFO_ERR_E_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_FIFO_ERR_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_FIFO_ERR_E_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006e4 bits 0) field RXMAC_FIFO_ERR_E of register PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN */
    reg_value = odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN_BIT_RXMAC_FIFO_ERR_E_MSK) >> OHP_CORE_REG_RXMAC_RXFIFO_ERR_EN_BIT_RXMAC_FIFO_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_CFC_FULL_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF,
                                                                   value[0]);

    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF,
                                                                   value[1]);

    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
    odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF,
                                                                   value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF;
    value[0] |= field_value;

    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF;
    value[1] |= field_value;

    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
    reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_CFC_FULL_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
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
        /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
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
        /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
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
        /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
        odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get", stop_bit, 95 );
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
    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF, field_value );
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
    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF, field_value );
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
    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
        reg_value = odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_MSK, OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
    if (start_bit <= 31) {
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
        return odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
        return odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
        return odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value[3],
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value[3],
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_CFC_FULL_I_poll", value[2] , value[1] , value[0] );

    /* (0x00000680 bits 31:0) bits 0:31 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT0 */
    return odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_MSK,
                                                     (value[0]<<OHP_CORE_REG_RXFA_CFC_FULL_INT0_BIT_RXFA_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x00000684 bits 31:0) bits 32:63 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT1 */
    return odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_MSK,
                                                     (value[1]<<OHP_CORE_REG_RXFA_CFC_FULL_INT1_BIT_RXFA_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x00000688 bits 31:0) bits 64:95 use field RXFA_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFA_CFC_FULL_INT2 */
    return odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_MSK,
                                                     (value[2]<<OHP_CORE_REG_RXFA_CFC_FULL_INT2_BIT_RXFA_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_CFC_FULL_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF,
                                                                   value[0]);

    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF,
                                                                   value[1]);

    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
    odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_MSK,
                                                                   OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF,
                                                                   value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                            odu_ohp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF;
    value[0] |= field_value;

    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF;
    value[1] |= field_value;

    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
    reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_MSK) >> OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_CFC_FULL_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
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
        /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
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
        /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
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
        /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
        odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                                       OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                    odu_ohp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get", stop_bit, 95 );
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
    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF, field_value );
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
    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF, field_value );
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
    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
        reg_value = odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_MSK)
                      >> OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_MSK, OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                   odu_ohp_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
    if (start_bit <= 31) {
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
        return odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
        return odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
        return odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         value << (OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value[3],
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_CFC_FULL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value[3],
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_CFC_FULL_I_poll", value[2] , value[1] , value[0] );

    /* (0x00000698 bits 31:0) bits 0:31 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT0 */
    return odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_MSK,
                                                     (value[0]<<OHP_CORE_REG_RXFB_CFC_FULL_INT0_BIT_RXFB_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x0000069c bits 31:0) bits 32:63 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT1 */
    return odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_MSK,
                                                     (value[1]<<OHP_CORE_REG_RXFB_CFC_FULL_INT1_BIT_RXFB_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x000006a0 bits 31:0) bits 64:95 use field RXFB_CFC_FULL_I of register PMC_OHP_CORE_REG_RXFB_CFC_FULL_INT2 */
    return odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_MSK,
                                                     (value[2]<<OHP_CORE_REG_RXFB_CFC_FULL_INT2_BIT_RXFB_CFC_FULL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_TXDLY_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
    odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF,
                                                                value[0]);

    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
    odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF,
                                                                value[1]);

    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
    odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF,
                                                                value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFA_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT0_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF;
    value[0] |= field_value;

    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT1_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF;
    value[1] |= field_value;

    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
    reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT2_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_MSK) >> OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_TXDLY_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
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
        /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
        odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
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
        /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
        odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
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
        /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
        odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFA_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_get", stop_bit, 95 );
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
    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT0_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_MSK, OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF, field_value );
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
    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT1_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_MSK, OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF, field_value );
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
    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
        reg_value = odu_ohp_core_reg_RXFA_TXDLY_INT2_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_MSK, OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
    if (start_bit <= 31) {
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
        return odu_ohp_core_reg_RXFA_TXDLY_INT0_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
        return odu_ohp_core_reg_RXFA_TXDLY_INT1_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
        return odu_ohp_core_reg_RXFA_TXDLY_INT2_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 value[3],
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 value[3],
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFA_TXDLY_I_poll", value[2] , value[1] , value[0] );

    /* (0x000006b0 bits 31:0) bits 0:31 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT0 */
    return odu_ohp_core_reg_RXFA_TXDLY_INT0_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_MSK,
                                                  (value[0]<<OHP_CORE_REG_RXFA_TXDLY_INT0_BIT_RXFA_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);

    /* (0x000006b4 bits 31:0) bits 32:63 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT1 */
    return odu_ohp_core_reg_RXFA_TXDLY_INT1_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_MSK,
                                                  (value[1]<<OHP_CORE_REG_RXFA_TXDLY_INT1_BIT_RXFA_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);

    /* (0x000006b8 bits 31:0) bits 64:95 use field RXFA_TXDLY_I of register PMC_OHP_CORE_REG_RXFA_TXDLY_INT2 */
    return odu_ohp_core_reg_RXFA_TXDLY_INT2_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_MSK,
                                                  (value[2]<<OHP_CORE_REG_RXFA_TXDLY_INT2_BIT_RXFA_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                  odu_ohp_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_TXDLY_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
    odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF,
                                                                value[0]);

    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
    odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF,
                                                                value[1]);

    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
    odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_MSK,
                                                                OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF,
                                                                value[2]);
}

static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_RXFB_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT0_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF;
    value[0] |= field_value;

    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT1_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF;
    value[1] |= field_value;

    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
    reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT2_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_MSK) >> OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_TXDLY_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
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
        /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
        odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
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
        /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
        odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
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
        /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
        odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                                    OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_RXFB_TXDLY_I_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_get", stop_bit, 95 );
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
    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT0_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_MSK, OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF, field_value );
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
    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT1_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_MSK, OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF, field_value );
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
    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
        reg_value = odu_ohp_core_reg_RXFB_TXDLY_INT2_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_MSK)
                      >> OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_MSK, OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
    if (start_bit <= 31) {
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
        return odu_ohp_core_reg_RXFB_TXDLY_INT0_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
        return odu_ohp_core_reg_RXFB_TXDLY_INT1_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
        return odu_ohp_core_reg_RXFB_TXDLY_INT2_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      value << (OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 value[3],
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_TXDLY_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                          odu_ohp_handle_t *h_ptr,
                                                                          UINT32 value[3],
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_ohp_core_lfield_RXFB_TXDLY_I_poll", value[2] , value[1] , value[0] );

    /* (0x000006c8 bits 31:0) bits 0:31 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT0 */
    return odu_ohp_core_reg_RXFB_TXDLY_INT0_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_MSK,
                                                  (value[0]<<OHP_CORE_REG_RXFB_TXDLY_INT0_BIT_RXFB_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);

    /* (0x000006cc bits 31:0) bits 32:63 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT1 */
    return odu_ohp_core_reg_RXFB_TXDLY_INT1_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_MSK,
                                                  (value[1]<<OHP_CORE_REG_RXFB_TXDLY_INT1_BIT_RXFB_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);

    /* (0x000006d0 bits 31:0) bits 64:95 use field RXFB_TXDLY_I of register PMC_OHP_CORE_REG_RXFB_TXDLY_INT2 */
    return odu_ohp_core_reg_RXFB_TXDLY_INT2_poll( b_ptr,
                                                  h_ptr,
                                                  OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_MSK,
                                                  (value[2]<<OHP_CORE_REG_RXFB_TXDLY_INT2_BIT_RXFB_TXDLY_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_field_RXMAC_UID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_UID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_I_set_to_clear", value );

    /* (0x000006e8 bits 0) field RXMAC_UID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT */
    odu_ohp_core_reg_RXMAC_UID_FAIL_INT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_MSK,
                                                                   OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_OFF,
                                                                   value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_UID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_UID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006e8 bits 0) field RXMAC_UID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT */
    reg_value = odu_ohp_core_reg_RXMAC_UID_FAIL_INT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_MSK) >> OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_UID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_UID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_ohp_core_field_RXMAC_UID_FAIL_I_poll", value );

    /* (0x000006e8 bits 0) field RXMAC_UID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_UID_FAIL_INT */
    return odu_ohp_core_reg_RXMAC_UID_FAIL_INT_poll( b_ptr,
                                                     h_ptr,
                                                     OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_MSK,
                                                     (value<<OHP_CORE_REG_RXMAC_UID_FAIL_INT_BIT_RXMAC_UID_FAIL_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                        odu_ohp_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_set_to_clear", value );

    /* (0x000006ec bits 0) field RXMAC_FRMRID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT */
    odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_MSK,
                                                                      OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_OFF,
                                                                      value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006ec bits 0) field RXMAC_FRMRID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT */
    reg_value = odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_MSK) >> OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                                odu_ohp_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_poll", value );

    /* (0x000006ec bits 0) field RXMAC_FRMRID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT */
    return odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_poll( b_ptr,
                                                        h_ptr,
                                                        OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_MSK,
                                                        (value<<OHP_CORE_REG_RXMAC_FRMRID_FAIL_INT_BIT_RXMAC_FRMRID_FAIL_I_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_field_RXMAC_CHID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_CHID_FAIL_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                      odu_ohp_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_I_set_to_clear", value );

    /* (0x000006f0 bits 0) field RXMAC_CHID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT */
    odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_MSK,
                                                                    OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_OFF,
                                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_CHID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_CHID_FAIL_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006f0 bits 0) field RXMAC_CHID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT */
    reg_value = odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_MSK) >> OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_CHID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                              odu_ohp_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_CHID_FAIL_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                              odu_ohp_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_ohp_core_field_RXMAC_CHID_FAIL_I_poll", value );

    /* (0x000006f0 bits 0) field RXMAC_CHID_FAIL_I of register PMC_OHP_CORE_REG_RXMAC_CHID_FAIL_INT */
    return odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_poll( b_ptr,
                                                      h_ptr,
                                                      OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_MSK,
                                                      (value<<OHP_CORE_REG_RXMAC_CHID_FAIL_INT_BIT_RXMAC_CHID_FAIL_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_field_RXMAC_PKTERR_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_PKTERR_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                   odu_ohp_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_I_set_to_clear", value );

    /* (0x000006f4 bits 0) field RXMAC_PKTERR_I of register PMC_OHP_CORE_REG_RXMAC_PKTERR_INT */
    odu_ohp_core_reg_RXMAC_PKTERR_INT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_MSK,
                                                                 OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_OFF,
                                                                 value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_PKTERR_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_PKTERR_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006f4 bits 0) field RXMAC_PKTERR_I of register PMC_OHP_CORE_REG_RXMAC_PKTERR_INT */
    reg_value = odu_ohp_core_reg_RXMAC_PKTERR_INT_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_MSK) >> OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_PKTERR_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_PKTERR_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                           odu_ohp_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_ohp_core_field_RXMAC_PKTERR_I_poll", value );

    /* (0x000006f4 bits 0) field RXMAC_PKTERR_I of register PMC_OHP_CORE_REG_RXMAC_PKTERR_INT */
    return odu_ohp_core_reg_RXMAC_PKTERR_INT_poll( b_ptr,
                                                   h_ptr,
                                                   OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_MSK,
                                                   (value<<OHP_CORE_REG_RXMAC_PKTERR_INT_BIT_RXMAC_PKTERR_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void odu_ohp_core_field_RXMAC_FIFO_ERR_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXMAC_FIFO_ERR_I_set_to_clear( odu_ohp_core_buffer_t *b_ptr,
                                                                     odu_ohp_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_I_set_to_clear", value );

    /* (0x000006f8 bits 0) field RXMAC_FIFO_ERR_I of register PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT */
    odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_MSK,
                                                                     OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_OFF,
                                                                     value);
}

static INLINE UINT32 odu_ohp_core_field_RXMAC_FIFO_ERR_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXMAC_FIFO_ERR_I_get( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000006f8 bits 0) field RXMAC_FIFO_ERR_I of register PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT */
    reg_value = odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_MSK) >> OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FIFO_ERR_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FIFO_ERR_I_poll( odu_ohp_core_buffer_t *b_ptr,
                                                                             odu_ohp_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_ohp_core_field_RXMAC_FIFO_ERR_I_poll", value );

    /* (0x000006f8 bits 0) field RXMAC_FIFO_ERR_I of register PMC_OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT */
    return odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_poll( b_ptr,
                                                       h_ptr,
                                                       OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_MSK,
                                                       (value<<OHP_CORE_REG_RXMAC_RXFIFO_ERR_INT_BIT_RXMAC_FIFO_ERR_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize96
 * ==================================================================================
 */
static INLINE void odu_ohp_core_lfield_SRV_CH_ID_A_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_SRV_CH_ID_A_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_SRV_CH_ID_A_set");
    odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         96,
                                                         value);
}

static INLINE void odu_ohp_core_lfield_SRV_CH_ID_A_get( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_SRV_CH_ID_A_get( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_SRV_CH_ID_A_get");
    odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        96,
                                                        (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_SRV_CH_ID_A_set( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_SRV_CH_ID_A_set( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_SRV_CH_ID_A_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_SRV_CH_ID_A_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_field_set( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       field_ofs,
                                                       value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_A_get( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_A_get( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_SRV_CH_ID_A_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_read( b_ptr,
                                                              h_ptr,
                                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_SRV_CH_ID_A_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_ohp_core_lfield_SRV_CH_ID_B_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_SRV_CH_ID_B_set( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_SRV_CH_ID_B_set");
    odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         96,
                                                         value);
}

static INLINE void odu_ohp_core_lfield_SRV_CH_ID_B_get( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_SRV_CH_ID_B_get( odu_ohp_core_buffer_t *b_ptr,
                                                        odu_ohp_handle_t *h_ptr,
                                                        UINT32 value[96] )
{
    IOLOG( "%s ", "odu_ohp_core_lfield_SRV_CH_ID_B_get");
    odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        96,
                                                        (UINT32 *)value);
}

static INLINE void odu_ohp_core_lfield_range_SRV_CH_ID_B_set( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_lfield_range_SRV_CH_ID_B_set( odu_ohp_core_buffer_t *b_ptr,
                                                              odu_ohp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_SRV_CH_ID_B_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_SRV_CH_ID_B_set", start_bit, stop_bit, value);
    odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_field_set( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       field_ofs,
                                                       value);
}

static INLINE UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_B_get( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_B_get( odu_ohp_core_buffer_t *b_ptr,
                                                                odu_ohp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_ohp_core_lfield_range_SRV_CH_ID_B_get", start_bit, stop_bit );
    }
    reg_value = odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_read( b_ptr,
                                                              h_ptr,
                                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_ohp_core_lfield_range_SRV_CH_ID_B_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void odu_ohp_core_field_RXFA_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFA_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_RXFA_EXT_MASK_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFA_EXT_MASK_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_ohp_core_field_RXFA_EXT_MASK_set", N, value );

    /* ((0x00000800 + (N) * 4) bits 15:0) field RXFA_EXT_MASK of register PMC_OHP_CORE_REG_RXFA_EXT_MASK index N=0..95 */
    odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_MSK,
                                                    OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_RXFA_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFA_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_RXFA_EXT_MASK_get", N, 95);
    /* ((0x00000800 + (N) * 4) bits 15:0) field RXFA_EXT_MASK of register PMC_OHP_CORE_REG_RXFA_EXT_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_RXFA_EXT_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_MSK) >> OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_ohp_core_field_RXFA_EXT_MASK_get", N, value );

    return value;
}
static INLINE void odu_ohp_core_field_range_RXFA_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_RXFA_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_RXFA_EXT_MASK_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFA_EXT_MASK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFA_EXT_MASK_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFA_EXT_MASK_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 4) bits 15:0) field RXFA_EXT_MASK of register PMC_OHP_CORE_REG_RXFA_EXT_MASK index N=0..95 */
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
        /* ((0x00000800 + (N) * 4) bits 15:0) field RXFA_EXT_MASK of register PMC_OHP_CORE_REG_RXFA_EXT_MASK index N=0..95 */
        odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF + subfield_offset),
                                                        OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_RXFA_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_RXFA_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_RXFA_EXT_MASK_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFA_EXT_MASK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFA_EXT_MASK_get", stop_bit, 15 );
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
    /* ((0x00000800 + (N) * 4) bits 15:0) field RXFA_EXT_MASK of register PMC_OHP_CORE_REG_RXFA_EXT_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_RXFA_EXT_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_MSK)
                  >> OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_MSK, OHP_CORE_REG_RXFA_EXT_MASK_BIT_RXFA_EXT_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFA_EXT_MASK_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_RXFB_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_RXFB_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_RXFB_EXT_MASK_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_RXFB_EXT_MASK_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_ohp_core_field_RXFB_EXT_MASK_set", N, value );

    /* ((0x00000a00 + (N) * 4) bits 15:0) field RXFB_EXT_MASK of register PMC_OHP_CORE_REG_RXFB_EXT_MASK index N=0..95 */
    odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_MSK,
                                                    OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_RXFB_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_RXFB_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_RXFB_EXT_MASK_get", N, 95);
    /* ((0x00000a00 + (N) * 4) bits 15:0) field RXFB_EXT_MASK of register PMC_OHP_CORE_REG_RXFB_EXT_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_RXFB_EXT_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_MSK) >> OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_ohp_core_field_RXFB_EXT_MASK_get", N, value );

    return value;
}
static INLINE void odu_ohp_core_field_range_RXFB_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_RXFB_EXT_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_RXFB_EXT_MASK_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFB_EXT_MASK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFB_EXT_MASK_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFB_EXT_MASK_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000a00 + (N) * 4) bits 15:0) field RXFB_EXT_MASK of register PMC_OHP_CORE_REG_RXFB_EXT_MASK index N=0..95 */
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
        /* ((0x00000a00 + (N) * 4) bits 15:0) field RXFB_EXT_MASK of register PMC_OHP_CORE_REG_RXFB_EXT_MASK index N=0..95 */
        odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF + subfield_offset),
                                                        OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_RXFB_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_RXFB_EXT_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_RXFB_EXT_MASK_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_RXFB_EXT_MASK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_RXFB_EXT_MASK_get", stop_bit, 15 );
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
    /* ((0x00000a00 + (N) * 4) bits 15:0) field RXFB_EXT_MASK of register PMC_OHP_CORE_REG_RXFB_EXT_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_RXFB_EXT_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_MSK)
                  >> OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_MSK, OHP_CORE_REG_RXFB_EXT_MASK_BIT_RXFB_EXT_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_RXFB_EXT_MASK_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_TXFA_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_TXFA_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_TXFA_INS_MASK_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_TXFA_INS_MASK_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_ohp_core_field_TXFA_INS_MASK_set", N, value );

    /* ((0x00000c00 + (N) * 4) bits 15:0) field TXFA_INS_MASK of register PMC_OHP_CORE_REG_TXFA_INS_MASK index N=0..95 */
    odu_ohp_core_reg_TXFA_INS_MASK_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_MSK,
                                                    OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_TXFA_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_TXFA_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_TXFA_INS_MASK_get", N, 95);
    /* ((0x00000c00 + (N) * 4) bits 15:0) field TXFA_INS_MASK of register PMC_OHP_CORE_REG_TXFA_INS_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_TXFA_INS_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_MSK) >> OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_ohp_core_field_TXFA_INS_MASK_get", N, value );

    return value;
}
static INLINE void odu_ohp_core_field_range_TXFA_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_TXFA_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_TXFA_INS_MASK_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFA_INS_MASK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFA_INS_MASK_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFA_INS_MASK_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 4) bits 15:0) field TXFA_INS_MASK of register PMC_OHP_CORE_REG_TXFA_INS_MASK index N=0..95 */
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
        /* ((0x00000c00 + (N) * 4) bits 15:0) field TXFA_INS_MASK of register PMC_OHP_CORE_REG_TXFA_INS_MASK index N=0..95 */
        odu_ohp_core_reg_TXFA_INS_MASK_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF + subfield_offset),
                                                        OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_TXFA_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_TXFA_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_TXFA_INS_MASK_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFA_INS_MASK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFA_INS_MASK_get", stop_bit, 15 );
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
    /* ((0x00000c00 + (N) * 4) bits 15:0) field TXFA_INS_MASK of register PMC_OHP_CORE_REG_TXFA_INS_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_TXFA_INS_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_MSK)
                  >> OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_MSK, OHP_CORE_REG_TXFA_INS_MASK_BIT_TXFA_INS_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFA_INS_MASK_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_ohp_core_field_TXFB_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_TXFB_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                         odu_ohp_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_TXFB_INS_MASK_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_ohp_core_field_TXFB_INS_MASK_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_ohp_core_field_TXFB_INS_MASK_set", N, value );

    /* ((0x00000e00 + (N) * 4) bits 15:0) field TXFB_INS_MASK of register PMC_OHP_CORE_REG_TXFB_INS_MASK index N=0..95 */
    odu_ohp_core_reg_TXFB_INS_MASK_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_MSK,
                                                    OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF,
                                                    value);
}

static INLINE UINT32 odu_ohp_core_field_TXFB_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_TXFB_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                           odu_ohp_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_TXFB_INS_MASK_get", N, 95);
    /* ((0x00000e00 + (N) * 4) bits 15:0) field TXFB_INS_MASK of register PMC_OHP_CORE_REG_TXFB_INS_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_TXFB_INS_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_MSK) >> OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_ohp_core_field_TXFB_INS_MASK_get", N, value );

    return value;
}
static INLINE void odu_ohp_core_field_range_TXFB_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_ohp_core_field_range_TXFB_INS_MASK_set( odu_ohp_core_buffer_t *b_ptr,
                                                               odu_ohp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_TXFB_INS_MASK_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFB_INS_MASK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFB_INS_MASK_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFB_INS_MASK_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000e00 + (N) * 4) bits 15:0) field TXFB_INS_MASK of register PMC_OHP_CORE_REG_TXFB_INS_MASK index N=0..95 */
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
        /* ((0x00000e00 + (N) * 4) bits 15:0) field TXFB_INS_MASK of register PMC_OHP_CORE_REG_TXFB_INS_MASK index N=0..95 */
        odu_ohp_core_reg_TXFB_INS_MASK_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF + subfield_offset),
                                                        OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_ohp_core_field_range_TXFB_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_ohp_core_field_range_TXFB_INS_MASK_get( odu_ohp_core_buffer_t *b_ptr,
                                                                 odu_ohp_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_ohp_core_field_range_TXFB_INS_MASK_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_ohp_core_field_range_TXFB_INS_MASK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_ohp_core_field_range_TXFB_INS_MASK_get", stop_bit, 15 );
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
    /* ((0x00000e00 + (N) * 4) bits 15:0) field TXFB_INS_MASK of register PMC_OHP_CORE_REG_TXFB_INS_MASK index N=0..95 */
    reg_value = odu_ohp_core_reg_TXFB_INS_MASK_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_MSK)
                  >> OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_MSK, OHP_CORE_REG_TXFB_INS_MASK_BIT_TXFB_INS_MASK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_ohp_core_field_range_TXFB_INS_MASK_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_OHP_CORE_IO_INLINE_H */
