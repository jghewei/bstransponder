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
 *     and register accessor functions for the ge_gfpt block
 *****************************************************************************/
#ifndef _GE_GFPT_IO_INLINE_H
#define _GE_GFPT_IO_INLINE_H

#include "enet_fege_loc.h"
#include "ge_gfpt_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GE_GFPT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for ge_gfpt
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
    enet_fege_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} ge_gfpt_buffer_t;
static INLINE void ge_gfpt_buffer_init( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void ge_gfpt_buffer_init( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "ge_gfpt_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void ge_gfpt_buffer_flush( ge_gfpt_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void ge_gfpt_buffer_flush( ge_gfpt_buffer_t *b_ptr )
{
    IOLOG( "ge_gfpt_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 ge_gfpt_reg_read( ge_gfpt_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_read( ge_gfpt_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
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
static INLINE void ge_gfpt_reg_write( ge_gfpt_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_write( ge_gfpt_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
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

static INLINE void ge_gfpt_field_set( ge_gfpt_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_set( ge_gfpt_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
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

static INLINE void ge_gfpt_action_on_write_field_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_action_on_write_field_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
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

static INLINE void ge_gfpt_burst_read( ge_gfpt_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_burst_read( ge_gfpt_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
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

static INLINE void ge_gfpt_burst_write( ge_gfpt_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_burst_write( ge_gfpt_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_poll( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_poll( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
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
 *  register access functions for ge_gfpt
 * ==================================================================================
 */

static INLINE void ge_gfpt_reg_GENERAL_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_GENERAL_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_GENERAL_CFG_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_GENERAL_CFG,
                       value);
}

static INLINE void ge_gfpt_reg_GENERAL_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_GENERAL_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_GENERAL_CFG_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_GENERAL_CFG,
                       mask,
                       PMC_GE_GFPT_REG_GENERAL_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_GENERAL_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_GENERAL_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_GENERAL_CFG);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_GENERAL_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_RX_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_RX_CFG_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_RX_CFG,
                       value);
}

static INLINE void ge_gfpt_reg_RX_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_RX_CFG_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_RX_CFG,
                       mask,
                       PMC_GE_GFPT_REG_RX_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_RX_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_RX_CFG);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_RX_20_BIT_PATTERN_write( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_20_BIT_PATTERN_write( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_RX_20_BIT_PATTERN_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_RX_20_BIT_PATTERN,
                       value);
}

static INLINE void ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_RX_20_BIT_PATTERN_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_RX_20_BIT_PATTERN,
                       mask,
                       PMC_GE_GFPT_REG_RX_20_BIT_PATTERN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_RX_20_BIT_PATTERN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_20_BIT_PATTERN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_RX_20_BIT_PATTERN);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_20_BIT_PATTERN_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_TX_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_CFG_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_TX_CFG_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_CFG,
                       value);
}

static INLINE void ge_gfpt_reg_TX_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_CFG_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_TX_CFG_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_CFG,
                       mask,
                       PMC_GE_GFPT_REG_TX_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_TX_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_CFG_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_TX_CFG);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_TX_20_BIT_PATTERN_write( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_20_BIT_PATTERN_write( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_TX_20_BIT_PATTERN_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_20_BIT_PATTERN,
                       value);
}

static INLINE void ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_TX_20_BIT_PATTERN_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_20_BIT_PATTERN,
                       mask,
                       PMC_GE_GFPT_REG_TX_20_BIT_PATTERN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_TX_20_BIT_PATTERN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_20_BIT_PATTERN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_TX_20_BIT_PATTERN);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_20_BIT_PATTERN_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_write( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_write( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL,
                       value);
}

static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set( ge_gfpt_buffer_t *b_ptr,
                                                                             enet_fege_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set( ge_gfpt_buffer_t *b_ptr,
                                                                             enet_fege_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL,
                       mask,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_write( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_write( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL,
                       value);
}

static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set( ge_gfpt_buffer_t *b_ptr,
                                                                             enet_fege_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set( ge_gfpt_buffer_t *b_ptr,
                                                                             enet_fege_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL,
                       mask,
                       PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_RX_PMON_UPDATE_write( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_PMON_UPDATE_write( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_RX_PMON_UPDATE_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_RX_PMON_UPDATE,
                       value);
}

static INLINE void ge_gfpt_reg_RX_PMON_UPDATE_field_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_RX_PMON_UPDATE_field_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_RX_PMON_UPDATE_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_RX_PMON_UPDATE,
                       mask,
                       PMC_GE_GFPT_REG_RX_PMON_UPDATE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_RX_PMON_UPDATE_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_PMON_UPDATE_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_PMON_UPDATE);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_PMON_UPDATE_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_TX_PMON_UPDATE_write( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_PMON_UPDATE_write( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_TX_PMON_UPDATE_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_TX_PMON_UPDATE,
                       value);
}

static INLINE void ge_gfpt_reg_TX_PMON_UPDATE_field_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_TX_PMON_UPDATE_field_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_TX_PMON_UPDATE_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_TX_PMON_UPDATE,
                       mask,
                       PMC_GE_GFPT_REG_TX_PMON_UPDATE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_TX_PMON_UPDATE_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_PMON_UPDATE_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_PMON_UPDATE);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_PMON_UPDATE_read", reg_value);
    return reg_value;
}

static INLINE void ge_gfpt_reg_INT_write( ge_gfpt_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_INT_write( ge_gfpt_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_INT_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_INT,
                       value);
}

static INLINE void ge_gfpt_reg_INT_action_on_write_field_set( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_INT_action_on_write_field_set( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_INT_action_on_write_field_set", mask, ofs, value );
    ge_gfpt_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_GE_GFPT_REG_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 ge_gfpt_reg_INT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_INT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_INT);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_reg_INT_poll( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_reg_INT_poll( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "ge_gfpt_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return ge_gfpt_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_GE_GFPT_REG_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void ge_gfpt_reg_INT_EN_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_INT_EN_write( ge_gfpt_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ge_gfpt_reg_INT_EN_write", value );
    ge_gfpt_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_INT_EN,
                       value);
}

static INLINE void ge_gfpt_reg_INT_EN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_reg_INT_EN_field_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ge_gfpt_reg_INT_EN_field_set", mask, ofs, value );
    ge_gfpt_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_GE_GFPT_REG_INT_EN,
                       mask,
                       PMC_GE_GFPT_REG_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 ge_gfpt_reg_INT_EN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_INT_EN_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_INT_EN);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_DEFECT_STAT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_DEFECT_STAT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_DEFECT_STAT);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_DEFECT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_reg_DEFECT_STAT_poll( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_reg_DEFECT_STAT_poll( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "ge_gfpt_reg_DEFECT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return ge_gfpt_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_GE_GFPT_REG_DEFECT_STAT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 ge_gfpt_reg_RX_LCV_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_LCV_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_LCV_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_LCV_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_LCV_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_LCV_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_LCV_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_LCV_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_PACKET_DELETED_CNT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_PACKET_DELETED_CNT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_PACKET_DELETED_CNT);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_PACKET_DELETED_CNT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_LCV_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_LCV_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_LCV_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_LCV_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_LCV_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_LCV_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_LCV_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_LCV_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ge_gfpt_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1);

    IOLOG( "%s -> 0x%08x;", "ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void ge_gfpt_field_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_SOFT_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_SOFT_RESET_set", value );

    /* (0x00002100 bits 0) field SOFT_RESET of register PMC_GE_GFPT_REG_GENERAL_CFG */
    ge_gfpt_reg_GENERAL_CFG_field_set( b_ptr,
                                       h_ptr,
                                       GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_MSK,
                                       GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_OFF,
                                       value);
}

static INLINE UINT32 ge_gfpt_field_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002100 bits 0) field SOFT_RESET of register PMC_GE_GFPT_REG_GENERAL_CFG */
    reg_value = ge_gfpt_reg_GENERAL_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_MSK) >> GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_SOFT_RESET_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_RESYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_RESYNC_COUNT_set", value );

    /* (0x00002104 bits 24:21) bits 0:3 use field RX_RESYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 24:21) bits 0:3 use field RX_RESYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_RESYNC_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_RX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_RX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_RESYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_RESYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_RESYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002104 bits 24:21) bits 0:3 use field RX_RESYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
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
        /* (0x00002104 bits 24:21) bits 0:3 use field RX_RESYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
        ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_RX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_RESYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_RESYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x00002104 bits 24:21) bits 0:3 use field RX_RESYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_MSK)
                  >> GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_MSK, GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_RESYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_RX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_LOS_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_LOS_COUNT_set", value );

    /* (0x00002104 bits 20:17) bits 0:3 use field RX_LOS_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 20:17) bits 0:3 use field RX_LOS_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_LOS_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_RX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_RX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_LOS_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_LOS_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_LOS_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002104 bits 20:17) bits 0:3 use field RX_LOS_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
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
        /* (0x00002104 bits 20:17) bits 0:3 use field RX_LOS_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
        ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_RX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_LOS_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_LOS_COUNT_get", stop_bit, 3 );
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
    /* (0x00002104 bits 20:17) bits 0:3 use field RX_LOS_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_MSK)
                  >> GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_MSK, GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_LOS_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_RX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_SYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_SYNC_COUNT_set", value );

    /* (0x00002104 bits 16:13) bits 0:3 use field RX_SYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 16:13) bits 0:3 use field RX_SYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_SYNC_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_RX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_RX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_SYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_SYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_SYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002104 bits 16:13) bits 0:3 use field RX_SYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
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
        /* (0x00002104 bits 16:13) bits 0:3 use field RX_SYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
        ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_RX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_SYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_SYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x00002104 bits 16:13) bits 0:3 use field RX_SYNC_COUNT of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_MSK)
                  >> GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_MSK, GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_SYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_RX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_10BIT_PATTERN_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_10BIT_PATTERN_set", value );

    /* (0x00002104 bits 12:3) bits 0:9 use field RX_10BIT_PATTERN of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 12:3) bits 0:9 use field RX_10BIT_PATTERN of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_10BIT_PATTERN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_RX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_RX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_10BIT_PATTERN_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_10BIT_PATTERN_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_10BIT_PATTERN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002104 bits 12:3) bits 0:9 use field RX_10BIT_PATTERN of register PMC_GE_GFPT_REG_RX_CFG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002104 bits 12:3) bits 0:9 use field RX_10BIT_PATTERN of register PMC_GE_GFPT_REG_RX_CFG */
        ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF + subfield_offset),
                                      GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_RX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_10BIT_PATTERN_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_10BIT_PATTERN_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00002104 bits 12:3) bits 0:9 use field RX_10BIT_PATTERN of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_MSK)
                  >> GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_MSK, GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_10BIT_PATTERN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_RX_10BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_10BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_EN_set", value );

    /* (0x00002104 bits 1) field RX_10BIT_PATT_EN of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 1) field RX_10BIT_PATT_EN of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_EN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_SOFT_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_SOFT_RESET_set", value );

    /* (0x00002104 bits 0) field RX_SOFT_RESET of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 0) field RX_SOFT_RESET of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_SOFT_RESET_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_20BIT_PATTERN_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_20BIT_PATTERN_set", value );

    /* (0x00002108 bits 21:2) bits 0:19 use field RX_20BIT_PATTERN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_MSK,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002108 bits 21:2) bits 0:19 use field RX_20BIT_PATTERN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_RX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_MSK) >> GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_20BIT_PATTERN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_RX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_RX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_20BIT_PATTERN_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_20BIT_PATTERN_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_20BIT_PATTERN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002108 bits 21:2) bits 0:19 use field RX_20BIT_PATTERN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
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
        /* (0x00002108 bits 21:2) bits 0:19 use field RX_20BIT_PATTERN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
        ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF + subfield_offset),
                                                 GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_RX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RX_20BIT_PATTERN_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RX_20BIT_PATTERN_get", stop_bit, 19 );
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
    /* (0x00002108 bits 21:2) bits 0:19 use field RX_20BIT_PATTERN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_RX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_MSK)
                  >> GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_MSK, GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RX_20BIT_PATTERN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_RX_20BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_20BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_EN_set", value );

    /* (0x00002108 bits 0) field RX_20BIT_PATT_EN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_MSK,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002108 bits 0) field RX_20BIT_PATT_EN of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_RX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_MSK) >> GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_EN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_RESYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_RESYNC_COUNT_set", value );

    /* (0x0000213c bits 26:23) bits 0:3 use field TX_RESYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 26:23) bits 0:3 use field TX_RESYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_RESYNC_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TX_RESYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_RESYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_RESYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_RESYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000213c bits 26:23) bits 0:3 use field TX_RESYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
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
        /* (0x0000213c bits 26:23) bits 0:3 use field TX_RESYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
        ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TX_RESYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_RESYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_RESYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x0000213c bits 26:23) bits 0:3 use field TX_RESYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_MSK)
                  >> GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_MSK, GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_RESYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_TX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_LOS_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_LOS_COUNT_set", value );

    /* (0x0000213c bits 22:19) bits 0:3 use field TX_LOS_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 22:19) bits 0:3 use field TX_LOS_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_LOS_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TX_LOS_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_LOS_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_LOS_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_LOS_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000213c bits 22:19) bits 0:3 use field TX_LOS_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
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
        /* (0x0000213c bits 22:19) bits 0:3 use field TX_LOS_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
        ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TX_LOS_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_LOS_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_LOS_COUNT_get", stop_bit, 3 );
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
    /* (0x0000213c bits 22:19) bits 0:3 use field TX_LOS_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_MSK)
                  >> GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_MSK, GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_LOS_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_TX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_SYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_SYNC_COUNT_set", value );

    /* (0x0000213c bits 18:15) bits 0:3 use field TX_SYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 18:15) bits 0:3 use field TX_SYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_SYNC_COUNT_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TX_SYNC_COUNT_set( ge_gfpt_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_SYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_SYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_SYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000213c bits 18:15) bits 0:3 use field TX_SYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
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
        /* (0x0000213c bits 18:15) bits 0:3 use field TX_SYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
        ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF + subfield_offset),
                                      GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TX_SYNC_COUNT_get( ge_gfpt_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_SYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_SYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x0000213c bits 18:15) bits 0:3 use field TX_SYNC_COUNT of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_MSK)
                  >> GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_MSK, GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_SYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_DISP_INVERSION_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_DISP_INVERSION_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_DISP_INVERSION_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_DISP_INVERSION_EN_set", value );

    /* (0x0000213c bits 14) field DISP_INVERSION_EN of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_DISP_INVERSION_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_DISP_INVERSION_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 14) field DISP_INVERSION_EN of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_MSK) >> GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_DISP_INVERSION_EN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_CLK_ADAPT_FIFO_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_CLK_ADAPT_FIFO_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_CLK_ADAPT_FIFO_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_CLK_ADAPT_FIFO_RESET_set", value );

    /* (0x0000213c bits 13) field CLK_ADAPT_FIFO_RESET of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_CLK_ADAPT_FIFO_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_CLK_ADAPT_FIFO_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 13) field CLK_ADAPT_FIFO_RESET of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_MSK) >> GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_CLK_ADAPT_FIFO_RESET_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_10BIT_PATTERN_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_10BIT_PATTERN_set", value );

    /* (0x0000213c bits 12:3) bits 0:9 use field TX_10BIT_PATTERN of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 12:3) bits 0:9 use field TX_10BIT_PATTERN of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_10BIT_PATTERN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TX_10BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_10BIT_PATTERN_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_10BIT_PATTERN_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_10BIT_PATTERN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000213c bits 12:3) bits 0:9 use field TX_10BIT_PATTERN of register PMC_GE_GFPT_REG_TX_CFG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000213c bits 12:3) bits 0:9 use field TX_10BIT_PATTERN of register PMC_GE_GFPT_REG_TX_CFG */
        ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF + subfield_offset),
                                      GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TX_10BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_10BIT_PATTERN_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_10BIT_PATTERN_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000213c bits 12:3) bits 0:9 use field TX_10BIT_PATTERN of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_MSK)
                  >> GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_MSK, GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_10BIT_PATTERN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_TX_10BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_10BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_EN_set", value );

    /* (0x0000213c bits 1) field TX_10BIT_PATT_EN of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 1) field TX_10BIT_PATT_EN of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_EN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_SOFT_RESET_set( ge_gfpt_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_SOFT_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_SOFT_RESET_set", value );

    /* (0x0000213c bits 0) field TX_SOFT_RESET of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_SOFT_RESET_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 0) field TX_SOFT_RESET of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_SOFT_RESET_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_20BIT_PATTERN_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_20BIT_PATTERN_set", value );

    /* (0x00002140 bits 21:2) bits 0:19 use field TX_20BIT_PATTERN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_MSK,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002140 bits 21:2) bits 0:19 use field TX_20BIT_PATTERN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_TX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_MSK) >> GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_20BIT_PATTERN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TX_20BIT_PATTERN_set( ge_gfpt_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_20BIT_PATTERN_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_20BIT_PATTERN_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_20BIT_PATTERN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002140 bits 21:2) bits 0:19 use field TX_20BIT_PATTERN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
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
        /* (0x00002140 bits 21:2) bits 0:19 use field TX_20BIT_PATTERN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
        ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF + subfield_offset),
                                                 GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TX_20BIT_PATTERN_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TX_20BIT_PATTERN_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TX_20BIT_PATTERN_get", stop_bit, 19 );
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
    /* (0x00002140 bits 21:2) bits 0:19 use field TX_20BIT_PATTERN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_TX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_MSK)
                  >> GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_MSK, GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TX_20BIT_PATTERN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_TX_20BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_20BIT_PATT_EN_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_EN_set", value );

    /* (0x00002140 bits 0) field TX_20BIT_PATT_EN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_MSK,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATT_EN_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002140 bits 0) field TX_20BIT_PATT_EN of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_TX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_MSK) >> GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_EN_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_set", value );

    /* (0x00002144 bits 15:0) bits 0:15 use field TXGE_GFPT_DELETE_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL */
    ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set( b_ptr,
                                                              h_ptr,
                                                              GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_MSK,
                                                              GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF,
                                                              value);
}

static INLINE UINT32 ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002144 bits 15:0) bits 0:15 use field TXGE_GFPT_DELETE_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL */
    reg_value = ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_MSK) >> GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002144 bits 15:0) bits 0:15 use field TXGE_GFPT_DELETE_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL */
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
        /* (0x00002144 bits 15:0) bits 0:15 use field TXGE_GFPT_DELETE_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL */
        ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF + subfield_offset),
                                                                  GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get", stop_bit, 15 );
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
    /* (0x00002144 bits 15:0) bits 0:15 use field TXGE_GFPT_DELETE_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL */
    reg_value = ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_MSK)
                  >> GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_MSK, GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_set", value );

    /* (0x00002148 bits 15:0) bits 0:15 use field TXGE_GFPT_INSERT_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL */
    ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set( b_ptr,
                                                              h_ptr,
                                                              GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_MSK,
                                                              GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF,
                                                              value);
}

static INLINE UINT32 ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002148 bits 15:0) bits 0:15 use field TXGE_GFPT_INSERT_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL */
    reg_value = ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_MSK) >> GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_get", value );

    return value;
}
static INLINE void ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002148 bits 15:0) bits 0:15 use field TXGE_GFPT_INSERT_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL */
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
        /* (0x00002148 bits 15:0) bits 0:15 use field TXGE_GFPT_INSERT_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL */
        ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF + subfield_offset),
                                                                  GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get", stop_bit, 15 );
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
    /* (0x00002148 bits 15:0) bits 0:15 use field TXGE_GFPT_INSERT_INTERVAL of register PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL */
    reg_value = ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read( b_ptr,
                                                                     h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_MSK)
                  >> GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_MSK, GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void ge_gfpt_field_RX_10BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_10BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_UPDATE_set", value );

    /* (0x00002104 bits 2) field RX_10BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_RX_CFG */
    ge_gfpt_reg_RX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_MSK,
                                  GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_10BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002104 bits 2) field RX_10BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_RX_CFG */
    reg_value = ge_gfpt_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_MSK) >> GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_10BIT_PATT_UPDATE_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_20BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_20BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_UPDATE_set", value );

    /* (0x00002108 bits 1) field RX_20BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    ge_gfpt_reg_RX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_MSK,
                                             GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_20BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002108 bits 1) field RX_20BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_RX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_RX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_MSK) >> GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_20BIT_PATT_UPDATE_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_PMON_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_PMON_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_PMON_UPDATE_set", value );

    /* (0x00002138 bits 0) field RX_PMON_UPDATE of register PMC_GE_GFPT_REG_RX_PMON_UPDATE */
    ge_gfpt_reg_RX_PMON_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_MSK,
                                          GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_OFF,
                                          value);
}

static INLINE UINT32 ge_gfpt_field_RX_PMON_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_PMON_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002138 bits 0) field RX_PMON_UPDATE of register PMC_GE_GFPT_REG_RX_PMON_UPDATE */
    reg_value = ge_gfpt_reg_RX_PMON_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_MSK) >> GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_PMON_UPDATE_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_10BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_10BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_UPDATE_set", value );

    /* (0x0000213c bits 2) field TX_10BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_TX_CFG */
    ge_gfpt_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_MSK,
                                  GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_10BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000213c bits 2) field TX_10BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_TX_CFG */
    reg_value = ge_gfpt_reg_TX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_MSK) >> GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_10BIT_PATT_UPDATE_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_20BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_20BIT_PATT_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_UPDATE_set", value );

    /* (0x00002140 bits 1) field TX_20BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    ge_gfpt_reg_TX_20_BIT_PATTERN_field_set( b_ptr,
                                             h_ptr,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_MSK,
                                             GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_OFF,
                                             value);
}

static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_20BIT_PATT_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002140 bits 1) field TX_20BIT_PATT_UPDATE of register PMC_GE_GFPT_REG_TX_20_BIT_PATTERN */
    reg_value = ge_gfpt_reg_TX_20_BIT_PATTERN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_MSK) >> GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_20BIT_PATT_UPDATE_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_PMON_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_PMON_UPDATE_set( ge_gfpt_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_PMON_UPDATE_set", value );

    /* (0x00002184 bits 0) field TX_PMON_UPDATE of register PMC_GE_GFPT_REG_TX_PMON_UPDATE */
    ge_gfpt_reg_TX_PMON_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_MSK,
                                          GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_OFF,
                                          value);
}

static INLINE UINT32 ge_gfpt_field_TX_PMON_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_PMON_UPDATE_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002184 bits 0) field TX_PMON_UPDATE of register PMC_GE_GFPT_REG_TX_PMON_UPDATE */
    reg_value = ge_gfpt_reg_TX_PMON_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_MSK) >> GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_PMON_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void ge_gfpt_field_TX_DISP_INVERT_IC_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_DISP_INVERT_IC_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_IC_set_to_clear", value );

    /* (0x00002190 bits 2) field TX_DISP_INVERT_IC of register PMC_GE_GFPT_REG_INT */
    ge_gfpt_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_MSK,
                                               GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_OFF,
                                               value);
}

static INLINE UINT32 ge_gfpt_field_TX_DISP_INVERT_IC_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_DISP_INVERT_IC_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002190 bits 2) field TX_DISP_INVERT_IC of register PMC_GE_GFPT_REG_INT */
    reg_value = ge_gfpt_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_MSK) >> GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_DISP_INVERT_IC_poll( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_DISP_INVERT_IC_poll( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "ge_gfpt_field_TX_DISP_INVERT_IC_poll", value );

    /* (0x00002190 bits 2) field TX_DISP_INVERT_IC of register PMC_GE_GFPT_REG_INT */
    return ge_gfpt_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_MSK,
                                 (value<<GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void ge_gfpt_field_TX_LOS_I_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_LOS_I_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_LOS_I_set_to_clear", value );

    /* (0x00002190 bits 1) field TX_LOS_I of register PMC_GE_GFPT_REG_INT */
    ge_gfpt_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               GE_GFPT_REG_INT_BIT_TX_LOS_I_MSK,
                                               GE_GFPT_REG_INT_BIT_TX_LOS_I_OFF,
                                               value);
}

static INLINE UINT32 ge_gfpt_field_TX_LOS_I_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_LOS_I_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002190 bits 1) field TX_LOS_I of register PMC_GE_GFPT_REG_INT */
    reg_value = ge_gfpt_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_BIT_TX_LOS_I_MSK) >> GE_GFPT_REG_INT_BIT_TX_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_LOS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_I_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_I_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "ge_gfpt_field_TX_LOS_I_poll", value );

    /* (0x00002190 bits 1) field TX_LOS_I of register PMC_GE_GFPT_REG_INT */
    return ge_gfpt_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 GE_GFPT_REG_INT_BIT_TX_LOS_I_MSK,
                                 (value<<GE_GFPT_REG_INT_BIT_TX_LOS_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void ge_gfpt_field_RX_LOS_I_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_LOS_I_set_to_clear( ge_gfpt_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_LOS_I_set_to_clear", value );

    /* (0x00002190 bits 0) field RX_LOS_I of register PMC_GE_GFPT_REG_INT */
    ge_gfpt_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               GE_GFPT_REG_INT_BIT_RX_LOS_I_MSK,
                                               GE_GFPT_REG_INT_BIT_RX_LOS_I_OFF,
                                               value);
}

static INLINE UINT32 ge_gfpt_field_RX_LOS_I_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_LOS_I_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002190 bits 0) field RX_LOS_I of register PMC_GE_GFPT_REG_INT */
    reg_value = ge_gfpt_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_BIT_RX_LOS_I_MSK) >> GE_GFPT_REG_INT_BIT_RX_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_LOS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_I_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_I_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "ge_gfpt_field_RX_LOS_I_poll", value );

    /* (0x00002190 bits 0) field RX_LOS_I of register PMC_GE_GFPT_REG_INT */
    return ge_gfpt_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 GE_GFPT_REG_INT_BIT_RX_LOS_I_MSK,
                                 (value<<GE_GFPT_REG_INT_BIT_RX_LOS_I_OFF),
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
static INLINE void ge_gfpt_field_TX_DISP_INVERT_E_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_DISP_INVERT_E_set( ge_gfpt_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_E_set", value );

    /* (0x0000218c bits 2) field TX_DISP_INVERT_E of register PMC_GE_GFPT_REG_INT_EN */
    ge_gfpt_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_MSK,
                                  GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_DISP_INVERT_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_DISP_INVERT_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000218c bits 2) field TX_DISP_INVERT_E of register PMC_GE_GFPT_REG_INT_EN */
    reg_value = ge_gfpt_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_MSK) >> GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_DISP_INVERT_E_get", value );

    return value;
}
static INLINE void ge_gfpt_field_TX_LOS_E_set( ge_gfpt_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_TX_LOS_E_set( ge_gfpt_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_TX_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_TX_LOS_E_set", value );

    /* (0x0000218c bits 1) field TX_LOS_E of register PMC_GE_GFPT_REG_INT_EN */
    ge_gfpt_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_MSK,
                                  GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_TX_LOS_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_LOS_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000218c bits 1) field TX_LOS_E of register PMC_GE_GFPT_REG_INT_EN */
    reg_value = ge_gfpt_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_MSK) >> GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_LOS_E_get", value );

    return value;
}
static INLINE void ge_gfpt_field_RX_LOS_E_set( ge_gfpt_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ge_gfpt_field_RX_LOS_E_set( ge_gfpt_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ge_gfpt_field_RX_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ge_gfpt_field_RX_LOS_E_set", value );

    /* (0x0000218c bits 0) field RX_LOS_E of register PMC_GE_GFPT_REG_INT_EN */
    ge_gfpt_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_MSK,
                                  GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_OFF,
                                  value);
}

static INLINE UINT32 ge_gfpt_field_RX_LOS_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_LOS_E_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000218c bits 0) field RX_LOS_E of register PMC_GE_GFPT_REG_INT_EN */
    reg_value = ge_gfpt_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_MSK) >> GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_LOS_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 ge_gfpt_field_TX_LOS_V_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_TX_LOS_V_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002188 bits 1) field TX_LOS_V of register PMC_GE_GFPT_REG_DEFECT_STAT */
    reg_value = ge_gfpt_reg_DEFECT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_MSK) >> GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_TX_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_V_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_V_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "ge_gfpt_field_TX_LOS_V_poll", value );

    /* (0x00002188 bits 1) field TX_LOS_V of register PMC_GE_GFPT_REG_DEFECT_STAT */
    return ge_gfpt_reg_DEFECT_STAT_poll( b_ptr,
                                         h_ptr,
                                         GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_MSK,
                                         (value<<GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 ge_gfpt_field_RX_LOS_V_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RX_LOS_V_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002188 bits 0) field RX_LOS_V of register PMC_GE_GFPT_REG_DEFECT_STAT */
    reg_value = ge_gfpt_reg_DEFECT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_MSK) >> GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RX_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_V_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_V_poll( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "ge_gfpt_field_RX_LOS_V_poll", value );

    /* (0x00002188 bits 0) field RX_LOS_V of register PMC_GE_GFPT_REG_DEFECT_STAT */
    return ge_gfpt_reg_DEFECT_STAT_poll( b_ptr,
                                         h_ptr,
                                         GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_MSK,
                                         (value<<GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset perf_cntr
 * ==================================================================================
 */
static INLINE void ge_gfpt_lfield_RXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_RXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002110 bits 31:0) bits 0:31 use field RXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_RX_LCV_CNT0 */
    reg_value = ge_gfpt_reg_RX_LCV_CNT0_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_MSK) >> GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002114 bits 7:0) bits 32:39 use field RXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_RX_LCV_CNT1 */
    reg_value = ge_gfpt_reg_RX_LCV_CNT1_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_MSK) >> GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_RXGE_GFPT_LCV_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get", stop_bit, 39 );
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
    /* (0x00002110 bits 31:0) bits 0:31 use field RXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_RX_LCV_CNT0 */
        reg_value = ge_gfpt_reg_RX_LCV_CNT0_read(  b_ptr, h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_MSK)
                      >> GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_MSK, GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_OFF, field_value );
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
    /* (0x00002114 bits 7:0) bits 32:39 use field RXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_RX_LCV_CNT1 */
        reg_value = ge_gfpt_reg_RX_LCV_CNT1_read(  b_ptr, h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_MSK)
                      >> GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_MSK, GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_RXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_RXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002118 bits 31:0) bits 0:31 use field RXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK) >> GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x0000211c bits 7:0) bits 32:39 use field RXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK) >> GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_RXGE_GFPT_IDLES_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x00002118 bits 31:0) bits 0:31 use field RXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK)
                      >> GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK, GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF, field_value );
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
    /* (0x0000211c bits 7:0) bits 32:39 use field RXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK)
                      >> GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK, GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_RXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_RXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002120 bits 31:0) bits 0:31 use field RXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK) >> GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002124 bits 7:0) bits 32:39 use field RXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK) >> GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_RXGE_GFPT_CONF_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x00002120 bits 31:0) bits 0:31 use field RXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read( b_ptr,
                                                                     h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK)
                      >> GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK, GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF, field_value );
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
    /* (0x00002124 bits 7:0) bits 32:39 use field RXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read( b_ptr,
                                                                  h_ptr);
        field_value = (reg_value & GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK)
                      >> GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK, GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_RXGE_GFPT_PROPGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_RXGE_GFPT_PROPGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002128 bits 31:0) bits 0:31 use field RXGE_GFPT_PROPGERR_DEL_CNT of register PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( b_ptr,
                                                                             h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK) >> GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x0000212c bits 7:0) bits 32:39 use field RXGE_GFPT_PROPGERR_DEL_CNT of register PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( b_ptr,
                                                                             h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK) >> GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_RXGE_GFPT_PROPGERR_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x00002128 bits 31:0) bits 0:31 use field RXGE_GFPT_PROPGERR_DEL_CNT of register PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( b_ptr,
                                                                                 h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK)
                      >> GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK, GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF, field_value );
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
    /* (0x0000212c bits 7:0) bits 32:39 use field RXGE_GFPT_PROPGERR_DEL_CNT of register PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( b_ptr,
                                                                                 h_ptr);
        field_value = (reg_value & GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK)
                      >> GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK, GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 ge_gfpt_field_RXGE_GFPT_PKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RXGE_GFPT_PKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002130 bits 23:0) bits 0:23 use field RXGE_GFPT_PKT_DEL_CNT of register PMC_GE_GFPT_REG_RX_PACKET_DELETED_CNT */
    reg_value = ge_gfpt_reg_RX_PACKET_DELETED_CNT_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_MSK) >> GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RXGE_GFPT_PKT_DEL_CNT_get", value );

    return value;
}
static INLINE UINT32 ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00002130 bits 23:0) bits 0:23 use field RXGE_GFPT_PKT_DEL_CNT of register PMC_GE_GFPT_REG_RX_PACKET_DELETED_CNT */
    reg_value = ge_gfpt_reg_RX_PACKET_DELETED_CNT_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_MSK)
                  >> GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_MSK, GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 ge_gfpt_field_RXGE_GFPT_PREPKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_RXGE_GFPT_PREPKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002134 bits 23:0) bits 0:23 use field RXGE_GFPT_PREPKT_DEL_CNT of register PMC_GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT */
    reg_value = ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_MSK) >> GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "ge_gfpt_field_RXGE_GFPT_PREPKT_DEL_CNT_get", value );

    return value;
}
static INLINE UINT32 ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00002134 bits 23:0) bits 0:23 use field RXGE_GFPT_PREPKT_DEL_CNT of register PMC_GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT */
    reg_value = ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_MSK)
                  >> GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_MSK, GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE void ge_gfpt_lfield_TXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000214c bits 31:0) bits 0:31 use field TXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_TX_LCV_CNT0 */
    reg_value = ge_gfpt_reg_TX_LCV_CNT0_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_MSK) >> GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002150 bits 7:0) bits 32:39 use field TXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_TX_LCV_CNT1 */
    reg_value = ge_gfpt_reg_TX_LCV_CNT1_read(  b_ptr, h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_MSK) >> GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_LCV_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get", stop_bit, 39 );
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
    /* (0x0000214c bits 31:0) bits 0:31 use field TXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_TX_LCV_CNT0 */
        reg_value = ge_gfpt_reg_TX_LCV_CNT0_read(  b_ptr, h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_MSK)
                      >> GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_MSK, GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_OFF, field_value );
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
    /* (0x00002150 bits 7:0) bits 32:39 use field TXGE_GFPT_LCV_CNT of register PMC_GE_GFPT_REG_TX_LCV_CNT1 */
        reg_value = ge_gfpt_reg_TX_LCV_CNT1_read(  b_ptr, h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_MSK)
                      >> GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_MSK, GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002154 bits 31:0) bits 0:31 use field TXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK) >> GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002158 bits 7:0) bits 32:39 use field TXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK) >> GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_IDLES_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x00002154 bits 31:0) bits 0:31 use field TXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK, GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF, field_value );
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
    /* (0x00002158 bits 7:0) bits 32:39 use field TXGE_GFPT_IDLES_DEL_CNT of register PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK, GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000215c bits 31:0) bits 0:31 use field TXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK) >> GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002160 bits 7:0) bits 32:39 use field TXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK) >> GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_CONF_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x0000215c bits 31:0) bits 0:31 use field TXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read( b_ptr,
                                                                     h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK, GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF, field_value );
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
    /* (0x00002160 bits 7:0) bits 32:39 use field TXGE_GFPT_CONF_DEL_CNT of register PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read( b_ptr,
                                                                     h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK, GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_PROPAGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_PROPAGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002164 bits 31:0) bits 0:31 use field TXGE_GFPT_PROPAGERR_DEL_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 */
    reg_value = ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( b_ptr,
                                                                             h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK) >> GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002168 bits 7:0) bits 32:39 use field TXGE_GFPT_PROPAGERR_DEL_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 */
    reg_value = ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( b_ptr,
                                                                             h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK) >> GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_PROPAGERR_DEL_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                           enet_fege_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                           enet_fege_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get", stop_bit, 39 );
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
    /* (0x00002164 bits 31:0) bits 0:31 use field TXGE_GFPT_PROPAGERR_DEL_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 */
        reg_value = ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read( b_ptr,
                                                                                 h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK, GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF, field_value );
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
    /* (0x00002168 bits 7:0) bits 32:39 use field TXGE_GFPT_PROPAGERR_DEL_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 */
        reg_value = ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read( b_ptr,
                                                                                 h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK)
                      >> GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK, GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_IDLES_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_IDLES_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000216c bits 31:0) bits 0:31 use field TXGE_GFPT_IDLES_INSRT_CNT of register PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT0 */
    reg_value = ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002170 bits 7:0) bits 32:39 use field TXGE_GFPT_IDLES_INSRT_CNT of register PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT1 */
    reg_value = ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_IDLES_INSRT_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get", stop_bit, 39 );
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
    /* (0x0000216c bits 31:0) bits 0:31 use field TXGE_GFPT_IDLES_INSRT_CNT of register PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT0 */
        reg_value = ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK, GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF, field_value );
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
    /* (0x00002170 bits 7:0) bits 32:39 use field TXGE_GFPT_IDLES_INSRT_CNT of register PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT1 */
        reg_value = ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK, GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_CONF_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_CONF_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00002174 bits 31:0) bits 0:31 use field TXGE_GFPT_CONF_INSRT_CNT of register PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0 */
    reg_value = ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read( b_ptr,
                                                                           h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002178 bits 7:0) bits 32:39 use field TXGE_GFPT_CONF_INSRT_CNT of register PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1 */
    reg_value = ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read( b_ptr,
                                                                           h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_CONF_INSRT_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get", stop_bit, 39 );
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
    /* (0x00002174 bits 31:0) bits 0:31 use field TXGE_GFPT_CONF_INSRT_CNT of register PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0 */
        reg_value = ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read( b_ptr,
                                                                               h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK, GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF, field_value );
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
    /* (0x00002178 bits 7:0) bits 32:39 use field TXGE_GFPT_CONF_INSRT_CNT of register PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1 */
        reg_value = ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read( b_ptr,
                                                                               h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK, GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void ge_gfpt_lfield_TXGE_GFPT_PROPG_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void ge_gfpt_lfield_TXGE_GFPT_PROPG_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000217c bits 31:0) bits 0:31 use field TXGE_GFPT_PROPG_INSRT_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0 */
    reg_value = ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read( b_ptr,
                                                                    h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF;
    value[0] |= field_value;

    /* (0x00002180 bits 7:0) bits 32:39 use field TXGE_GFPT_PROPG_INSRT_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1 */
    reg_value = ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read( b_ptr,
                                                                    h_ptr);
    field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK) >> GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "ge_gfpt_lfield_TXGE_GFPT_PROPG_INSRT_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get( ge_gfpt_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get", stop_bit, 39 );
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
    /* (0x0000217c bits 31:0) bits 0:31 use field TXGE_GFPT_PROPG_INSRT_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0 */
        reg_value = ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read( b_ptr,
                                                                        h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK, GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF, field_value );
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
    /* (0x00002180 bits 7:0) bits 32:39 use field TXGE_GFPT_PROPG_INSRT_CNT of register PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1 */
        reg_value = ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read( b_ptr,
                                                                        h_ptr);
        field_value = (reg_value & GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK)
                      >> GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK, GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GE_GFPT_IO_INLINE_H */
