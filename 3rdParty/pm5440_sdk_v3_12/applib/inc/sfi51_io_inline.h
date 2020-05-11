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
 *     and register accessor functions for the sfi51 block
 *****************************************************************************/
#ifndef _SFI51_IO_INLINE_H
#define _SFI51_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "sfi51_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SFI51_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sfi51
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
    lineotn_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sfi51_buffer_t;
static INLINE void sfi51_buffer_init( sfi51_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A ) ALWAYS_INLINE;
static INLINE void sfi51_buffer_init( sfi51_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "sfi51_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sfi51_buffer_flush( sfi51_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sfi51_buffer_flush( sfi51_buffer_t *b_ptr )
{
    IOLOG( "sfi51_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sfi51_reg_read( sfi51_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_read( sfi51_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg);
    return value;
}
static INLINE void sfi51_reg_write( sfi51_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_write( sfi51_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value);
}

static INLINE void sfi51_field_set( sfi51_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 mask,
                                    UINT32 unused_mask,
                                    UINT32 ofs,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_set( sfi51_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 A,
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
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void sfi51_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void sfi51_burst_read( sfi51_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_burst_read( sfi51_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void sfi51_burst_write( sfi51_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfi51_burst_write( sfi51_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE sfi51_poll( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 mask,
                                               UINT32 value,
                                               PMC_POLL_COMPARISON_TYPE cmp,
                                               UINT32 max_count,
                                               UINT32 *num_failed_polls,
                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_poll( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
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
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for sfi51
 * ==================================================================================
 */

static INLINE void sfi51_reg_RX_GLOBAL_CTRL_REG_write( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_GLOBAL_CTRL_REG_write( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_GLOBAL_CTRL_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_RX_GLOBAL_CTRL_REG,
                     value);
}

static INLINE void sfi51_reg_RX_GLOBAL_CTRL_REG_field_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_GLOBAL_CTRL_REG_field_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_GLOBAL_CTRL_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_RX_GLOBAL_CTRL_REG,
                     mask,
                     PMC_SFI51_REG_RX_GLOBAL_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_RX_GLOBAL_CTRL_REG_read( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_GLOBAL_CTRL_REG_read( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_SFI51_REG_RX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_GLOBAL_CTRL_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_TX_GLOBAL_CTRL_REG_write( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_GLOBAL_CTRL_REG_write( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_TX_GLOBAL_CTRL_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_TX_GLOBAL_CTRL_REG,
                     value);
}

static INLINE void sfi51_reg_TX_GLOBAL_CTRL_REG_field_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_GLOBAL_CTRL_REG_field_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_TX_GLOBAL_CTRL_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_TX_GLOBAL_CTRL_REG,
                     mask,
                     PMC_SFI51_REG_TX_GLOBAL_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_TX_GLOBAL_CTRL_REG_read( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_TX_GLOBAL_CTRL_REG_read( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_SFI51_REG_TX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_TX_GLOBAL_CTRL_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_write( sfi51_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_write( sfi51_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG,
                     value);
}

static INLINE void sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG,
                     mask,
                     PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read( sfi51_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read( sfi51_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_EXPANSION_HEADER_REG_write( sfi51_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_EXPANSION_HEADER_REG_write( sfi51_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_EXPANSION_HEADER_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_EXPANSION_HEADER_REG,
                     value);
}

static INLINE void sfi51_reg_EXPANSION_HEADER_REG_field_set( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_EXPANSION_HEADER_REG_field_set( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_EXPANSION_HEADER_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_CONFIG,
                     PMC_SFI51_REG_EXPANSION_HEADER_REG,
                     mask,
                     PMC_SFI51_REG_EXPANSION_HEADER_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_EXPANSION_HEADER_REG_read( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_EXPANSION_HEADER_REG_read( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_SFI51_REG_EXPANSION_HEADER_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_EXPANSION_HEADER_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_RX_INT_EN_write( sfi51_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_INT_EN_write( sfi51_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_INT_EN_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_INT_EN,
                     value);
}

static INLINE void sfi51_reg_RX_INT_EN_field_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_INT_EN_field_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_INT_EN_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_INT_EN,
                     mask,
                     PMC_SFI51_REG_RX_INT_EN_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_RX_INT_EN_read( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_INT_EN_read( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_write( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_write( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG,
                     value);
}

static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG,
                     mask,
                     PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_write( sfi51_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_write( sfi51_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG,
                     value);
}

static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG,
                     mask,
                     PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_TX_INT_EN_write( sfi51_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_INT_EN_write( sfi51_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_TX_INT_EN_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_TX_INT_EN,
                     value);
}

static INLINE void sfi51_reg_TX_INT_EN_field_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_INT_EN_field_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_TX_INT_EN_field_set", A, mask, ofs, value );
    sfi51_field_set( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_TX_INT_EN,
                     mask,
                     PMC_SFI51_REG_TX_INT_EN_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 sfi51_reg_TX_INT_EN_read( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_TX_INT_EN_read( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_TX_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_TX_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void sfi51_reg_RX_INT_REG_write( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_INT_REG_write( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_INT_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_INT_REG,
                     value);
}

static INLINE void sfi51_reg_RX_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    sfi51_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_SFI51_REG_RX_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 sfi51_reg_RX_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_REG_write( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_REG_write( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_DESKEW_OOA_INT_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG,
                     value);
}

static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    sfi51_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_DESKEW_OOA_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_DESKEW_OOA_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_write( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_write( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG,
                     value);
}

static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    sfi51_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void sfi51_reg_TX_INT_REG_write( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_INT_REG_write( sfi51_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfi51_reg_TX_INT_REG_write", value );
    sfi51_reg_write( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_SFI51_REG_TX_INT_REG,
                     value);
}

static INLINE void sfi51_reg_TX_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_reg_TX_INT_REG_action_on_write_field_set( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfi51_reg_TX_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    sfi51_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_SFI51_REG_TX_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 sfi51_reg_TX_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_TX_INT_REG_read( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_TX_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_TX_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_REG_poll( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_TX_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_TX_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 sfi51_reg_RX_INT_STAT_read( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_INT_STAT_read( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_INT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_STAT_poll( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_STAT_poll( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_INT_STAT,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read( sfi51_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read( sfi51_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll( sfi51_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll( sfi51_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll( sfi51_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 sfi51_reg_TX_INT_STAT_read( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_reg_TX_INT_STAT_read( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfi51_reg_read( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_SFI51_REG_TX_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "sfi51_reg_TX_INT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_STAT_poll( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_STAT_poll( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfi51_reg_TX_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfi51_poll( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFI51_REG_TX_INT_STAT,
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
static INLINE void sfi51_field_RX_LANE_SWAP_set( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_LANE_SWAP_set( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_LANE_SWAP_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_LANE_SWAP_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_LANE_SWAP_set", A, value );

    /* (0x0001a000 bits 2) field RX_LANE_SWAP of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    sfi51_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_MSK,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_OFF,
                                            value);
}

static INLINE UINT32 sfi51_field_RX_LANE_SWAP_get( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_LANE_SWAP_get( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_LANE_SWAP_get", A, 1);
    /* (0x0001a000 bits 2) field RX_LANE_SWAP of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    reg_value = sfi51_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_MSK) >> SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_LANE_SWAP_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_DISABLE_set( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_DISABLE_set( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_DISABLE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_DISABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_DISABLE_set", A, value );

    /* (0x0001a000 bits 1) field RX_DESKEW_DISABLE of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    sfi51_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_MSK,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_OFF,
                                            value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_DISABLE_get( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_DISABLE_get( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_DISABLE_get", A, 1);
    /* (0x0001a000 bits 1) field RX_DESKEW_DISABLE of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    reg_value = sfi51_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_MSK) >> SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_DISABLE_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_SYNC_RESET_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_SYNC_RESET_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_SYNC_RESET_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_SYNC_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_SYNC_RESET_set", A, value );

    /* (0x0001a000 bits 0) field RX_SYNC_RESET of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    sfi51_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_MSK,
                                            SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_OFF,
                                            value);
}

static INLINE UINT32 sfi51_field_RX_SYNC_RESET_get( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_SYNC_RESET_get( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_SYNC_RESET_get", A, 1);
    /* (0x0001a000 bits 0) field RX_SYNC_RESET of register PMC_SFI51_REG_RX_GLOBAL_CTRL_REG */
    reg_value = sfi51_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_MSK) >> SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_SYNC_RESET_get", A, value );

    return value;
}
static INLINE void sfi51_field_TX_LANE_SWAP_set( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_LANE_SWAP_set( sfi51_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_LANE_SWAP_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_LANE_SWAP_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_LANE_SWAP_set", A, value );

    /* (0x0001a004 bits 1) field TX_LANE_SWAP of register PMC_SFI51_REG_TX_GLOBAL_CTRL_REG */
    sfi51_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_MSK,
                                            SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_OFF,
                                            value);
}

static INLINE UINT32 sfi51_field_TX_LANE_SWAP_get( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_LANE_SWAP_get( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_LANE_SWAP_get", A, 1);
    /* (0x0001a004 bits 1) field TX_LANE_SWAP of register PMC_SFI51_REG_TX_GLOBAL_CTRL_REG */
    reg_value = sfi51_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_MSK) >> SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_LANE_SWAP_get", A, value );

    return value;
}
static INLINE void sfi51_field_TX_SYNC_RESET_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_SYNC_RESET_set( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_SYNC_RESET_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_SYNC_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_SYNC_RESET_set", A, value );

    /* (0x0001a004 bits 0) field TX_SYNC_RESET of register PMC_SFI51_REG_TX_GLOBAL_CTRL_REG */
    sfi51_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_MSK,
                                            SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_OFF,
                                            value);
}

static INLINE UINT32 sfi51_field_TX_SYNC_RESET_get( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_SYNC_RESET_get( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_SYNC_RESET_get", A, 1);
    /* (0x0001a004 bits 0) field TX_SYNC_RESET of register PMC_SFI51_REG_TX_GLOBAL_CTRL_REG */
    reg_value = sfi51_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_MSK) >> SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_SYNC_RESET_get", A, value );

    return value;
}
static INLINE void sfi51_field_DSC_FIFO_HI_RD_THRSH_set( sfi51_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_DSC_FIFO_HI_RD_THRSH_set( sfi51_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_DSC_FIFO_HI_RD_THRSH_set", A, 1);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_DSC_FIFO_HI_RD_THRSH_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_DSC_FIFO_HI_RD_THRSH_set", A, value );

    /* (0x0001a02c bits 4:0) field DSC_FIFO_HI_RD_THRSH of register PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG */
    sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_MSK,
                                                              SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF,
                                                              value);
}

static INLINE UINT32 sfi51_field_DSC_FIFO_HI_RD_THRSH_get( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_DSC_FIFO_HI_RD_THRSH_get( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_DSC_FIFO_HI_RD_THRSH_get", A, 1);
    /* (0x0001a02c bits 4:0) field DSC_FIFO_HI_RD_THRSH of register PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG */
    reg_value = sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_MSK) >> SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_DSC_FIFO_HI_RD_THRSH_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set( sfi51_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a02c bits 4:0) field DSC_FIFO_HI_RD_THRSH of register PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG */
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
        /* (0x0001a02c bits 4:0) field DSC_FIFO_HI_RD_THRSH of register PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG */
        sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF + subfield_offset),
                                                                  SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get( sfi51_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get( sfi51_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get", stop_bit, 4 );
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
    /* (0x0001a02c bits 4:0) field DSC_FIFO_HI_RD_THRSH of register PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG */
    reg_value = sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_MSK)
                  >> SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_MSK, SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_field_EXPN_HEADER_set( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_EXPN_HEADER_set( sfi51_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_EXPN_HEADER_set", A, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_EXPN_HEADER_set", A, value );

    /* (0x0001a03c bits 31:0) field EXPN_HEADER of register PMC_SFI51_REG_EXPANSION_HEADER_REG */
    sfi51_reg_EXPANSION_HEADER_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_MSK,
                                              SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF,
                                              value);
}

static INLINE UINT32 sfi51_field_EXPN_HEADER_get( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_EXPN_HEADER_get( sfi51_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_EXPN_HEADER_get", A, 1);
    /* (0x0001a03c bits 31:0) field EXPN_HEADER of register PMC_SFI51_REG_EXPANSION_HEADER_REG */
    reg_value = sfi51_reg_EXPANSION_HEADER_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_MSK) >> SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_EXPN_HEADER_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_EXPN_HEADER_set( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_EXPN_HEADER_set( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_EXPN_HEADER_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_EXPN_HEADER_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_EXPN_HEADER_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_EXPN_HEADER_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a03c bits 31:0) field EXPN_HEADER of register PMC_SFI51_REG_EXPANSION_HEADER_REG */
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
        /* (0x0001a03c bits 31:0) field EXPN_HEADER of register PMC_SFI51_REG_EXPANSION_HEADER_REG */
        sfi51_reg_EXPANSION_HEADER_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF + subfield_offset),
                                                  SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_EXPN_HEADER_get( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_EXPN_HEADER_get( sfi51_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_EXPN_HEADER_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_EXPN_HEADER_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_EXPN_HEADER_get", stop_bit, 31 );
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
    /* (0x0001a03c bits 31:0) field EXPN_HEADER of register PMC_SFI51_REG_EXPANSION_HEADER_REG */
    reg_value = sfi51_reg_EXPANSION_HEADER_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_MSK)
                  >> SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_MSK, SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_EXPN_HEADER_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void sfi51_field_RX_DESKEW_FIFO_OVFL_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_FIFO_OVFL_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_OVFL_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_E_set", A, value );

    /* (0x0001a00c bits 4) field RX_DESKEW_FIFO_OVFL_E of register PMC_SFI51_REG_RX_INT_EN */
    sfi51_reg_RX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_MSK,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_OVFL_E_get", A, 1);
    /* (0x0001a00c bits 4) field RX_DESKEW_FIFO_OVFL_E of register PMC_SFI51_REG_RX_INT_EN */
    reg_value = sfi51_reg_RX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_MSK) >> SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_FIFO_UNFL_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_FIFO_UNFL_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_UNFL_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_E_set", A, value );

    /* (0x0001a00c bits 3) field RX_DESKEW_FIFO_UNFL_E of register PMC_SFI51_REG_RX_INT_EN */
    sfi51_reg_RX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_MSK,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_UNFL_E_get", A, 1);
    /* (0x0001a00c bits 3) field RX_DESKEW_FIFO_UNFL_E of register PMC_SFI51_REG_RX_INT_EN */
    reg_value = sfi51_reg_RX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_MSK) >> SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_LOF_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_LOF_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOF_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_LOF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_E_set", A, value );

    /* (0x0001a00c bits 2) field RX_DESKEW_LOF_E of register PMC_SFI51_REG_RX_INT_EN */
    sfi51_reg_RX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_MSK,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOF_E_get", A, 1);
    /* (0x0001a00c bits 2) field RX_DESKEW_LOF_E of register PMC_SFI51_REG_RX_INT_EN */
    reg_value = sfi51_reg_RX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_MSK) >> SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_OOF_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_OOF_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOF_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_OOF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_E_set", A, value );

    /* (0x0001a00c bits 1) field RX_DESKEW_OOF_E of register PMC_SFI51_REG_RX_INT_EN */
    sfi51_reg_RX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_MSK,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOF_E_get", A, 1);
    /* (0x0001a00c bits 1) field RX_DESKEW_OOF_E of register PMC_SFI51_REG_RX_INT_EN */
    reg_value = sfi51_reg_RX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_MSK) >> SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_LOS_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_LOS_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOS_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_LOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_E_set", A, value );

    /* (0x0001a00c bits 0) field RX_DESKEW_LOS_E of register PMC_SFI51_REG_RX_INT_EN */
    sfi51_reg_RX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_MSK,
                                   SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOS_E_get", A, 1);
    /* (0x0001a00c bits 0) field RX_DESKEW_LOS_E of register PMC_SFI51_REG_RX_INT_EN */
    reg_value = sfi51_reg_RX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_MSK) >> SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_RX_DESKEW_OOA_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_OOA_E_set( sfi51_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOA_E_set", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_OOA_E_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_E_set", A, value );

    /* (0x0001a018 bits 15:0) field RX_DESKEW_OOA_E of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG */
    sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_MSK,
                                                  SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF,
                                                  value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_E_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOA_E_get", A, 1);
    /* (0x0001a018 bits 15:0) field RX_DESKEW_OOA_E of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_MSK) >> SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_RX_DESKEW_OOA_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_RX_DESKEW_OOA_E_set( sfi51_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_E_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_E_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_E_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a018 bits 15:0) field RX_DESKEW_OOA_E of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG */
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
        /* (0x0001a018 bits 15:0) field RX_DESKEW_OOA_E of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG */
        sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF + subfield_offset),
                                                      SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_E_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_E_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_E_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_E_get", stop_bit, 15 );
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
    /* (0x0001a018 bits 15:0) field RX_DESKEW_OOA_E of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_MSK)
                  >> SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_MSK, SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_field_RX_CHN_BIT_ERR_E_set( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_CHN_BIT_ERR_E_set( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_CHN_BIT_ERR_E_set", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_E_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_E_set", A, value );

    /* (0x0001a024 bits 15:0) field RX_CHN_BIT_ERR_E of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG */
    sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_MSK,
                                                       SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF,
                                                       value);
}

static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_E_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_E_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_CHN_BIT_ERR_E_get", A, 1);
    /* (0x0001a024 bits 15:0) field RX_CHN_BIT_ERR_E of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_MSK) >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_RX_CHN_BIT_ERR_E_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_RX_CHN_BIT_ERR_E_set( sfi51_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_E_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_E_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_E_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a024 bits 15:0) field RX_CHN_BIT_ERR_E of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG */
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
        /* (0x0001a024 bits 15:0) field RX_CHN_BIT_ERR_E of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG */
        sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF + subfield_offset),
                                                           SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_E_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_E_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_E_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_E_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_E_get", stop_bit, 15 );
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
    /* (0x0001a024 bits 15:0) field RX_CHN_BIT_ERR_E of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_MSK)
                  >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_MSK, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void sfi51_field_TX_FIFO_UNFL_E_set( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_FIFO_UNFL_E_set( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_UNFL_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_FIFO_UNFL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_E_set", A, value );

    /* (0x0001a034 bits 1) field TX_FIFO_UNFL_E of register PMC_SFI51_REG_TX_INT_EN */
    sfi51_reg_TX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_MSK,
                                   SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_E_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_E_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_UNFL_E_get", A, 1);
    /* (0x0001a034 bits 1) field TX_FIFO_UNFL_E of register PMC_SFI51_REG_TX_INT_EN */
    reg_value = sfi51_reg_TX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_MSK) >> SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_E_get", A, value );

    return value;
}
static INLINE void sfi51_field_TX_FIFO_OVFL_E_set( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_FIFO_OVFL_E_set( sfi51_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_OVFL_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_FIFO_OVFL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_E_set", A, value );

    /* (0x0001a034 bits 0) field TX_FIFO_OVFL_E of register PMC_SFI51_REG_TX_INT_EN */
    sfi51_reg_TX_INT_EN_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_MSK,
                                   SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_OFF,
                                   value);
}

static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_E_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_E_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_OVFL_E_get", A, 1);
    /* (0x0001a034 bits 0) field TX_FIFO_OVFL_E of register PMC_SFI51_REG_TX_INT_EN */
    reg_value = sfi51_reg_TX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_MSK) >> SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear", A, value );

    /* (0x0001a008 bits 4) field RX_DESKEW_FIFO_OVFL_I of register PMC_SFI51_REG_RX_INT_REG */
    sfi51_reg_RX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_MSK,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_get", A, 1);
    /* (0x0001a008 bits 4) field RX_DESKEW_FIFO_OVFL_I of register PMC_SFI51_REG_RX_INT_REG */
    reg_value = sfi51_reg_RX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_MSK) >> SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_I_poll", A, value );

    /* (0x0001a008 bits 4) field RX_DESKEW_FIFO_OVFL_I of register PMC_SFI51_REG_RX_INT_REG */
    return sfi51_reg_RX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_MSK,
                                      (value<<SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear", A, value );

    /* (0x0001a008 bits 3) field RX_DESKEW_FIFO_UNFL_I of register PMC_SFI51_REG_RX_INT_REG */
    sfi51_reg_RX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_MSK,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_get", A, 1);
    /* (0x0001a008 bits 3) field RX_DESKEW_FIFO_UNFL_I of register PMC_SFI51_REG_RX_INT_REG */
    reg_value = sfi51_reg_RX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_MSK) >> SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_I_poll", A, value );

    /* (0x0001a008 bits 3) field RX_DESKEW_FIFO_UNFL_I of register PMC_SFI51_REG_RX_INT_REG */
    return sfi51_reg_RX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_MSK,
                                      (value<<SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_DESKEW_LOF_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_LOF_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOF_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_LOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_I_set_to_clear", A, value );

    /* (0x0001a008 bits 2) field RX_DESKEW_LOF_I of register PMC_SFI51_REG_RX_INT_REG */
    sfi51_reg_RX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_MSK,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOF_I_get", A, 1);
    /* (0x0001a008 bits 2) field RX_DESKEW_LOF_I of register PMC_SFI51_REG_RX_INT_REG */
    reg_value = sfi51_reg_RX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_MSK) >> SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_I_poll", A, value );

    /* (0x0001a008 bits 2) field RX_DESKEW_LOF_I of register PMC_SFI51_REG_RX_INT_REG */
    return sfi51_reg_RX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_MSK,
                                      (value<<SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_DESKEW_OOF_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_OOF_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOF_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_OOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_I_set_to_clear", A, value );

    /* (0x0001a008 bits 1) field RX_DESKEW_OOF_I of register PMC_SFI51_REG_RX_INT_REG */
    sfi51_reg_RX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_MSK,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOF_I_get", A, 1);
    /* (0x0001a008 bits 1) field RX_DESKEW_OOF_I of register PMC_SFI51_REG_RX_INT_REG */
    reg_value = sfi51_reg_RX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_MSK) >> SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_I_poll", A, value );

    /* (0x0001a008 bits 1) field RX_DESKEW_OOF_I of register PMC_SFI51_REG_RX_INT_REG */
    return sfi51_reg_RX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_MSK,
                                      (value<<SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_DESKEW_LOS_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_LOS_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOS_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_I_set_to_clear", A, value );

    /* (0x0001a008 bits 0) field RX_DESKEW_LOS_I of register PMC_SFI51_REG_RX_INT_REG */
    sfi51_reg_RX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_MSK,
                                                    SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOS_I_get", A, 1);
    /* (0x0001a008 bits 0) field RX_DESKEW_LOS_I of register PMC_SFI51_REG_RX_INT_REG */
    reg_value = sfi51_reg_RX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_MSK) >> SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_I_poll", A, value );

    /* (0x0001a008 bits 0) field RX_DESKEW_LOS_I of register PMC_SFI51_REG_RX_INT_REG */
    return sfi51_reg_RX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_MSK,
                                      (value<<SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_DESKEW_OOA_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_DESKEW_OOA_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOA_I_set_to_clear", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_DESKEW_OOA_I_set_to_clear", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_I_set_to_clear", A, value );

    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
    sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK,
                                                               SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF,
                                                               value);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_I_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOA_I_get", A, 1);
    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK) >> SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_I_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
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
        /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
        sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   subfield_mask << (SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF + subfield_offset),
                                                                   SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_I_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_I_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_I_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_I_get", stop_bit, 15 );
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
    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK)
                  >> SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK, SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_I_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_I_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_I_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_I_poll", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
        return sfi51_reg_RX_DESKEW_OOA_INT_REG_poll( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF + subfield_offset),
                                                     value << (SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_I_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_I_poll", A, value );

    /* (0x0001a014 bits 15:0) field RX_DESKEW_OOA_I of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG */
    return sfi51_reg_RX_DESKEW_OOA_INT_REG_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK,
                                                 (value<<SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear", A, value );

    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
    sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK,
                                                                    SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF,
                                                                    value);
}

static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_I_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_I_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_CHN_BIT_ERR_I_get", A, 1);
    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK) >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_I_get", A, value );

    return value;
}
static INLINE void sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
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
        /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
        sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        A,
                                                                        subfield_mask << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF + subfield_offset),
                                                                        SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_I_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_I_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_I_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_get", stop_bit, 15 );
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
    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK)
                  >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_I_poll( sfi51_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_I_poll( sfi51_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_I_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_I_poll", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
        return sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF + subfield_offset),
                                                          value << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_I_poll( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_I_poll( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_I_poll", A, value );

    /* (0x0001a020 bits 15:0) field RX_CHN_BIT_ERR_I of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG */
    return sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK,
                                                      (value<<SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_TX_FIFO_UNFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_FIFO_UNFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_UNFL_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_FIFO_UNFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_I_set_to_clear", A, value );

    /* (0x0001a030 bits 1) field TX_FIFO_UNFL_I of register PMC_SFI51_REG_TX_INT_REG */
    sfi51_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_MSK,
                                                    SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_I_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_I_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_UNFL_I_get", A, 1);
    /* (0x0001a030 bits 1) field TX_FIFO_UNFL_I of register PMC_SFI51_REG_TX_INT_REG */
    reg_value = sfi51_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_MSK) >> SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_I_poll", A, value );

    /* (0x0001a030 bits 1) field TX_FIFO_UNFL_I of register PMC_SFI51_REG_TX_INT_REG */
    return sfi51_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_MSK,
                                      (value<<SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void sfi51_field_TX_FIFO_OVFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sfi51_field_TX_FIFO_OVFL_I_set_to_clear( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_OVFL_I_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfi51_field_TX_FIFO_OVFL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_I_set_to_clear", A, value );

    /* (0x0001a030 bits 0) field TX_FIFO_OVFL_I of register PMC_SFI51_REG_TX_INT_REG */
    sfi51_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_MSK,
                                                    SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_OFF,
                                                    value);
}

static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_I_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_I_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_OVFL_I_get", A, 1);
    /* (0x0001a030 bits 0) field TX_FIFO_OVFL_I of register PMC_SFI51_REG_TX_INT_REG */
    reg_value = sfi51_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_MSK) >> SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_I_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_I_poll", A, value );

    /* (0x0001a030 bits 0) field TX_FIFO_OVFL_I of register PMC_SFI51_REG_TX_INT_REG */
    return sfi51_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_MSK,
                                      (value<<SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_OFF),
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
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_OVFL_V_get", A, 1);
    /* (0x0001a010 bits 4) field RX_DESKEW_FIFO_OVFL_V of register PMC_SFI51_REG_RX_INT_STAT */
    reg_value = sfi51_reg_RX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_MSK) >> SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_OVFL_V_poll", A, value );

    /* (0x0001a010 bits 4) field RX_DESKEW_FIFO_OVFL_V of register PMC_SFI51_REG_RX_INT_STAT */
    return sfi51_reg_RX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_MSK,
                                       (value<<SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_FIFO_UNFL_V_get", A, 1);
    /* (0x0001a010 bits 3) field RX_DESKEW_FIFO_UNFL_V of register PMC_SFI51_REG_RX_INT_STAT */
    reg_value = sfi51_reg_RX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_MSK) >> SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_FIFO_UNFL_V_poll", A, value );

    /* (0x0001a010 bits 3) field RX_DESKEW_FIFO_UNFL_V of register PMC_SFI51_REG_RX_INT_STAT */
    return sfi51_reg_RX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_MSK,
                                       (value<<SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOF_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOF_V_get", A, 1);
    /* (0x0001a010 bits 2) field RX_DESKEW_LOF_V of register PMC_SFI51_REG_RX_INT_STAT */
    reg_value = sfi51_reg_RX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_MSK) >> SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOF_V_poll", A, value );

    /* (0x0001a010 bits 2) field RX_DESKEW_LOF_V of register PMC_SFI51_REG_RX_INT_STAT */
    return sfi51_reg_RX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_MSK,
                                       (value<<SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOF_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOF_V_get", A, 1);
    /* (0x0001a010 bits 1) field RX_DESKEW_OOF_V of register PMC_SFI51_REG_RX_INT_STAT */
    reg_value = sfi51_reg_RX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_MSK) >> SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOF_V_poll", A, value );

    /* (0x0001a010 bits 1) field RX_DESKEW_OOF_V of register PMC_SFI51_REG_RX_INT_STAT */
    return sfi51_reg_RX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_MSK,
                                       (value<<SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_LOS_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_LOS_V_get", A, 1);
    /* (0x0001a010 bits 0) field RX_DESKEW_LOS_V of register PMC_SFI51_REG_RX_INT_STAT */
    reg_value = sfi51_reg_RX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_MSK) >> SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_LOS_V_poll", A, value );

    /* (0x0001a010 bits 0) field RX_DESKEW_LOS_V of register PMC_SFI51_REG_RX_INT_STAT */
    return sfi51_reg_RX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_MSK,
                                       (value<<SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_DESKEW_OOA_V_get( sfi51_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_DESKEW_OOA_V_get", A, 1);
    /* (0x0001a01c bits 15:0) field RX_DESKEW_OOA_V of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_MSK) >> SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_V_get", A, value );

    return value;
}
static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_DESKEW_OOA_V_get( sfi51_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_V_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_V_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_V_get", stop_bit, 15 );
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
    /* (0x0001a01c bits 15:0) field RX_DESKEW_OOA_V of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG */
    reg_value = sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_MSK)
                  >> SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_MSK, SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_V_poll( sfi51_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_DESKEW_OOA_V_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_DESKEW_OOA_V_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_DESKEW_OOA_V_poll", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_DESKEW_OOA_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a01c bits 15:0) field RX_DESKEW_OOA_V of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001a01c bits 15:0) field RX_DESKEW_OOA_V of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG */
        return sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF + subfield_offset),
                                                          value << (SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_V_poll( sfi51_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_DESKEW_OOA_V_poll", A, value );

    /* (0x0001a01c bits 15:0) field RX_DESKEW_OOA_V of register PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG */
    return sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_MSK,
                                                      (value<<SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_V_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_RX_CHN_BIT_ERR_V_get( sfi51_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_RX_CHN_BIT_ERR_V_get", A, 1);
    /* (0x0001a028 bits 15:0) field RX_CHN_BIT_ERR_V of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_MSK) >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_V_get", A, value );

    return value;
}
static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_V_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_range_RX_CHN_BIT_ERR_V_get( sfi51_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_V_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_V_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_V_get", stop_bit, 15 );
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
    /* (0x0001a028 bits 15:0) field RX_CHN_BIT_ERR_V of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG */
    reg_value = sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_MSK)
                  >> SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_MSK, SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_V_poll( sfi51_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_V_poll( sfi51_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_range_RX_CHN_BIT_ERR_V_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfi51_field_range_RX_CHN_BIT_ERR_V_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfi51_field_range_RX_CHN_BIT_ERR_V_poll", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfi51_field_range_RX_CHN_BIT_ERR_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001a028 bits 15:0) field RX_CHN_BIT_ERR_V of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001a028 bits 15:0) field RX_CHN_BIT_ERR_V of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG */
        return sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF + subfield_offset),
                                                              value << (SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF + subfield_offset),
                                                              cmp,
                                                              max_count,
                                                              num_failed_polls,
                                                              delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_V_poll( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_V_poll( sfi51_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_RX_CHN_BIT_ERR_V_poll", A, value );

    /* (0x0001a028 bits 15:0) field RX_CHN_BIT_ERR_V of register PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG */
    return sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_MSK,
                                                          (value<<SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_V_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_UNFL_V_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_UNFL_V_get", A, 1);
    /* (0x0001a038 bits 1) field TX_FIFO_UNFL_V of register PMC_SFI51_REG_TX_INT_STAT */
    reg_value = sfi51_reg_TX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_MSK) >> SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_TX_FIFO_UNFL_V_poll", A, value );

    /* (0x0001a038 bits 1) field TX_FIFO_UNFL_V of register PMC_SFI51_REG_TX_INT_STAT */
    return sfi51_reg_TX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_MSK,
                                       (value<<SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_V_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfi51_field_TX_FIFO_OVFL_V_get( sfi51_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfi51_field_TX_FIFO_OVFL_V_get", A, 1);
    /* (0x0001a038 bits 0) field TX_FIFO_OVFL_V of register PMC_SFI51_REG_TX_INT_STAT */
    reg_value = sfi51_reg_TX_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_MSK) >> SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_V_poll( sfi51_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfi51_field_TX_FIFO_OVFL_V_poll", A, value );

    /* (0x0001a038 bits 0) field TX_FIFO_OVFL_V of register PMC_SFI51_REG_TX_INT_STAT */
    return sfi51_reg_TX_INT_STAT_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_MSK,
                                       (value<<SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SFI51_IO_INLINE_H */
