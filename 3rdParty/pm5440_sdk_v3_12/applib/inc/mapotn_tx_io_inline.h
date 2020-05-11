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
 *     and register accessor functions for the mapotn_tx block
 *****************************************************************************/
#ifndef _MAPOTN_TX_IO_INLINE_H
#define _MAPOTN_TX_IO_INLINE_H

#include "mapotn_loc.h"
#include "mapotn_tx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAPOTN_TX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mapotn_tx
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
    mapotn_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mapotn_tx_buffer_t;
static INLINE void mapotn_tx_buffer_init( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_tx_buffer_init( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mapotn_tx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mapotn_tx_buffer_flush( mapotn_tx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_tx_buffer_flush( mapotn_tx_buffer_t *b_ptr )
{
    IOLOG( "mapotn_tx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mapotn_tx_reg_read( mapotn_tx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_read( mapotn_tx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
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
static INLINE void mapotn_tx_reg_write( mapotn_tx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_write( mapotn_tx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tx_field_set( mapotn_tx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_set( mapotn_tx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tx_action_on_write_field_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_action_on_write_field_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tx_burst_read( mapotn_tx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_burst_read( mapotn_tx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
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

static INLINE void mapotn_tx_burst_write( mapotn_tx_buffer_t *b_ptr,
                                          mapotn_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_burst_write( mapotn_tx_buffer_t *b_ptr,
                                          mapotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_poll( mapotn_tx_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_poll( mapotn_tx_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
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
 *  register access functions for mapotn_tx
 * ==================================================================================
 */

static INLINE void mapotn_tx_reg_LOWPWR_MODE_write( mapotn_tx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_LOWPWR_MODE_write( mapotn_tx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_LOWPWR_MODE_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE,
                         value);
}

static INLINE void mapotn_tx_reg_LOWPWR_MODE_field_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_LOWPWR_MODE_field_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_LOWPWR_MODE_field_set", mask, ofs, value );
    mapotn_tx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE,
                         mask,
                         PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_tx_reg_LOWPWR_MODE_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_LOWPWR_MODE_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_LOWPWR_MODE_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tx_reg_RAM_LOWPWR_MODE_write( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_RAM_LOWPWR_MODE_write( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_RAM_LOWPWR_MODE_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE,
                         value);
}

static INLINE void mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( mapotn_tx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( mapotn_tx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_RAM_LOWPWR_MODE_field_set", mask, ofs, value );
    mapotn_tx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE,
                         mask,
                         PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_tx_reg_RAM_LOWPWR_MODE_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_RAM_LOWPWR_MODE_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_RAM_LOWPWR_MODE_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tx_reg_ENET_FIFO_CLR_write( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_CLR_write( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_ENET_FIFO_CLR_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR,
                         value);
}

static INLINE void mapotn_tx_reg_ENET_FIFO_CLR_field_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_CLR_field_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_ENET_FIFO_CLR_field_set", mask, ofs, value );
    mapotn_tx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR,
                         mask,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_CLR_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_CLR_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_ENET_FIFO_CLR_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_write( mapotn_tx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_write( mapotn_tx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN,
                         value);
}

static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_field_set( mapotn_tx_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_field_set( mapotn_tx_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_field_set", mask, ofs, value );
    mapotn_tx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN,
                         mask,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INTR_write( mapotn_tx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INTR_write( mapotn_tx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_ENET_FIFO_OVFL_INTR_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR,
                         value);
}

static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INTR_action_on_write_field_set( mapotn_tx_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_ENET_FIFO_OVFL_INTR_action_on_write_field_set( mapotn_tx_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_ENET_FIFO_OVFL_INTR_action_on_write_field_set", mask, ofs, value );
    mapotn_tx_action_on_write_field_set( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR,
                                         mask,
                                         ofs,
                                         value);

}

static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_OVFL_INTR_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_ENET_FIFO_OVFL_INTR_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_ENET_FIFO_OVFL_INTR_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_reg_ENET_FIFO_OVFL_INTR_poll( mapotn_tx_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_reg_ENET_FIFO_OVFL_INTR_poll( mapotn_tx_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_tx_reg_ENET_FIFO_OVFL_INTR_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_tx_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void mapotn_tx_reg_SOFT_RST_write( mapotn_tx_buffer_t *b_ptr,
                                                 mapotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_SOFT_RST_write( mapotn_tx_buffer_t *b_ptr,
                                                 mapotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_tx_reg_SOFT_RST_write", value );
    mapotn_tx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_TX_CORE_REG_SOFT_RST,
                         value);
}

static INLINE void mapotn_tx_reg_SOFT_RST_field_set( mapotn_tx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_reg_SOFT_RST_field_set( mapotn_tx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_tx_reg_SOFT_RST_field_set", mask, ofs, value );
    mapotn_tx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_TX_CORE_REG_SOFT_RST,
                         mask,
                         PMC_MAPOTN_TX_CORE_REG_SOFT_RST_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_tx_reg_SOFT_RST_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_reg_SOFT_RST_read( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_tx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_TX_CORE_REG_SOFT_RST);

    IOLOG( "%s -> 0x%08x;", "mapotn_tx_reg_SOFT_RST_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mapotn_tx_field_ODUKSC_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_ODUKSC_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_ODUKSC_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_ODUKSC_LOWPWR_set", value );

    /* (0x00080004 bits 3) field ODUKSC_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    mapotn_tx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_MSK,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_tx_field_ODUKSC_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_ODUKSC_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080004 bits 3) field ODUKSC_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_ODUKSC_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_MPMO_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_MPMO_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_MPMO_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_MPMO_LOWPWR_set", value );

    /* (0x00080004 bits 2) field MPMO_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    mapotn_tx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_MSK,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_tx_field_MPMO_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_MPMO_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080004 bits 2) field MPMO_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_MPMO_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGMP96_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGMP96_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGMP96_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGMP96_LOWPWR_set", value );

    /* (0x00080004 bits 1) field TGMP96_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    mapotn_tx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_MSK,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_tx_field_TGMP96_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGMP96_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080004 bits 1) field TGMP96_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGMP96_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGFPF96_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGFPF96_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGFPF96_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGFPF96_LOWPWR_set", value );

    /* (0x00080004 bits 0) field TGFPF96_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    mapotn_tx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_MSK,
                                         MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_tx_field_TGFPF96_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGFPF96_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080004 bits 0) field TGFPF96_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGFPF96_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_ODUKSC_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_ODUKSC_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_ODUKSC_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_ODUKSC_RAM_LOWPWR_set", value );

    /* (0x0008000c bits 3) field ODUKSC_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_MSK,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_tx_field_ODUKSC_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_ODUKSC_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0008000c bits 3) field ODUKSC_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_ODUKSC_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_MPMO_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_MPMO_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_MPMO_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_MPMO_RAM_LOWPWR_set", value );

    /* (0x0008000c bits 2) field MPMO_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_MSK,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_tx_field_MPMO_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_MPMO_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0008000c bits 2) field MPMO_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_MPMO_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGMP96_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGMP96_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGMP96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGMP96_RAM_LOWPWR_set", value );

    /* (0x0008000c bits 1) field TGMP96_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_MSK,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_tx_field_TGMP96_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGMP96_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0008000c bits 1) field TGMP96_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGMP96_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGFPF96_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGFPF96_RAM_LOWPWR_set( mapotn_tx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGFPF96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGFPF96_RAM_LOWPWR_set", value );

    /* (0x0008000c bits 0) field TGFPF96_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    mapotn_tx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_MSK,
                                             MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_tx_field_TGFPF96_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGFPF96_RAM_LOWPWR_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0008000c bits 0) field TGFPF96_RAM_LOWPWR of register PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_tx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_MSK) >> MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGFPF96_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_tx_field_FIFO_CLRB_set( mapotn_tx_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_FIFO_CLRB_set( mapotn_tx_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_FIFO_CLRB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_FIFO_CLRB_set", value );

    /* (0x00080010 bits 0) field FIFO_CLRB of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR */
    mapotn_tx_reg_ENET_FIFO_CLR_field_set( b_ptr,
                                           h_ptr,
                                           MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_MSK,
                                           MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_OFF,
                                           value);
}

static INLINE UINT32 mapotn_tx_field_FIFO_CLRB_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_FIFO_CLRB_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080010 bits 0) field FIFO_CLRB of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR */
    reg_value = mapotn_tx_reg_ENET_FIFO_CLR_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_MSK) >> MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_FIFO_CLRB_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mapotn_tx_field_ENET_FIFO_OVFL_E_set( mapotn_tx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_ENET_FIFO_OVFL_E_set( mapotn_tx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_E_set", value );

    /* (0x00080020 bits 0) field ENET_FIFO_OVFL_E of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN */
    mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_MSK,
                                                   MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_OFF,
                                                   value);
}

static INLINE UINT32 mapotn_tx_field_ENET_FIFO_OVFL_E_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_ENET_FIFO_OVFL_E_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080020 bits 0) field ENET_FIFO_OVFL_E of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN */
    reg_value = mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_MSK) >> MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mapotn_tx_field_ENET_FIFO_OVFL_I_set_to_clear( mapotn_tx_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_ENET_FIFO_OVFL_I_set_to_clear( mapotn_tx_buffer_t *b_ptr,
                                                                  mapotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_I_set_to_clear", value );

    /* (0x00080024 bits 0) field ENET_FIFO_OVFL_I of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR */
    mapotn_tx_reg_ENET_FIFO_OVFL_INTR_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_MSK,
                                                                 MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_OFF,
                                                                 value);
}

static INLINE UINT32 mapotn_tx_field_ENET_FIFO_OVFL_I_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_ENET_FIFO_OVFL_I_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080024 bits 0) field ENET_FIFO_OVFL_I of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR */
    reg_value = mapotn_tx_reg_ENET_FIFO_OVFL_INTR_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_MSK) >> MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_field_ENET_FIFO_OVFL_I_poll( mapotn_tx_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_tx_field_ENET_FIFO_OVFL_I_poll( mapotn_tx_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mapotn_tx_field_ENET_FIFO_OVFL_I_poll", value );

    /* (0x00080024 bits 0) field ENET_FIFO_OVFL_I of register PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR */
    return mapotn_tx_reg_ENET_FIFO_OVFL_INTR_poll( b_ptr,
                                                   h_ptr,
                                                   MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_MSK,
                                                   (value<<MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void mapotn_tx_field_ODUKSC_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_ODUKSC_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_ODUKSC_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_ODUKSC_SOFT_RST_set", value );

    /* (0x00080000 bits 4) field ODUKSC_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    mapotn_tx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_MSK,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_tx_field_ODUKSC_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_ODUKSC_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080000 bits 4) field ODUKSC_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    reg_value = mapotn_tx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_MSK) >> MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_ODUKSC_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_tx_field_MPMO_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_MPMO_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_MPMO_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_MPMO_SOFT_RST_set", value );

    /* (0x00080000 bits 3) field MPMO_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    mapotn_tx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_MSK,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_tx_field_MPMO_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_MPMO_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080000 bits 3) field MPMO_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    reg_value = mapotn_tx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_MSK) >> MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_MPMO_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGMP96_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGMP96_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGMP96_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGMP96_SOFT_RST_set", value );

    /* (0x00080000 bits 2) field TGMP96_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    mapotn_tx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_MSK,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_tx_field_TGMP96_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGMP96_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080000 bits 2) field TGMP96_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    reg_value = mapotn_tx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_MSK) >> MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGMP96_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_tx_field_TGFPF96_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_TGFPF96_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_TGFPF96_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_TGFPF96_SOFT_RST_set", value );

    /* (0x00080000 bits 1) field TGFPF96_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    mapotn_tx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_MSK,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_tx_field_TGFPF96_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_TGFPF96_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080000 bits 1) field TGFPF96_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    reg_value = mapotn_tx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_MSK) >> MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_TGFPF96_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_tx_field_MAPOTN_TX_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_tx_field_MAPOTN_TX_SOFT_RST_set( mapotn_tx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_tx_field_MAPOTN_TX_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_tx_field_MAPOTN_TX_SOFT_RST_set", value );

    /* (0x00080000 bits 0) field MAPOTN_TX_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    mapotn_tx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_MSK,
                                      MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_tx_field_MAPOTN_TX_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_tx_field_MAPOTN_TX_SOFT_RST_get( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00080000 bits 0) field MAPOTN_TX_SOFT_RST of register PMC_MAPOTN_TX_CORE_REG_SOFT_RST */
    reg_value = mapotn_tx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_MSK) >> MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_tx_field_MAPOTN_TX_SOFT_RST_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_TX_IO_INLINE_H */
