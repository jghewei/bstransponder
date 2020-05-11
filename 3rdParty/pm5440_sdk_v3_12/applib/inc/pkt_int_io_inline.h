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
 *     and register accessor functions for the pkt_int block
 *****************************************************************************/
#ifndef _PKT_INT_IO_INLINE_H
#define _PKT_INT_IO_INLINE_H

#include "cbrc_api.h"
#include "pkt_int_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PKT_INT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for pkt_int
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
    cbrc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} pkt_int_buffer_t;
static INLINE void pkt_int_buffer_init( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void pkt_int_buffer_init( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "pkt_int_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void pkt_int_buffer_flush( pkt_int_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void pkt_int_buffer_flush( pkt_int_buffer_t *b_ptr )
{
    IOLOG( "pkt_int_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 pkt_int_reg_read( pkt_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_read( pkt_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
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
static INLINE void pkt_int_reg_write( pkt_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_write( pkt_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
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

static INLINE void pkt_int_field_set( pkt_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_set( pkt_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
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

static INLINE void pkt_int_action_on_write_field_set( pkt_int_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_action_on_write_field_set( pkt_int_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
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

static INLINE void pkt_int_burst_read( pkt_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void pkt_int_burst_read( pkt_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
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

static INLINE void pkt_int_burst_write( pkt_int_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void pkt_int_burst_write( pkt_int_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE pkt_int_poll( pkt_int_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_poll( pkt_int_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
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
 *  register access functions for pkt_int
 * ==================================================================================
 */

static INLINE void pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_write( pkt_int_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_write( pkt_int_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_write", value );
    pkt_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET,
                       value);
}

static INLINE void pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_field_set( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_field_set( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_field_set", mask, ofs, value );
    pkt_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET,
                       mask,
                       PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void pkt_int_reg_CHANNEL_ID_REG_array_write( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CHANNEL_ID_REG_array_write( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pkt_int_reg_CHANNEL_ID_REG_array_write", value );
    pkt_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PKT_INT_REG_CHANNEL_ID_REG(N),
                       value);
}

static INLINE void pkt_int_reg_CHANNEL_ID_REG_array_field_set( pkt_int_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CHANNEL_ID_REG_array_field_set( pkt_int_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "pkt_int_reg_CHANNEL_ID_REG_array_field_set", N, mask, ofs, value );
    pkt_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PKT_INT_REG_CHANNEL_ID_REG(N),
                       mask,
                       PMC_PKT_INT_REG_CHANNEL_ID_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pkt_int_reg_CHANNEL_ID_REG_array_read( pkt_int_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_CHANNEL_ID_REG_array_read( pkt_int_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PKT_INT_REG_CHANNEL_ID_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "pkt_int_reg_CHANNEL_ID_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void pkt_int_reg_CHANNEL_ID_UPDATE_REG_write( pkt_int_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CHANNEL_ID_UPDATE_REG_write( pkt_int_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pkt_int_reg_CHANNEL_ID_UPDATE_REG_write", value );
    pkt_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG,
                       value);
}

static INLINE void pkt_int_reg_CHANNEL_ID_UPDATE_REG_field_set( pkt_int_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CHANNEL_ID_UPDATE_REG_field_set( pkt_int_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pkt_int_reg_CHANNEL_ID_UPDATE_REG_field_set", mask, ofs, value );
    pkt_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG,
                       mask,
                       PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pkt_int_reg_CHANNEL_ID_UPDATE_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_CHANNEL_ID_UPDATE_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG);

    IOLOG( "%s -> 0x%08x;", "pkt_int_reg_CHANNEL_ID_UPDATE_REG_read", reg_value);
    return reg_value;
}

static INLINE void pkt_int_reg_CFC_FIFO_INT_EN_REG_write( pkt_int_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CFC_FIFO_INT_EN_REG_write( pkt_int_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pkt_int_reg_CFC_FIFO_INT_EN_REG_write", value );
    pkt_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG,
                       value);
}

static INLINE void pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set", mask, ofs, value );
    pkt_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG,
                       mask,
                       PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_EN_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_EN_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pkt_int_reg_CFC_FIFO_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void pkt_int_reg_CFC_FIFO_INT_REG_write( pkt_int_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CFC_FIFO_INT_REG_write( pkt_int_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pkt_int_reg_CFC_FIFO_INT_REG_write", value );
    pkt_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PKT_INT_REG_CFC_FIFO_INT_REG,
                       value);
}

static INLINE void pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( pkt_int_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( pkt_int_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set", mask, ofs, value );
    pkt_int_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_PKT_INT_REG_CFC_FIFO_INT_REG,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PKT_INT_REG_CFC_FIFO_INT_REG);

    IOLOG( "%s -> 0x%08x;", "pkt_int_reg_CFC_FIFO_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_reg_CFC_FIFO_INT_REG_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_reg_CFC_FIFO_INT_REG_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "pkt_int_reg_CFC_FIFO_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return pkt_int_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_PKT_INT_REG_CFC_FIFO_INT_REG,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pkt_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "pkt_int_reg_CFC_FIFO_INT_STAT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( pkt_int_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( pkt_int_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return pkt_int_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void pkt_int_field_SOFT_RST_set( pkt_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_SOFT_RST_set( pkt_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_SOFT_RST_set", value );

    /* (0x00024050 bits 0) field SOFT_RST of register PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET */
    pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_field_set( b_ptr,
                                                       h_ptr,
                                                       PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET_BIT_SOFT_RST_MSK,
                                                       PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET_BIT_SOFT_RST_OFF,
                                                       value);
}

static INLINE UINT32 pkt_int_field_SOFT_RST_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_SOFT_RST_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024050 bits 0) field SOFT_RST of register PMC_PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET */
    reg_value = pkt_int_reg_PACKET_INTERLEAVER_SW_RESET_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET_BIT_SOFT_RST_MSK) >> PKT_INT_REG_PACKET_INTERLEAVER_SW_RESET_BIT_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_SOFT_RST_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void pkt_int_field_ID_set( pkt_int_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_ID_set( pkt_int_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pkt_int_field_ID_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_ID_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "pkt_int_field_ID_set", N, value );

    /* ((0x00024000 + (N) * 4) bits 3:0) field ID of register PMC_PKT_INT_REG_CHANNEL_ID_REG index N=0..11 */
    pkt_int_reg_CHANNEL_ID_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_MSK,
                                                PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF,
                                                value);
}

static INLINE UINT32 pkt_int_field_ID_get( pkt_int_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_ID_get( pkt_int_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pkt_int_field_ID_get", N, 11);
    /* ((0x00024000 + (N) * 4) bits 3:0) field ID of register PMC_PKT_INT_REG_CHANNEL_ID_REG index N=0..11 */
    reg_value = pkt_int_reg_CHANNEL_ID_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_MSK) >> PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "pkt_int_field_ID_get", N, value );

    return value;
}
static INLINE void pkt_int_field_range_ID_set( pkt_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_range_ID_set( pkt_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pkt_int_field_range_ID_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_ID_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_ID_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00024000 + (N) * 4) bits 3:0) field ID of register PMC_PKT_INT_REG_CHANNEL_ID_REG index N=0..11 */
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
        /* ((0x00024000 + (N) * 4) bits 3:0) field ID of register PMC_PKT_INT_REG_CHANNEL_ID_REG index N=0..11 */
        pkt_int_reg_CHANNEL_ID_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF + subfield_offset),
                                                    PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 pkt_int_field_range_ID_get( pkt_int_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_ID_get( pkt_int_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "pkt_int_field_range_ID_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_ID_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_ID_get", stop_bit, 3 );
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
    /* ((0x00024000 + (N) * 4) bits 3:0) field ID of register PMC_PKT_INT_REG_CHANNEL_ID_REG index N=0..11 */
    reg_value = pkt_int_reg_CHANNEL_ID_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_MSK)
                  >> PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_MSK, PKT_INT_REG_CHANNEL_ID_REG_BIT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_ID_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void pkt_int_field_ID_UPDATE_set( pkt_int_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_ID_UPDATE_set( pkt_int_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_ID_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_ID_UPDATE_set", value );

    /* (0x00024030 bits 0) field ID_UPDATE of register PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG */
    pkt_int_reg_CHANNEL_ID_UPDATE_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 PKT_INT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_MSK,
                                                 PKT_INT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_OFF,
                                                 value);
}

static INLINE UINT32 pkt_int_field_ID_UPDATE_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_ID_UPDATE_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024030 bits 0) field ID_UPDATE of register PMC_PKT_INT_REG_CHANNEL_ID_UPDATE_REG */
    reg_value = pkt_int_reg_CHANNEL_ID_UPDATE_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & PKT_INT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_MSK) >> PKT_INT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_ID_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void pkt_int_field_CFC_FIFO_UDR_E_set( pkt_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_CFC_FIFO_UDR_E_set( pkt_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_CFC_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_CFC_FIFO_UDR_E_set", value );

    /* (0x00024038 bits 23:12) field CFC_FIFO_UDR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( b_ptr,
                                               h_ptr,
                                               PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_MSK,
                                               PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF,
                                               value);
}

static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_E_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_E_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024038 bits 23:12) field CFC_FIFO_UDR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_MSK) >> PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void pkt_int_field_range_CFC_FIFO_UDR_E_set( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_range_CFC_FIFO_UDR_E_set( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024038 bits 23:12) field CFC_FIFO_UDR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
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
        /* (0x00024038 bits 23:12) field CFC_FIFO_UDR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
        pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF + subfield_offset),
                                                   PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_E_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_E_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x00024038 bits 23:12) field CFC_FIFO_UDR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_MSK, PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void pkt_int_field_CFC_FIFO_OVR_E_set( pkt_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_CFC_FIFO_OVR_E_set( pkt_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_CFC_FIFO_OVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_CFC_FIFO_OVR_E_set", value );

    /* (0x00024038 bits 11:0) field CFC_FIFO_OVR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( b_ptr,
                                               h_ptr,
                                               PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_MSK,
                                               PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF,
                                               value);
}

static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_E_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_E_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024038 bits 11:0) field CFC_FIFO_OVR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_MSK) >> PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void pkt_int_field_range_CFC_FIFO_OVR_E_set( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_range_CFC_FIFO_OVR_E_set( pkt_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024038 bits 11:0) field CFC_FIFO_OVR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
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
        /* (0x00024038 bits 11:0) field CFC_FIFO_OVR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
        pkt_int_reg_CFC_FIFO_INT_EN_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF + subfield_offset),
                                                   PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_E_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_E_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_E_get", stop_bit, 11 );
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
    /* (0x00024038 bits 11:0) field CFC_FIFO_OVR_E of register PMC_PKT_INT_REG_CFC_FIFO_INT_EN_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_MSK, PKT_INT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void pkt_int_field_CFC_FIFO_UDR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_CFC_FIFO_UDR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_CFC_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_CFC_FIFO_UDR_I_set_to_clear", value );

    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK,
                                                            PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF,
                                                            value);
}

static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_I_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_I_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK) >> PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void pkt_int_field_range_CFC_FIFO_UDR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_range_CFC_FIFO_UDR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
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
        /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
        pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF + subfield_offset),
                                                                PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_I_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_I_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK, PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_UDR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_UDR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
        return pkt_int_reg_CFC_FIFO_INT_REG_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF + subfield_offset),
                                                  value << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_UDR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_UDR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pkt_int_field_CFC_FIFO_UDR_I_poll", value );

    /* (0x00024034 bits 23:12) field CFC_FIFO_UDR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    return pkt_int_reg_CFC_FIFO_INT_REG_poll( b_ptr,
                                              h_ptr,
                                              PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK,
                                              (value<<PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void pkt_int_field_CFC_FIFO_OVR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_CFC_FIFO_OVR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pkt_int_field_CFC_FIFO_OVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "pkt_int_field_CFC_FIFO_OVR_I_set_to_clear", value );

    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK,
                                                            PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF,
                                                            value);
}

static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_I_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_I_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK) >> PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void pkt_int_field_range_CFC_FIFO_OVR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void pkt_int_field_range_CFC_FIFO_OVR_I_set_to_clear( pkt_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
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
        /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
        pkt_int_reg_CFC_FIFO_INT_REG_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF + subfield_offset),
                                                                PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_I_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_I_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_I_get", stop_bit, 11 );
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
    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK, PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_OVR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_OVR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
        return pkt_int_reg_CFC_FIFO_INT_REG_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF + subfield_offset),
                                                  value << (PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_OVR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_OVR_I_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pkt_int_field_CFC_FIFO_OVR_I_poll", value );

    /* (0x00024034 bits 11:0) field CFC_FIFO_OVR_I of register PMC_PKT_INT_REG_CFC_FIFO_INT_REG */
    return pkt_int_reg_CFC_FIFO_INT_REG_poll( b_ptr,
                                              h_ptr,
                                              PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK,
                                              (value<<PKT_INT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF),
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
static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_V_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_UDR_V_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0002403c bits 23:12) field CFC_FIFO_UDR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_MSK) >> PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_V_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_UDR_V_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_V_get", stop_bit, 11 );
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
    /* (0x0002403c bits 23:12) field CFC_FIFO_UDR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_MSK, PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_UDR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_UDR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_UDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002403c bits 23:12) field CFC_FIFO_UDR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0002403c bits 23:12) field CFC_FIFO_UDR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
        return pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF + subfield_offset),
                                                       value << (PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_UDR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_UDR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pkt_int_field_CFC_FIFO_UDR_V_poll", value );

    /* (0x0002403c bits 23:12) field CFC_FIFO_UDR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    return pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( b_ptr,
                                                   h_ptr,
                                                   PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_MSK,
                                                   (value<<PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_V_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_CFC_FIFO_OVR_V_get( pkt_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0002403c bits 11:0) field CFC_FIFO_OVR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_MSK) >> PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "pkt_int_field_CFC_FIFO_OVR_V_get", value );

    return value;
}
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_V_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pkt_int_field_range_CFC_FIFO_OVR_V_get( pkt_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_V_get", stop_bit, 11 );
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
    /* (0x0002403c bits 11:0) field CFC_FIFO_OVR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    reg_value = pkt_int_reg_CFC_FIFO_INT_STAT_REG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_MSK)
                  >> PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_MSK, PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_OVR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_range_CFC_FIFO_OVR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pkt_int_field_range_CFC_FIFO_OVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pkt_int_field_range_CFC_FIFO_OVR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pkt_int_field_range_CFC_FIFO_OVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002403c bits 11:0) field CFC_FIFO_OVR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0002403c bits 11:0) field CFC_FIFO_OVR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
        return pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF + subfield_offset),
                                                       value << (PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_OVR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pkt_int_field_CFC_FIFO_OVR_V_poll( pkt_int_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pkt_int_field_CFC_FIFO_OVR_V_poll", value );

    /* (0x0002403c bits 11:0) field CFC_FIFO_OVR_V of register PMC_PKT_INT_REG_CFC_FIFO_INT_STAT_REG */
    return pkt_int_reg_CFC_FIFO_INT_STAT_REG_poll( b_ptr,
                                                   h_ptr,
                                                   PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_MSK,
                                                   (value<<PKT_INT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PKT_INT_IO_INLINE_H */
