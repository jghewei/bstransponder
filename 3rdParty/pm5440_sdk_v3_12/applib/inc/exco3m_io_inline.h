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
 *     and register accessor functions for the exco3m block
 *****************************************************************************/
#ifndef _EXCO3M_IO_INLINE_H
#define _EXCO3M_IO_INLINE_H

#include "exco3m_loc.h"
#include "exco3m_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EXCO3M_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for exco3m
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
    exco3m_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} exco3m_buffer_t;
static INLINE void exco3m_buffer_init( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void exco3m_buffer_init( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "exco3m_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void exco3m_buffer_flush( exco3m_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void exco3m_buffer_flush( exco3m_buffer_t *b_ptr )
{
    IOLOG( "exco3m_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 exco3m_reg_read( exco3m_buffer_t *b_ptr,
                                      exco3m_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_read( exco3m_buffer_t *b_ptr,
                                      exco3m_handle_t *h_ptr,
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
static INLINE void exco3m_reg_write( exco3m_buffer_t *b_ptr,
                                     exco3m_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_write( exco3m_buffer_t *b_ptr,
                                     exco3m_handle_t *h_ptr,
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

static INLINE void exco3m_field_set( exco3m_buffer_t *b_ptr,
                                     exco3m_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_set( exco3m_buffer_t *b_ptr,
                                     exco3m_handle_t *h_ptr,
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

static INLINE void exco3m_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
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

static INLINE void exco3m_burst_read( exco3m_buffer_t *b_ptr,
                                      exco3m_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void exco3m_burst_read( exco3m_buffer_t *b_ptr,
                                      exco3m_handle_t *h_ptr,
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

static INLINE void exco3m_burst_write( exco3m_buffer_t *b_ptr,
                                       exco3m_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void exco3m_burst_write( exco3m_buffer_t *b_ptr,
                                       exco3m_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE exco3m_poll( exco3m_buffer_t *b_ptr,
                                                exco3m_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_poll( exco3m_buffer_t *b_ptr,
                                                exco3m_handle_t *h_ptr,
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
 *  register access functions for exco3m
 * ==================================================================================
 */

static INLINE void exco3m_reg_SW_RESET_write( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_SW_RESET_write( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_SW_RESET_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_SW_RESET,
                      value);
}

static INLINE void exco3m_reg_SW_RESET_field_set( exco3m_buffer_t *b_ptr,
                                                  exco3m_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_SW_RESET_field_set( exco3m_buffer_t *b_ptr,
                                                  exco3m_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_SW_RESET_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_SW_RESET,
                      mask,
                      PMC_EXCO3M_REG_SW_RESET_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_SW_RESET_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_SW_RESET_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_RX_PUSH_CFG_REG_write( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_PUSH_CFG_REG_write( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_PUSH_CFG_REG_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_PUSH_CFG_REG,
                      value);
}

static INLINE void exco3m_reg_RX_PUSH_CFG_REG_field_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_PUSH_CFG_REG_field_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_PUSH_CFG_REG_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_PUSH_CFG_REG,
                      mask,
                      PMC_EXCO3M_REG_RX_PUSH_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_RX_PUSH_CFG_REG_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_PUSH_CFG_REG_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_RX_PUSH_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_PUSH_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_TX_GLOBAL_CTRL_write( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_GLOBAL_CTRL_write( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_GLOBAL_CTRL_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_TX_GLOBAL_CTRL,
                      value);
}

static INLINE void exco3m_reg_TX_GLOBAL_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_GLOBAL_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_GLOBAL_CTRL_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_TX_GLOBAL_CTRL,
                      mask,
                      PMC_EXCO3M_REG_TX_GLOBAL_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_TX_GLOBAL_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_GLOBAL_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_TX_GLOBAL_CTRL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_GLOBAL_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_TX_FAULT_CTRL_write( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_FAULT_CTRL_write( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_FAULT_CTRL_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_TX_FAULT_CTRL,
                      value);
}

static INLINE void exco3m_reg_TX_FAULT_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_FAULT_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_FAULT_CTRL_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_TX_FAULT_CTRL,
                      mask,
                      PMC_EXCO3M_REG_TX_FAULT_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_TX_FAULT_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_FAULT_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_TX_FAULT_CTRL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_FAULT_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_RX_GLOBAL_CTRL_write( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_GLOBAL_CTRL_write( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_GLOBAL_CTRL_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_GLOBAL_CTRL,
                      value);
}

static INLINE void exco3m_reg_RX_GLOBAL_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_GLOBAL_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_GLOBAL_CTRL_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_GLOBAL_CTRL,
                      mask,
                      PMC_EXCO3M_REG_RX_GLOBAL_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_RX_GLOBAL_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_GLOBAL_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_RX_GLOBAL_CTRL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_GLOBAL_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_RX_FAULT_CTRL_write( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_FAULT_CTRL_write( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_FAULT_CTRL_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_FAULT_CTRL,
                      value);
}

static INLINE void exco3m_reg_RX_FAULT_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_FAULT_CTRL_field_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_FAULT_CTRL_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_EXCO3M_REG_RX_FAULT_CTRL,
                      mask,
                      PMC_EXCO3M_REG_RX_FAULT_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_RX_FAULT_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_FAULT_CTRL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_EXCO3M_REG_RX_FAULT_CTRL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_FAULT_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_TX_INT_EN_write( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_INT_EN_write( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_INT_EN_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_INT_EN,
                      value);
}

static INLINE void exco3m_reg_TX_INT_EN_field_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_INT_EN_field_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_INT_EN_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_INT_EN,
                      mask,
                      PMC_EXCO3M_REG_TX_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_TX_INT_EN_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_INT_EN_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_INT_EN);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_RX_INT_EN_write( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_INT_EN_write( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_INT_EN_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_INT_EN,
                      value);
}

static INLINE void exco3m_reg_RX_INT_EN_field_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_INT_EN_field_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_INT_EN_field_set", mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_INT_EN,
                      mask,
                      PMC_EXCO3M_REG_RX_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_RX_INT_EN_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_INT_EN_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_INT_EN);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void exco3m_reg_TX_LANE_INT_EN_array_write( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_LANE_INT_EN_array_write( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_LANE_INT_EN_array_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_LANE_INT_EN(N),
                      value);
}

static INLINE void exco3m_reg_TX_LANE_INT_EN_array_field_set( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_LANE_INT_EN_array_field_set( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_LANE_INT_EN_array_field_set", N, mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_LANE_INT_EN(N),
                      mask,
                      PMC_EXCO3M_REG_TX_LANE_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_TX_LANE_INT_EN_array_read( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_LANE_INT_EN_array_read( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_LANE_INT_EN(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_TX_LANE_INT_EN_array_read", reg_value, N);
    return reg_value;
}

static INLINE void exco3m_reg_RX_LANE_INT_EN_array_write( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_LANE_INT_EN_array_write( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_LANE_INT_EN_array_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_LANE_INT_EN(N),
                      value);
}

static INLINE void exco3m_reg_RX_LANE_INT_EN_array_field_set( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_LANE_INT_EN_array_field_set( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_LANE_INT_EN_array_field_set", N, mask, ofs, value );
    exco3m_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_LANE_INT_EN(N),
                      mask,
                      PMC_EXCO3M_REG_RX_LANE_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 exco3m_reg_RX_LANE_INT_EN_array_read( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_LANE_INT_EN_array_read( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_LANE_INT_EN(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_RX_LANE_INT_EN_array_read", reg_value, N);
    return reg_value;
}

static INLINE void exco3m_reg_TX_INT_STAT_write( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_INT_STAT_write( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_INT_STAT_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_INT_STAT,
                      value);
}

static INLINE void exco3m_reg_TX_INT_STAT_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_INT_STAT_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_INT_STAT_action_on_write_field_set", mask, ofs, value );
    exco3m_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXCO3M_REG_TX_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 exco3m_reg_TX_INT_STAT_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_INT_STAT_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_STAT_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_STAT_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_TX_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_TX_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void exco3m_reg_RX_INT_STAT_write( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_INT_STAT_write( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_INT_STAT_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_INT_STAT,
                      value);
}

static INLINE void exco3m_reg_RX_INT_STAT_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_INT_STAT_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_INT_STAT_action_on_write_field_set", mask, ofs, value );
    exco3m_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXCO3M_REG_RX_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 exco3m_reg_RX_INT_STAT_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_INT_STAT_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_STAT_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_STAT_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_RX_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_RX_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void exco3m_reg_TX_LANE_INT_STAT_array_write( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_LANE_INT_STAT_array_write( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_TX_LANE_INT_STAT_array_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_TX_LANE_INT_STAT(N),
                      value);
}

static INLINE void exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                                exco3m_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                                exco3m_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set", N, mask, ofs, value );
    exco3m_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXCO3M_REG_TX_LANE_INT_STAT(N),
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 exco3m_reg_TX_LANE_INT_STAT_array_read( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_LANE_INT_STAT_array_read( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_LANE_INT_STAT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_TX_LANE_INT_STAT_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_STAT_array_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_STAT_array_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_TX_LANE_INT_STAT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_TX_LANE_INT_STAT(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void exco3m_reg_RX_LANE_INT_STAT_array_write( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_LANE_INT_STAT_array_write( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "exco3m_reg_RX_LANE_INT_STAT_array_write", value );
    exco3m_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EXCO3M_REG_RX_LANE_INT_STAT(N),
                      value);
}

static INLINE void exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                                exco3m_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set( exco3m_buffer_t *b_ptr,
                                                                                exco3m_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set", N, mask, ofs, value );
    exco3m_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXCO3M_REG_RX_LANE_INT_STAT(N),
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 exco3m_reg_RX_LANE_INT_STAT_array_read( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_LANE_INT_STAT_array_read( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_LANE_INT_STAT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_RX_LANE_INT_STAT_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_STAT_array_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_STAT_array_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_RX_LANE_INT_STAT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_RX_LANE_INT_STAT(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 exco3m_reg_TX_INT_VAL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_INT_VAL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_VAL_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_VAL_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_TX_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_TX_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 exco3m_reg_RX_INT_VAL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_INT_VAL_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_VAL_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_VAL_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_RX_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_RX_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 exco3m_reg_TX_LANE_INT_VAL_array_read( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_LANE_INT_VAL_array_read( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_LANE_INT_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_TX_LANE_INT_VAL_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_VAL_array_poll( exco3m_buffer_t *b_ptr,
                                                                          exco3m_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_VAL_array_poll( exco3m_buffer_t *b_ptr,
                                                                          exco3m_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_TX_LANE_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_TX_LANE_INT_VAL(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 exco3m_reg_RX_LANE_INT_VAL_array_read( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_LANE_INT_VAL_array_read( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_LANE_INT_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_RX_LANE_INT_VAL_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_VAL_array_poll( exco3m_buffer_t *b_ptr,
                                                                          exco3m_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_VAL_array_poll( exco3m_buffer_t *b_ptr,
                                                                          exco3m_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "exco3m_reg_RX_LANE_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return exco3m_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_EXCO3M_REG_RX_LANE_INT_VAL(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 exco3m_reg_TX_PCS_BER_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_PCS_BER_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_PCS_BER_CNT_LOWER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_PCS_BER_CNT_LOWER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_TX_PCS_BER_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_PCS_BER_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_PCS_BER_CNT_UPPER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_PCS_BER_CNT_UPPER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_RX_1027B_BER_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_1027B_BER_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_1027B_BER_CNT_LOWER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_1027B_BER_CNT_LOWER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_RX_1027B_BER_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_1027B_BER_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_1027B_BER_CNT_UPPER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_1027B_BER_CNT_UPPER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER);

    IOLOG( "%s -> 0x%08x;", "exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read", reg_value);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_TX_LANE_INFO_array_read( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_LANE_INFO_array_read( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_LANE_INFO(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_TX_LANE_INFO_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = exco3m_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void exco3m_field_EXCO_CORE_SW_RESET_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_EXCO_CORE_SW_RESET_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_EXCO_CORE_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_EXCO_CORE_SW_RESET_set", value );

    /* (0x00000000 bits 0) field EXCO_CORE_SW_RESET of register PMC_EXCO3M_REG_SW_RESET */
    exco3m_reg_SW_RESET_field_set( b_ptr,
                                   h_ptr,
                                   EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_MSK,
                                   EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_OFF,
                                   value);
}

static INLINE UINT32 exco3m_field_EXCO_CORE_SW_RESET_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_EXCO_CORE_SW_RESET_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field EXCO_CORE_SW_RESET of register PMC_EXCO3M_REG_SW_RESET */
    reg_value = exco3m_reg_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_MSK) >> EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_EXCO_CORE_SW_RESET_get", value );

    return value;
}
static INLINE void exco3m_field_RX_PCS_PUSH_ENA_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_PUSH_ENA_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_PUSH_ENA_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_PCS_PUSH_ENA_set", value );

    /* (0x00000010 bits 0) field RX_PCS_PUSH_ENA of register PMC_EXCO3M_REG_RX_PUSH_CFG_REG */
    exco3m_reg_RX_PUSH_CFG_REG_field_set( b_ptr,
                                          h_ptr,
                                          EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_MSK,
                                          EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_OFF,
                                          value);
}

static INLINE UINT32 exco3m_field_RX_PCS_PUSH_ENA_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_PUSH_ENA_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field RX_PCS_PUSH_ENA of register PMC_EXCO3M_REG_RX_PUSH_CFG_REG */
    reg_value = exco3m_reg_RX_PUSH_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_MSK) >> EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_PCS_PUSH_ENA_get", value );

    return value;
}
static INLINE void exco3m_field_TX_XLGMII_MODE_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_XLGMII_MODE_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_XLGMII_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_XLGMII_MODE_set", value );

    /* (0x00000024 bits 14) field TX_XLGMII_MODE of register PMC_EXCO3M_REG_TX_GLOBAL_CTRL */
    exco3m_reg_TX_GLOBAL_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_MSK,
                                         EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_OFF,
                                         value);
}

static INLINE UINT32 exco3m_field_TX_XLGMII_MODE_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_XLGMII_MODE_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 14) field TX_XLGMII_MODE of register PMC_EXCO3M_REG_TX_GLOBAL_CTRL */
    reg_value = exco3m_reg_TX_GLOBAL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_MSK) >> EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_XLGMII_MODE_get", value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_ERR_INS_EN_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_ERR_INS_EN_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_ERR_INS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_ERR_INS_EN_set", value );

    /* (0x00000024 bits 1) field TX_PCS_ERR_INS_EN of register PMC_EXCO3M_REG_TX_GLOBAL_CTRL */
    exco3m_reg_TX_GLOBAL_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_MSK,
                                         EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_OFF,
                                         value);
}

static INLINE UINT32 exco3m_field_TX_PCS_ERR_INS_EN_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_ERR_INS_EN_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 1) field TX_PCS_ERR_INS_EN of register PMC_EXCO3M_REG_TX_GLOBAL_CTRL */
    reg_value = exco3m_reg_TX_GLOBAL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_MSK) >> EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_ERR_INS_EN_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_LF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_LF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_LF_OS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_LF_OS_set", value );

    /* (0x00000028 bits 15) field TX_FORCE_LF_OS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 15) field TX_FORCE_LF_OS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_LF_OS_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_RF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_RF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_RF_OS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_RF_OS_set", value );

    /* (0x00000028 bits 14) field TX_FORCE_RF_OS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_RF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_RF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 14) field TX_FORCE_RF_OS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_RF_OS_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_PCS_ERR_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_PCS_ERR_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_PCS_ERR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_PCS_ERR_set", value );

    /* (0x00000028 bits 13) field TX_FORCE_PCS_ERR of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_PCS_ERR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_PCS_ERR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 13) field TX_FORCE_PCS_ERR of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_PCS_ERR_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_IDLE_TEST_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_IDLE_TEST_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_IDLE_TEST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_IDLE_TEST_set", value );

    /* (0x00000028 bits 12) field TX_FORCE_IDLE_TEST of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_IDLE_TEST_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_IDLE_TEST_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 12) field TX_FORCE_IDLE_TEST of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_IDLE_TEST_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_LF_OS_LOS_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_LF_OS_LOS_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOS_set", value );

    /* (0x00000028 bits 11) field TX_FORCE_LF_OS_LOS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_LOS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_LOS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 11) field TX_FORCE_LF_OS_LOS of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOS_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_set( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_set( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_set", value );

    /* (0x00000028 bits 10) field TX_FORCE_LF_OS_PCS_LOBL of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 10) field TX_FORCE_LF_OS_PCS_LOBL of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_LF_OS_LOAM_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_LF_OS_LOAM_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOAM_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOAM_set", value );

    /* (0x00000028 bits 9) field TX_FORCE_LF_OS_LOAM of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_LOAM_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_LOAM_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 9) field TX_FORCE_LF_OS_LOAM of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_LF_OS_LOAM_get", value );

    return value;
}
static INLINE void exco3m_field_TX_FORCE_LF_OS_SKEW_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_FORCE_LF_OS_SKEW_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_FORCE_LF_OS_SKEW_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_FORCE_LF_OS_SKEW_set", value );

    /* (0x00000028 bits 8) field TX_FORCE_LF_OS_SKEW of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    exco3m_reg_TX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_MSK,
                                        EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_SKEW_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_FORCE_LF_OS_SKEW_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 8) field TX_FORCE_LF_OS_SKEW of register PMC_EXCO3M_REG_TX_FAULT_CTRL */
    reg_value = exco3m_reg_TX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_MSK) >> EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_FORCE_LF_OS_SKEW_get", value );

    return value;
}
static INLINE void exco3m_field_RX_XLGMII_MODE_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_XLGMII_MODE_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_XLGMII_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_XLGMII_MODE_set", value );

    /* (0x00000424 bits 14) field RX_XLGMII_MODE of register PMC_EXCO3M_REG_RX_GLOBAL_CTRL */
    exco3m_reg_RX_GLOBAL_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_MSK,
                                         EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_OFF,
                                         value);
}

static INLINE UINT32 exco3m_field_RX_XLGMII_MODE_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_XLGMII_MODE_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000424 bits 14) field RX_XLGMII_MODE of register PMC_EXCO3M_REG_RX_GLOBAL_CTRL */
    reg_value = exco3m_reg_RX_GLOBAL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_MSK) >> EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_XLGMII_MODE_get", value );

    return value;
}
static INLINE void exco3m_field_RX_PCS_ERR_INS_EN_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_ERR_INS_EN_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_ERR_INS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_PCS_ERR_INS_EN_set", value );

    /* (0x00000424 bits 1) field RX_PCS_ERR_INS_EN of register PMC_EXCO3M_REG_RX_GLOBAL_CTRL */
    exco3m_reg_RX_GLOBAL_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_MSK,
                                         EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_OFF,
                                         value);
}

static INLINE UINT32 exco3m_field_RX_PCS_ERR_INS_EN_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_ERR_INS_EN_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000424 bits 1) field RX_PCS_ERR_INS_EN of register PMC_EXCO3M_REG_RX_GLOBAL_CTRL */
    reg_value = exco3m_reg_RX_GLOBAL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_MSK) >> EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_PCS_ERR_INS_EN_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_LF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_LF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_LF_OS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_LF_OS_set", value );

    /* (0x00000428 bits 15) field RX_FORCE_LF_OS of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 15) field RX_FORCE_LF_OS of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_LF_OS_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_RF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_RF_OS_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_RF_OS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_RF_OS_set", value );

    /* (0x00000428 bits 14) field RX_FORCE_RF_OS of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_RF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_RF_OS_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 14) field RX_FORCE_RF_OS of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_RF_OS_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_PCS_ERR_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_PCS_ERR_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_PCS_ERR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_PCS_ERR_set", value );

    /* (0x00000428 bits 13) field RX_FORCE_PCS_ERR of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_PCS_ERR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_PCS_ERR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 13) field RX_FORCE_PCS_ERR of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_PCS_ERR_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_IDLE_TEST_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_IDLE_TEST_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_IDLE_TEST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_IDLE_TEST_set", value );

    /* (0x00000428 bits 12) field RX_FORCE_IDLE_TEST of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_IDLE_TEST_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_IDLE_TEST_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 12) field RX_FORCE_IDLE_TEST of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_IDLE_TEST_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_LF_OS_TSF_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_LF_OS_TSF_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_LF_OS_TSF_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_LF_OS_TSF_set", value );

    /* (0x00000428 bits 11) field RX_FORCE_LF_OS_TSF of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_TSF_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_TSF_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 11) field RX_FORCE_LF_OS_TSF of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_LF_OS_TSF_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_LF_OS_LOBL_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_LF_OS_LOBL_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOBL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOBL_set", value );

    /* (0x00000428 bits 10) field RX_FORCE_LF_OS_LOBL of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_LOBL_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_LOBL_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 10) field RX_FORCE_LF_OS_LOBL of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOBL_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_LF_OS_LOAM_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_LF_OS_LOAM_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOAM_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOAM_set", value );

    /* (0x00000428 bits 9) field RX_FORCE_LF_OS_LOAM of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_LOAM_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_LOAM_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 9) field RX_FORCE_LF_OS_LOAM of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_LF_OS_LOAM_get", value );

    return value;
}
static INLINE void exco3m_field_RX_FORCE_LF_OS_SKEW_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_FORCE_LF_OS_SKEW_set( exco3m_buffer_t *b_ptr,
                                                         exco3m_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_FORCE_LF_OS_SKEW_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_FORCE_LF_OS_SKEW_set", value );

    /* (0x00000428 bits 8) field RX_FORCE_LF_OS_SKEW of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    exco3m_reg_RX_FAULT_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_MSK,
                                        EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_OFF,
                                        value);
}

static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_SKEW_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_FORCE_LF_OS_SKEW_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 8) field RX_FORCE_LF_OS_SKEW of register PMC_EXCO3M_REG_RX_FAULT_CTRL */
    reg_value = exco3m_reg_RX_FAULT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_MSK) >> EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_FORCE_LF_OS_SKEW_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void exco3m_field_TX_LOS_E_set( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LOS_E_set( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_LOS_E_set", value );

    /* (0x00000038 bits 15) field TX_LOS_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_LOS_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOS_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 15) field TX_LOS_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_LOS_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_LF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LF_OS_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_LF_OS_DET_E_set", value );

    /* (0x00000038 bits 14) field TX_LF_OS_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_LF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 14) field TX_LF_OS_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_LF_OS_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_RF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_RF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_RF_OS_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_RF_OS_DET_E_set", value );

    /* (0x00000038 bits 13) field TX_RF_OS_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_RF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_RF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 13) field TX_RF_OS_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_RF_OS_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_IDLE_TEST_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_IDLE_TEST_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_E_set", value );

    /* (0x00000038 bits 12) field TX_IDLE_TEST_ERR_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_IDLE_TEST_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_IDLE_TEST_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 12) field TX_IDLE_TEST_ERR_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_SKEW_RANGE_VIOL_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_SKEW_RANGE_VIOL_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_E_set", value );

    /* (0x00000038 bits 11) field TX_SKEW_RANGE_VIOL_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_SKEW_RANGE_VIOL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_SKEW_RANGE_VIOL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 11) field TX_SKEW_RANGE_VIOL_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_INV_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_INV_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_E_set", value );

    /* (0x00000038 bits 10) field TX_PCS_INV_BLK_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_PCS_INV_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_INV_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 10) field TX_PCS_INV_BLK_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_ERR_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_ERR_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_E_set", value );

    /* (0x00000038 bits 9) field TX_PCS_ERR_BLK_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_PCS_ERR_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_ERR_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 9) field TX_PCS_ERR_BLK_DET_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_HBER_E_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_HBER_E_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_HBER_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_HBER_E_set", value );

    /* (0x00000038 bits 8) field TX_PCS_HBER_E of register PMC_EXCO3M_REG_TX_INT_EN */
    exco3m_reg_TX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_MSK,
                                    EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_TX_PCS_HBER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_HBER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 8) field TX_PCS_HBER_E of register PMC_EXCO3M_REG_TX_INT_EN */
    reg_value = exco3m_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_MSK) >> EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_HBER_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_TSF_E_set( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_TSF_E_set( exco3m_buffer_t *b_ptr,
                                              exco3m_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_TSF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_TSF_E_set", value );

    /* (0x00000438 bits 15) field RX_TSF_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_TSF_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_TSF_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 15) field RX_TSF_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_TSF_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_LF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LF_OS_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_LF_OS_DET_E_set", value );

    /* (0x00000438 bits 14) field RX_LF_OS_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_LF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 14) field RX_LF_OS_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_LF_OS_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_RF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_RF_OS_DET_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_RF_OS_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_RF_OS_DET_E_set", value );

    /* (0x00000438 bits 13) field RX_RF_OS_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_RF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_RF_OS_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 13) field RX_RF_OS_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_RF_OS_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_IDLE_TEST_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_IDLE_TEST_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_E_set", value );

    /* (0x00000438 bits 12) field RX_IDLE_TEST_ERR_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_IDLE_TEST_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_IDLE_TEST_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 12) field RX_IDLE_TEST_ERR_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_PCS_ERR_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_ERR_BLK_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_E_set", value );

    /* (0x00000438 bits 11) field RX_PCS_ERR_BLK_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_PCS_ERR_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_ERR_BLK_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 11) field RX_PCS_ERR_BLK_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_1027B_LOBL_E_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_LOBL_E_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_LOBL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_LOBL_E_set", value );

    /* (0x00000438 bits 10) field RX_1027B_LOBL_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_1027B_LOBL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_LOBL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 10) field RX_1027B_LOBL_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_LOBL_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_1027B_HBER_E_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_HBER_E_set( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_HBER_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_HBER_E_set", value );

    /* (0x00000438 bits 9) field RX_1027B_HBER_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_1027B_HBER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_HBER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 9) field RX_1027B_HBER_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_HBER_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_1027B_BER_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_BER_E_set( exco3m_buffer_t *b_ptr,
                                                    exco3m_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_BER_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_BER_E_set", value );

    /* (0x00000438 bits 8) field RX_1027B_BER_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_1027B_BER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_BER_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 8) field RX_1027B_BER_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_BER_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_SKEW_RANGE_VIOL_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_SKEW_RANGE_VIOL_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_E_set", value );

    /* (0x00000438 bits 7) field RX_SKEW_RANGE_VIOL_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_SKEW_RANGE_VIOL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_SKEW_RANGE_VIOL_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 7) field RX_SKEW_RANGE_VIOL_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_513B_POS_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_513B_POS_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                           exco3m_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_E_set", value );

    /* (0x00000438 bits 5) field RX_513B_POS_ERR_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_513B_POS_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_513B_POS_ERR_DET_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 5) field RX_513B_POS_ERR_DET_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_WELL_PKT_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_WELL_PKT_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_E_set", value );

    /* (0x00000438 bits 4) field RX_WELL_PKT_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_WELL_PKT_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_WELL_PKT_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 4) field RX_WELL_PKT_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_LANE_ALGN_MRK_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LANE_ALGN_MRK_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_E_set", value );

    /* (0x00000438 bits 2) field RX_LANE_ALGN_MRK_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_LANE_ALGN_MRK_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LANE_ALGN_MRK_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 2) field RX_LANE_ALGN_MRK_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_E_get", value );

    return value;
}
static INLINE void exco3m_field_RX_1027B_PAR_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_PAR_ERR_E_set( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_E_set", value );

    /* (0x00000438 bits 1) field RX_1027B_PAR_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    exco3m_reg_RX_INT_EN_field_set( b_ptr,
                                    h_ptr,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_MSK,
                                    EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_OFF,
                                    value);
}

static INLINE UINT32 exco3m_field_RX_1027B_PAR_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_PAR_ERR_E_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 1) field RX_1027B_PAR_ERR_E of register PMC_EXCO3M_REG_RX_INT_EN */
    reg_value = exco3m_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_MSK) >> EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_size4
 * ==================================================================================
 */
static INLINE void exco3m_field_TX_PCS_LOBL_E_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_LOBL_E_set( exco3m_buffer_t *b_ptr,
                                                   exco3m_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_LOBL_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_LOBL_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_E_set", N, value );

    /* ((0x00000200 + (N) * 4) bits 15) field TX_PCS_LOBL_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    exco3m_reg_TX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_MSK,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_TX_PCS_LOBL_E_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_LOBL_E_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_LOBL_E_get", N, 3);
    /* ((0x00000200 + (N) * 4) bits 15) field TX_PCS_LOBL_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_MSK) >> EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_BER_DET_E_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_BER_DET_E_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BER_DET_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_BER_DET_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_BER_DET_E_set", N, value );

    /* ((0x00000200 + (N) * 4) bits 14) field TX_PCS_BER_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    exco3m_reg_TX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_MSK,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_TX_PCS_BER_DET_E_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BER_DET_E_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BER_DET_E_get", N, 3);
    /* ((0x00000200 + (N) * 4) bits 14) field TX_PCS_BER_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_MSK) >> EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_BER_DET_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_TX_PCS_BIP_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_BIP_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BIP_ERR_DET_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_E_set", N, value );

    /* ((0x00000200 + (N) * 4) bits 13) field TX_PCS_BIP_ERR_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    exco3m_reg_TX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_MSK,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BIP_ERR_DET_E_get", N, 3);
    /* ((0x00000200 + (N) * 4) bits 13) field TX_PCS_BIP_ERR_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_MSK) >> EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_TX_LOAM_E_set( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LOAM_E_set( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LOAM_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LOAM_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_LOAM_E_set", N, value );

    /* ((0x00000200 + (N) * 4) bits 12) field TX_LOAM_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    exco3m_reg_TX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_MSK,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_TX_LOAM_E_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOAM_E_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LOAM_E_get", N, 3);
    /* ((0x00000200 + (N) * 4) bits 12) field TX_LOAM_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_MSK) >> EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_LOAM_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_TX_MRK_INV_DET_E_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_MRK_INV_DET_E_set( exco3m_buffer_t *b_ptr,
                                                      exco3m_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_MRK_INV_DET_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_MRK_INV_DET_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_MRK_INV_DET_E_set", N, value );

    /* ((0x00000200 + (N) * 4) bits 11) field TX_MRK_INV_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    exco3m_reg_TX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_MSK,
                                               EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_TX_MRK_INV_DET_E_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_MRK_INV_DET_E_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_MRK_INV_DET_E_get", N, 3);
    /* ((0x00000200 + (N) * 4) bits 11) field TX_MRK_INV_DET_E of register PMC_EXCO3M_REG_TX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_MSK) >> EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_MRK_INV_DET_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_RX_OTN_BIP_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_OTN_BIP_ERR_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_OTN_BIP_ERR_DET_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_E_set", N, value );

    /* ((0x00000680 + (N) * 4) bits 15) field RX_OTN_BIP_ERR_DET_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    exco3m_reg_RX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_MSK,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_OTN_BIP_ERR_DET_E_get", N, 3);
    /* ((0x00000680 + (N) * 4) bits 15) field RX_OTN_BIP_ERR_DET_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_MSK) >> EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_RX_LOAM_E_set( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LOAM_E_set( exco3m_buffer_t *b_ptr,
                                               exco3m_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_LOAM_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LOAM_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_LOAM_E_set", N, value );

    /* ((0x00000680 + (N) * 4) bits 14) field RX_LOAM_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    exco3m_reg_RX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_MSK,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_RX_LOAM_E_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LOAM_E_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_LOAM_E_get", N, 3);
    /* ((0x00000680 + (N) * 4) bits 14) field RX_LOAM_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_MSK) >> EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_LOAM_E_get", N, value );

    return value;
}
static INLINE void exco3m_field_RX_PCS_MRK_INV_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_MRK_INV_DET_E_set( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_PCS_MRK_INV_DET_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_E_set", N, value );

    /* ((0x00000680 + (N) * 4) bits 13) field RX_PCS_MRK_INV_DET_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    exco3m_reg_RX_LANE_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_MSK,
                                               EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_OFF,
                                               value);
}

static INLINE UINT32 exco3m_field_RX_PCS_MRK_INV_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_MRK_INV_DET_E_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_PCS_MRK_INV_DET_E_get", N, 3);
    /* ((0x00000680 + (N) * 4) bits 13) field RX_PCS_MRK_INV_DET_E of register PMC_EXCO3M_REG_RX_LANE_INT_EN index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_MSK) >> EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_E_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void exco3m_field_TX_LOS_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LOS_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_LOS_I_set_to_clear", value );

    /* (0x00000034 bits 15) field TX_LOS_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_LOS_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOS_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 15) field TX_LOS_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_LOS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_I_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_I_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_LOS_I_poll", value );

    /* (0x00000034 bits 15) field TX_LOS_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_LF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LF_OS_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_LF_OS_DET_I_set_to_clear", value );

    /* (0x00000034 bits 14) field TX_LF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_LF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 14) field TX_LF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_LF_OS_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_LF_OS_DET_I_poll", value );

    /* (0x00000034 bits 14) field TX_LF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_RF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_RF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_RF_OS_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_RF_OS_DET_I_set_to_clear", value );

    /* (0x00000034 bits 13) field TX_RF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_RF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_RF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 13) field TX_RF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_RF_OS_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_RF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_RF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_RF_OS_DET_I_poll", value );

    /* (0x00000034 bits 13) field TX_RF_OS_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear", value );

    /* (0x00000034 bits 12) field TX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_IDLE_TEST_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_IDLE_TEST_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 12) field TX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_IDLE_TEST_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_IDLE_TEST_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_IDLE_TEST_ERR_DET_I_poll", value );

    /* (0x00000034 bits 12) field TX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear", value );

    /* (0x00000034 bits 11) field TX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_SKEW_RANGE_VIOL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_SKEW_RANGE_VIOL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 11) field TX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_SKEW_RANGE_VIOL_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_SKEW_RANGE_VIOL_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_SKEW_RANGE_VIOL_I_poll", value );

    /* (0x00000034 bits 11) field TX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_PCS_INV_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_INV_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_I_set_to_clear", value );

    /* (0x00000034 bits 10) field TX_PCS_INV_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_PCS_INV_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_INV_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 10) field TX_PCS_INV_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_INV_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_INV_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_PCS_INV_BLK_DET_I_poll", value );

    /* (0x00000034 bits 10) field TX_PCS_INV_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear", value );

    /* (0x00000034 bits 9) field TX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_PCS_ERR_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_ERR_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 9) field TX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_ERR_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_ERR_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_PCS_ERR_BLK_DET_I_poll", value );

    /* (0x00000034 bits 9) field TX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_PCS_HBER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_HBER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_HBER_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_TX_PCS_HBER_I_set_to_clear", value );

    /* (0x00000034 bits 8) field TX_PCS_HBER_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    exco3m_reg_TX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_MSK,
                                                      EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_TX_PCS_HBER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_HBER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 8) field TX_PCS_HBER_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    reg_value = exco3m_reg_TX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_MSK) >> EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_HBER_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_I_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_I_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_PCS_HBER_I_poll", value );

    /* (0x00000034 bits 8) field TX_PCS_HBER_I of register PMC_EXCO3M_REG_TX_INT_STAT */
    return exco3m_reg_TX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_MSK,
                                        (value<<EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_TSF_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_TSF_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                       exco3m_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_TSF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_TSF_I_set_to_clear", value );

    /* (0x00000434 bits 15) field RX_TSF_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_TSF_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_TSF_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 15) field RX_TSF_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_TSF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_I_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_I_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_TSF_I_poll", value );

    /* (0x00000434 bits 15) field RX_TSF_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_LF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LF_OS_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_LF_OS_DET_I_set_to_clear", value );

    /* (0x00000434 bits 14) field RX_LF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_LF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 14) field RX_LF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_LF_OS_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_LF_OS_DET_I_poll", value );

    /* (0x00000434 bits 14) field RX_LF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_RF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_RF_OS_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_RF_OS_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_RF_OS_DET_I_set_to_clear", value );

    /* (0x00000434 bits 13) field RX_RF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_RF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_RF_OS_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 13) field RX_RF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_RF_OS_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_RF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_RF_OS_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_RF_OS_DET_I_poll", value );

    /* (0x00000434 bits 13) field RX_RF_OS_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_IDLE_TEST_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_IDLE_TEST_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_I_set_to_clear", value );

    /* (0x00000434 bits 12) field RX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_IDLE_TEST_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_IDLE_TEST_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 12) field RX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_IDLE_TEST_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_IDLE_TEST_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_IDLE_TEST_ERR_DET_I_poll", value );

    /* (0x00000434 bits 12) field RX_IDLE_TEST_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_PCS_ERR_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_ERR_BLK_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_I_set_to_clear", value );

    /* (0x00000434 bits 11) field RX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_PCS_ERR_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_ERR_BLK_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 11) field RX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_ERR_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_ERR_BLK_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_PCS_ERR_BLK_DET_I_poll", value );

    /* (0x00000434 bits 11) field RX_PCS_ERR_BLK_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_1027B_LOBL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_LOBL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_LOBL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_LOBL_I_set_to_clear", value );

    /* (0x00000434 bits 10) field RX_1027B_LOBL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_1027B_LOBL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_LOBL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 10) field RX_1027B_LOBL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_LOBL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_I_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_I_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_LOBL_I_poll", value );

    /* (0x00000434 bits 10) field RX_1027B_LOBL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_1027B_HBER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_HBER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_HBER_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_HBER_I_set_to_clear", value );

    /* (0x00000434 bits 9) field RX_1027B_HBER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_1027B_HBER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_HBER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 9) field RX_1027B_HBER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_HBER_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_I_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_I_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_HBER_I_poll", value );

    /* (0x00000434 bits 9) field RX_1027B_HBER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_1027B_BER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_BER_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                             exco3m_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_BER_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_BER_I_set_to_clear", value );

    /* (0x00000434 bits 8) field RX_1027B_BER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_1027B_BER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_BER_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 8) field RX_1027B_BER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_BER_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_BER_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_BER_I_poll( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_BER_I_poll", value );

    /* (0x00000434 bits 8) field RX_1027B_BER_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_SKEW_RANGE_VIOL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_SKEW_RANGE_VIOL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_I_set_to_clear", value );

    /* (0x00000434 bits 7) field RX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_SKEW_RANGE_VIOL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_SKEW_RANGE_VIOL_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 7) field RX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_SKEW_RANGE_VIOL_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_SKEW_RANGE_VIOL_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_SKEW_RANGE_VIOL_I_poll", value );

    /* (0x00000434 bits 7) field RX_SKEW_RANGE_VIOL_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_513B_POS_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_513B_POS_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_I_set_to_clear", value );

    /* (0x00000434 bits 5) field RX_513B_POS_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_513B_POS_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_513B_POS_ERR_DET_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 5) field RX_513B_POS_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_513B_POS_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                            exco3m_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_513B_POS_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                            exco3m_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_513B_POS_ERR_DET_I_poll", value );

    /* (0x00000434 bits 5) field RX_513B_POS_ERR_DET_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_WELL_PKT_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_WELL_PKT_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_I_set_to_clear", value );

    /* (0x00000434 bits 4) field RX_WELL_PKT_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_WELL_PKT_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_WELL_PKT_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 4) field RX_WELL_PKT_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_WELL_PKT_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_WELL_PKT_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                        exco3m_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_WELL_PKT_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                        exco3m_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_WELL_PKT_ERR_I_poll", value );

    /* (0x00000434 bits 4) field RX_WELL_PKT_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_LANE_ALGN_MRK_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LANE_ALGN_MRK_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                     exco3m_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_I_set_to_clear", value );

    /* (0x00000434 bits 2) field RX_LANE_ALGN_MRK_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_LANE_ALGN_MRK_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LANE_ALGN_MRK_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 2) field RX_LANE_ALGN_MRK_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LANE_ALGN_MRK_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LANE_ALGN_MRK_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                             exco3m_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_LANE_ALGN_MRK_ERR_I_poll", value );

    /* (0x00000434 bits 2) field RX_LANE_ALGN_MRK_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_1027B_PAR_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                 exco3m_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_1027B_PAR_ERR_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                 exco3m_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_I_set_to_clear", value );

    /* (0x00000434 bits 1) field RX_1027B_PAR_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    exco3m_reg_RX_INT_STAT_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_MSK,
                                                      EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_OFF,
                                                      value);
}

static INLINE UINT32 exco3m_field_RX_1027B_PAR_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_PAR_ERR_I_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000434 bits 1) field RX_1027B_PAR_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    reg_value = exco3m_reg_RX_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_MSK) >> EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_PAR_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_PAR_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                         exco3m_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_PAR_ERR_I_poll( exco3m_buffer_t *b_ptr,
                                                                         exco3m_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_PAR_ERR_I_poll", value );

    /* (0x00000434 bits 1) field RX_1027B_PAR_ERR_I of register PMC_EXCO3M_REG_RX_INT_STAT */
    return exco3m_reg_RX_INT_STAT_poll( b_ptr,
                                        h_ptr,
                                        EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_MSK,
                                        (value<<EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_size4
 * ==================================================================================
 */
static INLINE void exco3m_field_TX_PCS_LOBL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_LOBL_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_LOBL_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_LOBL_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_I_set_to_clear", N, value );

    /* ((0x00000180 + (N) * 4) bits 15) field TX_PCS_LOBL_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_MSK,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_TX_PCS_LOBL_I_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_LOBL_I_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_LOBL_I_get", N, 3);
    /* ((0x00000180 + (N) * 4) bits 15) field TX_PCS_LOBL_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_MSK) >> EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_I_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_I_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_I_poll", N, value );

    /* ((0x00000180 + (N) * 4) bits 15) field TX_PCS_LOBL_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_TX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_MSK,
                                                   (value<<EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_PCS_BER_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_BER_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BER_DET_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_BER_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_BER_DET_I_set_to_clear", N, value );

    /* ((0x00000180 + (N) * 4) bits 14) field TX_PCS_BER_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_MSK,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_TX_PCS_BER_DET_I_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BER_DET_I_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BER_DET_I_get", N, 3);
    /* ((0x00000180 + (N) * 4) bits 14) field TX_PCS_BER_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_MSK) >> EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_BER_DET_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BER_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                       exco3m_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BER_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                       exco3m_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_PCS_BER_DET_I_poll", N, value );

    /* ((0x00000180 + (N) * 4) bits 14) field TX_PCS_BER_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_TX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_MSK,
                                                   (value<<EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear", N, value );

    /* ((0x00000180 + (N) * 4) bits 13) field TX_PCS_BIP_ERR_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_MSK,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BIP_ERR_DET_I_get", N, 3);
    /* ((0x00000180 + (N) * 4) bits 13) field TX_PCS_BIP_ERR_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_MSK) >> EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BIP_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BIP_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_DET_I_poll", N, value );

    /* ((0x00000180 + (N) * 4) bits 13) field TX_PCS_BIP_ERR_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_TX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_MSK,
                                                   (value<<EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_LOAM_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_LOAM_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LOAM_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_LOAM_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_LOAM_I_set_to_clear", N, value );

    /* ((0x00000180 + (N) * 4) bits 12) field TX_LOAM_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_MSK,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_TX_LOAM_I_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOAM_I_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LOAM_I_get", N, 3);
    /* ((0x00000180 + (N) * 4) bits 12) field TX_LOAM_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_MSK) >> EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_LOAM_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_I_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_I_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_LOAM_I_poll", N, value );

    /* ((0x00000180 + (N) * 4) bits 12) field TX_LOAM_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_TX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_MSK,
                                                   (value<<EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_TX_MRK_INV_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_TX_MRK_INV_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_MRK_INV_DET_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_TX_MRK_INV_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_TX_MRK_INV_DET_I_set_to_clear", N, value );

    /* ((0x00000180 + (N) * 4) bits 11) field TX_MRK_INV_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_MSK,
                                                                 EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_TX_MRK_INV_DET_I_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_MRK_INV_DET_I_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_MRK_INV_DET_I_get", N, 3);
    /* ((0x00000180 + (N) * 4) bits 11) field TX_MRK_INV_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_MSK) >> EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_MRK_INV_DET_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_MRK_INV_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                       exco3m_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_MRK_INV_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                       exco3m_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_MRK_INV_DET_I_poll", N, value );

    /* ((0x00000180 + (N) * 4) bits 11) field TX_MRK_INV_DET_I of register PMC_EXCO3M_REG_TX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_TX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_MSK,
                                                   (value<<EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear", N, value );

    /* ((0x00000600 + (N) * 4) bits 15) field RX_OTN_BIP_ERR_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_MSK,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_OTN_BIP_ERR_DET_I_get", N, 3);
    /* ((0x00000600 + (N) * 4) bits 15) field RX_OTN_BIP_ERR_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_MSK) >> EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_OTN_BIP_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_OTN_BIP_ERR_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_DET_I_poll", N, value );

    /* ((0x00000600 + (N) * 4) bits 15) field RX_OTN_BIP_ERR_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_RX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_MSK,
                                                   (value<<EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_LOAM_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_LOAM_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_LOAM_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_LOAM_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_LOAM_I_set_to_clear", N, value );

    /* ((0x00000600 + (N) * 4) bits 14) field RX_LOAM_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_MSK,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_RX_LOAM_I_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LOAM_I_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_LOAM_I_get", N, 3);
    /* ((0x00000600 + (N) * 4) bits 14) field RX_LOAM_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_MSK) >> EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_LOAM_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_I_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_I_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_RX_LOAM_I_poll", N, value );

    /* ((0x00000600 + (N) * 4) bits 14) field RX_LOAM_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_RX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_MSK,
                                                   (value<<EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear( exco3m_buffer_t *b_ptr,
                                                                   exco3m_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear", N, value );

    /* ((0x00000600 + (N) * 4) bits 13) field RX_PCS_MRK_INV_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_MSK,
                                                                 EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_OFF,
                                                                 value);
}

static INLINE UINT32 exco3m_field_RX_PCS_MRK_INV_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_MRK_INV_DET_I_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_PCS_MRK_INV_DET_I_get", N, 3);
    /* ((0x00000600 + (N) * 4) bits 13) field RX_PCS_MRK_INV_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_STAT_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_MSK) >> EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_MRK_INV_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_MRK_INV_DET_I_poll( exco3m_buffer_t *b_ptr,
                                                                           exco3m_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_RX_PCS_MRK_INV_DET_I_poll", N, value );

    /* ((0x00000600 + (N) * 4) bits 13) field RX_PCS_MRK_INV_DET_I of register PMC_EXCO3M_REG_RX_LANE_INT_STAT index N=0..3 */
    return exco3m_reg_RX_LANE_INT_STAT_array_poll( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_MSK,
                                                   (value<<EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_OFF),
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
static INLINE UINT32 exco3m_field_TX_LOS_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOS_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 15) field TX_LOS_V of register PMC_EXCO3M_REG_TX_INT_VAL */
    reg_value = exco3m_reg_TX_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_MSK) >> EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_V_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_V_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_LOS_V_poll", value );

    /* (0x00000030 bits 15) field TX_LOS_V of register PMC_EXCO3M_REG_TX_INT_VAL */
    return exco3m_reg_TX_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_MSK,
                                       (value<<EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_TX_PCS_HBER_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_HBER_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 8) field TX_PCS_HBER_V of register PMC_EXCO3M_REG_TX_INT_VAL */
    reg_value = exco3m_reg_TX_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_MSK) >> EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_HBER_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_V_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_V_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_TX_PCS_HBER_V_poll", value );

    /* (0x00000030 bits 8) field TX_PCS_HBER_V of register PMC_EXCO3M_REG_TX_INT_VAL */
    return exco3m_reg_TX_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_MSK,
                                       (value<<EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_RX_TSF_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_TSF_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 15) field RX_TSF_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    reg_value = exco3m_reg_RX_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_MSK) >> EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_TSF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_V_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_V_poll( exco3m_buffer_t *b_ptr,
                                                               exco3m_handle_t *h_ptr,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_TSF_V_poll", value );

    /* (0x00000430 bits 15) field RX_TSF_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    return exco3m_reg_RX_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_MSK,
                                       (value<<EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_RX_1027B_LOBL_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_LOBL_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 10) field RX_1027B_LOBL_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    reg_value = exco3m_reg_RX_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_MSK) >> EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_LOBL_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_V_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_V_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_LOBL_V_poll", value );

    /* (0x00000430 bits 10) field RX_1027B_LOBL_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    return exco3m_reg_RX_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_MSK,
                                       (value<<EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_RX_1027B_HBER_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_HBER_V_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 9) field RX_1027B_HBER_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    reg_value = exco3m_reg_RX_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_MSK) >> EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_OFF;
    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_HBER_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_V_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_V_poll( exco3m_buffer_t *b_ptr,
                                                                      exco3m_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "exco3m_field_RX_1027B_HBER_V_poll", value );

    /* (0x00000430 bits 9) field RX_1027B_HBER_V of register PMC_EXCO3M_REG_RX_INT_VAL */
    return exco3m_reg_RX_INT_VAL_poll( b_ptr,
                                       h_ptr,
                                       EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_MSK,
                                       (value<<EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_N_size4
 * ==================================================================================
 */
static INLINE UINT32 exco3m_field_TX_PCS_LOBL_V_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_LOBL_V_get( exco3m_buffer_t *b_ptr,
                                                     exco3m_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_LOBL_V_get", N, 3);
    /* ((0x00000100 + (N) * 4) bits 15) field TX_PCS_LOBL_V of register PMC_EXCO3M_REG_TX_LANE_INT_VAL index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_VAL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_MSK) >> EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_V_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_V_poll( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_PCS_LOBL_V_poll", N, value );

    /* ((0x00000100 + (N) * 4) bits 15) field TX_PCS_LOBL_V of register PMC_EXCO3M_REG_TX_LANE_INT_VAL index N=0..3 */
    return exco3m_reg_TX_LANE_INT_VAL_array_poll( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_MSK,
                                                  (value<<EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_TX_LOAM_V_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LOAM_V_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LOAM_V_get", N, 3);
    /* ((0x00000100 + (N) * 4) bits 12) field TX_LOAM_V of register PMC_EXCO3M_REG_TX_LANE_INT_VAL index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INT_VAL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_MSK) >> EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_LOAM_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_V_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_V_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_TX_LOAM_V_poll", N, value );

    /* ((0x00000100 + (N) * 4) bits 12) field TX_LOAM_V of register PMC_EXCO3M_REG_TX_LANE_INT_VAL index N=0..3 */
    return exco3m_reg_TX_LANE_INT_VAL_array_poll( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_MSK,
                                                  (value<<EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 exco3m_field_RX_LOAM_V_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_LOAM_V_get( exco3m_buffer_t *b_ptr,
                                                 exco3m_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_LOAM_V_get", N, 3);
    /* ((0x00000580 + (N) * 4) bits 14) field RX_LOAM_V of register PMC_EXCO3M_REG_RX_LANE_INT_VAL index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_INT_VAL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_MSK) >> EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_LOAM_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_V_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_V_poll( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "exco3m_field_RX_LOAM_V_poll", N, value );

    /* ((0x00000580 + (N) * 4) bits 14) field RX_LOAM_V of register PMC_EXCO3M_REG_RX_LANE_INT_VAL index N=0..3 */
    return exco3m_reg_RX_LANE_INT_VAL_array_poll( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_MSK,
                                                  (value<<EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_OFF),
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
static INLINE UINT32 exco3m_field_TX_PCS_BER_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BER_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000040 bits 15:0) bits 0:15 use field TX_PCS_BER_CTR of register PMC_EXCO3M_REG_TX_PCS_BER_CNT_LOWER */
    reg_value = exco3m_reg_TX_PCS_BER_CNT_LOWER_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_MSK) >> EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_OFF;
    value |= field_value;

    /* (0x00000044 bits 5:0) bits 16:21 use field TX_PCS_BER_CTR of register PMC_EXCO3M_REG_TX_PCS_BER_CNT_UPPER */
    reg_value = exco3m_reg_TX_PCS_BER_CNT_UPPER_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_MSK) >> EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_BER_CTR_get", value );

    return value;
}
static INLINE UINT32 exco3m_field_range_TX_PCS_BER_CTR_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_TX_PCS_BER_CTR_get( exco3m_buffer_t *b_ptr,
                                                            exco3m_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_TX_PCS_BER_CTR_get", stop_bit, start_bit );
    if (stop_bit > 21) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_TX_PCS_BER_CTR_get", stop_bit, 21 );
    if (start_bit <= 15) {
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
    /* (0x00000040 bits 15:0) bits 0:15 use field TX_PCS_BER_CTR of register PMC_EXCO3M_REG_TX_PCS_BER_CNT_LOWER */
        reg_value = exco3m_reg_TX_PCS_BER_CNT_LOWER_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_MSK)
                      >> EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_MSK, EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 21) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 21) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 21;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000044 bits 5:0) bits 16:21 use field TX_PCS_BER_CTR of register PMC_EXCO3M_REG_TX_PCS_BER_CNT_UPPER */
        reg_value = exco3m_reg_TX_PCS_BER_CNT_UPPER_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_MSK)
                      >> EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_MSK, EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_TX_PCS_BER_CTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_TX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000048 bits 15:0) bits 0:15 use field TX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER */
    reg_value = exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_MSK) >> EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_OFF;
    value |= field_value;

    /* (0x0000004c bits 5:0) bits 16:21 use field TX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER */
    reg_value = exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_MSK) >> EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "exco3m_field_TX_PCS_BLK_ERR_CTR_get", value );

    return value;
}
static INLINE UINT32 exco3m_field_range_TX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_TX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_TX_PCS_BLK_ERR_CTR_get", stop_bit, start_bit );
    if (stop_bit > 21) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_TX_PCS_BLK_ERR_CTR_get", stop_bit, 21 );
    if (start_bit <= 15) {
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
    /* (0x00000048 bits 15:0) bits 0:15 use field TX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER */
        reg_value = exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_MSK)
                      >> EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_MSK, EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 21) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 21) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 21;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000004c bits 5:0) bits 16:21 use field TX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER */
        reg_value = exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_MSK)
                      >> EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_MSK, EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_TX_PCS_BLK_ERR_CTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_RX_1027B_BER_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_1027B_BER_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000440 bits 15:0) bits 0:15 use field RX_1027B_BER_CTR of register PMC_EXCO3M_REG_RX_1027B_BER_CNT_LOWER */
    reg_value = exco3m_reg_RX_1027B_BER_CNT_LOWER_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_MSK) >> EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_OFF;
    value |= field_value;

    /* (0x00000444 bits 5:0) bits 16:21 use field RX_1027B_BER_CTR of register PMC_EXCO3M_REG_RX_1027B_BER_CNT_UPPER */
    reg_value = exco3m_reg_RX_1027B_BER_CNT_UPPER_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_MSK) >> EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_1027B_BER_CTR_get", value );

    return value;
}
static INLINE UINT32 exco3m_field_range_RX_1027B_BER_CTR_get( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_RX_1027B_BER_CTR_get( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_RX_1027B_BER_CTR_get", stop_bit, start_bit );
    if (stop_bit > 21) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_RX_1027B_BER_CTR_get", stop_bit, 21 );
    if (start_bit <= 15) {
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
    /* (0x00000440 bits 15:0) bits 0:15 use field RX_1027B_BER_CTR of register PMC_EXCO3M_REG_RX_1027B_BER_CNT_LOWER */
        reg_value = exco3m_reg_RX_1027B_BER_CNT_LOWER_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_MSK)
                      >> EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_MSK, EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 21) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 21) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 21;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000444 bits 5:0) bits 16:21 use field RX_1027B_BER_CTR of register PMC_EXCO3M_REG_RX_1027B_BER_CNT_UPPER */
        reg_value = exco3m_reg_RX_1027B_BER_CNT_UPPER_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_MSK)
                      >> EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_MSK, EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_RX_1027B_BER_CTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_RX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000448 bits 15:0) bits 0:15 use field RX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER */
    reg_value = exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_MSK) >> EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_OFF;
    value |= field_value;

    /* (0x0000044c bits 5:0) bits 16:21 use field RX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER */
    reg_value = exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_MSK) >> EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "exco3m_field_RX_PCS_BLK_ERR_CTR_get", value );

    return value;
}
static INLINE UINT32 exco3m_field_range_RX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_RX_PCS_BLK_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_RX_PCS_BLK_ERR_CTR_get", stop_bit, start_bit );
    if (stop_bit > 21) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_RX_PCS_BLK_ERR_CTR_get", stop_bit, 21 );
    if (start_bit <= 15) {
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
    /* (0x00000448 bits 15:0) bits 0:15 use field RX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER */
        reg_value = exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_MSK)
                      >> EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_MSK, EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 21) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 21) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 21;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000044c bits 5:0) bits 16:21 use field RX_PCS_BLK_ERR_CTR of register PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER */
        reg_value = exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_MSK)
                      >> EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_MSK, EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_RX_PCS_BLK_ERR_CTR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size4
 * ==================================================================================
 */
static INLINE UINT32 exco3m_field_TX_LANE_ID_get( exco3m_buffer_t *b_ptr,
                                                  exco3m_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LANE_ID_get( exco3m_buffer_t *b_ptr,
                                                  exco3m_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LANE_ID_get", N, 3);
    /* ((0x00000080 + (N) * 4) bits 15:11) bits 0:4 use field TX_LANE_ID of register PMC_EXCO3M_REG_TX_LANE_INFO index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INFO_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_MSK) >> EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_LANE_ID_get", N, value );

    return value;
}
static INLINE UINT32 exco3m_field_range_TX_LANE_ID_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_TX_LANE_ID_get( exco3m_buffer_t *b_ptr,
                                                        exco3m_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_range_TX_LANE_ID_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_TX_LANE_ID_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_TX_LANE_ID_get", stop_bit, 4 );
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
    /* ((0x00000080 + (N) * 4) bits 15:11) bits 0:4 use field TX_LANE_ID of register PMC_EXCO3M_REG_TX_LANE_INFO index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INFO_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_MSK)
                  >> EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_MSK, EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_TX_LANE_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_TX_LANE_SKEW_FIFO_ADDR_get( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_LANE_SKEW_FIFO_ADDR_get( exco3m_buffer_t *b_ptr,
                                                              exco3m_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_LANE_SKEW_FIFO_ADDR_get", N, 3);
    /* ((0x00000080 + (N) * 4) bits 6:0) bits 0:6 use field TX_LANE_SKEW_FIFO_ADDR of register PMC_EXCO3M_REG_TX_LANE_INFO index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INFO_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_MSK) >> EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_LANE_SKEW_FIFO_ADDR_get", N, value );

    return value;
}
static INLINE UINT32 exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get( exco3m_buffer_t *b_ptr,
                                                                    exco3m_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get", stop_bit, 6 );
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
    /* ((0x00000080 + (N) * 4) bits 6:0) bits 0:6 use field TX_LANE_SKEW_FIFO_ADDR of register PMC_EXCO3M_REG_TX_LANE_INFO index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_INFO_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_MSK)
                  >> EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_MSK, EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_TX_PCS_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_TX_PCS_BIP_ERR_CTR_get", N, 3);
    /* ((0x00000300 + (N) * 4) bits 15:0) bits 0:15 use field TX_PCS_BIP_ERR_CTR of register PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_MSK) >> EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_TX_PCS_BIP_ERR_CTR_get", N, value );

    return value;
}
static INLINE UINT32 exco3m_field_range_TX_PCS_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_TX_PCS_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_range_TX_PCS_BIP_ERR_CTR_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_TX_PCS_BIP_ERR_CTR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_TX_PCS_BIP_ERR_CTR_get", stop_bit, 15 );
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
    /* ((0x00000300 + (N) * 4) bits 15:0) bits 0:15 use field TX_PCS_BIP_ERR_CTR of register PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT index N=0..3 */
    reg_value = exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_MSK)
                  >> EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_MSK, EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_TX_PCS_BIP_ERR_CTR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_RX_OTN_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                          exco3m_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_RX_OTN_BIP_ERR_CTR_get", N, 3);
    /* ((0x00000700 + (N) * 4) bits 15:0) bits 0:15 use field RX_OTN_BIP_ERR_CTR of register PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_MSK) >> EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "exco3m_field_RX_OTN_BIP_ERR_CTR_get", N, value );

    return value;
}
static INLINE UINT32 exco3m_field_range_RX_OTN_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 exco3m_field_range_RX_OTN_BIP_ERR_CTR_get( exco3m_buffer_t *b_ptr,
                                                                exco3m_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "exco3m_field_range_RX_OTN_BIP_ERR_CTR_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "exco3m_field_range_RX_OTN_BIP_ERR_CTR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "exco3m_field_range_RX_OTN_BIP_ERR_CTR_get", stop_bit, 15 );
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
    /* ((0x00000700 + (N) * 4) bits 15:0) bits 0:15 use field RX_OTN_BIP_ERR_CTR of register PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT index N=0..3 */
    reg_value = exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_MSK)
                  >> EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_MSK, EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "exco3m_field_range_RX_OTN_BIP_ERR_CTR_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXCO3M_IO_INLINE_H */
