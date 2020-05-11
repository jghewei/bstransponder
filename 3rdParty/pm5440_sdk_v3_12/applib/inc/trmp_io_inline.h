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
 *     and register accessor functions for the trmp block
 *****************************************************************************/
#ifndef _TRMP_IO_INLINE_H
#define _TRMP_IO_INLINE_H

#include "cbrc_api.h"
#include "trmp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TRMP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for trmp
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
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} trmp_buffer_t;
static INLINE void trmp_buffer_init( trmp_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void trmp_buffer_init( trmp_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "trmp_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void trmp_buffer_flush( trmp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void trmp_buffer_flush( trmp_buffer_t *b_ptr )
{
    IOLOG( "trmp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 trmp_reg_read( trmp_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_read( trmp_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x40),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg);
    return value;
}
static INLINE void trmp_reg_write( trmp_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_write( trmp_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x40),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value);
}

static INLINE void trmp_field_set( trmp_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_set( trmp_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x40),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void trmp_action_on_write_field_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_action_on_write_field_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x40),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value<<ofs);
}

static INLINE void trmp_burst_read( trmp_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void trmp_burst_read( trmp_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x40),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                          len,
                          value);
}

static INLINE void trmp_burst_write( trmp_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void trmp_burst_write( trmp_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x40),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE trmp_poll( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE trmp_poll( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x40),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for trmp
 * ==================================================================================
 */

static INLINE void trmp_reg_CFG_write( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_CFG_write( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_CFG_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_CFG,
                    value);
}

static INLINE void trmp_reg_CFG_field_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_CFG_field_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_CFG_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_CFG,
                    mask,
                    PMC_TRMP_REG_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_CFG_read( trmp_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_CFG_read( trmp_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_INSERTION_write( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_INSERTION_write( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_INSERTION_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_INSERTION,
                    value);
}

static INLINE void trmp_reg_INSERTION_field_set( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_INSERTION_field_set( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_INSERTION_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_INSERTION,
                    mask,
                    PMC_TRMP_REG_INSERTION_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_INSERTION_read( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_INSERTION_read( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_INSERTION);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_INSERTION_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_J0_AND_Z0_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_J0_AND_Z0_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_J0_AND_Z0_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_J0_AND_Z0,
                    value);
}

static INLINE void trmp_reg_TX_J0_AND_Z0_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_J0_AND_Z0_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_J0_AND_Z0_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_J0_AND_Z0,
                    mask,
                    PMC_TRMP_REG_TX_J0_AND_Z0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_J0_AND_Z0_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_J0_AND_Z0_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_J0_AND_Z0);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_J0_AND_Z0_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_E1_AND_F1_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_E1_AND_F1_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_E1_AND_F1_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_E1_AND_F1,
                    value);
}

static INLINE void trmp_reg_TX_E1_AND_F1_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_E1_AND_F1_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_E1_AND_F1_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_E1_AND_F1,
                    mask,
                    PMC_TRMP_REG_TX_E1_AND_F1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_E1_AND_F1_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_E1_AND_F1_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_E1_AND_F1);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_E1_AND_F1_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_D1D3_AND_D4D12_write( trmp_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_D1D3_AND_D4D12_write( trmp_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_D1D3_AND_D4D12_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_D1D3_AND_D4D12,
                    value);
}

static INLINE void trmp_reg_TX_D1D3_AND_D4D12_field_set( trmp_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_D1D3_AND_D4D12_field_set( trmp_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_D1D3_AND_D4D12_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_D1D3_AND_D4D12,
                    mask,
                    PMC_TRMP_REG_TX_D1D3_AND_D4D12_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_D1D3_AND_D4D12_read( trmp_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_D1D3_AND_D4D12_read( trmp_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_D1D3_AND_D4D12);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_D1D3_AND_D4D12_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_K1_AND_K2_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_K1_AND_K2_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_K1_AND_K2_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_K1_AND_K2,
                    value);
}

static INLINE void trmp_reg_TX_K1_AND_K2_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_K1_AND_K2_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_K1_AND_K2_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_K1_AND_K2,
                    mask,
                    PMC_TRMP_REG_TX_K1_AND_K2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_K1_AND_K2_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_K1_AND_K2_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_K1_AND_K2);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_K1_AND_K2_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_S1_AND_Z1_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_S1_AND_Z1_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_S1_AND_Z1_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_S1_AND_Z1,
                    value);
}

static INLINE void trmp_reg_TX_S1_AND_Z1_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_S1_AND_Z1_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_S1_AND_Z1_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_S1_AND_Z1,
                    mask,
                    PMC_TRMP_REG_TX_S1_AND_Z1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_S1_AND_Z1_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_S1_AND_Z1_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_S1_AND_Z1);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_S1_AND_Z1_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_Z2_AND_E2_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_Z2_AND_E2_write( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_Z2_AND_E2_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_Z2_AND_E2,
                    value);
}

static INLINE void trmp_reg_TX_Z2_AND_E2_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_Z2_AND_E2_field_set( trmp_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_Z2_AND_E2_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_Z2_AND_E2,
                    mask,
                    PMC_TRMP_REG_TX_Z2_AND_E2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_Z2_AND_E2_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_Z2_AND_E2_read( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_Z2_AND_E2);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_Z2_AND_E2_read", reg_value, A);
    return reg_value;
}

static INLINE void trmp_reg_TX_NATIONAL_write( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_NATIONAL_write( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "trmp_reg_TX_NATIONAL_write", value );
    trmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_NATIONAL,
                    value);
}

static INLINE void trmp_reg_TX_NATIONAL_field_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_reg_TX_NATIONAL_field_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "trmp_reg_TX_NATIONAL_field_set", A, mask, ofs, value );
    trmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_TRMP_REG_TX_NATIONAL,
                    mask,
                    PMC_TRMP_REG_TX_NATIONAL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 trmp_reg_TX_NATIONAL_read( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_reg_TX_NATIONAL_read( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = trmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_TRMP_REG_TX_NATIONAL);

    IOLOG( "%s -> 0x%08x; A=%d", "trmp_reg_TX_NATIONAL_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void trmp_field_SCRAMA1A2SEL_set( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_SCRAMA1A2SEL_set( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_SCRAMA1A2SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_SCRAMA1A2SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_SCRAMA1A2SEL_set", A, value );

    /* (0x00001040 bits 15) field SCRAMA1A2SEL of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_SCRAMA1A2SEL_MSK,
                            TRMP_REG_CFG_BIT_SCRAMA1A2SEL_OFF,
                            value);
}

static INLINE UINT32 trmp_field_SCRAMA1A2SEL_get( trmp_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_SCRAMA1A2SEL_get( trmp_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_SCRAMA1A2SEL_get", A, 15);
    /* (0x00001040 bits 15) field SCRAMA1A2SEL of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_SCRAMA1A2SEL_MSK) >> TRMP_REG_CFG_BIT_SCRAMA1A2SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_SCRAMA1A2SEL_get", A, value );

    return value;
}
static INLINE void trmp_field_SCRAMJ0SEL_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_SCRAMJ0SEL_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_SCRAMJ0SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_SCRAMJ0SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_SCRAMJ0SEL_set", A, value );

    /* (0x00001040 bits 14) field SCRAMJ0SEL of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_SCRAMJ0SEL_MSK,
                            TRMP_REG_CFG_BIT_SCRAMJ0SEL_OFF,
                            value);
}

static INLINE UINT32 trmp_field_SCRAMJ0SEL_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_SCRAMJ0SEL_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_SCRAMJ0SEL_get", A, 15);
    /* (0x00001040 bits 14) field SCRAMJ0SEL of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_SCRAMJ0SEL_MSK) >> TRMP_REG_CFG_BIT_SCRAMJ0SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_SCRAMJ0SEL_get", A, value );

    return value;
}
static INLINE void trmp_field_FRM768SEL_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_FRM768SEL_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_FRM768SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_FRM768SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_FRM768SEL_set", A, value );

    /* (0x00001040 bits 13) field FRM768SEL of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_FRM768SEL_MSK,
                            TRMP_REG_CFG_BIT_FRM768SEL_OFF,
                            value);
}

static INLINE UINT32 trmp_field_FRM768SEL_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_FRM768SEL_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_FRM768SEL_get", A, 15);
    /* (0x00001040 bits 13) field FRM768SEL of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_FRM768SEL_MSK) >> TRMP_REG_CFG_BIT_FRM768SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_FRM768SEL_get", A, value );

    return value;
}
static INLINE void trmp_field_M0EN_set( trmp_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_M0EN_set( trmp_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_M0EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_M0EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_M0EN_set", A, value );

    /* (0x00001040 bits 12) field M0EN of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_M0EN_MSK,
                            TRMP_REG_CFG_BIT_M0EN_OFF,
                            value);
}

static INLINE UINT32 trmp_field_M0EN_get( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_M0EN_get( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_M0EN_get", A, 15);
    /* (0x00001040 bits 12) field M0EN of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_M0EN_MSK) >> TRMP_REG_CFG_BIT_M0EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_M0EN_get", A, value );

    return value;
}
static INLINE void trmp_field_STRACEEN_set( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_STRACEEN_set( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_STRACEEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_STRACEEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_STRACEEN_set", A, value );

    /* (0x00001040 bits 3) field STRACEEN of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_STRACEEN_MSK,
                            TRMP_REG_CFG_BIT_STRACEEN_OFF,
                            value);
}

static INLINE UINT32 trmp_field_STRACEEN_get( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_STRACEEN_get( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_STRACEEN_get", A, 15);
    /* (0x00001040 bits 3) field STRACEEN of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_STRACEEN_MSK) >> TRMP_REG_CFG_BIT_STRACEEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_STRACEEN_get", A, value );

    return value;
}
static INLINE void trmp_field_J0Z0INCEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_J0Z0INCEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0Z0INCEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_J0Z0INCEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_J0Z0INCEN_set", A, value );

    /* (0x00001040 bits 2) field J0Z0INCEN of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_J0Z0INCEN_MSK,
                            TRMP_REG_CFG_BIT_J0Z0INCEN_OFF,
                            value);
}

static INLINE UINT32 trmp_field_J0Z0INCEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_J0Z0INCEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0Z0INCEN_get", A, 15);
    /* (0x00001040 bits 2) field J0Z0INCEN of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_J0Z0INCEN_MSK) >> TRMP_REG_CFG_BIT_J0Z0INCEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_J0Z0INCEN_get", A, value );

    return value;
}
static INLINE void trmp_field_Z0DEF_set( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z0DEF_set( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0DEF_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z0DEF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z0DEF_set", A, value );

    /* (0x00001040 bits 1) field Z0DEF of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_Z0DEF_MSK,
                            TRMP_REG_CFG_BIT_Z0DEF_OFF,
                            value);
}

static INLINE UINT32 trmp_field_Z0DEF_get( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z0DEF_get( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0DEF_get", A, 15);
    /* (0x00001040 bits 1) field Z0DEF of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_Z0DEF_MSK) >> TRMP_REG_CFG_BIT_Z0DEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z0DEF_get", A, value );

    return value;
}
static INLINE void trmp_field_A1A2EN_set( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_A1A2EN_set( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_A1A2EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_A1A2EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_A1A2EN_set", A, value );

    /* (0x00001040 bits 0) field A1A2EN of register PMC_TRMP_REG_CFG */
    trmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            TRMP_REG_CFG_BIT_A1A2EN_MSK,
                            TRMP_REG_CFG_BIT_A1A2EN_OFF,
                            value);
}

static INLINE UINT32 trmp_field_A1A2EN_get( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_A1A2EN_get( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_A1A2EN_get", A, 15);
    /* (0x00001040 bits 0) field A1A2EN of register PMC_TRMP_REG_CFG */
    reg_value = trmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_CFG_BIT_A1A2EN_MSK) >> TRMP_REG_CFG_BIT_A1A2EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_A1A2EN_get", A, value );

    return value;
}
static INLINE void trmp_field_NATIONALEN_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_NATIONALEN_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_NATIONALEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_NATIONALEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_NATIONALEN_set", A, value );

    /* (0x00001044 bits 12) field NATIONALEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_NATIONALEN_MSK,
                                  TRMP_REG_INSERTION_BIT_NATIONALEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_NATIONALEN_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_NATIONALEN_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_NATIONALEN_get", A, 15);
    /* (0x00001044 bits 12) field NATIONALEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_NATIONALEN_MSK) >> TRMP_REG_INSERTION_BIT_NATIONALEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_NATIONALEN_get", A, value );

    return value;
}
static INLINE void trmp_field_E2REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_E2REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E2REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_E2REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_E2REGEN_set", A, value );

    /* (0x00001044 bits 10) field E2REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_E2REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_E2REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_E2REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_E2REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E2REGEN_get", A, 15);
    /* (0x00001044 bits 10) field E2REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_E2REGEN_MSK) >> TRMP_REG_INSERTION_BIT_E2REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_E2REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_Z2REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z2REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z2REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z2REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z2REGEN_set", A, value );

    /* (0x00001044 bits 9) field Z2REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_Z2REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_Z2REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_Z2REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z2REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z2REGEN_get", A, 15);
    /* (0x00001044 bits 9) field Z2REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_Z2REGEN_MSK) >> TRMP_REG_INSERTION_BIT_Z2REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z2REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_Z1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z1REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z1REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z1REGEN_set", A, value );

    /* (0x00001044 bits 8) field Z1REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_Z1REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_Z1REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_Z1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z1REGEN_get", A, 15);
    /* (0x00001044 bits 8) field Z1REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_Z1REGEN_MSK) >> TRMP_REG_INSERTION_BIT_Z1REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z1REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_S1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_S1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_S1REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_S1REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_S1REGEN_set", A, value );

    /* (0x00001044 bits 7) field S1REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_S1REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_S1REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_S1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_S1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_S1REGEN_get", A, 15);
    /* (0x00001044 bits 7) field S1REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_S1REGEN_MSK) >> TRMP_REG_INSERTION_BIT_S1REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_S1REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_D4D12REGEN_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_D4D12REGEN_set( trmp_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D4D12REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_D4D12REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_D4D12REGEN_set", A, value );

    /* (0x00001044 bits 6) field D4D12REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_D4D12REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_D4D12REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_D4D12REGEN_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_D4D12REGEN_get( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D4D12REGEN_get", A, 15);
    /* (0x00001044 bits 6) field D4D12REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_D4D12REGEN_MSK) >> TRMP_REG_INSERTION_BIT_D4D12REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_D4D12REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_K1K2REGEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_K1K2REGEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K1K2REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_K1K2REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_K1K2REGEN_set", A, value );

    /* (0x00001044 bits 5) field K1K2REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_K1K2REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_K1K2REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_K1K2REGEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_K1K2REGEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K1K2REGEN_get", A, 15);
    /* (0x00001044 bits 5) field K1K2REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_K1K2REGEN_MSK) >> TRMP_REG_INSERTION_BIT_K1K2REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_K1K2REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_D1D3REGEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_D1D3REGEN_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D1D3REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_D1D3REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_D1D3REGEN_set", A, value );

    /* (0x00001044 bits 4) field D1D3REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_D1D3REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_D1D3REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_D1D3REGEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_D1D3REGEN_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D1D3REGEN_get", A, 15);
    /* (0x00001044 bits 4) field D1D3REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_D1D3REGEN_MSK) >> TRMP_REG_INSERTION_BIT_D1D3REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_D1D3REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_F1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_F1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_F1REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_F1REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_F1REGEN_set", A, value );

    /* (0x00001044 bits 3) field F1REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_F1REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_F1REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_F1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_F1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_F1REGEN_get", A, 15);
    /* (0x00001044 bits 3) field F1REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_F1REGEN_MSK) >> TRMP_REG_INSERTION_BIT_F1REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_F1REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_E1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_E1REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E1REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_E1REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_E1REGEN_set", A, value );

    /* (0x00001044 bits 2) field E1REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_E1REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_E1REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_E1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_E1REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E1REGEN_get", A, 15);
    /* (0x00001044 bits 2) field E1REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_E1REGEN_MSK) >> TRMP_REG_INSERTION_BIT_E1REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_E1REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_Z0REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z0REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z0REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z0REGEN_set", A, value );

    /* (0x00001044 bits 1) field Z0REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_Z0REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_Z0REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_Z0REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z0REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0REGEN_get", A, 15);
    /* (0x00001044 bits 1) field Z0REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_Z0REGEN_MSK) >> TRMP_REG_INSERTION_BIT_Z0REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z0REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_J0REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_J0REGEN_set( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0REGEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_J0REGEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_J0REGEN_set", A, value );

    /* (0x00001044 bits 0) field J0REGEN of register PMC_TRMP_REG_INSERTION */
    trmp_reg_INSERTION_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  TRMP_REG_INSERTION_BIT_J0REGEN_MSK,
                                  TRMP_REG_INSERTION_BIT_J0REGEN_OFF,
                                  value);
}

static INLINE UINT32 trmp_field_J0REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_J0REGEN_get( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0REGEN_get", A, 15);
    /* (0x00001044 bits 0) field J0REGEN of register PMC_TRMP_REG_INSERTION */
    reg_value = trmp_reg_INSERTION_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_INSERTION_BIT_J0REGEN_MSK) >> TRMP_REG_INSERTION_BIT_J0REGEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_J0REGEN_get", A, value );

    return value;
}
static INLINE void trmp_field_J0V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_J0V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_J0V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_J0V_set", A, value );

    /* (0x0000104c bits 15:8) bits 0:7 use field J0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    trmp_reg_TX_J0_AND_Z0_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_J0_AND_Z0_BIT_J0V_MSK,
                                     TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_J0V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_J0V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_J0V_get", A, 15);
    /* (0x0000104c bits 15:8) bits 0:7 use field J0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    reg_value = trmp_reg_TX_J0_AND_Z0_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_J0_AND_Z0_BIT_J0V_MSK) >> TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_J0V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_J0V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_J0V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_J0V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_J0V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_J0V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_J0V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000104c bits 15:8) bits 0:7 use field J0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
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
        /* (0x0000104c bits 15:8) bits 0:7 use field J0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
        trmp_reg_TX_J0_AND_Z0_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF + subfield_offset),
                                         TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_J0V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_J0V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_J0V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_J0V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_J0V_get", stop_bit, 7 );
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
    /* (0x0000104c bits 15:8) bits 0:7 use field J0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    reg_value = trmp_reg_TX_J0_AND_Z0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_J0_AND_Z0_BIT_J0V_MSK)
                  >> TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_J0_AND_Z0_BIT_J0V_MSK, TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_J0V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_Z0V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z0V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z0V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z0V_set", A, value );

    /* (0x0000104c bits 7:0) bits 0:7 use field Z0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    trmp_reg_TX_J0_AND_Z0_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_MSK,
                                     TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_Z0V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z0V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z0V_get", A, 15);
    /* (0x0000104c bits 7:0) bits 0:7 use field Z0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    reg_value = trmp_reg_TX_J0_AND_Z0_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_MSK) >> TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z0V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_Z0V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_Z0V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z0V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z0V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z0V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z0V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000104c bits 7:0) bits 0:7 use field Z0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
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
        /* (0x0000104c bits 7:0) bits 0:7 use field Z0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
        trmp_reg_TX_J0_AND_Z0_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF + subfield_offset),
                                         TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_Z0V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_Z0V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z0V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z0V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z0V_get", stop_bit, 7 );
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
    /* (0x0000104c bits 7:0) bits 0:7 use field Z0V of register PMC_TRMP_REG_TX_J0_AND_Z0 */
    reg_value = trmp_reg_TX_J0_AND_Z0_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_MSK)
                  >> TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_MSK, TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z0V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_E1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_E1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E1V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_E1V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_E1V_set", A, value );

    /* (0x00001050 bits 15:8) bits 0:7 use field E1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    trmp_reg_TX_E1_AND_F1_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_E1_AND_F1_BIT_E1V_MSK,
                                     TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_E1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_E1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E1V_get", A, 15);
    /* (0x00001050 bits 15:8) bits 0:7 use field E1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    reg_value = trmp_reg_TX_E1_AND_F1_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_E1_AND_F1_BIT_E1V_MSK) >> TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_E1V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_E1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_E1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_E1V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_E1V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_E1V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_E1V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001050 bits 15:8) bits 0:7 use field E1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
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
        /* (0x00001050 bits 15:8) bits 0:7 use field E1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
        trmp_reg_TX_E1_AND_F1_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF + subfield_offset),
                                         TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_E1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_E1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_E1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_E1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_E1V_get", stop_bit, 7 );
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
    /* (0x00001050 bits 15:8) bits 0:7 use field E1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    reg_value = trmp_reg_TX_E1_AND_F1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_E1_AND_F1_BIT_E1V_MSK)
                  >> TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_E1_AND_F1_BIT_E1V_MSK, TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_E1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_F1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_F1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_F1V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_F1V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_F1V_set", A, value );

    /* (0x00001050 bits 7:0) bits 0:7 use field F1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    trmp_reg_TX_E1_AND_F1_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_E1_AND_F1_BIT_F1V_MSK,
                                     TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_F1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_F1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_F1V_get", A, 15);
    /* (0x00001050 bits 7:0) bits 0:7 use field F1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    reg_value = trmp_reg_TX_E1_AND_F1_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_E1_AND_F1_BIT_F1V_MSK) >> TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_F1V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_F1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_F1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_F1V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_F1V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_F1V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_F1V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001050 bits 7:0) bits 0:7 use field F1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
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
        /* (0x00001050 bits 7:0) bits 0:7 use field F1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
        trmp_reg_TX_E1_AND_F1_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF + subfield_offset),
                                         TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_F1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_F1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_F1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_F1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_F1V_get", stop_bit, 7 );
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
    /* (0x00001050 bits 7:0) bits 0:7 use field F1V of register PMC_TRMP_REG_TX_E1_AND_F1 */
    reg_value = trmp_reg_TX_E1_AND_F1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_E1_AND_F1_BIT_F1V_MSK)
                  >> TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_E1_AND_F1_BIT_F1V_MSK, TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_F1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_D1D3V_set( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_D1D3V_set( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D1D3V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_D1D3V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_D1D3V_set", A, value );

    /* (0x00001054 bits 15:8) bits 0:7 use field D1D3V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    trmp_reg_TX_D1D3_AND_D4D12_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_MSK,
                                          TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF,
                                          value);
}

static INLINE UINT32 trmp_field_D1D3V_get( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_D1D3V_get( trmp_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D1D3V_get", A, 15);
    /* (0x00001054 bits 15:8) bits 0:7 use field D1D3V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    reg_value = trmp_reg_TX_D1D3_AND_D4D12_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_MSK) >> TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_D1D3V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_D1D3V_set( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_D1D3V_set( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_D1D3V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_D1D3V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_D1D3V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_D1D3V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001054 bits 15:8) bits 0:7 use field D1D3V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
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
        /* (0x00001054 bits 15:8) bits 0:7 use field D1D3V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
        trmp_reg_TX_D1D3_AND_D4D12_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF + subfield_offset),
                                              TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_D1D3V_get( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_D1D3V_get( trmp_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_D1D3V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_D1D3V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_D1D3V_get", stop_bit, 7 );
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
    /* (0x00001054 bits 15:8) bits 0:7 use field D1D3V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    reg_value = trmp_reg_TX_D1D3_AND_D4D12_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_MSK)
                  >> TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_MSK, TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_D1D3V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_D4D12V_set( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_D4D12V_set( trmp_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D4D12V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_D4D12V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_D4D12V_set", A, value );

    /* (0x00001054 bits 7:0) bits 0:7 use field D4D12V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    trmp_reg_TX_D1D3_AND_D4D12_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_MSK,
                                          TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF,
                                          value);
}

static INLINE UINT32 trmp_field_D4D12V_get( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_D4D12V_get( trmp_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_D4D12V_get", A, 15);
    /* (0x00001054 bits 7:0) bits 0:7 use field D4D12V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    reg_value = trmp_reg_TX_D1D3_AND_D4D12_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_MSK) >> TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_D4D12V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_D4D12V_set( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_D4D12V_set( trmp_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_D4D12V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_D4D12V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_D4D12V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_D4D12V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001054 bits 7:0) bits 0:7 use field D4D12V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
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
        /* (0x00001054 bits 7:0) bits 0:7 use field D4D12V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
        trmp_reg_TX_D1D3_AND_D4D12_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF + subfield_offset),
                                              TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_D4D12V_get( trmp_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_D4D12V_get( trmp_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_D4D12V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_D4D12V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_D4D12V_get", stop_bit, 7 );
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
    /* (0x00001054 bits 7:0) bits 0:7 use field D4D12V of register PMC_TRMP_REG_TX_D1D3_AND_D4D12 */
    reg_value = trmp_reg_TX_D1D3_AND_D4D12_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_MSK)
                  >> TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_MSK, TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_D4D12V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_K1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_K1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K1V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_K1V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_K1V_set", A, value );

    /* (0x00001058 bits 15:8) bits 0:7 use field K1V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    trmp_reg_TX_K1_AND_K2_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_K1_AND_K2_BIT_K1V_MSK,
                                     TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_K1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_K1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K1V_get", A, 15);
    /* (0x00001058 bits 15:8) bits 0:7 use field K1V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    reg_value = trmp_reg_TX_K1_AND_K2_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_K1_AND_K2_BIT_K1V_MSK) >> TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_K1V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_K1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_K1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_K1V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_K1V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_K1V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_K1V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001058 bits 15:8) bits 0:7 use field K1V of register PMC_TRMP_REG_TX_K1_AND_K2 */
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
        /* (0x00001058 bits 15:8) bits 0:7 use field K1V of register PMC_TRMP_REG_TX_K1_AND_K2 */
        trmp_reg_TX_K1_AND_K2_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF + subfield_offset),
                                         TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_K1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_K1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_K1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_K1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_K1V_get", stop_bit, 7 );
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
    /* (0x00001058 bits 15:8) bits 0:7 use field K1V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    reg_value = trmp_reg_TX_K1_AND_K2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_K1_AND_K2_BIT_K1V_MSK)
                  >> TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_K1_AND_K2_BIT_K1V_MSK, TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_K1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_K2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_K2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K2V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_K2V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_K2V_set", A, value );

    /* (0x00001058 bits 7:0) bits 0:7 use field K2V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    trmp_reg_TX_K1_AND_K2_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_K1_AND_K2_BIT_K2V_MSK,
                                     TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_K2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_K2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_K2V_get", A, 15);
    /* (0x00001058 bits 7:0) bits 0:7 use field K2V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    reg_value = trmp_reg_TX_K1_AND_K2_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_K1_AND_K2_BIT_K2V_MSK) >> TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_K2V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_K2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_K2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_K2V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_K2V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_K2V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_K2V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001058 bits 7:0) bits 0:7 use field K2V of register PMC_TRMP_REG_TX_K1_AND_K2 */
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
        /* (0x00001058 bits 7:0) bits 0:7 use field K2V of register PMC_TRMP_REG_TX_K1_AND_K2 */
        trmp_reg_TX_K1_AND_K2_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF + subfield_offset),
                                         TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_K2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_K2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_K2V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_K2V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_K2V_get", stop_bit, 7 );
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
    /* (0x00001058 bits 7:0) bits 0:7 use field K2V of register PMC_TRMP_REG_TX_K1_AND_K2 */
    reg_value = trmp_reg_TX_K1_AND_K2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_K1_AND_K2_BIT_K2V_MSK)
                  >> TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_K1_AND_K2_BIT_K2V_MSK, TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_K2V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_S1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_S1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_S1V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_S1V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_S1V_set", A, value );

    /* (0x0000105c bits 15:8) bits 0:7 use field S1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    trmp_reg_TX_S1_AND_Z1_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_S1_AND_Z1_BIT_S1V_MSK,
                                     TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_S1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_S1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_S1V_get", A, 15);
    /* (0x0000105c bits 15:8) bits 0:7 use field S1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    reg_value = trmp_reg_TX_S1_AND_Z1_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_S1_AND_Z1_BIT_S1V_MSK) >> TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_S1V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_S1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_S1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_S1V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_S1V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_S1V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_S1V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000105c bits 15:8) bits 0:7 use field S1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
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
        /* (0x0000105c bits 15:8) bits 0:7 use field S1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
        trmp_reg_TX_S1_AND_Z1_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF + subfield_offset),
                                         TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_S1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_S1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_S1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_S1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_S1V_get", stop_bit, 7 );
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
    /* (0x0000105c bits 15:8) bits 0:7 use field S1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    reg_value = trmp_reg_TX_S1_AND_Z1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_S1_AND_Z1_BIT_S1V_MSK)
                  >> TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_S1_AND_Z1_BIT_S1V_MSK, TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_S1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_Z1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z1V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z1V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z1V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z1V_set", A, value );

    /* (0x0000105c bits 7:0) bits 0:7 use field Z1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    trmp_reg_TX_S1_AND_Z1_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_MSK,
                                     TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_Z1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z1V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z1V_get", A, 15);
    /* (0x0000105c bits 7:0) bits 0:7 use field Z1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    reg_value = trmp_reg_TX_S1_AND_Z1_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_MSK) >> TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z1V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_Z1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_Z1V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z1V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z1V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z1V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z1V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000105c bits 7:0) bits 0:7 use field Z1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
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
        /* (0x0000105c bits 7:0) bits 0:7 use field Z1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
        trmp_reg_TX_S1_AND_Z1_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF + subfield_offset),
                                         TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_Z1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_Z1V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z1V_get", stop_bit, 7 );
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
    /* (0x0000105c bits 7:0) bits 0:7 use field Z1V of register PMC_TRMP_REG_TX_S1_AND_Z1 */
    reg_value = trmp_reg_TX_S1_AND_Z1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_MSK)
                  >> TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_MSK, TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_Z2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_Z2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z2V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_Z2V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_Z2V_set", A, value );

    /* (0x00001060 bits 15:8) bits 0:7 use field Z2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    trmp_reg_TX_Z2_AND_E2_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_MSK,
                                     TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_Z2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_Z2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_Z2V_get", A, 15);
    /* (0x00001060 bits 15:8) bits 0:7 use field Z2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    reg_value = trmp_reg_TX_Z2_AND_E2_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_MSK) >> TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_Z2V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_Z2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_Z2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z2V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z2V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z2V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z2V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001060 bits 15:8) bits 0:7 use field Z2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
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
        /* (0x00001060 bits 15:8) bits 0:7 use field Z2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
        trmp_reg_TX_Z2_AND_E2_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF + subfield_offset),
                                         TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_Z2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_Z2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_Z2V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_Z2V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_Z2V_get", stop_bit, 7 );
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
    /* (0x00001060 bits 15:8) bits 0:7 use field Z2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    reg_value = trmp_reg_TX_Z2_AND_E2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_MSK)
                  >> TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_MSK, TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_Z2V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_E2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_E2V_set( trmp_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E2V_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_E2V_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_E2V_set", A, value );

    /* (0x00001060 bits 7:0) bits 0:7 use field E2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    trmp_reg_TX_Z2_AND_E2_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     TRMP_REG_TX_Z2_AND_E2_BIT_E2V_MSK,
                                     TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF,
                                     value);
}

static INLINE UINT32 trmp_field_E2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_E2V_get( trmp_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_E2V_get", A, 15);
    /* (0x00001060 bits 7:0) bits 0:7 use field E2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    reg_value = trmp_reg_TX_Z2_AND_E2_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_Z2_AND_E2_BIT_E2V_MSK) >> TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_E2V_get", A, value );

    return value;
}
static INLINE void trmp_field_range_E2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_E2V_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_E2V_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_E2V_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_E2V_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_E2V_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001060 bits 7:0) bits 0:7 use field E2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
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
        /* (0x00001060 bits 7:0) bits 0:7 use field E2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
        trmp_reg_TX_Z2_AND_E2_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF + subfield_offset),
                                         TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_E2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_E2V_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_E2V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_E2V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_E2V_get", stop_bit, 7 );
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
    /* (0x00001060 bits 7:0) bits 0:7 use field E2V of register PMC_TRMP_REG_TX_Z2_AND_E2 */
    reg_value = trmp_reg_TX_Z2_AND_E2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_Z2_AND_E2_BIT_E2V_MSK)
                  >> TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_Z2_AND_E2_BIT_E2V_MSK, TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_E2V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void trmp_field_NATIONALV_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_NATIONALV_set( trmp_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_NATIONALV_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "trmp_field_NATIONALV_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "trmp_field_NATIONALV_set", A, value );

    /* (0x0000106c bits 7:0) bits 0:7 use field NATIONALV of register PMC_TRMP_REG_TX_NATIONAL */
    trmp_reg_TX_NATIONAL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    TRMP_REG_TX_NATIONAL_BIT_NATIONALV_MSK,
                                    TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF,
                                    value);
}

static INLINE UINT32 trmp_field_NATIONALV_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_NATIONALV_get( trmp_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_NATIONALV_get", A, 15);
    /* (0x0000106c bits 7:0) bits 0:7 use field NATIONALV of register PMC_TRMP_REG_TX_NATIONAL */
    reg_value = trmp_reg_TX_NATIONAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & TRMP_REG_TX_NATIONAL_BIT_NATIONALV_MSK) >> TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "trmp_field_NATIONALV_get", A, value );

    return value;
}
static INLINE void trmp_field_range_NATIONALV_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void trmp_field_range_NATIONALV_set( trmp_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_NATIONALV_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_NATIONALV_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_NATIONALV_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_NATIONALV_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000106c bits 7:0) bits 0:7 use field NATIONALV of register PMC_TRMP_REG_TX_NATIONAL */
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
        /* (0x0000106c bits 7:0) bits 0:7 use field NATIONALV of register PMC_TRMP_REG_TX_NATIONAL */
        trmp_reg_TX_NATIONAL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF + subfield_offset),
                                        TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 trmp_field_range_NATIONALV_get( trmp_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 trmp_field_range_NATIONALV_get( trmp_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "trmp_field_range_NATIONALV_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "trmp_field_range_NATIONALV_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "trmp_field_range_NATIONALV_get", stop_bit, 7 );
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
    /* (0x0000106c bits 7:0) bits 0:7 use field NATIONALV of register PMC_TRMP_REG_TX_NATIONAL */
    reg_value = trmp_reg_TX_NATIONAL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & TRMP_REG_TX_NATIONAL_BIT_NATIONALV_MSK)
                  >> TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TRMP_REG_TX_NATIONAL_BIT_NATIONALV_MSK, TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "trmp_field_range_NATIONALV_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TRMP_IO_INLINE_H */
