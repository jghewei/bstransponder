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
 *     and register accessor functions for the copi block
 *****************************************************************************/
#ifndef _COPI_IO_INLINE_H
#define _COPI_IO_INLINE_H

#include "sifd_api.h"
#include "copi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define COPI_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for copi
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
    sifd_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} copi_buffer_t;
static INLINE void copi_buffer_init( copi_buffer_t *b_ptr,
                                     sifd_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void copi_buffer_init( copi_buffer_t *b_ptr,
                                     sifd_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x4000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "copi_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void copi_buffer_flush( copi_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void copi_buffer_flush( copi_buffer_t *b_ptr )
{
    IOLOG( "copi_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 copi_reg_read( copi_buffer_t *b_ptr,
                                    sifd_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 copi_reg_read( copi_buffer_t *b_ptr,
                                    sifd_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x4000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg);
    return value;
}
static INLINE void copi_reg_write( copi_buffer_t *b_ptr,
                                   sifd_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_write( copi_buffer_t *b_ptr,
                                   sifd_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x4000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                         value);
}

static INLINE void copi_field_set( copi_buffer_t *b_ptr,
                                   sifd_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_set( copi_buffer_t *b_ptr,
                                   sifd_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x4000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void copi_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x4000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                         value<<ofs);
}

static INLINE void copi_burst_read( copi_buffer_t *b_ptr,
                                    sifd_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void copi_burst_read( copi_buffer_t *b_ptr,
                                    sifd_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x4000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                          len,
                          value);
}

static INLINE void copi_burst_write( copi_buffer_t *b_ptr,
                                     sifd_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void copi_burst_write( copi_buffer_t *b_ptr,
                                     sifd_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x4000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE copi_poll( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_poll( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x4000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for copi
 * ==================================================================================
 */

static INLINE void copi_reg_SOFT_RESET_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SOFT_RESET_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_SOFT_RESET_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_SOFT_RESET,
                    value);
}

static INLINE void copi_reg_SOFT_RESET_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SOFT_RESET_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_SOFT_RESET_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_SOFT_RESET,
                    mask,
                    PMC_COPI_REG_SOFT_RESET_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_SOFT_RESET_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_SOFT_RESET_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_SOFT_RESET);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_SOFT_RESET_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_CFG_write( copi_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_CFG_write( copi_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_CFG_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_CFG,
                    value);
}

static INLINE void copi_reg_CFG_field_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_CFG_field_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_CFG_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_CFG,
                    mask,
                    PMC_COPI_REG_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_CFG_read( copi_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_CFG_read( copi_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_TX_INTF_CFG_write( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_CFG_write( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_INTF_CFG_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_INTF_CFG,
                    value);
}

static INLINE void copi_reg_TX_INTF_CFG_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_CFG_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_INTF_CFG_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_INTF_CFG,
                    mask,
                    PMC_COPI_REG_TX_INTF_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_INTF_CFG_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_INTF_CFG_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_TX_INTF_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TX_INTF_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_INTF_CFG_write( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_CFG_write( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_INTF_CFG_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_INTF_CFG,
                    value);
}

static INLINE void copi_reg_RX_INTF_CFG_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_CFG_field_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_INTF_CFG_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_INTF_CFG,
                    mask,
                    PMC_COPI_REG_RX_INTF_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_INTF_CFG_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_INTF_CFG_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_RX_INTF_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_INTF_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_FIFO_TH_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_TH_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_FIFO_TH_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_FIFO_TH,
                    value);
}

static INLINE void copi_reg_RX_FIFO_TH_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_TH_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_FIFO_TH_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_FIFO_TH,
                    mask,
                    PMC_COPI_REG_RX_FIFO_TH_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_FIFO_TH_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_FIFO_TH_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_RX_FIFO_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_FIFO_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_TX_FLOW_CTRL_CFG_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FLOW_CTRL_CFG_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_FLOW_CTRL_CFG_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_FLOW_CTRL_CFG,
                    value);
}

static INLINE void copi_reg_TX_FLOW_CTRL_CFG_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FLOW_CTRL_CFG_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_FLOW_CTRL_CFG_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_FLOW_CTRL_CFG,
                    mask,
                    PMC_COPI_REG_TX_FLOW_CTRL_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_FLOW_CTRL_CFG_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_FLOW_CTRL_CFG_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_TX_FLOW_CTRL_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TX_FLOW_CTRL_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_FLOW_CTRL_CFG_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FLOW_CTRL_CFG_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_FLOW_CTRL_CFG_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_FLOW_CTRL_CFG,
                    value);
}

static INLINE void copi_reg_RX_FLOW_CTRL_CFG_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FLOW_CTRL_CFG_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_FLOW_CTRL_CFG_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_FLOW_CTRL_CFG,
                    mask,
                    PMC_COPI_REG_RX_FLOW_CTRL_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_FLOW_CTRL_CFG_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_FLOW_CTRL_CFG_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_RX_FLOW_CTRL_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_FLOW_CTRL_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_DPI_LB_write( copi_buffer_t *b_ptr,
                                          sifd_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_DPI_LB_write( copi_buffer_t *b_ptr,
                                          sifd_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_DPI_LB_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_DPI_LB,
                    value);
}

static INLINE void copi_reg_DPI_LB_field_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_DPI_LB_field_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_DPI_LB_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_DPI_LB,
                    mask,
                    PMC_COPI_REG_DPI_LB_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_DPI_LB_read( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_DPI_LB_read( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_DPI_LB);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_DPI_LB_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_ODUK_2B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_2B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ODUK_2B_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_ODUK_2B_LK(N),
                    value);
}

static INLINE void copi_reg_RX_ODUK_2B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_2B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ODUK_2B_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_RX_ODUK_2B_LK(N),
                    mask,
                    PMC_COPI_REG_RX_ODUK_2B_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ODUK_2B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_2B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_RX_ODUK_2B_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_RX_ODUK_2B_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_TX_FIFO_TH_array_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_TH_array_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_FIFO_TH_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_FIFO_TH(N),
                    value);
}

static INLINE void copi_reg_TX_FIFO_TH_array_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_TH_array_field_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_FIFO_TH_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_COPI_REG_TX_FIFO_TH(N),
                    mask,
                    PMC_COPI_REG_TX_FIFO_TH_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_FIFO_TH_array_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_FIFO_TH_array_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_COPI_REG_TX_FIFO_TH(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_FIFO_TH_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_RX_ODUK_H2B_CAM_ERR_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H2B_CAM_ERR_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ODUK_H2B_CAM_ERR_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR,
                    value);
}

static INLINE void copi_reg_RX_ODUK_H2B_CAM_ERR_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H2B_CAM_ERR_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ODUK_H2B_CAM_ERR_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR,
                    mask,
                    PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ODUK_H2B_CAM_ERR_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_H2B_CAM_ERR_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_ODUK_H2B_CAM_ERR_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_ODUK_H2B_CMPR_ERR_write( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H2B_CMPR_ERR_write( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ODUK_H2B_CMPR_ERR_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR,
                    value);
}

static INLINE void copi_reg_RX_ODUK_H2B_CMPR_ERR_field_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H2B_CMPR_ERR_field_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ODUK_H2B_CMPR_ERR_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR,
                    mask,
                    PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ODUK_H2B_CMPR_ERR_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_H2B_CMPR_ERR_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_ODUK_H2B_CMPR_ERR_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_ODUK_H4B_CMPR_ERR0_write( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H4B_CMPR_ERR0_write( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ODUK_H4B_CMPR_ERR0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0,
                    value);
}

static INLINE void copi_reg_RX_ODUK_H4B_CMPR_ERR0_field_set( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_H4B_CMPR_ERR0_field_set( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ODUK_H4B_CMPR_ERR0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0,
                    mask,
                    PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ODUK_H4B_CMPR_ERR0_read( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_H4B_CMPR_ERR0_read( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_ODUK_H4B_CMPR_ERR0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_TX_FIFO_INT_E0_write( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_INT_E0_write( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_FIFO_INT_E0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_FIFO_INT_E0,
                    value);
}

static INLINE void copi_reg_TX_FIFO_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_FIFO_INT_E0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_FIFO_INT_E0,
                    mask,
                    PMC_COPI_REG_TX_FIFO_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_FIFO_INT_E0_read( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_FIFO_INT_E0_read( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_FIFO_INT_E0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TX_FIFO_INT_E0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_FIFO_INT_E0_write( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_INT_E0_write( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_FIFO_INT_E0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_FIFO_INT_E0,
                    value);
}

static INLINE void copi_reg_RX_FIFO_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_FIFO_INT_E0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_FIFO_INT_E0,
                    mask,
                    PMC_COPI_REG_RX_FIFO_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_FIFO_INT_E0_read( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_FIFO_INT_E0_read( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_FIFO_INT_E0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_FIFO_INT_E0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_SERDES_RX_LOS_INT_E0_write( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SERDES_RX_LOS_INT_E0_write( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_SERDES_RX_LOS_INT_E0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_SERDES_RX_LOS_INT_E0,
                    value);
}

static INLINE void copi_reg_SERDES_RX_LOS_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SERDES_RX_LOS_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_SERDES_RX_LOS_INT_E0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_SERDES_RX_LOS_INT_E0,
                    mask,
                    PMC_COPI_REG_SERDES_RX_LOS_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_SERDES_RX_LOS_INT_E0_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_SERDES_RX_LOS_INT_E0_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_SERDES_RX_LOS_INT_E0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_SERDES_RX_LOS_INT_E0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_RX_INTF_ERR_INT_E0_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_ERR_INT_E0_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_INTF_ERR_INT_E0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_INTF_ERR_INT_E0,
                    value);
}

static INLINE void copi_reg_RX_INTF_ERR_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_ERR_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_INTF_ERR_INT_E0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_INTF_ERR_INT_E0,
                    mask,
                    PMC_COPI_REG_RX_INTF_ERR_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_INTF_ERR_INT_E0_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_INTF_ERR_INT_E0_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_INTF_ERR_INT_E0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_INTF_ERR_INT_E0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_COM_INT_E0_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_COM_INT_E0_write( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_COM_INT_E0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_COM_INT_E0,
                    value);
}

static INLINE void copi_reg_COM_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_COM_INT_E0_field_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_COM_INT_E0_field_set", A, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_COM_INT_E0,
                    mask,
                    PMC_COPI_REG_COM_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_COM_INT_E0_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_COM_INT_E0_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_COM_INT_E0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_COM_INT_E0_read", reg_value, A);
    return reg_value;
}

static INLINE void copi_reg_TX_INTF_ERR_INT_E0_array_write( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_ERR_INT_E0_array_write( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_INTF_ERR_INT_E0_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_INTF_ERR_INT_E0(N),
                    value);
}

static INLINE void copi_reg_TX_INTF_ERR_INT_E0_array_field_set( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_ERR_INT_E0_array_field_set( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_INTF_ERR_INT_E0_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_INTF_ERR_INT_E0(N),
                    mask,
                    PMC_COPI_REG_TX_INTF_ERR_INT_E0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_INTF_ERR_INT_E0_array_read( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_INTF_ERR_INT_E0_array_read( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_INTF_ERR_INT_E0(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_INTF_ERR_INT_E0_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 copi_reg_TOP_SUMMARY_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TOP_SUMMARY_read( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TOP_SUMMARY);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TOP_SUMMARY_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TOP_SUMMARY_poll( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TOP_SUMMARY_poll( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_TOP_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_TOP_SUMMARY,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_TX_FIFO_INT0_write( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_INT0_write( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_FIFO_INT0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_FIFO_INT0,
                    value);
}

static INLINE void copi_reg_TX_FIFO_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_FIFO_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_FIFO_INT0_action_on_write_field_set", A, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_TX_FIFO_INT0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_TX_FIFO_INT0_read( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_FIFO_INT0_read( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_FIFO_INT0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TX_FIFO_INT0_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_TX_FIFO_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_TX_FIFO_INT0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_RX_FIFO_INT0_write( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_INT0_write( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_FIFO_INT0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_FIFO_INT0,
                    value);
}

static INLINE void copi_reg_RX_FIFO_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_FIFO_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_FIFO_INT0_action_on_write_field_set", A, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_RX_FIFO_INT0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_RX_FIFO_INT0_read( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_FIFO_INT0_read( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_FIFO_INT0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_FIFO_INT0_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_RX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_RX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_RX_FIFO_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_RX_FIFO_INT0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_SERDES_RX_LOS_INT0_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SERDES_RX_LOS_INT0_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_SERDES_RX_LOS_INT0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_SERDES_RX_LOS_INT0,
                    value);
}

static INLINE void copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set", A, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_SERDES_RX_LOS_INT0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_SERDES_RX_LOS_INT0_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_SERDES_RX_LOS_INT0_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_SERDES_RX_LOS_INT0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_SERDES_RX_LOS_INT0_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_SERDES_RX_LOS_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_SERDES_RX_LOS_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_SERDES_RX_LOS_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_SERDES_RX_LOS_INT0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_RX_INTF_ERR_INT0_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_ERR_INT0_write( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_INTF_ERR_INT0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_INTF_ERR_INT0,
                    value);
}

static INLINE void copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set", A, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_RX_INTF_ERR_INT0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_RX_INTF_ERR_INT0_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_INTF_ERR_INT0_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_INTF_ERR_INT0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_INTF_ERR_INT0_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_RX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_RX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_RX_INTF_ERR_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_RX_INTF_ERR_INT0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_TIP_INT0_write( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TIP_INT0_write( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TIP_INT0_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TIP_INT0,
                    value);
}

static INLINE void copi_reg_TIP_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TIP_INT0_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TIP_INT0_action_on_write_field_set", A, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_TIP_INT0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_TIP_INT0_read( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TIP_INT0_read( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TIP_INT0);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_TIP_INT0_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TIP_INT0_poll( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TIP_INT0_poll( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_TIP_INT0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_TIP_INT0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_TX_INTF_ERR_INT0_array_write( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_ERR_INT0_array_write( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_INTF_ERR_INT0_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_INTF_ERR_INT0(N),
                    value);
}

static INLINE void copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set", A, N, mask, ofs, value );
    copi_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_COPI_REG_TX_INTF_ERR_INT0(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 copi_reg_TX_INTF_ERR_INT0_array_read( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_INTF_ERR_INT0_array_read( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_INTF_ERR_INT0(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_INTF_ERR_INT0_array_read", reg_value, A, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TX_INTF_ERR_INT0_array_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_reg_TX_INTF_ERR_INT0_array_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "copi_reg_TX_INTF_ERR_INT0_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return copi_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_COPI_REG_TX_INTF_ERR_INT0(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void copi_reg_TX_ODUK_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_ODUK_ARB_CH_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_ARB_CH_LK(N),
                    value);
}

static INLINE void copi_reg_TX_ODUK_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_ODUK_ARB_CH_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_ARB_CH_LK(N),
                    mask,
                    PMC_COPI_REG_TX_ODUK_ARB_CH_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_ODUK_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_ODUK_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_ODUK_ARB_CH_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_ODUK_ARB_CH_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_TX_ODUK_4B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_4B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_ODUK_4B_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_4B_LK(N),
                    value);
}

static INLINE void copi_reg_TX_ODUK_4B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_4B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_ODUK_4B_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_4B_LK(N),
                    mask,
                    PMC_COPI_REG_TX_ODUK_4B_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_ODUK_4B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_ODUK_4B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_ODUK_4B_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_ODUK_4B_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_TX_ODUK_2B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_2B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_ODUK_2B_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_2B_LK(N),
                    value);
}

static INLINE void copi_reg_TX_ODUK_2B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_ODUK_2B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_ODUK_2B_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_ODUK_2B_LK(N),
                    mask,
                    PMC_COPI_REG_TX_ODUK_2B_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_ODUK_2B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_ODUK_2B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_ODUK_2B_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_ODUK_2B_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_RX_ODUK_4B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_4B_LK_array_write( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ODUK_4B_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_4B_LK(N),
                    value);
}

static INLINE void copi_reg_RX_ODUK_4B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ODUK_4B_LK_array_field_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ODUK_4B_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ODUK_4B_LK(N),
                    mask,
                    PMC_COPI_REG_RX_ODUK_4B_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ODUK_4B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_4B_LK_array_read( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ODUK_4B_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_RX_ODUK_4B_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_TX_CPB_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_CPB_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_TX_CPB_ARB_CH_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_CPB_ARB_CH_LK(N),
                    value);
}

static INLINE void copi_reg_TX_CPB_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_TX_CPB_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_TX_CPB_ARB_CH_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_TX_CPB_ARB_CH_LK(N),
                    mask,
                    PMC_COPI_REG_TX_CPB_ARB_CH_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_TX_CPB_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_CPB_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_CPB_ARB_CH_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_CPB_ARB_CH_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void copi_reg_RX_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ARB_CH_LK_array_write( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "copi_reg_RX_ARB_CH_LK_array_write", value );
    copi_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ARB_CH_LK(N),
                    value);
}

static INLINE void copi_reg_RX_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_reg_RX_ARB_CH_LK_array_field_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "copi_reg_RX_ARB_CH_LK_array_field_set", A, N, mask, ofs, value );
    copi_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_COPI_REG_RX_ARB_CH_LK(N),
                    mask,
                    PMC_COPI_REG_RX_ARB_CH_LK_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 copi_reg_RX_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ARB_CH_LK_array_read( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ARB_CH_LK(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_RX_ARB_CH_LK_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 copi_reg_RX_ERR_CNT_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ERR_CNT_read( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 copi_reg_RX_ODUK_H4B_CMPR_ERR1_read( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_ODUK_H4B_CMPR_ERR1_read( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR1);

    IOLOG( "%s -> 0x%08x; A=%d", "copi_reg_RX_ODUK_H4B_CMPR_ERR1_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 copi_reg_TX_ERR_CNT_array_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_ERR_CNT_array_read( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_ERR_CNT_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 copi_reg_TX_DIAG_IN_CNT_array_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_TX_DIAG_IN_CNT_array_read( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_TX_DIAG_IN_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_TX_DIAG_IN_CNT_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 copi_reg_RX_DIAG_CNT_array_read( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_reg_RX_DIAG_CNT_array_read( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = copi_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_COPI_REG_RX_DIAG_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "copi_reg_RX_DIAG_CNT_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void copi_field_OOB_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_OOB_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_OOB_LOWPWR_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_OOB_LOWPWR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_OOB_LOWPWR_EN_set", A, value );

    /* (0x00004000 bits 18) field OOB_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_MSK,
                                   COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_OFF,
                                   value);
}

static INLINE UINT32 copi_field_OOB_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_OOB_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_OOB_LOWPWR_EN_get", A, 1);
    /* (0x00004000 bits 18) field OOB_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_MSK) >> COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_OOB_LOWPWR_EN_get", A, value );

    return value;
}
static INLINE void copi_field_ODUK_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_LOWPWR_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_LOWPWR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_LOWPWR_EN_set", A, value );

    /* (0x00004000 bits 17) field ODUK_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_MSK,
                                   COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_OFF,
                                   value);
}

static INLINE UINT32 copi_field_ODUK_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_LOWPWR_EN_get", A, 1);
    /* (0x00004000 bits 17) field ODUK_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_MSK) >> COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_LOWPWR_EN_get", A, value );

    return value;
}
static INLINE void copi_field_CPB_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_LOWPWR_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_LOWPWR_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_LOWPWR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_LOWPWR_EN_set", A, value );

    /* (0x00004000 bits 16) field CPB_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_MSK,
                                   COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_OFF,
                                   value);
}

static INLINE UINT32 copi_field_CPB_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_LOWPWR_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_LOWPWR_EN_get", A, 1);
    /* (0x00004000 bits 16) field CPB_LOWPWR_EN of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_MSK) >> COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_LOWPWR_EN_get", A, value );

    return value;
}
static INLINE void copi_field_DIAG_CNTR_MODE_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DIAG_CNTR_MODE_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DIAG_CNTR_MODE_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DIAG_CNTR_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DIAG_CNTR_MODE_set", A, value );

    /* (0x00004004 bits 9:8) bits 0:1 use field DIAG_CNTR_MODE of register PMC_COPI_REG_CFG */
    copi_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            COPI_REG_CFG_BIT_DIAG_CNTR_MODE_MSK,
                            COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF,
                            value);
}

static INLINE UINT32 copi_field_DIAG_CNTR_MODE_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DIAG_CNTR_MODE_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DIAG_CNTR_MODE_get", A, 1);
    /* (0x00004004 bits 9:8) bits 0:1 use field DIAG_CNTR_MODE of register PMC_COPI_REG_CFG */
    reg_value = copi_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_CFG_BIT_DIAG_CNTR_MODE_MSK) >> COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DIAG_CNTR_MODE_get", A, value );

    return value;
}
static INLINE void copi_field_range_DIAG_CNTR_MODE_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_DIAG_CNTR_MODE_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_DIAG_CNTR_MODE_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_DIAG_CNTR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_DIAG_CNTR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_DIAG_CNTR_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004004 bits 9:8) bits 0:1 use field DIAG_CNTR_MODE of register PMC_COPI_REG_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004004 bits 9:8) bits 0:1 use field DIAG_CNTR_MODE of register PMC_COPI_REG_CFG */
        copi_reg_CFG_field_set( b_ptr,
                                h_ptr,
                                A,
                                subfield_mask << (COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF + subfield_offset),
                                COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF + subfield_offset,
                                value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_DIAG_CNTR_MODE_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_DIAG_CNTR_MODE_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_DIAG_CNTR_MODE_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_DIAG_CNTR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_DIAG_CNTR_MODE_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004004 bits 9:8) bits 0:1 use field DIAG_CNTR_MODE of register PMC_COPI_REG_CFG */
    reg_value = copi_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_CFG_BIT_DIAG_CNTR_MODE_MSK)
                  >> COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_CFG_BIT_DIAG_CNTR_MODE_MSK, COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_DIAG_CNTR_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_6B_INS_EN_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_6B_INS_EN_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_6B_INS_EN_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_6B_INS_EN_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_6B_INS_EN_set", A, value );

    /* (0x00004008 bits 11:10) field TX_ODUK_6B_INS_EN of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_ODUK_6B_INS_EN_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_6B_INS_EN_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_6B_INS_EN_get", A, 1);
    /* (0x00004008 bits 11:10) field TX_ODUK_6B_INS_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_6B_INS_EN_get", A, value );

    return value;
}
static INLINE void copi_field_range_TX_ODUK_6B_INS_EN_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_ODUK_6B_INS_EN_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_6B_INS_EN_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_6B_INS_EN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_6B_INS_EN_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_6B_INS_EN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 11:10) field TX_ODUK_6B_INS_EN of register PMC_COPI_REG_TX_INTF_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 11:10) field TX_ODUK_6B_INS_EN of register PMC_COPI_REG_TX_INTF_CFG */
        copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF + subfield_offset),
                                        COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_ODUK_6B_INS_EN_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ODUK_6B_INS_EN_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_6B_INS_EN_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_6B_INS_EN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_6B_INS_EN_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004008 bits 11:10) field TX_ODUK_6B_INS_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_MSK)
                  >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_MSK, COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_6B_INS_EN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_CHEXT_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_CHEXT_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_CHEXT_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_CHEXT_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_CHEXT_EN_set", A, value );

    /* (0x00004008 bits 9) field TX_ODUK_CHEXT_EN of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_ODUK_CHEXT_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_CHEXT_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_CHEXT_EN_get", A, 1);
    /* (0x00004008 bits 9) field TX_ODUK_CHEXT_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_CHEXT_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_BURSTMAX_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_BURSTMAX_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_BURSTMAX_set", A, value );

    /* (0x00004008 bits 8:7) field TX_ODUK_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_ODUK_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_BURSTMAX_get", A, 1);
    /* (0x00004008 bits 8:7) field TX_ODUK_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_BURSTMAX_get", A, value );

    return value;
}
static INLINE void copi_field_range_TX_ODUK_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_ODUK_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_BURSTMAX_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_BURSTMAX_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_BURSTMAX_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_BURSTMAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 8:7) field TX_ODUK_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 8:7) field TX_ODUK_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
        copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF + subfield_offset),
                                        COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_ODUK_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ODUK_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_BURSTMAX_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_BURSTMAX_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_BURSTMAX_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004008 bits 8:7) field TX_ODUK_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_MSK)
                  >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_MSK, COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_BURSTMAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_INTERFACE_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_INTERFACE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_INTERFACE_EN_set", A, value );

    /* (0x00004008 bits 6) field TX_ODUK_INTERFACE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_ODUK_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_INTERFACE_EN_get", A, 1);
    /* (0x00004008 bits 6) field TX_ODUK_INTERFACE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_INTERFACE_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_PKT_MODE_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_PKT_MODE_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_PKT_MODE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_PKT_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_PKT_MODE_set", A, value );

    /* (0x00004008 bits 5) field TX_ODUK_PKT_MODE of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_ODUK_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_PKT_MODE_get", A, 1);
    /* (0x00004008 bits 5) field TX_ODUK_PKT_MODE of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_PKT_MODE_get", A, value );

    return value;
}
static INLINE void copi_field_TX_CPB_TYPE_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_TYPE_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_TYPE_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_TYPE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_CPB_TYPE_EN_set", A, value );

    /* (0x00004008 bits 4) field TX_CPB_TYPE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_CPB_TYPE_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_TYPE_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_TYPE_EN_get", A, 1);
    /* (0x00004008 bits 4) field TX_CPB_TYPE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_CPB_TYPE_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_CPB_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_BURSTMAX_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_BURSTMAX_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_CPB_BURSTMAX_set", A, value );

    /* (0x00004008 bits 3:2) field TX_CPB_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_CPB_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_BURSTMAX_get", A, 1);
    /* (0x00004008 bits 3:2) field TX_CPB_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_CPB_BURSTMAX_get", A, value );

    return value;
}
static INLINE void copi_field_range_TX_CPB_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_CPB_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_CPB_BURSTMAX_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_CPB_BURSTMAX_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_CPB_BURSTMAX_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_CPB_BURSTMAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004008 bits 3:2) field TX_CPB_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004008 bits 3:2) field TX_CPB_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
        copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF + subfield_offset),
                                        COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_CPB_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_CPB_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_CPB_BURSTMAX_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_CPB_BURSTMAX_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_CPB_BURSTMAX_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004008 bits 3:2) field TX_CPB_BURSTMAX of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_MSK)
                  >> COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_MSK, COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_CPB_BURSTMAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_CPB_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_INTERFACE_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_INTERFACE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_CPB_INTERFACE_EN_set", A, value );

    /* (0x00004008 bits 1) field TX_CPB_INTERFACE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_CPB_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_INTERFACE_EN_get", A, 1);
    /* (0x00004008 bits 1) field TX_CPB_INTERFACE_EN of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_CPB_INTERFACE_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_CPB_PKT_MODE_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_PKT_MODE_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_PKT_MODE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_PKT_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_CPB_PKT_MODE_set", A, value );

    /* (0x00004008 bits 0) field TX_CPB_PKT_MODE of register PMC_COPI_REG_TX_INTF_CFG */
    copi_reg_TX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_MSK,
                                    COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_OFF,
                                    value);
}

static INLINE UINT32 copi_field_TX_CPB_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_PKT_MODE_get", A, 1);
    /* (0x00004008 bits 0) field TX_CPB_PKT_MODE of register PMC_COPI_REG_TX_INTF_CFG */
    reg_value = copi_reg_TX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_MSK) >> COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_CPB_PKT_MODE_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_2B_MODE_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_2B_MODE_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_2B_MODE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_2B_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_2B_MODE_set", A, value );

    /* (0x00004010 bits 18) field RX_ODUK_2B_MODE of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_ODUK_2B_MODE_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_2B_MODE_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_2B_MODE_get", A, 1);
    /* (0x00004010 bits 18) field RX_ODUK_2B_MODE of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_2B_MODE_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_6B_CHK_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_6B_CHK_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_6B_CHK_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_6B_CHK_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_6B_CHK_set", A, value );

    /* (0x00004010 bits 17:16) field RX_ODUK_6B_CHK of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_ODUK_6B_CHK_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_6B_CHK_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_6B_CHK_get", A, 1);
    /* (0x00004010 bits 17:16) field RX_ODUK_6B_CHK of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_6B_CHK_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_ODUK_6B_CHK_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ODUK_6B_CHK_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_6B_CHK_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_6B_CHK_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_6B_CHK_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_6B_CHK_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 17:16) field RX_ODUK_6B_CHK of register PMC_COPI_REG_RX_INTF_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004010 bits 17:16) field RX_ODUK_6B_CHK of register PMC_COPI_REG_RX_INTF_CFG */
        copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF + subfield_offset),
                                        COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ODUK_6B_CHK_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_6B_CHK_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_6B_CHK_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_6B_CHK_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_6B_CHK_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004010 bits 17:16) field RX_ODUK_6B_CHK of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_MSK)
                  >> COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_MSK, COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_6B_CHK_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_CHEXT_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_CHEXT_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_CHEXT_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_CHEXT_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_CHEXT_EN_set", A, value );

    /* (0x00004010 bits 9) field RX_ODUK_CHEXT_EN of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_ODUK_CHEXT_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_CHEXT_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_CHEXT_EN_get", A, 1);
    /* (0x00004010 bits 9) field RX_ODUK_CHEXT_EN of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_CHEXT_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_CPB_TYPE_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_CPB_TYPE_EN_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_TYPE_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_CPB_TYPE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_CPB_TYPE_EN_set", A, value );

    /* (0x00004010 bits 4) field RX_CPB_TYPE_EN of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_CPB_TYPE_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_CPB_TYPE_EN_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_TYPE_EN_get", A, 1);
    /* (0x00004010 bits 4) field RX_CPB_TYPE_EN of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_CPB_TYPE_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_BURSTMAX_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BURSTMAX_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURSTMAX_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BURSTMAX_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BURSTMAX_set", A, value );

    /* (0x00004010 bits 3:2) field RX_BURSTMAX of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURSTMAX_get", A, 1);
    /* (0x00004010 bits 3:2) field RX_BURSTMAX of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BURSTMAX_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_BURSTMAX_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BURSTMAX_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BURSTMAX_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BURSTMAX_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BURSTMAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 3:2) field RX_BURSTMAX of register PMC_COPI_REG_RX_INTF_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004010 bits 3:2) field RX_BURSTMAX of register PMC_COPI_REG_RX_INTF_CFG */
        copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF + subfield_offset),
                                        COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_BURSTMAX_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BURSTMAX_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BURSTMAX_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BURSTMAX_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004010 bits 3:2) field RX_BURSTMAX of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_MSK)
                  >> COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_MSK, COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BURSTMAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_INTERFACE_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_INTERFACE_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_INTERFACE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_INTERFACE_EN_set", A, value );

    /* (0x00004010 bits 1) field RX_INTERFACE_EN of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_INTERFACE_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_INTERFACE_EN_get", A, 1);
    /* (0x00004010 bits 1) field RX_INTERFACE_EN of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_INTERFACE_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_PKT_MODE_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_PKT_MODE_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_MODE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_PKT_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_PKT_MODE_set", A, value );

    /* (0x00004010 bits 0) field RX_PKT_MODE of register PMC_COPI_REG_RX_INTF_CFG */
    copi_reg_RX_INTF_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_MSK,
                                    COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_OFF,
                                    value);
}

static INLINE UINT32 copi_field_RX_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_PKT_MODE_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_MODE_get", A, 1);
    /* (0x00004010 bits 0) field RX_PKT_MODE of register PMC_COPI_REG_RX_INTF_CFG */
    reg_value = copi_reg_RX_INTF_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_MSK) >> COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_PKT_MODE_get", A, value );

    return value;
}
static INLINE void copi_field_RX_H_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_H_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_H_WM_set", A, 1);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_H_WM_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_H_WM_set", A, value );

    /* (0x0000401c bits 24:16) bits 0:8 use field RX_H_WM of register PMC_COPI_REG_RX_FIFO_TH */
    copi_reg_RX_FIFO_TH_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_MSK,
                                   COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF,
                                   value);
}

static INLINE UINT32 copi_field_RX_H_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_H_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_H_WM_get", A, 1);
    /* (0x0000401c bits 24:16) bits 0:8 use field RX_H_WM of register PMC_COPI_REG_RX_FIFO_TH */
    reg_value = copi_reg_RX_FIFO_TH_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_MSK) >> COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_H_WM_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_H_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_H_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_H_WM_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_H_WM_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_H_WM_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_H_WM_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000401c bits 24:16) bits 0:8 use field RX_H_WM of register PMC_COPI_REG_RX_FIFO_TH */
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
        if (stop_bit < 8) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 8;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000401c bits 24:16) bits 0:8 use field RX_H_WM of register PMC_COPI_REG_RX_FIFO_TH */
        copi_reg_RX_FIFO_TH_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       subfield_mask << (COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF + subfield_offset),
                                       COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_H_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_H_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_H_WM_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_H_WM_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_H_WM_get", stop_bit, 8 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 8) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 8;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000401c bits 24:16) bits 0:8 use field RX_H_WM of register PMC_COPI_REG_RX_FIFO_TH */
    reg_value = copi_reg_RX_FIFO_TH_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_MSK)
                  >> COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_MSK, COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_H_WM_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_L_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_L_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_L_WM_set", A, 1);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_L_WM_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_L_WM_set", A, value );

    /* (0x0000401c bits 8:0) bits 0:8 use field RX_L_WM of register PMC_COPI_REG_RX_FIFO_TH */
    copi_reg_RX_FIFO_TH_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_MSK,
                                   COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF,
                                   value);
}

static INLINE UINT32 copi_field_RX_L_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_L_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_L_WM_get", A, 1);
    /* (0x0000401c bits 8:0) bits 0:8 use field RX_L_WM of register PMC_COPI_REG_RX_FIFO_TH */
    reg_value = copi_reg_RX_FIFO_TH_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_MSK) >> COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_L_WM_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_L_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_L_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_L_WM_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_L_WM_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_L_WM_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_L_WM_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000401c bits 8:0) bits 0:8 use field RX_L_WM of register PMC_COPI_REG_RX_FIFO_TH */
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
        if (stop_bit < 8) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 8;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000401c bits 8:0) bits 0:8 use field RX_L_WM of register PMC_COPI_REG_RX_FIFO_TH */
        copi_reg_RX_FIFO_TH_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       subfield_mask << (COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF + subfield_offset),
                                       COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_L_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_L_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_L_WM_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_L_WM_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_L_WM_get", stop_bit, 8 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 8) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 8;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000401c bits 8:0) bits 0:8 use field RX_L_WM of register PMC_COPI_REG_RX_FIFO_TH */
    reg_value = copi_reg_RX_FIFO_TH_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_MSK)
                  >> COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_MSK, COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_L_WM_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_CH_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_CH_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_CH_FC_EN_set", A, value );

    /* (0x00004020 bits 17) field TX_ODUK_CH_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    copi_reg_TX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_MSK,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_ODUK_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_CH_FC_EN_get", A, 1);
    /* (0x00004020 bits 17) field TX_ODUK_CH_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    reg_value = copi_reg_TX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_MSK) >> COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_CH_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_LINK_FC_EN_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_LINK_FC_EN_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_LINK_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_LINK_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_ODUK_LINK_FC_EN_set", A, value );

    /* (0x00004020 bits 16) field TX_ODUK_LINK_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    copi_reg_TX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_MSK,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_ODUK_LINK_FC_EN_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_LINK_FC_EN_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_LINK_FC_EN_get", A, 1);
    /* (0x00004020 bits 16) field TX_ODUK_LINK_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    reg_value = copi_reg_TX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_MSK) >> COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_ODUK_LINK_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_CPB_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_CH_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_CH_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_CPB_CH_FC_EN_set", A, value );

    /* (0x00004020 bits 5) field TX_CPB_CH_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    copi_reg_TX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_MSK,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_CPB_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_CH_FC_EN_get", A, 1);
    /* (0x00004020 bits 5) field TX_CPB_CH_FC_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    reg_value = copi_reg_TX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_MSK) >> COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_CPB_CH_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_OOB_EN_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_OOB_EN_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_OOB_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_OOB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_OOB_EN_set", A, value );

    /* (0x00004020 bits 1) field TX_OOB_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    copi_reg_TX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_MSK,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_OOB_EN_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_OOB_EN_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_OOB_EN_get", A, 1);
    /* (0x00004020 bits 1) field TX_OOB_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    reg_value = copi_reg_TX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_MSK) >> COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_OOB_EN_get", A, value );

    return value;
}
static INLINE void copi_field_TX_IB_EN_set( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_IB_EN_set( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_IB_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_IB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TX_IB_EN_set", A, value );

    /* (0x00004020 bits 0) field TX_IB_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    copi_reg_TX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_MSK,
                                         COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_IB_EN_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_IB_EN_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_IB_EN_get", A, 1);
    /* (0x00004020 bits 0) field TX_IB_EN of register PMC_COPI_REG_TX_FLOW_CTRL_CFG */
    reg_value = copi_reg_TX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_MSK) >> COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_IB_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_ZONE_HI_TH_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_ZONE_HI_TH_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_ZONE_HI_TH_set", A, value );

    /* (0x00004024 bits 21:20) bits 0:1 use field RX_ODUK_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_ZONE_HI_TH_get", A, 1);
    /* (0x00004024 bits 21:20) bits 0:1 use field RX_ODUK_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_ZONE_HI_TH_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_ODUK_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ODUK_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_ZONE_HI_TH_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_ZONE_HI_TH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_ZONE_HI_TH_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_ZONE_HI_TH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004024 bits 21:20) bits 0:1 use field RX_ODUK_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004024 bits 21:20) bits 0:1 use field RX_ODUK_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
        copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF + subfield_offset),
                                             COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ODUK_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_ZONE_HI_TH_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_ZONE_HI_TH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_ZONE_HI_TH_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004024 bits 21:20) bits 0:1 use field RX_ODUK_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_MSK)
                  >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_MSK, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_ZONE_HI_TH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_ZONE_LW_TH_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_ZONE_LW_TH_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_ZONE_LW_TH_set", A, value );

    /* (0x00004024 bits 19:18) bits 0:1 use field RX_ODUK_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_ZONE_LW_TH_get", A, 1);
    /* (0x00004024 bits 19:18) bits 0:1 use field RX_ODUK_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_ZONE_LW_TH_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_ODUK_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ODUK_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_ZONE_LW_TH_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_ZONE_LW_TH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_ZONE_LW_TH_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_ZONE_LW_TH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004024 bits 19:18) bits 0:1 use field RX_ODUK_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004024 bits 19:18) bits 0:1 use field RX_ODUK_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
        copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF + subfield_offset),
                                             COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ODUK_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_ZONE_LW_TH_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_ZONE_LW_TH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_ZONE_LW_TH_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004024 bits 19:18) bits 0:1 use field RX_ODUK_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_MSK)
                  >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_MSK, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_ZONE_LW_TH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_CH_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_CH_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_CH_FC_EN_set", A, value );

    /* (0x00004024 bits 16) field RX_ODUK_CH_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_CH_FC_EN_get", A, 1);
    /* (0x00004024 bits 16) field RX_ODUK_CH_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_CH_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_CPB_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_CPB_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ZONE_HI_TH_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_CPB_ZONE_HI_TH_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_CPB_ZONE_HI_TH_set", A, value );

    /* (0x00004024 bits 9:8) bits 0:1 use field RX_CPB_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_CPB_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_CPB_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ZONE_HI_TH_get", A, 1);
    /* (0x00004024 bits 9:8) bits 0:1 use field RX_CPB_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_CPB_ZONE_HI_TH_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_CPB_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_CPB_ZONE_HI_TH_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_CPB_ZONE_HI_TH_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_CPB_ZONE_HI_TH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_CPB_ZONE_HI_TH_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_CPB_ZONE_HI_TH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004024 bits 9:8) bits 0:1 use field RX_CPB_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004024 bits 9:8) bits 0:1 use field RX_CPB_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
        copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF + subfield_offset),
                                             COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_CPB_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_CPB_ZONE_HI_TH_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_CPB_ZONE_HI_TH_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_CPB_ZONE_HI_TH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_CPB_ZONE_HI_TH_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004024 bits 9:8) bits 0:1 use field RX_CPB_ZONE_HI_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_MSK)
                  >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_MSK, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_CPB_ZONE_HI_TH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_CPB_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_CPB_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ZONE_LW_TH_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_CPB_ZONE_LW_TH_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_CPB_ZONE_LW_TH_set", A, value );

    /* (0x00004024 bits 7:6) bits 0:1 use field RX_CPB_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_CPB_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_CPB_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ZONE_LW_TH_get", A, 1);
    /* (0x00004024 bits 7:6) bits 0:1 use field RX_CPB_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_CPB_ZONE_LW_TH_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_CPB_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_CPB_ZONE_LW_TH_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_CPB_ZONE_LW_TH_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_CPB_ZONE_LW_TH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_CPB_ZONE_LW_TH_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_CPB_ZONE_LW_TH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004024 bits 7:6) bits 0:1 use field RX_CPB_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004024 bits 7:6) bits 0:1 use field RX_CPB_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
        copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF + subfield_offset),
                                             COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_CPB_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_CPB_ZONE_LW_TH_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_CPB_ZONE_LW_TH_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_CPB_ZONE_LW_TH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_CPB_ZONE_LW_TH_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004024 bits 7:6) bits 0:1 use field RX_CPB_ZONE_LW_TH of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_MSK)
                  >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_MSK, COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_CPB_ZONE_LW_TH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_LINK_FC_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_LINK_FC_EN_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_LINK_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_LINK_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_LINK_FC_EN_set", A, value );

    /* (0x00004024 bits 5) field RX_LINK_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_LINK_FC_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_LINK_FC_EN_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_LINK_FC_EN_get", A, 1);
    /* (0x00004024 bits 5) field RX_LINK_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_LINK_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_CPB_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_CPB_CH_FC_EN_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_CH_FC_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_CPB_CH_FC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_CPB_CH_FC_EN_set", A, value );

    /* (0x00004024 bits 4) field RX_CPB_CH_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_CPB_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_CPB_CH_FC_EN_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_CH_FC_EN_get", A, 1);
    /* (0x00004024 bits 4) field RX_CPB_CH_FC_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_CPB_CH_FC_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_OOB_EN_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_OOB_EN_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_OOB_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_OOB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_OOB_EN_set", A, value );

    /* (0x00004024 bits 1) field RX_OOB_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_OOB_EN_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_OOB_EN_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_OOB_EN_get", A, 1);
    /* (0x00004024 bits 1) field RX_OOB_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_OOB_EN_get", A, value );

    return value;
}
static INLINE void copi_field_RX_IB_EN_set( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IB_EN_set( copi_buffer_t *b_ptr,
                                            sifd_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IB_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IB_EN_set", A, value );

    /* (0x00004024 bits 0) field RX_IB_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    copi_reg_RX_FLOW_CTRL_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_MSK,
                                         COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_OFF,
                                         value);
}

static INLINE UINT32 copi_field_RX_IB_EN_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IB_EN_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IB_EN_get", A, 1);
    /* (0x00004024 bits 0) field RX_IB_EN of register PMC_COPI_REG_RX_FLOW_CTRL_CFG */
    reg_value = copi_reg_RX_FLOW_CTRL_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_MSK) >> COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IB_EN_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_FC_EXT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_FC_EXT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_EXT_ODUK_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_FC_EXT_ODUK_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_FC_EXT_ODUK_LB_set", A, value );

    /* (0x00004038 bits 11) field DPI_FC_EXT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_FC_EXT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_FC_EXT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_EXT_ODUK_LB_get", A, 1);
    /* (0x00004038 bits 11) field DPI_FC_EXT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_FC_EXT_ODUK_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_FC_EXT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_FC_EXT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_EXT_CPB_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_FC_EXT_CPB_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_FC_EXT_CPB_LB_set", A, value );

    /* (0x00004038 bits 10) field DPI_FC_EXT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_FC_EXT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_FC_EXT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_EXT_CPB_LB_get", A, 1);
    /* (0x00004038 bits 10) field DPI_FC_EXT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_FC_EXT_CPB_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_FC_INT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_FC_INT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_INT_ODUK_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_FC_INT_ODUK_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_FC_INT_ODUK_LB_set", A, value );

    /* (0x00004038 bits 9) field DPI_FC_INT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_FC_INT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_FC_INT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_INT_ODUK_LB_get", A, 1);
    /* (0x00004038 bits 9) field DPI_FC_INT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_FC_INT_ODUK_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_FC_INT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_FC_INT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_INT_CPB_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_FC_INT_CPB_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_FC_INT_CPB_LB_set", A, value );

    /* (0x00004038 bits 8) field DPI_FC_INT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_FC_INT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_FC_INT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_FC_INT_CPB_LB_get", A, 1);
    /* (0x00004038 bits 8) field DPI_FC_INT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_FC_INT_CPB_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_EXT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_EXT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_EXT_ODUK_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_EXT_ODUK_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_EXT_ODUK_LB_set", A, value );

    /* (0x00004038 bits 3) field DPI_EXT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_EXT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_EXT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_EXT_ODUK_LB_get", A, 1);
    /* (0x00004038 bits 3) field DPI_EXT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_EXT_ODUK_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_EXT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_EXT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_EXT_CPB_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_EXT_CPB_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_EXT_CPB_LB_set", A, value );

    /* (0x00004038 bits 2) field DPI_EXT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_EXT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_EXT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_EXT_CPB_LB_get", A, 1);
    /* (0x00004038 bits 2) field DPI_EXT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_EXT_CPB_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_INT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_INT_ODUK_LB_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_INT_ODUK_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_INT_ODUK_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_INT_ODUK_LB_set", A, value );

    /* (0x00004038 bits 1) field DPI_INT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_INT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_INT_ODUK_LB_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_INT_ODUK_LB_get", A, 1);
    /* (0x00004038 bits 1) field DPI_INT_ODUK_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_INT_ODUK_LB_get", A, value );

    return value;
}
static INLINE void copi_field_DPI_INT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_DPI_INT_CPB_LB_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_INT_CPB_LB_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_DPI_INT_CPB_LB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_DPI_INT_CPB_LB_set", A, value );

    /* (0x00004038 bits 0) field DPI_INT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    copi_reg_DPI_LB_field_set( b_ptr,
                               h_ptr,
                               A,
                               COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_MSK,
                               COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_OFF,
                               value);
}

static INLINE UINT32 copi_field_DPI_INT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_DPI_INT_CPB_LB_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_DPI_INT_CPB_LB_get", A, 1);
    /* (0x00004038 bits 0) field DPI_INT_CPB_LB of register PMC_COPI_REG_DPI_LB */
    reg_value = copi_reg_DPI_LB_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_MSK) >> COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_DPI_INT_CPB_LB_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size104
 * ==================================================================================
 */
static INLINE void copi_field_RX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_2B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ODUK_2B_LK_set", N, 103);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_2B_LK_set", value, 65535);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_RX_ODUK_2B_LK_set", A, N, value );

    /* ((0x00005100 + (N) * 0x4) bits 15:0) field RX_ODUK_2B_LK of register PMC_COPI_REG_RX_ODUK_2B_LK index N=0..103 */
    copi_reg_RX_ODUK_2B_LK_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_MSK,
                                            COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF,
                                            value);
}

static INLINE UINT32 copi_field_RX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_2B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ODUK_2B_LK_get", N, 103);
    /* ((0x00005100 + (N) * 0x4) bits 15:0) field RX_ODUK_2B_LK of register PMC_COPI_REG_RX_ODUK_2B_LK index N=0..103 */
    reg_value = copi_reg_RX_ODUK_2B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_MSK) >> COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_RX_ODUK_2B_LK_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_RX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_2B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ODUK_2B_LK_set", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_2B_LK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_2B_LK_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_2B_LK_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00005100 + (N) * 0x4) bits 15:0) field RX_ODUK_2B_LK of register PMC_COPI_REG_RX_ODUK_2B_LK index N=0..103 */
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
        /* ((0x00005100 + (N) * 0x4) bits 15:0) field RX_ODUK_2B_LK of register PMC_COPI_REG_RX_ODUK_2B_LK index N=0..103 */
        copi_reg_RX_ODUK_2B_LK_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF + subfield_offset),
                                                COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_2B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ODUK_2B_LK_get", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_2B_LK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_2B_LK_get", stop_bit, 15 );
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
    /* ((0x00005100 + (N) * 0x4) bits 15:0) field RX_ODUK_2B_LK of register PMC_COPI_REG_RX_ODUK_2B_LK index N=0..103 */
    reg_value = copi_reg_RX_ODUK_2B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_MSK)
                  >> COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_MSK, COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_2B_LK_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size2
 * ==================================================================================
 */
static INLINE void copi_field_TX_H_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_H_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32  N,
                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_H_WM_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_H_WM_set", N, 1);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_H_WM_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_H_WM_set", A, N, value );

    /* ((0x00004014 + (N) * 0x4) bits 22:16) bits 0:6 use field TX_H_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    copi_reg_TX_FIFO_TH_array_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         N,
                                         COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_MSK,
                                         COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_H_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_H_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_H_WM_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_H_WM_get", N, 1);
    /* ((0x00004014 + (N) * 0x4) bits 22:16) bits 0:6 use field TX_H_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    reg_value = copi_reg_TX_FIFO_TH_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    value = (reg_value & COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_MSK) >> COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_H_WM_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_H_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_H_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_H_WM_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_H_WM_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_H_WM_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_H_WM_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_H_WM_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004014 + (N) * 0x4) bits 22:16) bits 0:6 use field TX_H_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00004014 + (N) * 0x4) bits 22:16) bits 0:6 use field TX_H_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
        copi_reg_TX_FIFO_TH_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             subfield_mask << (COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF + subfield_offset),
                                             COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_H_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_H_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_H_WM_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_H_WM_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_H_WM_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_H_WM_get", stop_bit, 6 );
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
    /* ((0x00004014 + (N) * 0x4) bits 22:16) bits 0:6 use field TX_H_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    reg_value = copi_reg_TX_FIFO_TH_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    field_value = (reg_value & COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_MSK)
                  >> COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_MSK, COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_H_WM_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_L_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_L_WM_set( copi_buffer_t *b_ptr,
                                           sifd_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32  N,
                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_L_WM_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_L_WM_set", N, 1);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_L_WM_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_L_WM_set", A, N, value );

    /* ((0x00004014 + (N) * 0x4) bits 6:0) bits 0:6 use field TX_L_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    copi_reg_TX_FIFO_TH_array_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         N,
                                         COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_MSK,
                                         COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF,
                                         value);
}

static INLINE UINT32 copi_field_TX_L_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_L_WM_get( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_L_WM_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_L_WM_get", N, 1);
    /* ((0x00004014 + (N) * 0x4) bits 6:0) bits 0:6 use field TX_L_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    reg_value = copi_reg_TX_FIFO_TH_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    value = (reg_value & COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_MSK) >> COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_L_WM_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_L_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_L_WM_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_L_WM_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_L_WM_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_L_WM_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_L_WM_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_L_WM_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004014 + (N) * 0x4) bits 6:0) bits 0:6 use field TX_L_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00004014 + (N) * 0x4) bits 6:0) bits 0:6 use field TX_L_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
        copi_reg_TX_FIFO_TH_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             subfield_mask << (COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF + subfield_offset),
                                             COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_L_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_L_WM_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_L_WM_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_L_WM_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_L_WM_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_L_WM_get", stop_bit, 6 );
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
    /* ((0x00004014 + (N) * 0x4) bits 6:0) bits 0:6 use field TX_L_WM of register PMC_COPI_REG_TX_FIFO_TH index N=0..1 */
    reg_value = copi_reg_TX_FIFO_TH_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    field_value = (reg_value & COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_MSK)
                  >> COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_MSK, COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_L_WM_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void copi_field_PMON_UPDATE_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_PMON_UPDATE_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_PMON_UPDATE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_PMON_UPDATE_set", A, value );

    /* (0x00004004 bits 0) field PMON_UPDATE of register PMC_COPI_REG_CFG */
    copi_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            COPI_REG_CFG_BIT_PMON_UPDATE_MSK,
                            COPI_REG_CFG_BIT_PMON_UPDATE_OFF,
                            value);
}

static INLINE UINT32 copi_field_PMON_UPDATE_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_PMON_UPDATE_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_PMON_UPDATE_get", A, 1);
    /* (0x00004004 bits 0) field PMON_UPDATE of register PMC_COPI_REG_CFG */
    reg_value = copi_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_CFG_BIT_PMON_UPDATE_MSK) >> COPI_REG_CFG_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_PMON_UPDATE_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_SET_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_SET_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_SET_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_SET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_SET_set", A, value );

    /* (0x000052a0 bits 31) field RX_ODUK_H2B_CAM_ERR_SET of register PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR */
    copi_reg_RX_ODUK_H2B_CAM_ERR_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_MSK,
                                            COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_OFF,
                                            value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_SET_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_SET_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_SET_get", A, 1);
    /* (0x000052a0 bits 31) field RX_ODUK_H2B_CAM_ERR_SET of register PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CAM_ERR_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_MSK) >> COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_SET_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set", A, value );

    /* (0x000052a4 bits 31) field RX_ODUK_H2B_CMPR_ERR_SET of register PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR */
    copi_reg_RX_ODUK_H2B_CMPR_ERR_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_MSK,
                                             COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_OFF,
                                             value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_SET_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_SET_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_SET_get", A, 1);
    /* (0x000052a4 bits 31) field RX_ODUK_H2B_CMPR_ERR_SET of register PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CMPR_ERR_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_MSK) >> COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_SET_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set", A, value );

    /* (0x000052a8 bits 31) field RX_ODUK_H4B_CMPR_ERR_SET of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0 */
    copi_reg_RX_ODUK_H4B_CMPR_ERR0_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_MSK,
                                              COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_OFF,
                                              value);
}

static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_SET_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_SET_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_SET_get", A, 1);
    /* (0x000052a8 bits 31) field RX_ODUK_H4B_CMPR_ERR_SET of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0 */
    reg_value = copi_reg_RX_ODUK_H4B_CMPR_ERR0_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_MSK) >> COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_SET_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set", A, value );

    /* (0x00004134 bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_MSK,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_ODUK_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_ALIGN_INT_E0_get", A, 1);
    /* (0x00004134 bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_MSK) >> COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004134 bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004134 bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
        copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004134 bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_MSK)
                  >> COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_MSK, COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_ODUK_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_UN_INT_E0_set", A, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_E0_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_E0_set", A, value );

    /* (0x00004134 bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_MSK,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_ODUK_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_UN_INT_E0_get", A, 1);
    /* (0x00004134 bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_MSK) >> COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_ODUK_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_ODUK_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004134 bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
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
        /* (0x00004134 bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
        copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_ODUK_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_ODUK_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_get", stop_bit, 3 );
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
    /* (0x00004134 bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_MSK)
                  >> COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_MSK, COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_UN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_CPB_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_ALIGN_INT_E0_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_E0_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_E0_set", A, value );

    /* (0x00004134 bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_MSK,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_CPB_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_ALIGN_INT_E0_get", A, 1);
    /* (0x00004134 bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_MSK) >> COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004134 bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004134 bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
        copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004134 bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_MSK)
                  >> COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_MSK, COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_CPB_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_UN_INT_E0_set", A, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_E0_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_E0_set", A, value );

    /* (0x00004134 bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_MSK,
                                       COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_CPB_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_UN_INT_E0_get", A, 1);
    /* (0x00004134 bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_MSK) >> COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_CPB_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_CPB_TX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_UN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004134 bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
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
        /* (0x00004134 bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
        copi_reg_TX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_CPB_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_CPB_TX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_UN_INT_E0_get", stop_bit, 3 );
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
    /* (0x00004134 bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_E0 of register PMC_COPI_REG_TX_FIFO_INT_E0 */
    reg_value = copi_reg_TX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_MSK)
                  >> COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_MSK, COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_UN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_ALIGN_INT_E0_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BUF_ALIGN_INT_E0_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BUF_ALIGN_INT_E0_set", A, value );

    /* (0x00004140 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    copi_reg_RX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_MSK,
                                       COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_RX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_ALIGN_INT_E0_get", A, 1);
    /* (0x00004140 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    reg_value = copi_reg_RX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_MSK) >> COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BUF_ALIGN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_BUF_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_ALIGN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_ALIGN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_ALIGN_INT_E0_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_ALIGN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004140 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004140 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
        copi_reg_RX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_BUF_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_ALIGN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_ALIGN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_ALIGN_INT_E0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004140 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    reg_value = copi_reg_RX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_MSK)
                  >> COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_MSK, COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_ALIGN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_UN_INT_E0_set", A, 1);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BUF_UN_INT_E0_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BUF_UN_INT_E0_set", A, value );

    /* (0x00004140 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    copi_reg_RX_FIFO_INT_E0_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_MSK,
                                       COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF,
                                       value);
}

static INLINE UINT32 copi_field_RX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_UN_INT_E0_get", A, 1);
    /* (0x00004140 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    reg_value = copi_reg_RX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_MSK) >> COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BUF_UN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_BUF_UN_INT_E0_set( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_UN_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_UN_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_UN_INT_E0_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_UN_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004140 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
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
        /* (0x00004140 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
        copi_reg_RX_FIFO_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF + subfield_offset),
                                           COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_BUF_UN_INT_E0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_UN_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_UN_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_UN_INT_E0_get", stop_bit, 2 );
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
    /* (0x00004140 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_E0 of register PMC_COPI_REG_RX_FIFO_INT_E0 */
    reg_value = copi_reg_RX_FIFO_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_MSK)
                  >> COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_MSK, COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_UN_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_SER_RX_LOS_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_SER_RX_LOS_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_SER_RX_LOS_INT_E0_set", A, 1);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_SER_RX_LOS_INT_E0_set", value, 16777215);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_SER_RX_LOS_INT_E0_set", A, value );

    /* (0x00004164 bits 23:0) bits 0:23 use field SER_RX_LOS_INT_E0 of register PMC_COPI_REG_SERDES_RX_LOS_INT_E0 */
    copi_reg_SERDES_RX_LOS_INT_E0_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_MSK,
                                             COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF,
                                             value);
}

static INLINE UINT32 copi_field_SER_RX_LOS_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_SER_RX_LOS_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_SER_RX_LOS_INT_E0_get", A, 1);
    /* (0x00004164 bits 23:0) bits 0:23 use field SER_RX_LOS_INT_E0 of register PMC_COPI_REG_SERDES_RX_LOS_INT_E0 */
    reg_value = copi_reg_SERDES_RX_LOS_INT_E0_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_MSK) >> COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_SER_RX_LOS_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_SER_RX_LOS_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_SER_RX_LOS_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_SER_RX_LOS_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_SER_RX_LOS_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_SER_RX_LOS_INT_E0_set", stop_bit, 23 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_SER_RX_LOS_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004164 bits 23:0) bits 0:23 use field SER_RX_LOS_INT_E0 of register PMC_COPI_REG_SERDES_RX_LOS_INT_E0 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004164 bits 23:0) bits 0:23 use field SER_RX_LOS_INT_E0 of register PMC_COPI_REG_SERDES_RX_LOS_INT_E0 */
        copi_reg_SERDES_RX_LOS_INT_E0_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF + subfield_offset),
                                                 COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_SER_RX_LOS_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_SER_RX_LOS_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_SER_RX_LOS_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_SER_RX_LOS_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_SER_RX_LOS_INT_E0_get", stop_bit, 23 );
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
    /* (0x00004164 bits 23:0) bits 0:23 use field SER_RX_LOS_INT_E0 of register PMC_COPI_REG_SERDES_RX_LOS_INT_E0 */
    reg_value = copi_reg_SERDES_RX_LOS_INT_E0_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_MSK)
                  >> COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_MSK, COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_SER_RX_LOS_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 19) field RX_ODUK_H4B_CMPR_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 19) field RX_ODUK_H4B_CMPR_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 18) field RX_ODUK_H2B_CMPR_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 18) field RX_ODUK_H2B_CMPR_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 17) field RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 17) field RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 16) field RX_ODUK_H2B_CAM_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 16) field RX_ODUK_H2B_CAM_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_IL_DAT_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_DAT_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_DAT_ERR_INT_E0_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_E0_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_IL_DAT_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_DAT_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_DAT_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_IL_DAT_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_IL_DAT_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_IL_DAT_ERR_INT_E0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004188 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004188 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
        copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF + subfield_offset),
                                               COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_IL_DAT_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_IL_DAT_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_IL_DAT_ERR_INT_E0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004188 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_MSK)
                  >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_MSK, COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_IL_DAT_ERR_INT_E0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_IL_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_ALIGN_INT_E0_set( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ALIGN_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_ALIGN_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_ALIGN_INT_E0_set", A, value );

    /* (0x00004188 bits 9) field RX_IL_ALIGN_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_IL_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_ALIGN_INT_E0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ALIGN_INT_E0_get", A, 1);
    /* (0x00004188 bits 9) field RX_IL_ALIGN_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_ALIGN_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_IL_ERR_ALL_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_ERR_ALL_INT_E0_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ERR_ALL_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_E0_set", A, value );

    /* (0x00004188 bits 8) field RX_IL_ERR_ALL_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_IL_ERR_ALL_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_ERR_ALL_INT_E0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ERR_ALL_INT_E0_get", A, 1);
    /* (0x00004188 bits 8) field RX_IL_ERR_ALL_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_EOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_EOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOP_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_EOP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_EOP_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 7) field RX_EOP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_EOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_EOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOP_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 7) field RX_EOP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_EOP_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_SOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_SOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOP_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_SOP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_SOP_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 6) field RX_SOP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_SOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_SOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOP_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 6) field RX_SOP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_SOP_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_EOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_EOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOB_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_EOB_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_EOB_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 5) field RX_EOB_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_EOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_EOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOB_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 5) field RX_EOB_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_EOB_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 3) field RX_PKT_CH_CHANGE_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 3) field RX_PKT_CH_CHANGE_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_BURST_DROP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BURST_DROP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_DROP_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 2) field RX_BURST_DROP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_BURST_DROP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BURST_DROP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_DROP_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 2) field RX_BURST_DROP_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_BURST_LEN_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BURST_LEN_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_LEN_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 1) field RX_BURST_LEN_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_BURST_LEN_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BURST_LEN_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_LEN_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 1) field RX_BURST_LEN_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_RX_SOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_SOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOB_ERR_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_SOB_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_SOB_ERR_INT_E0_set", A, value );

    /* (0x00004188 bits 0) field RX_SOB_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    copi_reg_RX_INTF_ERR_INT_E0_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_MSK,
                                           COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_SOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_SOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOB_ERR_INT_E0_get", A, 1);
    /* (0x00004188 bits 0) field RX_SOB_ERR_INT_E0 of register PMC_COPI_REG_RX_INTF_ERR_INT_E0 */
    reg_value = copi_reg_RX_INTF_ERR_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_MSK) >> COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_SOB_ERR_INT_E0_get", A, value );

    return value;
}
static INLINE void copi_field_TIP_INT_E0_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TIP_INT_E0_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TIP_INT_E0_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TIP_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TIP_INT_E0_set", A, value );

    /* (0x000041ac bits 0) field TIP_INT_E0 of register PMC_COPI_REG_COM_INT_E0 */
    copi_reg_COM_INT_E0_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_MSK,
                                   COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_OFF,
                                   value);
}

static INLINE UINT32 copi_field_TIP_INT_E0_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TIP_INT_E0_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TIP_INT_E0_get", A, 1);
    /* (0x000041ac bits 0) field TIP_INT_E0 of register PMC_COPI_REG_COM_INT_E0 */
    reg_value = copi_reg_COM_INT_E0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_MSK) >> COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TIP_INT_E0_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_size2
 * ==================================================================================
 */
static INLINE void copi_field_TX_EOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_EOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOP_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOP_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_EOP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_EOP_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 7) field TX_EOP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_EOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_EOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOP_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOP_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 7) field TX_EOP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_EOP_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_SOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_SOP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOP_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOP_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_SOP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_SOP_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 6) field TX_SOP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_SOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_SOP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOP_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOP_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 6) field TX_SOP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_SOP_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_EOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_EOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOB_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOB_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_EOB_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_EOB_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 5) field TX_EOB_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_EOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_EOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOB_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOB_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 5) field TX_EOB_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_EOB_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 3) field TX_PKT_CH_CHANGE_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 3) field TX_PKT_CH_CHANGE_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_BURST_DROP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_BURST_DROP_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 2) field TX_BURST_DROP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_BURST_DROP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_BURST_DROP_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 2) field TX_BURST_DROP_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_BURST_LEN_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_BURST_LEN_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 1) field TX_BURST_LEN_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_BURST_LEN_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_BURST_LEN_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 1) field TX_BURST_LEN_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_E0_get", A, N, value );

    return value;
}
static INLINE void copi_field_TX_SOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_SOB_ERR_INT_E0_set( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOB_ERR_INT_E0_set", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOB_ERR_INT_E0_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_SOB_ERR_INT_E0_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_SOB_ERR_INT_E0_set", A, N, value );

    /* ((0x00004170 + (N) * 0x4) bits 0) field TX_SOB_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT_E0_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_MSK,
                                                 COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TX_SOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_SOB_ERR_INT_E0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOB_ERR_INT_E0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOB_ERR_INT_E0_get", N, 1);
    /* ((0x00004170 + (N) * 0x4) bits 0) field TX_SOB_ERR_INT_E0 of register PMC_COPI_REG_TX_INTF_ERR_INT_E0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT_E0_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_MSK) >> COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_SOB_ERR_INT_E0_get", A, N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 copi_field_TIP_INT0_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TIP_INT0_get( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TIP_INT0_get", A, 1);
    /* (0x00004130 bits 10) field TIP_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TIP_INT0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TIP_INT0_poll( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TIP_INT0_poll( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_TIP_INT0_poll", A, value );

    /* (0x00004130 bits 10) field TIP_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 copi_field_RX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_INTF_ERR_INT0_get", A, 1);
    /* (0x00004130 bits 7) field RX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_INTF_ERR_INT0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_INTF_ERR_INT0_poll", A, value );

    /* (0x00004130 bits 7) field RX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 copi_field_TX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_INTF_ERR_INT0_get", A, 1);
    /* (0x00004130 bits 6:5) bits 0:1 use field TX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_INTF_ERR_INT0_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_TX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_INTF_ERR_INT0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_INTF_ERR_INT0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_INTF_ERR_INT0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_INTF_ERR_INT0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004130 bits 6:5) bits 0:1 use field TX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_MSK)
                  >> COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_MSK, COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_INTF_ERR_INT0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_TX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_TX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_INTF_ERR_INT0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_INTF_ERR_INT0_poll", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_INTF_ERR_INT0_poll", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_INTF_ERR_INT0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004130 bits 6:5) bits 0:1 use field TX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004130 bits 6:5) bits 0:1 use field TX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
        return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF + subfield_offset),
                                          value << (COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF + subfield_offset),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_INTF_ERR_INT0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_TX_INTF_ERR_INT0_poll", A, value );

    /* (0x00004130 bits 6:5) bits 0:1 use field TX_INTF_ERR_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 copi_field_SERDES_RX_LOS_INT0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_SERDES_RX_LOS_INT0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_SERDES_RX_LOS_INT0_get", A, 1);
    /* (0x00004130 bits 4) field SERDES_RX_LOS_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_SERDES_RX_LOS_INT0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_SERDES_RX_LOS_INT0_poll( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_SERDES_RX_LOS_INT0_poll( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_SERDES_RX_LOS_INT0_poll", A, value );

    /* (0x00004130 bits 4) field SERDES_RX_LOS_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 copi_field_RX_FIFO_INT0_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_FIFO_INT0_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_FIFO_INT0_get", A, 1);
    /* (0x00004130 bits 1) field RX_FIFO_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_FIFO_INT0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_FIFO_INT0_poll", A, value );

    /* (0x00004130 bits 1) field RX_FIFO_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 copi_field_TX_FIFO_INT0_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_FIFO_INT0_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_FIFO_INT0_get", A, 1);
    /* (0x00004130 bits 0) field TX_FIFO_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    reg_value = copi_reg_TOP_SUMMARY_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_MSK) >> COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TX_FIFO_INT0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_FIFO_INT0_poll( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_TX_FIFO_INT0_poll", A, value );

    /* (0x00004130 bits 0) field TX_FIFO_INT0 of register PMC_COPI_REG_TOP_SUMMARY */
    return copi_reg_TOP_SUMMARY_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_MSK,
                                      (value<<COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_OFF),
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
static INLINE void copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", A, value );

    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK,
                                                     COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_ODUK_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_get", A, 1);
    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK) >> COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK)
                  >> COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK, COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                   sifd_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                   sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_ODUK_TX_BUF_ALIGN_INT_I0_poll", A, value );

    /* (0x0000413c bits 29:28) bits 0:1 use field ODUK_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK,
                                       (value<<COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear", A, value );

    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK,
                                                     COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_ODUK_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_BUF_UN_INT_I0_get", A, 1);
    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK) >> COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_ODUK_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_ODUK_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_get", stop_bit, 3 );
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
    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK)
                  >> COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK, COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_ODUK_TX_BUF_UN_INT_I0_poll", A, value );

    /* (0x0000413c bits 23:20) bits 0:3 use field ODUK_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK,
                                       (value<<COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", A, value );

    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK,
                                                     COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_CPB_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_get", A, 1);
    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK) >> COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK)
                  >> COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK, COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_CPB_TX_BUF_ALIGN_INT_I0_poll", A, value );

    /* (0x0000413c bits 13:12) bits 0:1 use field CPB_TX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK,
                                       (value<<COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear", A, value );

    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK,
                                                     COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_CPB_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_BUF_UN_INT_I0_get", A, 1);
    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK) >> COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        copi_reg_TX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_CPB_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_CPB_TX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_get", stop_bit, 3 );
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
    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    reg_value = copi_reg_TX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK)
                  >> COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK, COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_UN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_CPB_TX_BUF_UN_INT_I0_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_CPB_TX_BUF_UN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
        return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_CPB_TX_BUF_UN_INT_I0_poll", A, value );

    /* (0x0000413c bits 7:4) bits 0:3 use field CPB_TX_BUF_UN_INT_I0 of register PMC_COPI_REG_TX_FIFO_INT0 */
    return copi_reg_TX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK,
                                       (value<<COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear", A, value );

    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    copi_reg_RX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK,
                                                     COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_RX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_ALIGN_INT_I0_get", A, 1);
    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    reg_value = copi_reg_RX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK) >> COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BUF_ALIGN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
        copi_reg_RX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_BUF_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_ALIGN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    reg_value = copi_reg_RX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK)
                  >> COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK, COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_ALIGN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_ALIGN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_ALIGN_INT_I0_poll", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_ALIGN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
        return copi_reg_RX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BUF_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_BUF_ALIGN_INT_I0_poll", A, value );

    /* (0x00004148 bits 10:9) bits 0:1 use field RX_BUF_ALIGN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    return copi_reg_RX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK,
                                       (value<<COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BUF_UN_INT_I0_set_to_clear", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BUF_UN_INT_I0_set_to_clear", A, value );

    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    copi_reg_RX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK,
                                                     COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF,
                                                     value);
}

static INLINE UINT32 copi_field_RX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BUF_UN_INT_I0_get", A, 1);
    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    reg_value = copi_reg_RX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK) >> COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BUF_UN_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_BUF_UN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_UN_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_UN_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_UN_INT_I0_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_UN_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
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
        /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
        copi_reg_RX_FIFO_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF + subfield_offset),
                                                         COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_BUF_UN_INT_I0_get( copi_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_UN_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_UN_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_UN_INT_I0_get", stop_bit, 2 );
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
    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    reg_value = copi_reg_RX_FIFO_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK)
                  >> COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK, COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_UN_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_BUF_UN_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_BUF_UN_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_BUF_UN_INT_I0_poll", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_BUF_UN_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
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
        /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
        return copi_reg_RX_FIFO_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           value << (COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BUF_UN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_BUF_UN_INT_I0_poll", A, value );

    /* (0x00004148 bits 5:3) bits 0:2 use field RX_BUF_UN_INT_I0 of register PMC_COPI_REG_RX_FIFO_INT0 */
    return copi_reg_RX_FIFO_INT0_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK,
                                       (value<<COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void copi_field_SER_RX_LOS_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_SER_RX_LOS_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_SER_RX_LOS_INT_I0_set_to_clear", A, 1);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_SER_RX_LOS_INT_I0_set_to_clear", value, 16777215);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_SER_RX_LOS_INT_I0_set_to_clear", A, value );

    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
    copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK,
                                                           COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF,
                                                           value);
}

static INLINE UINT32 copi_field_SER_RX_LOS_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_SER_RX_LOS_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_SER_RX_LOS_INT_I0_get", A, 1);
    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
    reg_value = copi_reg_SERDES_RX_LOS_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK) >> COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_SER_RX_LOS_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_SER_RX_LOS_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_SER_RX_LOS_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_SER_RX_LOS_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_SER_RX_LOS_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_SER_RX_LOS_INT_I0_set_to_clear", stop_bit, 23 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_SER_RX_LOS_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
        copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               subfield_mask << (COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF + subfield_offset),
                                                               COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_SER_RX_LOS_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_SER_RX_LOS_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_SER_RX_LOS_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_SER_RX_LOS_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_SER_RX_LOS_INT_I0_get", stop_bit, 23 );
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
    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
    reg_value = copi_reg_SERDES_RX_LOS_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK)
                  >> COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK, COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_SER_RX_LOS_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_SER_RX_LOS_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_SER_RX_LOS_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_SER_RX_LOS_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_SER_RX_LOS_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_SER_RX_LOS_INT_I0_poll", stop_bit, 23 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_SER_RX_LOS_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
        return copi_reg_SERDES_RX_LOS_INT0_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF + subfield_offset),
                                                 value << (COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_SER_RX_LOS_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_SER_RX_LOS_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_SER_RX_LOS_INT_I0_poll", A, value );

    /* (0x0000416c bits 23:0) bits 0:23 use field SER_RX_LOS_INT_I0 of register PMC_COPI_REG_SERDES_RX_LOS_INT0 */
    return copi_reg_SERDES_RX_LOS_INT0_poll( b_ptr,
                                             h_ptr,
                                             A,
                                             COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK,
                                             (value<<COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 19) field RX_ODUK_H4B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 19) field RX_ODUK_H4B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 19) field RX_ODUK_H4B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 18) field RX_ODUK_H2B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 18) field RX_ODUK_H2B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 18) field RX_ODUK_H2B_CMPR_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 17) field RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 17) field RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                      sifd_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                      sifd_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 17) field RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 16) field RX_ODUK_H2B_CAM_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 16) field RX_ODUK_H2B_CAM_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 16) field RX_ODUK_H2B_CAM_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_IL_DAT_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_DAT_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_DAT_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE void copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
        copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF + subfield_offset),
                                                             COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_IL_DAT_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_IL_DAT_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK)
                  >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK, COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_IL_DAT_ERR_INT_I0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_IL_DAT_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_range_RX_IL_DAT_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_poll", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_IL_DAT_ERR_INT_I0_poll", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_IL_DAT_ERR_INT_I0_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
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
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
        return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF + subfield_offset),
                                               value << (COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_DAT_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_DAT_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_IL_DAT_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 11:10) bits 0:1 use field RX_IL_DAT_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_IL_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_ALIGN_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ALIGN_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_ALIGN_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_ALIGN_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 9) field RX_IL_ALIGN_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_IL_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_ALIGN_INT_I0_get( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ALIGN_INT_I0_get", A, 1);
    /* (0x00004190 bits 9) field RX_IL_ALIGN_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_ALIGN_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_ALIGN_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_IL_ALIGN_INT_I0_poll", A, value );

    /* (0x00004190 bits 9) field RX_IL_ALIGN_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 8) field RX_IL_ERR_ALL_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_IL_ERR_ALL_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_IL_ERR_ALL_INT_I0_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_IL_ERR_ALL_INT_I0_get", A, 1);
    /* (0x00004190 bits 8) field RX_IL_ERR_ALL_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_ERR_ALL_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_IL_ERR_ALL_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_IL_ERR_ALL_INT_I0_poll", A, value );

    /* (0x00004190 bits 8) field RX_IL_ERR_ALL_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_EOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_EOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOP_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_EOP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_EOP_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 7) field RX_EOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_EOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_EOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOP_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 7) field RX_EOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_EOP_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_EOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_EOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_EOP_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 7) field RX_EOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_SOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_SOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOP_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_SOP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_SOP_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 6) field RX_SOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_SOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_SOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOP_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 6) field RX_SOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_SOP_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_SOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_SOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_SOP_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 6) field RX_SOP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_EOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_EOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOB_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_EOB_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_EOB_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 5) field RX_EOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_EOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_EOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_EOB_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 5) field RX_EOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_EOB_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_EOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_EOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_EOB_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 5) field RX_EOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 3) field RX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 3) field RX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 3) field RX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 2) field RX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_BURST_DROP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BURST_DROP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_DROP_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 2) field RX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BURST_DROP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BURST_DROP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_BURST_DROP_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 2) field RX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 1) field RX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_BURST_LEN_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_BURST_LEN_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_BURST_LEN_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 1) field RX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BURST_LEN_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_BURST_LEN_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_BURST_LEN_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 1) field RX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_RX_SOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_SOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOB_ERR_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_SOB_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_RX_SOB_ERR_INT_I0_set_to_clear", A, value );

    /* (0x00004190 bits 0) field RX_SOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_MSK,
                                                         COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_OFF,
                                                         value);
}

static INLINE UINT32 copi_field_RX_SOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_SOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_SOB_ERR_INT_I0_get", A, 1);
    /* (0x00004190 bits 0) field RX_SOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    reg_value = copi_reg_RX_INTF_ERR_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_MSK) >> COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_SOB_ERR_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_SOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_RX_SOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_RX_SOB_ERR_INT_I0_poll", A, value );

    /* (0x00004190 bits 0) field RX_SOB_ERR_INT_I0 of register PMC_COPI_REG_RX_INTF_ERR_INT0 */
    return copi_reg_RX_INTF_ERR_INT0_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_MSK,
                                           (value<<COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TIP_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TIP_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TIP_INT_I0_set_to_clear", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TIP_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_TIP_INT_I0_set_to_clear", A, value );

    /* (0x000041b4 bits 0) field TIP_INT_I0 of register PMC_COPI_REG_TIP_INT0 */
    copi_reg_TIP_INT0_action_on_write_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COPI_REG_TIP_INT0_BIT_TIP_INT_I0_MSK,
                                                 COPI_REG_TIP_INT0_BIT_TIP_INT_I0_OFF,
                                                 value);
}

static INLINE UINT32 copi_field_TIP_INT_I0_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TIP_INT_I0_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TIP_INT_I0_get", A, 1);
    /* (0x000041b4 bits 0) field TIP_INT_I0 of register PMC_COPI_REG_TIP_INT0 */
    reg_value = copi_reg_TIP_INT0_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_TIP_INT0_BIT_TIP_INT_I0_MSK) >> COPI_REG_TIP_INT0_BIT_TIP_INT_I0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_TIP_INT_I0_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TIP_INT_I0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TIP_INT_I0_poll( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "copi_field_TIP_INT_I0_poll", A, value );

    /* (0x000041b4 bits 0) field TIP_INT_I0 of register PMC_COPI_REG_TIP_INT0 */
    return copi_reg_TIP_INT0_poll( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_TIP_INT0_BIT_TIP_INT_I0_MSK,
                                   (value<<COPI_REG_TIP_INT0_BIT_TIP_INT_I0_OFF),
                                   cmp,
                                   max_count,
                                   num_failed_polls,
                                   delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_size2
 * ==================================================================================
 */
static INLINE void copi_field_TX_EOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_EOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOP_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOP_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_EOP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_EOP_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 7) field TX_EOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_EOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_EOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOP_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOP_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 7) field TX_EOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_EOP_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_EOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_EOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_EOP_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 7) field TX_EOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_SOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_SOP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOP_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOP_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_SOP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_SOP_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 6) field TX_SOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_SOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_SOP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOP_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOP_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 6) field TX_SOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_SOP_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_SOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_SOP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_SOP_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 6) field TX_SOP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_EOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_EOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOB_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOB_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_EOB_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_EOB_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 5) field TX_EOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_EOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_EOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_EOB_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_EOB_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 5) field TX_EOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_EOB_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_EOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_EOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_EOB_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 5) field TX_EOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 3) field TX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 3) field TX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 3) field TX_PKT_CH_CHANGE_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 2) field TX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_BURST_DROP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_BURST_DROP_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_DROP_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 2) field TX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_BURST_DROP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_BURST_DROP_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_BURST_DROP_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 2) field TX_BURST_DROP_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 1) field TX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_BURST_LEN_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_BURST_LEN_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_BURST_LEN_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 1) field TX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_BURST_LEN_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_BURST_LEN_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_BURST_LEN_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 1) field TX_BURST_LEN_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void copi_field_TX_SOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_SOB_ERR_INT_I0_set_to_clear( copi_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOB_ERR_INT_I0_set_to_clear", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOB_ERR_INT_I0_set_to_clear", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_SOB_ERR_INT_I0_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_SOB_ERR_INT_I0_set_to_clear", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 0) field TX_SOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_MSK,
                                                               COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_OFF,
                                                               value);
}

static INLINE UINT32 copi_field_TX_SOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_SOB_ERR_INT_I0_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_SOB_ERR_INT_I0_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_SOB_ERR_INT_I0_get", N, 1);
    /* ((0x00004180 + (N) * 0x4) bits 0) field TX_SOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    reg_value = copi_reg_TX_INTF_ERR_INT0_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_MSK) >> COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_SOB_ERR_INT_I0_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_SOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE copi_field_TX_SOB_ERR_INT_I0_poll( copi_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "copi_field_TX_SOB_ERR_INT_I0_poll", A, N, value );

    /* ((0x00004180 + (N) * 0x4) bits 0) field TX_SOB_ERR_INT_I0 of register PMC_COPI_REG_TX_INTF_ERR_INT0 index N=0..1 */
    return copi_reg_TX_INTF_ERR_INT0_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_MSK,
                                                 (value<<COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size104
 * ==================================================================================
 */
static INLINE void copi_field_TX_ODUK_ARB_CH_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_ARB_CH_set( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_ARB_CH_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_ARB_CH_set", N, 103);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_ARB_CH_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_ODUK_ARB_CH_set", A, N, value );

    /* ((0x00004500 + (N) * 0x4) bits 7:0) field TX_ODUK_ARB_CH of register PMC_COPI_REG_TX_ODUK_ARB_CH_LK index N=0..103 */
    copi_reg_TX_ODUK_ARB_CH_LK_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_MSK,
                                                COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF,
                                                value);
}

static INLINE UINT32 copi_field_TX_ODUK_ARB_CH_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_ARB_CH_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_ARB_CH_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_ARB_CH_get", N, 103);
    /* ((0x00004500 + (N) * 0x4) bits 7:0) field TX_ODUK_ARB_CH of register PMC_COPI_REG_TX_ODUK_ARB_CH_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_ARB_CH_LK_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_MSK) >> COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_ODUK_ARB_CH_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_ODUK_ARB_CH_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_ODUK_ARB_CH_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_ARB_CH_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_ARB_CH_set", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_ARB_CH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_ARB_CH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_ARB_CH_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004500 + (N) * 0x4) bits 7:0) field TX_ODUK_ARB_CH of register PMC_COPI_REG_TX_ODUK_ARB_CH_LK index N=0..103 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00004500 + (N) * 0x4) bits 7:0) field TX_ODUK_ARB_CH of register PMC_COPI_REG_TX_ODUK_ARB_CH_LK index N=0..103 */
        copi_reg_TX_ODUK_ARB_CH_LK_array_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N,
                                                    subfield_mask << (COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF + subfield_offset),
                                                    COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_ODUK_ARB_CH_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ODUK_ARB_CH_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_ARB_CH_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_ARB_CH_get", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_ARB_CH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_ARB_CH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00004500 + (N) * 0x4) bits 7:0) field TX_ODUK_ARB_CH of register PMC_COPI_REG_TX_ODUK_ARB_CH_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_ARB_CH_LK_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    field_value = (reg_value & COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_MSK)
                  >> COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_MSK, COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_ARB_CH_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_4B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_4B_LK_set", N, 103);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_ODUK_4B_LK_set", A, N, value );

    /* ((0x00004700 + (N) * 0x4) bits 31:0) field TX_ODUK_4B_LK of register PMC_COPI_REG_TX_ODUK_4B_LK index N=0..103 */
    copi_reg_TX_ODUK_4B_LK_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_MSK,
                                            COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF,
                                            value);
}

static INLINE UINT32 copi_field_TX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_4B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_4B_LK_get", N, 103);
    /* ((0x00004700 + (N) * 0x4) bits 31:0) field TX_ODUK_4B_LK of register PMC_COPI_REG_TX_ODUK_4B_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_4B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_MSK) >> COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_ODUK_4B_LK_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_4B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_4B_LK_set", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_4B_LK_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_4B_LK_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_4B_LK_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004700 + (N) * 0x4) bits 31:0) field TX_ODUK_4B_LK of register PMC_COPI_REG_TX_ODUK_4B_LK index N=0..103 */
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
        /* ((0x00004700 + (N) * 0x4) bits 31:0) field TX_ODUK_4B_LK of register PMC_COPI_REG_TX_ODUK_4B_LK index N=0..103 */
        copi_reg_TX_ODUK_4B_LK_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF + subfield_offset),
                                                COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_4B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_4B_LK_get", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_4B_LK_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_4B_LK_get", stop_bit, 31 );
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
    /* ((0x00004700 + (N) * 0x4) bits 31:0) field TX_ODUK_4B_LK of register PMC_COPI_REG_TX_ODUK_4B_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_4B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_MSK)
                  >> COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_MSK, COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_4B_LK_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_TX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_2B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_2B_LK_set", N, 103);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_ODUK_2B_LK_set", value, 65535);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_ODUK_2B_LK_set", A, N, value );

    /* ((0x00004900 + (N) * 0x4) bits 15:0) field TX_ODUK_2B_LK of register PMC_COPI_REG_TX_ODUK_2B_LK index N=0..103 */
    copi_reg_TX_ODUK_2B_LK_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_MSK,
                                            COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF,
                                            value);
}

static INLINE UINT32 copi_field_TX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ODUK_2B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ODUK_2B_LK_get", N, 103);
    /* ((0x00004900 + (N) * 0x4) bits 15:0) field TX_ODUK_2B_LK of register PMC_COPI_REG_TX_ODUK_2B_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_2B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_MSK) >> COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_ODUK_2B_LK_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_ODUK_2B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_2B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_2B_LK_set", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_2B_LK_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_2B_LK_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_2B_LK_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004900 + (N) * 0x4) bits 15:0) field TX_ODUK_2B_LK of register PMC_COPI_REG_TX_ODUK_2B_LK index N=0..103 */
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
        /* ((0x00004900 + (N) * 0x4) bits 15:0) field TX_ODUK_2B_LK of register PMC_COPI_REG_TX_ODUK_2B_LK index N=0..103 */
        copi_reg_TX_ODUK_2B_LK_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF + subfield_offset),
                                                COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ODUK_2B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ODUK_2B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ODUK_2B_LK_get", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ODUK_2B_LK_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ODUK_2B_LK_get", stop_bit, 15 );
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
    /* ((0x00004900 + (N) * 0x4) bits 15:0) field TX_ODUK_2B_LK of register PMC_COPI_REG_TX_ODUK_2B_LK index N=0..103 */
    reg_value = copi_reg_TX_ODUK_2B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_MSK)
                  >> COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_MSK, COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ODUK_2B_LK_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void copi_field_RX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_4B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ODUK_4B_LK_set", N, 103);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_RX_ODUK_4B_LK_set", A, N, value );

    /* ((0x00004f00 + (N) * 0x4) bits 31:0) field RX_ODUK_4B_LK of register PMC_COPI_REG_RX_ODUK_4B_LK index N=0..103 */
    copi_reg_RX_ODUK_4B_LK_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_MSK,
                                            COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF,
                                            value);
}

static INLINE UINT32 copi_field_RX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_4B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ODUK_4B_LK_get", N, 103);
    /* ((0x00004f00 + (N) * 0x4) bits 31:0) field RX_ODUK_4B_LK of register PMC_COPI_REG_RX_ODUK_4B_LK index N=0..103 */
    reg_value = copi_reg_RX_ODUK_4B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_MSK) >> COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_RX_ODUK_4B_LK_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_RX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ODUK_4B_LK_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_4B_LK_set", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ODUK_4B_LK_set", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_4B_LK_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_4B_LK_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_4B_LK_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004f00 + (N) * 0x4) bits 31:0) field RX_ODUK_4B_LK of register PMC_COPI_REG_RX_ODUK_4B_LK index N=0..103 */
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
        /* ((0x00004f00 + (N) * 0x4) bits 31:0) field RX_ODUK_4B_LK of register PMC_COPI_REG_RX_ODUK_4B_LK index N=0..103 */
        copi_reg_RX_ODUK_4B_LK_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF + subfield_offset),
                                                COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_4B_LK_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_4B_LK_get", A, 1);
    if (N > 103)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ODUK_4B_LK_get", N, 103);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_4B_LK_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_4B_LK_get", stop_bit, 31 );
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
    /* ((0x00004f00 + (N) * 0x4) bits 31:0) field RX_ODUK_4B_LK of register PMC_COPI_REG_RX_ODUK_4B_LK index N=0..103 */
    reg_value = copi_reg_RX_ODUK_4B_LK_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_MSK)
                  >> COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_MSK, COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_4B_LK_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size150
 * ==================================================================================
 */
static INLINE void copi_field_TX_CPB_ARB_CH_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_TX_CPB_ARB_CH_set( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_ARB_CH_set", A, 1);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_CPB_ARB_CH_set", N, 149);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_TX_CPB_ARB_CH_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_TX_CPB_ARB_CH_set", A, N, value );

    /* ((0x00004200 + (N) * 0x4) bits 7:0) field TX_CPB_ARB_CH of register PMC_COPI_REG_TX_CPB_ARB_CH_LK index N=0..149 */
    copi_reg_TX_CPB_ARB_CH_LK_array_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               N,
                                               COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_MSK,
                                               COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF,
                                               value);
}

static INLINE UINT32 copi_field_TX_CPB_ARB_CH_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_CPB_ARB_CH_get( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_CPB_ARB_CH_get", A, 1);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_CPB_ARB_CH_get", N, 149);
    /* ((0x00004200 + (N) * 0x4) bits 7:0) field TX_CPB_ARB_CH of register PMC_COPI_REG_TX_CPB_ARB_CH_LK index N=0..149 */
    reg_value = copi_reg_TX_CPB_ARB_CH_LK_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_MSK) >> COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_CPB_ARB_CH_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_TX_CPB_ARB_CH_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_TX_CPB_ARB_CH_set( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_CPB_ARB_CH_set", A, 1);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_CPB_ARB_CH_set", N, 149);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_CPB_ARB_CH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_CPB_ARB_CH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_CPB_ARB_CH_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004200 + (N) * 0x4) bits 7:0) field TX_CPB_ARB_CH of register PMC_COPI_REG_TX_CPB_ARB_CH_LK index N=0..149 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00004200 + (N) * 0x4) bits 7:0) field TX_CPB_ARB_CH of register PMC_COPI_REG_TX_CPB_ARB_CH_LK index N=0..149 */
        copi_reg_TX_CPB_ARB_CH_LK_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   subfield_mask << (COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF + subfield_offset),
                                                   COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_TX_CPB_ARB_CH_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_CPB_ARB_CH_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_CPB_ARB_CH_get", A, 1);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_CPB_ARB_CH_get", N, 149);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_CPB_ARB_CH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_CPB_ARB_CH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00004200 + (N) * 0x4) bits 7:0) field TX_CPB_ARB_CH of register PMC_COPI_REG_TX_CPB_ARB_CH_LK index N=0..149 */
    reg_value = copi_reg_TX_CPB_ARB_CH_LK_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    field_value = (reg_value & COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_MSK)
                  >> COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_MSK, COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_CPB_ARB_CH_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size254
 * ==================================================================================
 */
static INLINE void copi_field_RX_CPB_ODUK_PATH_SEL_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_CPB_ODUK_PATH_SEL_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ODUK_PATH_SEL_set", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_CPB_ODUK_PATH_SEL_set", N, 253);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_CPB_ODUK_PATH_SEL_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_RX_CPB_ODUK_PATH_SEL_set", A, N, value );

    /* ((0x00004b00 + (N) * 0x4) bits 8) field RX_CPB_ODUK_PATH_SEL of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
    copi_reg_RX_ARB_CH_LK_array_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           N,
                                           COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_MSK,
                                           COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_CPB_ODUK_PATH_SEL_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_CPB_ODUK_PATH_SEL_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_CPB_ODUK_PATH_SEL_get", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_CPB_ODUK_PATH_SEL_get", N, 253);
    /* ((0x00004b00 + (N) * 0x4) bits 8) field RX_CPB_ODUK_PATH_SEL of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
    reg_value = copi_reg_RX_ARB_CH_LK_array_read( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N);
    value = (reg_value & COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_MSK) >> COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_RX_CPB_ODUK_PATH_SEL_get", A, N, value );

    return value;
}
static INLINE void copi_field_RX_ARB_CH_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_RX_ARB_CH_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32  N,
                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ARB_CH_set", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ARB_CH_set", N, 253);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_RX_ARB_CH_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "copi_field_RX_ARB_CH_set", A, N, value );

    /* ((0x00004b00 + (N) * 0x4) bits 7:0) field RX_ARB_CH of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
    copi_reg_RX_ARB_CH_LK_array_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           N,
                                           COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_MSK,
                                           COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF,
                                           value);
}

static INLINE UINT32 copi_field_RX_ARB_CH_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ARB_CH_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ARB_CH_get", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_ARB_CH_get", N, 253);
    /* ((0x00004b00 + (N) * 0x4) bits 7:0) field RX_ARB_CH of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
    reg_value = copi_reg_RX_ARB_CH_LK_array_read( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N);
    value = (reg_value & COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_MSK) >> COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_RX_ARB_CH_get", A, N, value );

    return value;
}
static INLINE void copi_field_range_RX_ARB_CH_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_range_RX_ARB_CH_set( copi_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ARB_CH_set", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ARB_CH_set", N, 253);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ARB_CH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ARB_CH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ARB_CH_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00004b00 + (N) * 0x4) bits 7:0) field RX_ARB_CH of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00004b00 + (N) * 0x4) bits 7:0) field RX_ARB_CH of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
        copi_reg_RX_ARB_CH_LK_array_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               N,
                                               subfield_mask << (COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF + subfield_offset),
                                               COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 copi_field_range_RX_ARB_CH_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ARB_CH_get( copi_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ARB_CH_get", A, 1);
    if (N > 253)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_ARB_CH_get", N, 253);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ARB_CH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ARB_CH_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00004b00 + (N) * 0x4) bits 7:0) field RX_ARB_CH of register PMC_COPI_REG_RX_ARB_CH_LK index N=0..253 */
    reg_value = copi_reg_RX_ARB_CH_LK_array_read( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N);
    field_value = (reg_value & COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_MSK)
                  >> COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_MSK, COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ARB_CH_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 copi_field_RX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ERR_CNT_get", A, 1);
    /* (0x000041c0 bits 31:0) bits 0:31 use field RX_ERR_CNT of register PMC_COPI_REG_RX_ERR_CNT */
    reg_value = copi_reg_RX_ERR_CNT_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_MSK) >> COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ERR_CNT_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ERR_CNT_get", stop_bit, 31 );
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
    /* (0x000041c0 bits 31:0) bits 0:31 use field RX_ERR_CNT of register PMC_COPI_REG_RX_ERR_CNT */
    reg_value = copi_reg_RX_ERR_CNT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_MSK)
                  >> COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_MSK, COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_get( copi_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CAM_ERR_get", A, 1);
    /* (0x000052a0 bits 23:0) bits 0:23 use field RX_ODUK_H2B_CAM_ERR of register PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CAM_ERR_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_MSK) >> COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CAM_ERR_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_ODUK_H2B_CAM_ERR_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_H2B_CAM_ERR_get( copi_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_H2B_CAM_ERR_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_H2B_CAM_ERR_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_H2B_CAM_ERR_get", stop_bit, 23 );
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
    /* (0x000052a0 bits 23:0) bits 0:23 use field RX_ODUK_H2B_CAM_ERR of register PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CAM_ERR_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_MSK)
                  >> COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_MSK, COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_H2B_CAM_ERR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H2B_CMPR_ERR_get", A, 1);
    /* (0x000052a4 bits 23:0) bits 0:23 use field RX_ODUK_H2B_CMPR_ERR of register PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CMPR_ERR_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_MSK) >> COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H2B_CMPR_ERR_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_ODUK_H2B_CMPR_ERR_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_H2B_CMPR_ERR_get( copi_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_H2B_CMPR_ERR_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_H2B_CMPR_ERR_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_H2B_CMPR_ERR_get", stop_bit, 23 );
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
    /* (0x000052a4 bits 23:0) bits 0:23 use field RX_ODUK_H2B_CMPR_ERR of register PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR */
    reg_value = copi_reg_RX_ODUK_H2B_CMPR_ERR_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_MSK)
                  >> COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_MSK, COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_H2B_CMPR_ERR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR0_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR0_get", A, 1);
    /* (0x000052a8 bits 23:16) bits 0:7 use field RX_ODUK_H4B_CMPR_ERR0 of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0 */
    reg_value = copi_reg_RX_ODUK_H4B_CMPR_ERR0_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_MSK) >> COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR0_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000052a8 bits 23:16) bits 0:7 use field RX_ODUK_H4B_CMPR_ERR0 of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0 */
    reg_value = copi_reg_RX_ODUK_H4B_CMPR_ERR0_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_MSK)
                  >> COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_MSK, COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR1_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR1_get( copi_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_ODUK_H4B_CMPR_ERR1_get", A, 1);
    /* (0x000052ac bits 31:0) bits 0:31 use field RX_ODUK_H4B_CMPR_ERR1 of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR1 */
    reg_value = copi_reg_RX_ODUK_H4B_CMPR_ERR1_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_MSK) >> COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_RX_ODUK_H4B_CMPR_ERR1_get", A, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get( copi_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get", stop_bit, 31 );
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
    /* (0x000052ac bits 31:0) bits 0:31 use field RX_ODUK_H4B_CMPR_ERR1 of register PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR1 */
    reg_value = copi_reg_RX_ODUK_H4B_CMPR_ERR1_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_MSK)
                  >> COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_MSK, COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size2
 * ==================================================================================
 */
static INLINE UINT32 copi_field_TX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_ERR_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_ERR_CNT_get", N, 1);
    /* ((0x000041b8 + (N) * 0x4) bits 31:0) bits 0:31 use field TX_ERR_CNT of register PMC_COPI_REG_TX_ERR_CNT index N=0..1 */
    reg_value = copi_reg_TX_ERR_CNT_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    value = (reg_value & COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_MSK) >> COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_ERR_CNT_get", A, N, value );

    return value;
}
static INLINE UINT32 copi_field_range_TX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_ERR_CNT_get( copi_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_ERR_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_ERR_CNT_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_ERR_CNT_get", stop_bit, 31 );
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
    /* ((0x000041b8 + (N) * 0x4) bits 31:0) bits 0:31 use field TX_ERR_CNT of register PMC_COPI_REG_TX_ERR_CNT index N=0..1 */
    reg_value = copi_reg_TX_ERR_CNT_array_read( b_ptr,
                                                h_ptr,
                                                A,
                                                N);
    field_value = (reg_value & COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_MSK)
                  >> COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_MSK, COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_ERR_CNT_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_TX_DIAG_CNT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_TX_DIAG_CNT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_TX_DIAG_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_TX_DIAG_CNT_get", N, 1);
    /* ((0x000041c4 + (N) * 0x4) bits 31:0) bits 0:31 use field TX_DIAG_CNT of register PMC_COPI_REG_TX_DIAG_IN_CNT index N=0..1 */
    reg_value = copi_reg_TX_DIAG_IN_CNT_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_MSK) >> COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_TX_DIAG_CNT_get", A, N, value );

    return value;
}
static INLINE UINT32 copi_field_range_TX_DIAG_CNT_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_TX_DIAG_CNT_get( copi_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_TX_DIAG_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_TX_DIAG_CNT_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_TX_DIAG_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_TX_DIAG_CNT_get", stop_bit, 31 );
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
    /* ((0x000041c4 + (N) * 0x4) bits 31:0) bits 0:31 use field TX_DIAG_CNT of register PMC_COPI_REG_TX_DIAG_IN_CNT index N=0..1 */
    reg_value = copi_reg_TX_DIAG_IN_CNT_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    field_value = (reg_value & COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_MSK)
                  >> COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_MSK, COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_TX_DIAG_CNT_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 copi_field_RX_DIAG_IN_CNT_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_RX_DIAG_IN_CNT_get( copi_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_RX_DIAG_IN_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_RX_DIAG_IN_CNT_get", N, 1);
    /* ((0x000041cc + (N) * 0x4) bits 31:0) bits 0:31 use field RX_DIAG_IN_CNT of register PMC_COPI_REG_RX_DIAG_CNT index N=0..1 */
    reg_value = copi_reg_RX_DIAG_CNT_array_read( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N);
    value = (reg_value & COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_MSK) >> COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "copi_field_RX_DIAG_IN_CNT_get", A, N, value );

    return value;
}
static INLINE UINT32 copi_field_range_RX_DIAG_IN_CNT_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_range_RX_DIAG_IN_CNT_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_range_RX_DIAG_IN_CNT_get", A, 1);
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "copi_field_range_RX_DIAG_IN_CNT_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "copi_field_range_RX_DIAG_IN_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "copi_field_range_RX_DIAG_IN_CNT_get", stop_bit, 31 );
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
    /* ((0x000041cc + (N) * 0x4) bits 31:0) bits 0:31 use field RX_DIAG_IN_CNT of register PMC_COPI_REG_RX_DIAG_CNT index N=0..1 */
    reg_value = copi_reg_RX_DIAG_CNT_array_read( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N);
    field_value = (reg_value & COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_MSK)
                  >> COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_MSK, COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "copi_field_range_RX_DIAG_IN_CNT_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void copi_field_ODUK_TX_SOFT_set( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ODUK_TX_SOFT_set( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ODUK_TX_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ODUK_TX_SOFT_set", A, value );

    /* (0x00004000 bits 6) field ODUK_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_ODUK_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ODUK_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ODUK_TX_SOFT_get", A, 1);
    /* (0x00004000 bits 6) field ODUK_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ODUK_TX_SOFT_get", A, value );

    return value;
}
static INLINE void copi_field_CPB_TX_SOFT_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_CPB_TX_SOFT_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_CPB_TX_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_CPB_TX_SOFT_set", A, value );

    /* (0x00004000 bits 5) field CPB_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_CPB_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_CPB_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_CPB_TX_SOFT_get", A, 1);
    /* (0x00004000 bits 5) field CPB_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_CPB_TX_SOFT_get", A, value );

    return value;
}
static INLINE void copi_field_IL_SOFT_RST_RX_FC_X2_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_IL_SOFT_RST_RX_FC_X2_set( copi_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_SOFT_RST_RX_FC_X2_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_IL_SOFT_RST_RX_FC_X2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_IL_SOFT_RST_RX_FC_X2_set", A, value );

    /* (0x00004000 bits 4) field IL_SOFT_RST_RX_FC_X2 of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_MSK,
                                   COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_OFF,
                                   value);
}

static INLINE UINT32 copi_field_IL_SOFT_RST_RX_FC_X2_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_IL_SOFT_RST_RX_FC_X2_get( copi_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_SOFT_RST_RX_FC_X2_get", A, 1);
    /* (0x00004000 bits 4) field IL_SOFT_RST_RX_FC_X2 of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_MSK) >> COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_IL_SOFT_RST_RX_FC_X2_get", A, value );

    return value;
}
static INLINE void copi_field_IL_RX_SOFT_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_IL_RX_SOFT_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_RX_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_IL_RX_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_IL_RX_SOFT_set", A, value );

    /* (0x00004000 bits 3) field IL_RX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_IL_RX_SOFT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_IL_RX_SOFT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_RX_SOFT_get", A, 1);
    /* (0x00004000 bits 3) field IL_RX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_IL_RX_SOFT_get", A, value );

    return value;
}
static INLINE void copi_field_IL_TX_SOFT_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_IL_TX_SOFT_set( copi_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_TX_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_IL_TX_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_IL_TX_SOFT_set", A, value );

    /* (0x00004000 bits 2) field IL_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_IL_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_IL_TX_SOFT_get( copi_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_IL_TX_SOFT_get", A, 1);
    /* (0x00004000 bits 2) field IL_TX_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_IL_TX_SOFT_get", A, value );

    return value;
}
static INLINE void copi_field_ILAKEN_SOFT_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_ILAKEN_SOFT_set( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ILAKEN_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_ILAKEN_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_ILAKEN_SOFT_set", A, value );

    /* (0x00004000 bits 1) field ILAKEN_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_ILAKEN_SOFT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_ILAKEN_SOFT_get( copi_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_ILAKEN_SOFT_get", A, 1);
    /* (0x00004000 bits 1) field ILAKEN_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_ILAKEN_SOFT_get", A, value );

    return value;
}
static INLINE void copi_field_COPI_SOFT_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void copi_field_COPI_SOFT_set( copi_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_COPI_SOFT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "copi_field_COPI_SOFT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "copi_field_COPI_SOFT_set", A, value );

    /* (0x00004000 bits 0) field COPI_SOFT of register PMC_COPI_REG_SOFT_RESET */
    copi_reg_SOFT_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   COPI_REG_SOFT_RESET_BIT_COPI_SOFT_MSK,
                                   COPI_REG_SOFT_RESET_BIT_COPI_SOFT_OFF,
                                   value);
}

static INLINE UINT32 copi_field_COPI_SOFT_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 copi_field_COPI_SOFT_get( copi_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "copi_field_COPI_SOFT_get", A, 1);
    /* (0x00004000 bits 0) field COPI_SOFT of register PMC_COPI_REG_SOFT_RESET */
    reg_value = copi_reg_SOFT_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & COPI_REG_SOFT_RESET_BIT_COPI_SOFT_MSK) >> COPI_REG_SOFT_RESET_BIT_COPI_SOFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "copi_field_COPI_SOFT_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COPI_IO_INLINE_H */
