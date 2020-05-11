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
 *     and register accessor functions for the rttp block
 *****************************************************************************/
#ifndef _RTTP_IO_INLINE_H
#define _RTTP_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "rttp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RTTP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rttp
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
    sdh_pmg_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} rttp_buffer_t;
static INLINE void rttp_buffer_init( rttp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void rttp_buffer_init( rttp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "rttp_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rttp_buffer_flush( rttp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_buffer_flush( rttp_buffer_t *b_ptr )
{
    IOLOG( "rttp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rttp_reg_read( rttp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rttp_reg_read( rttp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
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
static INLINE void rttp_reg_write( rttp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_reg_write( rttp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
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

static INLINE void rttp_field_set( rttp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_set( rttp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
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

static INLINE void rttp_action_on_write_field_set( rttp_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_action_on_write_field_set( rttp_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
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

static INLINE void rttp_burst_read( rttp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rttp_burst_read( rttp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
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

static INLINE void rttp_burst_write( rttp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rttp_burst_write( rttp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE rttp_poll( rttp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rttp_poll( rttp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
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
 *  register access functions for rttp
 * ==================================================================================
 */

static INLINE void rttp_reg_IND_ADR_write( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_reg_IND_ADR_write( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rttp_reg_IND_ADR_write", value );
    rttp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RTTP_REG_IND_ADR,
                    value);
}

static INLINE void rttp_reg_IND_ADR_field_set( rttp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_reg_IND_ADR_field_set( rttp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rttp_reg_IND_ADR_field_set", A, mask, ofs, value );
    rttp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RTTP_REG_IND_ADR,
                    mask,
                    PMC_RTTP_REG_IND_ADR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rttp_reg_IND_ADR_read( rttp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_reg_IND_ADR_read( rttp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = rttp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RTTP_REG_IND_ADR);

    IOLOG( "%s -> 0x%08x; A=%d", "rttp_reg_IND_ADR_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rttp_reg_IND_ADR_poll( rttp_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 value,
                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                          UINT32 max_count,
                                                          UINT32 *num_failed_polls,
                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rttp_reg_IND_ADR_poll( rttp_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 value,
                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                          UINT32 max_count,
                                                          UINT32 *num_failed_polls,
                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rttp_reg_IND_ADR_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rttp_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_RTTP_REG_IND_ADR,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rttp_reg_IND_DATA_write( rttp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_reg_IND_DATA_write( rttp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rttp_reg_IND_DATA_write", value );
    rttp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RTTP_REG_IND_DATA,
                    value);
}

static INLINE void rttp_reg_IND_DATA_field_set( rttp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_reg_IND_DATA_field_set( rttp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rttp_reg_IND_DATA_field_set", A, mask, ofs, value );
    rttp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RTTP_REG_IND_DATA,
                    mask,
                    PMC_RTTP_REG_IND_DATA_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rttp_reg_IND_DATA_read( rttp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_reg_IND_DATA_read( rttp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = rttp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RTTP_REG_IND_DATA);

    IOLOG( "%s -> 0x%08x; A=%d", "rttp_reg_IND_DATA_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rttp_reg_PATH_INT_STAT_1_read( rttp_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_reg_PATH_INT_STAT_1_read( rttp_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = rttp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RTTP_REG_PATH_INT_STAT_1);

    IOLOG( "%s -> 0x%08x; A=%d", "rttp_reg_PATH_INT_STAT_1_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void rttp_field_RWB_set( rttp_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_RWB_set( rttp_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_RWB_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_field_RWB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rttp_field_RWB_set", A, value );

    /* (0x00000500 bits 14) field RWB of register PMC_RTTP_REG_IND_ADR */
    rttp_reg_IND_ADR_field_set( b_ptr,
                                h_ptr,
                                A,
                                RTTP_REG_IND_ADR_BIT_RWB_MSK,
                                RTTP_REG_IND_ADR_BIT_RWB_OFF,
                                value);
}

static INLINE UINT32 rttp_field_RWB_get( rttp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_RWB_get( rttp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_RWB_get", A, 3);
    /* (0x00000500 bits 14) field RWB of register PMC_RTTP_REG_IND_ADR */
    reg_value = rttp_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & RTTP_REG_IND_ADR_BIT_RWB_MSK) >> RTTP_REG_IND_ADR_BIT_RWB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rttp_field_RWB_get", A, value );

    return value;
}
static INLINE void rttp_field_IADDR_set( rttp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_IADDR_set( rttp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_IADDR_set", A, 3);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_field_IADDR_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "rttp_field_IADDR_set", A, value );

    /* (0x00000500 bits 13:6) bits 0:7 use field IADDR of register PMC_RTTP_REG_IND_ADR */
    rttp_reg_IND_ADR_field_set( b_ptr,
                                h_ptr,
                                A,
                                RTTP_REG_IND_ADR_BIT_IADDR_MSK,
                                RTTP_REG_IND_ADR_BIT_IADDR_OFF,
                                value);
}

static INLINE UINT32 rttp_field_IADDR_get( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_IADDR_get( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_IADDR_get", A, 3);
    /* (0x00000500 bits 13:6) bits 0:7 use field IADDR of register PMC_RTTP_REG_IND_ADR */
    reg_value = rttp_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & RTTP_REG_IND_ADR_BIT_IADDR_MSK) >> RTTP_REG_IND_ADR_BIT_IADDR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rttp_field_IADDR_get", A, value );

    return value;
}
static INLINE void rttp_field_range_IADDR_set( rttp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_range_IADDR_set( rttp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_range_IADDR_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rttp_field_range_IADDR_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rttp_field_range_IADDR_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "rttp_field_range_IADDR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000500 bits 13:6) bits 0:7 use field IADDR of register PMC_RTTP_REG_IND_ADR */
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
        /* (0x00000500 bits 13:6) bits 0:7 use field IADDR of register PMC_RTTP_REG_IND_ADR */
        rttp_reg_IND_ADR_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    subfield_mask << (RTTP_REG_IND_ADR_BIT_IADDR_OFF + subfield_offset),
                                    RTTP_REG_IND_ADR_BIT_IADDR_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rttp_field_range_IADDR_get( rttp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_range_IADDR_get( rttp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_range_IADDR_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rttp_field_range_IADDR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rttp_field_range_IADDR_get", stop_bit, 7 );
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
    /* (0x00000500 bits 13:6) bits 0:7 use field IADDR of register PMC_RTTP_REG_IND_ADR */
    reg_value = rttp_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RTTP_REG_IND_ADR_BIT_IADDR_MSK)
                  >> RTTP_REG_IND_ADR_BIT_IADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RTTP_REG_IND_ADR_BIT_IADDR_MSK, RTTP_REG_IND_ADR_BIT_IADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rttp_field_range_IADDR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void rttp_field_DATA_set( rttp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_DATA_set( rttp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_DATA_set", A, 3);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_field_DATA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "rttp_field_DATA_set", A, value );

    /* (0x00000504 bits 15:0) bits 0:15 use field DATA of register PMC_RTTP_REG_IND_DATA */
    rttp_reg_IND_DATA_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RTTP_REG_IND_DATA_BIT_DATA_MSK,
                                 RTTP_REG_IND_DATA_BIT_DATA_OFF,
                                 value);
}

static INLINE UINT32 rttp_field_DATA_get( rttp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_DATA_get( rttp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_DATA_get", A, 3);
    /* (0x00000504 bits 15:0) bits 0:15 use field DATA of register PMC_RTTP_REG_IND_DATA */
    reg_value = rttp_reg_IND_DATA_read(  b_ptr, h_ptr, A);
    value = (reg_value & RTTP_REG_IND_DATA_BIT_DATA_MSK) >> RTTP_REG_IND_DATA_BIT_DATA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rttp_field_DATA_get", A, value );

    return value;
}
static INLINE void rttp_field_range_DATA_set( rttp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_field_range_DATA_set( rttp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_range_DATA_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rttp_field_range_DATA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rttp_field_range_DATA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "rttp_field_range_DATA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000504 bits 15:0) bits 0:15 use field DATA of register PMC_RTTP_REG_IND_DATA */
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
        /* (0x00000504 bits 15:0) bits 0:15 use field DATA of register PMC_RTTP_REG_IND_DATA */
        rttp_reg_IND_DATA_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (RTTP_REG_IND_DATA_BIT_DATA_OFF + subfield_offset),
                                     RTTP_REG_IND_DATA_BIT_DATA_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 rttp_field_range_DATA_get( rttp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_range_DATA_get( rttp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_range_DATA_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rttp_field_range_DATA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rttp_field_range_DATA_get", stop_bit, 15 );
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
    /* (0x00000504 bits 15:0) bits 0:15 use field DATA of register PMC_RTTP_REG_IND_DATA */
    reg_value = rttp_reg_IND_DATA_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RTTP_REG_IND_DATA_BIT_DATA_MSK)
                  >> RTTP_REG_IND_DATA_BIT_DATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RTTP_REG_IND_DATA_BIT_DATA_MSK, RTTP_REG_IND_DATA_BIT_DATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rttp_field_range_DATA_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 rttp_field_BUSY_get( rttp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_BUSY_get( rttp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_BUSY_get", A, 3);
    /* (0x00000500 bits 15) field BUSY of register PMC_RTTP_REG_IND_ADR */
    reg_value = rttp_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & RTTP_REG_IND_ADR_BIT_BUSY_MSK) >> RTTP_REG_IND_ADR_BIT_BUSY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rttp_field_BUSY_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rttp_field_BUSY_poll( rttp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rttp_field_BUSY_poll( rttp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "rttp_field_BUSY_poll", A, value );

    /* (0x00000500 bits 15) field BUSY of register PMC_RTTP_REG_IND_ADR */
    return rttp_reg_IND_ADR_poll( b_ptr,
                                  h_ptr,
                                  A,
                                  RTTP_REG_IND_ADR_BIT_BUSY_MSK,
                                  (value<<RTTP_REG_IND_ADR_BIT_BUSY_OFF),
                                  cmp,
                                  max_count,
                                  num_failed_polls,
                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 rttp_field_S_INT_get( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_field_S_INT_get( rttp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rttp_field_S_INT_get", A, 3);
    /* (0x00000508 bits 0) field S_INT of register PMC_RTTP_REG_PATH_INT_STAT_1 */
    reg_value = rttp_reg_PATH_INT_STAT_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & RTTP_REG_PATH_INT_STAT_1_BIT_S_INT_MSK) >> RTTP_REG_PATH_INT_STAT_1_BIT_S_INT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rttp_field_S_INT_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTTP_IO_INLINE_H */
