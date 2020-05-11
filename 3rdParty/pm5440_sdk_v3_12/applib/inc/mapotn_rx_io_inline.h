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
 *     and register accessor functions for the mapotn_rx block
 *****************************************************************************/
#ifndef _MAPOTN_RX_IO_INLINE_H
#define _MAPOTN_RX_IO_INLINE_H

#include "mapotn_loc.h"
#include "mapotn_rx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAPOTN_RX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mapotn_rx
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
} mapotn_rx_buffer_t;
static INLINE void mapotn_rx_buffer_init( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_rx_buffer_init( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mapotn_rx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mapotn_rx_buffer_flush( mapotn_rx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mapotn_rx_buffer_flush( mapotn_rx_buffer_t *b_ptr )
{
    IOLOG( "mapotn_rx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mapotn_rx_reg_read( mapotn_rx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_read( mapotn_rx_buffer_t *b_ptr,
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
static INLINE void mapotn_rx_reg_write( mapotn_rx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_write( mapotn_rx_buffer_t *b_ptr,
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

static INLINE void mapotn_rx_field_set( mapotn_rx_buffer_t *b_ptr,
                                        mapotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_set( mapotn_rx_buffer_t *b_ptr,
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

static INLINE void mapotn_rx_action_on_write_field_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_action_on_write_field_set( mapotn_rx_buffer_t *b_ptr,
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

static INLINE void mapotn_rx_burst_read( mapotn_rx_buffer_t *b_ptr,
                                         mapotn_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_burst_read( mapotn_rx_buffer_t *b_ptr,
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

static INLINE void mapotn_rx_burst_write( mapotn_rx_buffer_t *b_ptr,
                                          mapotn_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_burst_write( mapotn_rx_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_poll( mapotn_rx_buffer_t *b_ptr,
                                                   mapotn_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_poll( mapotn_rx_buffer_t *b_ptr,
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
 *  register access functions for mapotn_rx
 * ==================================================================================
 */

static INLINE void mapotn_rx_reg_LOWPWR_MODE_write( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_LOWPWR_MODE_write( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_LOWPWR_MODE_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE,
                         value);
}

static INLINE void mapotn_rx_reg_LOWPWR_MODE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_LOWPWR_MODE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_LOWPWR_MODE_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE,
                         mask,
                         PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_LOWPWR_MODE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_LOWPWR_MODE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_LOWPWR_MODE_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_write( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_write( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB,
                         value);
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                    mapotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB,
                         mask,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_RAM_LOWPWR_MODE_write( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_RAM_LOWPWR_MODE_write( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_RAM_LOWPWR_MODE_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE,
                         value);
}

static INLINE void mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_RAM_LOWPWR_MODE_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE,
                         mask,
                         PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_RAM_LOWPWR_MODE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_RAM_LOWPWR_MODE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_RAM_LOWPWR_MODE_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write( mapotn_rx_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write( mapotn_rx_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write", 3, ofs, len);
    mapotn_rx_burst_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER(ofs),
                           len,
                           value);

}

static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                          mapotn_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER(N),
                         mask,
                         PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read( mapotn_rx_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read( mapotn_rx_buffer_t *b_ptr,
                                                                           mapotn_handle_t *h_ptr,
                                                                           UINT32 ofs,
                                                                           UINT32 len,
                                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read", ofs, len, 3 );
    mapotn_rx_burst_read( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER(ofs),
                          len,
                          value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_read( mapotn_rx_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_read( mapotn_rx_buffer_t *b_ptr,
                                                                       mapotn_handle_t *h_ptr,
                                                                       UINT32 N )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER(N));

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_INTR_EN_REG_array_write( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_INTR_EN_REG_array_write( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_INTR_EN_REG_array_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_INTR_EN_REG(N),
                         value);
}

static INLINE void mapotn_rx_reg_INTR_EN_REG_array_field_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_INTR_EN_REG_array_field_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_INTR_EN_REG_array_field_set", N, mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_MAPOTN_RX_REG_REG_INTR_EN_REG(N),
                         mask,
                         PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_INTR_EN_REG_array_read( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_INTR_EN_REG_array_read( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_MAPOTN_RX_REG_REG_INTR_EN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mapotn_rx_reg_INTR_EN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_write( mapotn_rx_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_write( mapotn_rx_buffer_t *b_ptr,
                                                                            mapotn_handle_t *h_ptr,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE,
                         value);
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                                mapotn_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE,
                         mask,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_write( mapotn_rx_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_write( mapotn_rx_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR,
                         value);
}

static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_action_on_write_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 mask,
                                                                                         UINT32 ofs,
                                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_action_on_write_field_set( mapotn_rx_buffer_t *b_ptr,
                                                                                         mapotn_handle_t *h_ptr,
                                                                                         UINT32 mask,
                                                                                         UINT32 ofs,
                                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_action_on_write_field_set", mask, ofs, value );
    mapotn_rx_action_on_write_field_set( b_ptr,
                                         h_ptr,
                                         MEM_TYPE_STATUS,
                                         PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR,
                                         mask,
                                         ofs,
                                         value);

}

static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_poll( mapotn_rx_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_poll( mapotn_rx_buffer_t *b_ptr,
                                                                                    mapotn_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mapotn_rx_poll( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE UINT32 mapotn_rx_reg_INTR_REG_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_INTR_REG_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_RX_REG_REG_INTR_REG);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_INTR_REG_read", reg_value);
    return reg_value;
}

static INLINE void mapotn_rx_reg_SOFT_RST_write( mapotn_rx_buffer_t *b_ptr,
                                                 mapotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_SOFT_RST_write( mapotn_rx_buffer_t *b_ptr,
                                                 mapotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mapotn_rx_reg_SOFT_RST_write", value );
    mapotn_rx_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_RX_REG_REG_SOFT_RST,
                         value);
}

static INLINE void mapotn_rx_reg_SOFT_RST_field_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_reg_SOFT_RST_field_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mapotn_rx_reg_SOFT_RST_field_set", mask, ofs, value );
    mapotn_rx_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_MAPOTN_RX_REG_REG_SOFT_RST,
                         mask,
                         PMC_MAPOTN_RX_REG_REG_SOFT_RST_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 mapotn_rx_reg_SOFT_RST_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_reg_SOFT_RST_read( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mapotn_rx_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_MAPOTN_RX_REG_REG_SOFT_RST);

    IOLOG( "%s -> 0x%08x;", "mapotn_rx_reg_SOFT_RST_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mapotn_rx_field_MPMA_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MPMA_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MPMA_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_MPMA_LOWPWR_set", value );

    /* (0x00000004 bits 2) field MPMA_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    mapotn_rx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_MSK,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_rx_field_MPMA_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMA_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2) field MPMA_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MPMA_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGMP96_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGMP96_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGMP96_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGMP96_LOWPWR_set", value );

    /* (0x00000004 bits 1) field RGMP96_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    mapotn_rx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_MSK,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_rx_field_RGMP96_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGMP96_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field RGMP96_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGMP96_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGFPF96_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGFPF96_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGFPF96_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGFPF96_LOWPWR_set", value );

    /* (0x00000004 bits 0) field RGFPF96_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    mapotn_rx_reg_LOWPWR_MODE_field_set( b_ptr,
                                         h_ptr,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_MSK,
                                         MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 mapotn_rx_field_RGFPF96_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGFPF96_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field RGFPF96_LOWPWR of register PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGFPF96_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_FIFO_RSTB_set( mapotn_rx_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_FIFO_RSTB_set( mapotn_rx_buffer_t *b_ptr,
                                                  mapotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_FIFO_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_FIFO_RSTB_set", value );

    /* (0x00000008 bits 0) field FIFO_RSTB of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB */
    mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_field_set( b_ptr,
                                                     h_ptr,
                                                     MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_MSK,
                                                     MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_OFF,
                                                     value);
}

static INLINE UINT32 mapotn_rx_field_FIFO_RSTB_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_FIFO_RSTB_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field FIFO_RSTB of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB */
    reg_value = mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_MSK) >> MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_FIFO_RSTB_get", value );

    return value;
}
static INLINE void mapotn_rx_field_TOP_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_TOP_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_TOP_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_TOP_RAM_LOWPWR_set", value );

    /* (0x00000018 bits 3) field TOP_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_MSK,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_rx_field_TOP_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_TOP_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 3) field TOP_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_TOP_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_MPMA_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MPMA_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MPMA_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_MPMA_RAM_LOWPWR_set", value );

    /* (0x00000018 bits 2) field MPMA_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_MSK,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_rx_field_MPMA_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMA_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 2) field MPMA_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MPMA_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGMP96_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGMP96_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGMP96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGMP96_RAM_LOWPWR_set", value );

    /* (0x00000018 bits 1) field RGMP96_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_MSK,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_rx_field_RGMP96_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGMP96_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 1) field RGMP96_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGMP96_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGFPF96_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGFPF96_RAM_LOWPWR_set( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGFPF96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGFPF96_RAM_LOWPWR_set", value );

    /* (0x00000018 bits 0) field RGFPF96_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    mapotn_rx_reg_RAM_LOWPWR_MODE_field_set( b_ptr,
                                             h_ptr,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_MSK,
                                             MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_OFF,
                                             value);
}

static INLINE UINT32 mapotn_rx_field_RGFPF96_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGFPF96_RAM_LOWPWR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 0) field RGFPF96_RAM_LOWPWR of register PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE */
    reg_value = mapotn_rx_reg_RAM_LOWPWR_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_MSK) >> MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGFPF96_RAM_LOWPWR_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize3
 * ==================================================================================
 */
static INLINE void mapotn_rx_lfield_DCI2DPI_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_rx_lfield_DCI2DPI_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_rx_lfield_DCI2DPI_EN_set");
    mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write( b_ptr,
                                                             h_ptr,
                                                             0,
                                                             3,
                                                             value);
}

static INLINE void mapotn_rx_lfield_DCI2DPI_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mapotn_rx_lfield_DCI2DPI_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value[3] )
{
    IOLOG( "%s ", "mapotn_rx_lfield_DCI2DPI_EN_get");
    mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read( b_ptr,
                                                            h_ptr,
                                                            0,
                                                            3,
                                                            (UINT32 *)value);
}

static INLINE void mapotn_rx_lfield_range_DCI2DPI_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_lfield_range_DCI2DPI_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rx_lfield_range_DCI2DPI_EN_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_rx_lfield_range_DCI2DPI_EN_set", start_bit, stop_bit, value);
    mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_field_set( b_ptr,
                                                           h_ptr,
                                                           word_number,
                                                           mask,
                                                           field_ofs,
                                                           value);
}

static INLINE UINT32 mapotn_rx_lfield_range_DCI2DPI_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_lfield_range_DCI2DPI_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rx_lfield_range_DCI2DPI_EN_get", start_bit, stop_bit );
    }
    reg_value = mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_read( b_ptr,
                                                                  h_ptr,
                                                                  word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mapotn_rx_lfield_range_DCI2DPI_EN_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void mapotn_rx_field_MPMO_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MPMO_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MPMO_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MPMO_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_MPMO_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 8) field MPMO_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_MPMO_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMO_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MPMO_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 8) field MPMO_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_MPMO_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_ODUKSC_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_ODUKSC_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_ODUKSC_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_ODUKSC_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_ODUKSC_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 7) field ODUKSC_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_ODUKSC_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_ODUKSC_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_ODUKSC_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 7) field ODUKSC_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_ODUKSC_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_TGMP96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_TGMP96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_TGMP96_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_TGMP96_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_TGMP96_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 6) field TGMP96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_TGMP96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_TGMP96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_TGMP96_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 6) field TGMP96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_TGMP96_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_TGFPF96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_TGFPF96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_TGFPF96_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_TGFPF96_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_TGFPF96_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 5) field TGFPF96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_TGFPF96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_TGFPF96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_TGFPF96_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 5) field TGFPF96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_TGFPF96_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 4) field MAPOTN_TX_TOP_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 4) field MAPOTN_TX_TOP_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_MPMA_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MPMA_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                     mapotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MPMA_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MPMA_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_MPMA_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 3) field MPMA_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_MPMA_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMA_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MPMA_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 3) field MPMA_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_MPMA_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_RGMP96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGMP96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                       mapotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_RGMP96_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGMP96_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_RGMP96_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 2) field RGMP96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_RGMP96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGMP96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_RGMP96_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 2) field RGMP96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_RGMP96_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_RGFPF96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGFPF96_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_RGFPF96_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGFPF96_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_RGFPF96_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 1) field RGFPF96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_RGFPF96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGFPF96_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                          mapotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_RGFPF96_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 1) field RGFPF96_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_RGFPF96_INTR_EN_get", N, value );

    return value;
}
static INLINE void mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set( mapotn_rx_buffer_t *b_ptr,
                                                              mapotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set", N, value );

    /* ((0x00000300 + (N) * 0x4) bits 0) field MAPOTN_RX_TOP_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    mapotn_rx_reg_INTR_EN_REG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_MSK,
                                               MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_OFF,
                                               value);
}

static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_get( mapotn_rx_buffer_t *b_ptr,
                                                                mapotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_get", N, 3);
    /* ((0x00000300 + (N) * 0x4) bits 0) field MAPOTN_RX_TOP_INTR_EN of register PMC_MAPOTN_RX_REG_REG_INTR_EN_REG index N=0..3 */
    reg_value = mapotn_rx_reg_INTR_EN_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_MSK) >> MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mapotn_rx_field_FIFO_FULL_E_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_FIFO_FULL_E_set( mapotn_rx_buffer_t *b_ptr,
                                                    mapotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_FIFO_FULL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_FIFO_FULL_E_set", value );

    /* (0x0000000c bits 0) field FIFO_FULL_E of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE */
    mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_field_set( b_ptr,
                                                                 h_ptr,
                                                                 MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_MSK,
                                                                 MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_OFF,
                                                                 value);
}

static INLINE UINT32 mapotn_rx_field_FIFO_FULL_E_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_FIFO_FULL_E_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 0) field FIFO_FULL_E of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE */
    reg_value = mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_MSK) >> MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_FIFO_FULL_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mapotn_rx_field_FIFO_FULL_I_set_to_clear( mapotn_rx_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_FIFO_FULL_I_set_to_clear( mapotn_rx_buffer_t *b_ptr,
                                                             mapotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_FIFO_FULL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_FIFO_FULL_I_set_to_clear", value );

    /* (0x00000010 bits 0) field FIFO_FULL_I of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR */
    mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_MSK,
                                                                          MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_OFF,
                                                                          value);
}

static INLINE UINT32 mapotn_rx_field_FIFO_FULL_I_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_FIFO_FULL_I_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field FIFO_FULL_I of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR */
    reg_value = mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_MSK) >> MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_FIFO_FULL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_field_FIFO_FULL_I_poll( mapotn_rx_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mapotn_rx_field_FIFO_FULL_I_poll( mapotn_rx_buffer_t *b_ptr,
                                                                     mapotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mapotn_rx_field_FIFO_FULL_I_poll", value );

    /* (0x00000010 bits 0) field FIFO_FULL_I of register PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR */
    return mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_poll( b_ptr,
                                                            h_ptr,
                                                            MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_MSK,
                                                            (value<<MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_OFF),
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
static INLINE UINT32 mapotn_rx_field_MAPOTN_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 19:16) field MAPOTN_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MAPOTN_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_range_MAPOTN_INTR_get( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_range_MAPOTN_INTR_get( mapotn_rx_buffer_t *b_ptr,
                                                            mapotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mapotn_rx_field_range_MAPOTN_INTR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mapotn_rx_field_range_MAPOTN_INTR_get", stop_bit, 3 );
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
    /* (0x00000310 bits 19:16) field MAPOTN_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_MSK)
                  >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_MSK, MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mapotn_rx_field_range_MAPOTN_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_MPMO_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMO_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 8) field MPMO_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMO_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMO_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MPMO_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_ODUKSC_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_ODUKSC_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 7) field ODUKSC_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_ODUKSC_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_ODUKSC_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_ODUKSC_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_TGMP96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_TGMP96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 6) field TGMP96_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_TGMP96_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_TGMP96_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_TGMP96_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_TGFPF96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_TGFPF96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 5) field TGFPF96_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_TGFPF96_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_TGFPF96_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_TGFPF96_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 4) field MAPOTN_TX_TOP_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_TX_TOP_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_TX_TOP_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MAPOTN_TX_TOP_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_MPMA_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMA_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 3) field MPMA_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMA_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMA_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MPMA_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_RGMP96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGMP96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 2) field RGMP96_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_RGMP96_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_RGMP96_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGMP96_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_RGFPF96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGFPF96_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 1) field RGFPF96_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_RGFPF96_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_RGFPF96_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGFPF96_INTR_get", value );

    return value;
}
static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000310 bits 0) field MAPOTN_RX_TOP_INTR of register PMC_MAPOTN_RX_REG_REG_INTR_REG */
    reg_value = mapotn_rx_reg_INTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_RX_TOP_INTR_MSK) >> MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_RX_TOP_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MAPOTN_RX_TOP_INTR_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void mapotn_rx_field_MPMA_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MPMA_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                      mapotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MPMA_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_MPMA_SOFT_RST_set", value );

    /* (0x00000000 bits 3) field MPMA_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    mapotn_rx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_MSK,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_rx_field_MPMA_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MPMA_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field MPMA_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    reg_value = mapotn_rx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_MSK) >> MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MPMA_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGMP96_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGMP96_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                        mapotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGMP96_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGMP96_SOFT_RST_set", value );

    /* (0x00000000 bits 2) field RGMP96_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    mapotn_rx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_MSK,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_rx_field_RGMP96_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGMP96_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field RGMP96_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    reg_value = mapotn_rx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_MSK) >> MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGMP96_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_rx_field_RGFPF96_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_RGFPF96_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                         mapotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_RGFPF96_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_RGFPF96_SOFT_RST_set", value );

    /* (0x00000000 bits 1) field RGFPF96_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    mapotn_rx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_MSK,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_rx_field_RGFPF96_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_RGFPF96_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field RGFPF96_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    reg_value = mapotn_rx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_MSK) >> MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_RGFPF96_SOFT_RST_get", value );

    return value;
}
static INLINE void mapotn_rx_field_MAPOTN_RX_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mapotn_rx_field_MAPOTN_RX_SOFT_RST_set( mapotn_rx_buffer_t *b_ptr,
                                                           mapotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mapotn_rx_field_MAPOTN_RX_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mapotn_rx_field_MAPOTN_RX_SOFT_RST_set", value );

    /* (0x00000000 bits 0) field MAPOTN_RX_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    mapotn_rx_reg_SOFT_RST_field_set( b_ptr,
                                      h_ptr,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_MSK,
                                      MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_OFF,
                                      value);
}

static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mapotn_rx_field_MAPOTN_RX_SOFT_RST_get( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MAPOTN_RX_SOFT_RST of register PMC_MAPOTN_RX_REG_REG_SOFT_RST */
    reg_value = mapotn_rx_reg_SOFT_RST_read(  b_ptr, h_ptr);
    value = (reg_value & MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_MSK) >> MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mapotn_rx_field_MAPOTN_RX_SOFT_RST_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_RX_IO_INLINE_H */
