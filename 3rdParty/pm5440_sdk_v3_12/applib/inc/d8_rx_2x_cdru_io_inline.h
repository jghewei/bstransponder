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
 *     and register accessor functions for the d8_rx_2x_cdru block
 *****************************************************************************/
#ifndef _D8_RX_2X_CDRU_IO_INLINE_H
#define _D8_RX_2X_CDRU_IO_INLINE_H

#include "d8_rx_2x.h"
#include "d8_rx_2x_cdru_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define D8_RX_2X_CDRU_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for d8_rx_2x_cdru
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
    d8_rx_2x_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} d8_rx_2x_cdru_buffer_t;
static INLINE void d8_rx_2x_cdru_buffer_init( d8_rx_2x_cdru_buffer_t *b_ptr,
                                              d8_rx_2x_handle_t *h_ptr,
                                              UINT32 A ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_buffer_init( d8_rx_2x_cdru_buffer_t *b_ptr,
                                              d8_rx_2x_handle_t *h_ptr,
                                              UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "d8_rx_2x_cdru_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void d8_rx_2x_cdru_buffer_flush( d8_rx_2x_cdru_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_buffer_flush( d8_rx_2x_cdru_buffer_t *b_ptr )
{
    IOLOG( "d8_rx_2x_cdru_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 d8_rx_2x_cdru_reg_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                             d8_rx_2x_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_reg_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                             d8_rx_2x_handle_t *h_ptr,
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
static INLINE void d8_rx_2x_cdru_reg_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                            d8_rx_2x_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_reg_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                            d8_rx_2x_handle_t *h_ptr,
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

static INLINE void d8_rx_2x_cdru_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                            d8_rx_2x_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 mask,
                                            UINT32 unused_mask,
                                            UINT32 ofs,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                            d8_rx_2x_handle_t *h_ptr,
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

static INLINE void d8_rx_2x_cdru_action_on_write_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                            d8_rx_2x_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mem_type,
                                                            UINT32 reg,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_action_on_write_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                            d8_rx_2x_handle_t *h_ptr,
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

static INLINE void d8_rx_2x_cdru_burst_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                             d8_rx_2x_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_burst_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                             d8_rx_2x_handle_t *h_ptr,
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

static INLINE void d8_rx_2x_cdru_burst_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                              d8_rx_2x_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 len,
                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_burst_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                              d8_rx_2x_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE d8_rx_2x_cdru_poll( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                       d8_rx_2x_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 value,
                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                       UINT32 max_count,
                                                       UINT32 *num_failed_polls,
                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_rx_2x_cdru_poll( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                       d8_rx_2x_handle_t *h_ptr,
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
 *  register access functions for d8_rx_2x_cdru
 * ==================================================================================
 */

static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                              d8_rx_2x_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                              d8_rx_2x_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_write", value );
    d8_rx_2x_cdru_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1,
                             value);
}

static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                                  d8_rx_2x_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                                  d8_rx_2x_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set", A, mask, ofs, value );
    d8_rx_2x_cdru_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1,
                             mask,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                               d8_rx_2x_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                               d8_rx_2x_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = d8_rx_2x_cdru_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1);

    IOLOG( "%s -> 0x%08x; A=%d", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read", reg_value, A);
    return reg_value;
}

static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                              d8_rx_2x_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_write( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                              d8_rx_2x_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_write", value );
    d8_rx_2x_cdru_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4,
                             value);
}

static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                                  d8_rx_2x_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                                  d8_rx_2x_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set", A, mask, ofs, value );
    d8_rx_2x_cdru_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4,
                             mask,
                             PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                               d8_rx_2x_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                               d8_rx_2x_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = d8_rx_2x_cdru_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4);

    IOLOG( "%s -> 0x%08x; A=%d", "d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void d8_rx_2x_cdru_field_FILT_EN_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_FILT_EN_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_FILT_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_FILT_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_FILT_EN_set", A, value );

    /* (0x00000200 bits 18) field FILT_EN of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_FILT_EN_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_FILT_EN_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_FILT_EN_get", A, 1);
    /* (0x00000200 bits 18) field FILT_EN of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_FILT_EN_get", A, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_IPD_EN_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                   d8_rx_2x_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_IPD_EN_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                   d8_rx_2x_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_IPD_EN_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_IPD_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_IPD_EN_set", A, value );

    /* (0x00000200 bits 17) field IPD_EN of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_IPD_EN_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                     d8_rx_2x_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_IPD_EN_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                     d8_rx_2x_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_IPD_EN_get", A, 1);
    /* (0x00000200 bits 17) field IPD_EN of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_IPD_EN_get", A, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_INV_REC_DATA_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                         d8_rx_2x_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_INV_REC_DATA_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                         d8_rx_2x_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_INV_REC_DATA_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_INV_REC_DATA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_INV_REC_DATA_set", A, value );

    /* (0x00000200 bits 16) field INV_REC_DATA of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_INV_REC_DATA_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                           d8_rx_2x_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_INV_REC_DATA_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                           d8_rx_2x_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_INV_REC_DATA_get", A, 1);
    /* (0x00000200 bits 16) field INV_REC_DATA of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_INV_REC_DATA_get", A, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_TED_SEL_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_TED_SEL_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_TED_SEL_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_TED_SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_TED_SEL_set", A, value );

    /* (0x00000200 bits 1) field TED_SEL of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_TED_SEL_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_TED_SEL_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_TED_SEL_get", A, 1);
    /* (0x00000200 bits 1) field TED_SEL of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_TED_SEL_get", A, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_LOS_CNT_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_LOS_CNT_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                    d8_rx_2x_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_LOS_CNT_set", A, 1);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_LOS_CNT_set", value, 1023);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_LOS_CNT_set", A, value );

    /* (0x0000020c bits 30:21) field LOS_CNT of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_LOS_CNT_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_LOS_CNT_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                      d8_rx_2x_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_LOS_CNT_get", A, 1);
    /* (0x0000020c bits 30:21) field LOS_CNT of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_LOS_CNT_get", A, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_range_LOS_CNT_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                          d8_rx_2x_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_range_LOS_CNT_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                          d8_rx_2x_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_range_LOS_CNT_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_rx_2x_cdru_field_range_LOS_CNT_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_rx_2x_cdru_field_range_LOS_CNT_set", stop_bit, 9 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "d8_rx_2x_cdru_field_range_LOS_CNT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 30:21) field LOS_CNT of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
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
        /* (0x0000020c bits 30:21) field LOS_CNT of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
        d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF + subfield_offset),
                                                       DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 d8_rx_2x_cdru_field_range_LOS_CNT_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                            d8_rx_2x_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_range_LOS_CNT_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                            d8_rx_2x_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_range_LOS_CNT_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_rx_2x_cdru_field_range_LOS_CNT_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_rx_2x_cdru_field_range_LOS_CNT_get", stop_bit, 9 );
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
    /* (0x0000020c bits 30:21) field LOS_CNT of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_MSK)
                  >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_MSK, DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "d8_rx_2x_cdru_field_range_LOS_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_rx_2x_cdru_field_LOS_ENABLE_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                       d8_rx_2x_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_rx_2x_cdru_field_LOS_ENABLE_set( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                       d8_rx_2x_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_LOS_ENABLE_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_rx_2x_cdru_field_LOS_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "d8_rx_2x_cdru_field_LOS_ENABLE_set", A, value );

    /* (0x0000020c bits 20) field LOS_ENABLE of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
    d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_MSK,
                                                   DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 d8_rx_2x_cdru_field_LOS_ENABLE_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                         d8_rx_2x_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_rx_2x_cdru_field_LOS_ENABLE_get( d8_rx_2x_cdru_buffer_t *b_ptr,
                                                         d8_rx_2x_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "d8_rx_2x_cdru_field_LOS_ENABLE_get", A, 1);
    /* (0x0000020c bits 20) field LOS_ENABLE of register PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 */
    reg_value = d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_MSK) >> DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "d8_rx_2x_cdru_field_LOS_ENABLE_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _D8_RX_2X_CDRU_IO_INLINE_H */
