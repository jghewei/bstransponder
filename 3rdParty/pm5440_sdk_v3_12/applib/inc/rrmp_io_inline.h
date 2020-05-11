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
 *     and register accessor functions for the rrmp block
 *****************************************************************************/
#ifndef _RRMP_IO_INLINE_H
#define _RRMP_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "rrmp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RRMP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rrmp
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
} rrmp_buffer_t;
static INLINE void rrmp_buffer_init( rrmp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void rrmp_buffer_init( rrmp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "rrmp_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rrmp_buffer_flush( rrmp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rrmp_buffer_flush( rrmp_buffer_t *b_ptr )
{
    IOLOG( "rrmp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rrmp_reg_read( rrmp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_read( rrmp_buffer_t *b_ptr,
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
static INLINE void rrmp_reg_write( rrmp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_write( rrmp_buffer_t *b_ptr,
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

static INLINE void rrmp_field_set( rrmp_buffer_t *b_ptr,
                                   sdh_pmg_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_set( rrmp_buffer_t *b_ptr,
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

static INLINE void rrmp_action_on_write_field_set( rrmp_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_action_on_write_field_set( rrmp_buffer_t *b_ptr,
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

static INLINE void rrmp_burst_read( rrmp_buffer_t *b_ptr,
                                    sdh_pmg_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rrmp_burst_read( rrmp_buffer_t *b_ptr,
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

static INLINE void rrmp_burst_write( rrmp_buffer_t *b_ptr,
                                     sdh_pmg_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rrmp_burst_write( rrmp_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE rrmp_poll( rrmp_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE rrmp_poll( rrmp_buffer_t *b_ptr,
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
 *  register access functions for rrmp
 * ==================================================================================
 */

static INLINE void rrmp_reg_CFG_write( rrmp_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_CFG_write( rrmp_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rrmp_reg_CFG_write", value );
    rrmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_CFG,
                    value);
}

static INLINE void rrmp_reg_CFG_field_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_CFG_field_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mask,
                                           UINT32 ofs,
                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rrmp_reg_CFG_field_set", A, mask, ofs, value );
    rrmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_CFG,
                    mask,
                    PMC_RRMP_REG_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rrmp_reg_CFG_read( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_CFG_read( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RRMP_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_CFG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rrmp_reg_CFG_poll( rrmp_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rrmp_reg_CFG_poll( rrmp_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rrmp_reg_CFG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rrmp_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_RRMP_REG_CFG,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rrmp_reg_CFG_2_write( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_CFG_2_write( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rrmp_reg_CFG_2_write", value );
    rrmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_CFG_2,
                    value);
}

static INLINE void rrmp_reg_CFG_2_field_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_CFG_2_field_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rrmp_reg_CFG_2_field_set", A, mask, ofs, value );
    rrmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_CFG_2,
                    mask,
                    PMC_RRMP_REG_CFG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rrmp_reg_CFG_2_read( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_CFG_2_read( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RRMP_REG_CFG_2);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_CFG_2_read", reg_value, A);
    return reg_value;
}

static INLINE void rrmp_reg_INT_EN_write( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_INT_EN_write( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rrmp_reg_INT_EN_write", value );
    rrmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_INT_EN,
                    value);
}

static INLINE void rrmp_reg_INT_EN_field_set( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_INT_EN_field_set( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rrmp_reg_INT_EN_field_set", A, mask, ofs, value );
    rrmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_INT_EN,
                    mask,
                    PMC_RRMP_REG_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rrmp_reg_INT_EN_read( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_INT_EN_read( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RRMP_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void rrmp_reg_RXD_SSM_write( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_RXD_SSM_write( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rrmp_reg_RXD_SSM_write", value );
    rrmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_RXD_SSM,
                    value);
}

static INLINE void rrmp_reg_RXD_SSM_field_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_RXD_SSM_field_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rrmp_reg_RXD_SSM_field_set", A, mask, ofs, value );
    rrmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_RRMP_REG_RXD_SSM,
                    mask,
                    PMC_RRMP_REG_RXD_SSM_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rrmp_reg_RXD_SSM_read( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_RXD_SSM_read( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_RRMP_REG_RXD_SSM);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_RXD_SSM_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_STAT_read( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_STAT_read( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void rrmp_reg_INT_STAT_write( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_INT_STAT_write( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rrmp_reg_INT_STAT_write", value );
    rrmp_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_RRMP_REG_INT_STAT,
                    value);
}

static INLINE void rrmp_reg_INT_STAT_field_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_reg_INT_STAT_field_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "rrmp_reg_INT_STAT_field_set", A, mask, ofs, value );
    rrmp_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_RRMP_REG_INT_STAT,
                    mask,
                    PMC_RRMP_REG_INT_STAT_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rrmp_reg_INT_STAT_read( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_INT_STAT_read( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_INT_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_RX_APS_read( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_RX_APS_read( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_RX_APS);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_RX_APS_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_SECTION_BIP_ERR_CNT_read( rrmp_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_SECTION_BIP_ERR_CNT_read( rrmp_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_SECTION_BIP_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_SECTION_BIP_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_LINE_BIP_ERR_CNT_LSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_LINE_BIP_ERR_CNT_LSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_LINE_BIP_ERR_CNT_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_LINE_BIP_ERR_CNT_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_LINE_BIP_ERR_CNT_MSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_LINE_BIP_ERR_CNT_MSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_LINE_BIP_ERR_CNT_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_LINE_BIP_ERR_CNT_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_LINE_REI_ERR_CNT_LSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_LINE_REI_ERR_CNT_LSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_LINE_REI_ERR_CNT_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_LINE_REI_ERR_CNT_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_LINE_REI_ERR_CNT_MSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_LINE_REI_ERR_CNT_MSB_read( rrmp_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_LINE_REI_ERR_CNT_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_LINE_REI_ERR_CNT_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 rrmp_reg_RXD_J0_BYTE_read( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_reg_RXD_J0_BYTE_read( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = rrmp_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_RRMP_REG_RXD_J0_BYTE);

    IOLOG( "%s -> 0x%08x; A=%d", "rrmp_reg_RXD_J0_BYTE_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void rrmp_field_EXTAPSDIS_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_EXTAPSDIS_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_EXTAPSDIS_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_EXTAPSDIS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_EXTAPSDIS_set", A, value );

    /* (0x00000100 bits 14) field EXTAPSDIS of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_EXTAPSDIS_MSK,
                            RRMP_REG_CFG_BIT_EXTAPSDIS_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_EXTAPSDIS_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_EXTAPSDIS_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_EXTAPSDIS_get", A, 15);
    /* (0x00000100 bits 14) field EXTAPSDIS of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_EXTAPSDIS_MSK) >> RRMP_REG_CFG_BIT_EXTAPSDIS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_EXTAPSDIS_get", A, value );

    return value;
}
static INLINE void rrmp_field_LREIACCBLK_set( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LREIACCBLK_set( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIACCBLK_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LREIACCBLK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LREIACCBLK_set", A, value );

    /* (0x00000100 bits 12) field LREIACCBLK of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LREIACCBLK_MSK,
                            RRMP_REG_CFG_BIT_LREIACCBLK_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LREIACCBLK_get( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LREIACCBLK_get( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIACCBLK_get", A, 15);
    /* (0x00000100 bits 12) field LREIACCBLK of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LREIACCBLK_MSK) >> RRMP_REG_CFG_BIT_LREIACCBLK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LREIACCBLK_get", A, value );

    return value;
}
static INLINE void rrmp_field_LBIPEREIBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LBIPEREIBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEREIBLK_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LBIPEREIBLK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LBIPEREIBLK_set", A, value );

    /* (0x00000100 bits 11) field LBIPEREIBLK of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LBIPEREIBLK_MSK,
                            RRMP_REG_CFG_BIT_LBIPEREIBLK_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LBIPEREIBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPEREIBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEREIBLK_get", A, 15);
    /* (0x00000100 bits 11) field LBIPEREIBLK of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LBIPEREIBLK_MSK) >> RRMP_REG_CFG_BIT_LBIPEREIBLK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPEREIBLK_get", A, value );

    return value;
}
static INLINE void rrmp_field_LBIPEACCBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LBIPEACCBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEACCBLK_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LBIPEACCBLK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LBIPEACCBLK_set", A, value );

    /* (0x00000100 bits 9) field LBIPEACCBLK of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LBIPEACCBLK_MSK,
                            RRMP_REG_CFG_BIT_LBIPEACCBLK_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LBIPEACCBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPEACCBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEACCBLK_get", A, 15);
    /* (0x00000100 bits 9) field LBIPEACCBLK of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LBIPEACCBLK_MSK) >> RRMP_REG_CFG_BIT_LBIPEACCBLK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPEACCBLK_get", A, value );

    return value;
}
static INLINE void rrmp_field_SBIPEACCBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_SBIPEACCBLK_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPEACCBLK_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_SBIPEACCBLK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_SBIPEACCBLK_set", A, value );

    /* (0x00000100 bits 7) field SBIPEACCBLK of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_SBIPEACCBLK_MSK,
                            RRMP_REG_CFG_BIT_SBIPEACCBLK_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_SBIPEACCBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_SBIPEACCBLK_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPEACCBLK_get", A, 15);
    /* (0x00000100 bits 7) field SBIPEACCBLK of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_SBIPEACCBLK_MSK) >> RRMP_REG_CFG_BIT_SBIPEACCBLK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_SBIPEACCBLK_get", A, value );

    return value;
}
static INLINE void rrmp_field_LBIPEREISAT_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LBIPEREISAT_set( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEREISAT_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LBIPEREISAT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LBIPEREISAT_set", A, value );

    /* (0x00000100 bits 6) field LBIPEREISAT of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LBIPEREISAT_MSK,
                            RRMP_REG_CFG_BIT_LBIPEREISAT_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LBIPEREISAT_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPEREISAT_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPEREISAT_get", A, 15);
    /* (0x00000100 bits 6) field LBIPEREISAT of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LBIPEREISAT_MSK) >> RRMP_REG_CFG_BIT_LBIPEREISAT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPEREISAT_get", A, value );

    return value;
}
static INLINE void rrmp_field_APSUNST_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_APSUNST_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSUNST_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_APSUNST_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_APSUNST_set", A, value );

    /* (0x00000100 bits 5:4) bits 0:1 use field APSUNST of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_APSUNST_MSK,
                            RRMP_REG_CFG_BIT_APSUNST_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_APSUNST_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_APSUNST_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSUNST_get", A, 15);
    /* (0x00000100 bits 5:4) bits 0:1 use field APSUNST of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_APSUNST_MSK) >> RRMP_REG_CFG_BIT_APSUNST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_APSUNST_get", A, value );

    return value;
}
static INLINE void rrmp_field_range_APSUNST_set( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_range_APSUNST_set( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_APSUNST_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_APSUNST_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_APSUNST_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_APSUNST_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000100 bits 5:4) bits 0:1 use field APSUNST of register PMC_RRMP_REG_CFG */
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
        /* (0x00000100 bits 5:4) bits 0:1 use field APSUNST of register PMC_RRMP_REG_CFG */
        rrmp_reg_CFG_field_set( b_ptr,
                                h_ptr,
                                A,
                                subfield_mask << (RRMP_REG_CFG_BIT_APSUNST_OFF + subfield_offset),
                                RRMP_REG_CFG_BIT_APSUNST_OFF + subfield_offset,
                                value >> subfield_shift);
    }
}

static INLINE UINT32 rrmp_field_range_APSUNST_get( rrmp_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_APSUNST_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_APSUNST_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_APSUNST_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_APSUNST_get", stop_bit, 1 );
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
    /* (0x00000100 bits 5:4) bits 0:1 use field APSUNST of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RRMP_REG_CFG_BIT_APSUNST_MSK)
                  >> RRMP_REG_CFG_BIT_APSUNST_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_CFG_BIT_APSUNST_MSK, RRMP_REG_CFG_BIT_APSUNST_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_APSUNST_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void rrmp_field_LRDI3_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LRDI3_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI3_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LRDI3_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LRDI3_set", A, value );

    /* (0x00000100 bits 3) field LRDI3 of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LRDI3_MSK,
                            RRMP_REG_CFG_BIT_LRDI3_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LRDI3_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LRDI3_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI3_get", A, 15);
    /* (0x00000100 bits 3) field LRDI3 of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LRDI3_MSK) >> RRMP_REG_CFG_BIT_LRDI3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LRDI3_get", A, value );

    return value;
}
static INLINE void rrmp_field_LAIS3_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LAIS3_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS3_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LAIS3_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LAIS3_set", A, value );

    /* (0x00000100 bits 2) field LAIS3 of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_LAIS3_MSK,
                            RRMP_REG_CFG_BIT_LAIS3_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_LAIS3_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LAIS3_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS3_get", A, 15);
    /* (0x00000100 bits 2) field LAIS3 of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_LAIS3_MSK) >> RRMP_REG_CFG_BIT_LAIS3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LAIS3_get", A, value );

    return value;
}
static INLINE void rrmp_field_ALGO2_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_ALGO2_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_ALGO2_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_ALGO2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_ALGO2_set", A, value );

    /* (0x00000100 bits 1) field ALGO2 of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_ALGO2_MSK,
                            RRMP_REG_CFG_BIT_ALGO2_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_ALGO2_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_ALGO2_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_ALGO2_get", A, 15);
    /* (0x00000100 bits 1) field ALGO2 of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_ALGO2_MSK) >> RRMP_REG_CFG_BIT_ALGO2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_ALGO2_get", A, value );

    return value;
}
static INLINE void rrmp_field_FOOF_set( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_FOOF_set( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FOOF_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_FOOF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_FOOF_set", A, value );

    /* (0x00000100 bits 0) field FOOF of register PMC_RRMP_REG_CFG */
    rrmp_reg_CFG_field_set( b_ptr,
                            h_ptr,
                            A,
                            RRMP_REG_CFG_BIT_FOOF_MSK,
                            RRMP_REG_CFG_BIT_FOOF_OFF,
                            value);
}

static INLINE UINT32 rrmp_field_FOOF_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_FOOF_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FOOF_get", A, 15);
    /* (0x00000100 bits 0) field FOOF of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_FOOF_MSK) >> RRMP_REG_CFG_BIT_FOOF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_FOOF_get", A, value );

    return value;
}
static INLINE void rrmp_field_DESCRAMA1A2SEL_set( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_DESCRAMA1A2SEL_set( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_DESCRAMA1A2SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_DESCRAMA1A2SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_DESCRAMA1A2SEL_set", A, value );

    /* (0x00000104 bits 15) field DESCRAMA1A2SEL of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_MSK,
                              RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_DESCRAMA1A2SEL_get( rrmp_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_DESCRAMA1A2SEL_get( rrmp_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_DESCRAMA1A2SEL_get", A, 15);
    /* (0x00000104 bits 15) field DESCRAMA1A2SEL of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_MSK) >> RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_DESCRAMA1A2SEL_get", A, value );

    return value;
}
static INLINE void rrmp_field_DESCRAMJ0SEL_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_DESCRAMJ0SEL_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_DESCRAMJ0SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_DESCRAMJ0SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_DESCRAMJ0SEL_set", A, value );

    /* (0x00000104 bits 14) field DESCRAMJ0SEL of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_MSK,
                              RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_DESCRAMJ0SEL_get( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_DESCRAMJ0SEL_get( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_DESCRAMJ0SEL_get", A, 15);
    /* (0x00000104 bits 14) field DESCRAMJ0SEL of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_MSK) >> RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_DESCRAMJ0SEL_get", A, value );

    return value;
}
static INLINE void rrmp_field_FRM768SEL_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_FRM768SEL_set( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FRM768SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_FRM768SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_FRM768SEL_set", A, value );

    /* (0x00000104 bits 13) field FRM768SEL of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_FRM768SEL_MSK,
                              RRMP_REG_CFG_2_BIT_FRM768SEL_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_FRM768SEL_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_FRM768SEL_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FRM768SEL_get", A, 15);
    /* (0x00000104 bits 13) field FRM768SEL of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_FRM768SEL_MSK) >> RRMP_REG_CFG_2_BIT_FRM768SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_FRM768SEL_get", A, value );

    return value;
}
static INLINE void rrmp_field_THREE_FRMSSM_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_THREE_FRMSSM_set( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_THREE_FRMSSM_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_THREE_FRMSSM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_THREE_FRMSSM_set", A, value );

    /* (0x00000104 bits 12) field THREE_FRMSSM of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_THREE_FRMSSM_MSK,
                              RRMP_REG_CFG_2_BIT_THREE_FRMSSM_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_THREE_FRMSSM_get( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_THREE_FRMSSM_get( rrmp_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_THREE_FRMSSM_get", A, 15);
    /* (0x00000104 bits 12) field THREE_FRMSSM of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_THREE_FRMSSM_MSK) >> RRMP_REG_CFG_2_BIT_THREE_FRMSSM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_THREE_FRMSSM_get", A, value );

    return value;
}
static INLINE void rrmp_field_LOSSPEN_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LOSSPEN_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOSSPEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LOSSPEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LOSSPEN_set", A, value );

    /* (0x00000104 bits 5) field LOSSPEN of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_LOSSPEN_MSK,
                              RRMP_REG_CFG_2_BIT_LOSSPEN_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_LOSSPEN_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOSSPEN_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOSSPEN_get", A, 15);
    /* (0x00000104 bits 5) field LOSSPEN of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_LOSSPEN_MSK) >> RRMP_REG_CFG_2_BIT_LOSSPEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOSSPEN_get", A, value );

    return value;
}
static INLINE void rrmp_field_B2MASKEN_set( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_B2MASKEN_set( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_B2MASKEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_B2MASKEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_B2MASKEN_set", A, value );

    /* (0x00000104 bits 3) field B2MASKEN of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_B2MASKEN_MSK,
                              RRMP_REG_CFG_2_BIT_B2MASKEN_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_B2MASKEN_get( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_B2MASKEN_get( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_B2MASKEN_get", A, 15);
    /* (0x00000104 bits 3) field B2MASKEN of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_B2MASKEN_MSK) >> RRMP_REG_CFG_2_BIT_B2MASKEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_B2MASKEN_get", A, value );

    return value;
}
static INLINE void rrmp_field_B1MASKEN_set( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_B1MASKEN_set( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_B1MASKEN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_B1MASKEN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_B1MASKEN_set", A, value );

    /* (0x00000104 bits 1) field B1MASKEN of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_B1MASKEN_MSK,
                              RRMP_REG_CFG_2_BIT_B1MASKEN_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_B1MASKEN_get( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_B1MASKEN_get( rrmp_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_B1MASKEN_get", A, 15);
    /* (0x00000104 bits 1) field B1MASKEN of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_B1MASKEN_MSK) >> RRMP_REG_CFG_2_BIT_B1MASKEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_B1MASKEN_get", A, value );

    return value;
}
static INLINE void rrmp_field_M0EN_set( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_M0EN_set( rrmp_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_M0EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_M0EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_M0EN_set", A, value );

    /* (0x00000104 bits 0) field M0EN of register PMC_RRMP_REG_CFG_2 */
    rrmp_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_2_BIT_M0EN_MSK,
                              RRMP_REG_CFG_2_BIT_M0EN_OFF,
                              value);
}

static INLINE UINT32 rrmp_field_M0EN_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_M0EN_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_M0EN_get", A, 15);
    /* (0x00000104 bits 0) field M0EN of register PMC_RRMP_REG_CFG_2 */
    reg_value = rrmp_reg_CFG_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_2_BIT_M0EN_MSK) >> RRMP_REG_CFG_2_BIT_M0EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_M0EN_get", A, value );

    return value;
}
static INLINE void rrmp_field_COK2_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COK2_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COK2_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COK2_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COK2_E_set", A, value );

    /* (0x0000010c bits 11) field COK2_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_COK2_E_MSK,
                               RRMP_REG_INT_EN_BIT_COK2_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_COK2_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COK2_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COK2_E_get", A, 15);
    /* (0x0000010c bits 11) field COK2_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_COK2_E_MSK) >> RRMP_REG_INT_EN_BIT_COK2_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COK2_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LREIE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LREIE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIE_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LREIE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LREIE_E_set", A, value );

    /* (0x0000010c bits 10) field LREIE_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LREIE_E_MSK,
                               RRMP_REG_INT_EN_BIT_LREIE_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LREIE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LREIE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIE_E_get", A, 15);
    /* (0x0000010c bits 10) field LREIE_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LREIE_E_MSK) >> RRMP_REG_INT_EN_BIT_LREIE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LREIE_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LBIPE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LBIPE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPE_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LBIPE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LBIPE_E_set", A, value );

    /* (0x0000010c bits 9) field LBIPE_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LBIPE_E_MSK,
                               RRMP_REG_INT_EN_BIT_LBIPE_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LBIPE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPE_E_get", A, 15);
    /* (0x0000010c bits 9) field LBIPE_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LBIPE_E_MSK) >> RRMP_REG_INT_EN_BIT_LBIPE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPE_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_SBIPE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_SBIPE_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPE_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_SBIPE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_SBIPE_E_set", A, value );

    /* (0x0000010c bits 8) field SBIPE_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_SBIPE_E_MSK,
                               RRMP_REG_INT_EN_BIT_SBIPE_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_SBIPE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_SBIPE_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPE_E_get", A, 15);
    /* (0x0000010c bits 8) field SBIPE_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_SBIPE_E_MSK) >> RRMP_REG_INT_EN_BIT_SBIPE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_SBIPE_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_COSSM_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COSSM_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COSSM_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COSSM_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COSSM_E_set", A, value );

    /* (0x0000010c bits 7) field COSSM_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_COSSM_E_MSK,
                               RRMP_REG_INT_EN_BIT_COSSM_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_COSSM_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COSSM_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COSSM_E_get", A, 15);
    /* (0x0000010c bits 7) field COSSM_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_COSSM_E_MSK) >> RRMP_REG_INT_EN_BIT_COSSM_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COSSM_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_COAPS_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COAPS_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COAPS_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COAPS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COAPS_E_set", A, value );

    /* (0x0000010c bits 6) field COAPS_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_COAPS_E_MSK,
                               RRMP_REG_INT_EN_BIT_COAPS_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_COAPS_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COAPS_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COAPS_E_get", A, 15);
    /* (0x0000010c bits 6) field COAPS_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_COAPS_E_MSK) >> RRMP_REG_INT_EN_BIT_COAPS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COAPS_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_APSBF_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_APSBF_E_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSBF_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_APSBF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_APSBF_E_set", A, value );

    /* (0x0000010c bits 5) field APSBF_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_APSBF_E_MSK,
                               RRMP_REG_INT_EN_BIT_APSBF_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_APSBF_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_APSBF_E_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSBF_E_get", A, 15);
    /* (0x0000010c bits 5) field APSBF_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_APSBF_E_MSK) >> RRMP_REG_INT_EN_BIT_APSBF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_APSBF_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LRDI_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LRDI_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LRDI_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LRDI_E_set", A, value );

    /* (0x0000010c bits 4) field LRDI_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LRDI_E_MSK,
                               RRMP_REG_INT_EN_BIT_LRDI_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LRDI_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LRDI_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI_E_get", A, 15);
    /* (0x0000010c bits 4) field LRDI_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LRDI_E_MSK) >> RRMP_REG_INT_EN_BIT_LRDI_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LRDI_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LAIS_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LAIS_E_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LAIS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LAIS_E_set", A, value );

    /* (0x0000010c bits 3) field LAIS_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LAIS_E_MSK,
                               RRMP_REG_INT_EN_BIT_LAIS_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LAIS_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LAIS_E_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS_E_get", A, 15);
    /* (0x0000010c bits 3) field LAIS_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LAIS_E_MSK) >> RRMP_REG_INT_EN_BIT_LAIS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LAIS_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LOS_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LOS_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOS_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LOS_E_set", A, value );

    /* (0x0000010c bits 2) field LOS_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LOS_E_MSK,
                               RRMP_REG_INT_EN_BIT_LOS_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LOS_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOS_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOS_E_get", A, 15);
    /* (0x0000010c bits 2) field LOS_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LOS_E_MSK) >> RRMP_REG_INT_EN_BIT_LOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOS_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_LOF_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LOF_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOF_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LOF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LOF_E_set", A, value );

    /* (0x0000010c bits 1) field LOF_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_LOF_E_MSK,
                               RRMP_REG_INT_EN_BIT_LOF_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_LOF_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOF_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOF_E_get", A, 15);
    /* (0x0000010c bits 1) field LOF_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_LOF_E_MSK) >> RRMP_REG_INT_EN_BIT_LOF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOF_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_OOF_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_OOF_E_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_OOF_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_OOF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_OOF_E_set", A, value );

    /* (0x0000010c bits 0) field OOF_E of register PMC_RRMP_REG_INT_EN */
    rrmp_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               A,
                               RRMP_REG_INT_EN_BIT_OOF_E_MSK,
                               RRMP_REG_INT_EN_BIT_OOF_E_OFF,
                               value);
}

static INLINE UINT32 rrmp_field_OOF_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_OOF_E_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_OOF_E_get", A, 15);
    /* (0x0000010c bits 0) field OOF_E of register PMC_RRMP_REG_INT_EN */
    reg_value = rrmp_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_EN_BIT_OOF_E_MSK) >> RRMP_REG_INT_EN_BIT_OOF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_OOF_E_get", A, value );

    return value;
}
static INLINE void rrmp_field_BYTESSM_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_BYTESSM_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_BYTESSM_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_BYTESSM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_BYTESSM_set", A, value );

    /* (0x00000118 bits 15) field BYTESSM of register PMC_RRMP_REG_RXD_SSM */
    rrmp_reg_RXD_SSM_field_set( b_ptr,
                                h_ptr,
                                A,
                                RRMP_REG_RXD_SSM_BIT_BYTESSM_MSK,
                                RRMP_REG_RXD_SSM_BIT_BYTESSM_OFF,
                                value);
}

static INLINE UINT32 rrmp_field_BYTESSM_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_BYTESSM_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_BYTESSM_get", A, 15);
    /* (0x00000118 bits 15) field BYTESSM of register PMC_RRMP_REG_RXD_SSM */
    reg_value = rrmp_reg_RXD_SSM_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RXD_SSM_BIT_BYTESSM_MSK) >> RRMP_REG_RXD_SSM_BIT_BYTESSM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_BYTESSM_get", A, value );

    return value;
}
static INLINE void rrmp_field_FLTRSSM_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_FLTRSSM_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FLTRSSM_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_FLTRSSM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_FLTRSSM_set", A, value );

    /* (0x00000118 bits 14) field FLTRSSM of register PMC_RRMP_REG_RXD_SSM */
    rrmp_reg_RXD_SSM_field_set( b_ptr,
                                h_ptr,
                                A,
                                RRMP_REG_RXD_SSM_BIT_FLTRSSM_MSK,
                                RRMP_REG_RXD_SSM_BIT_FLTRSSM_OFF,
                                value);
}

static INLINE UINT32 rrmp_field_FLTRSSM_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_FLTRSSM_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_FLTRSSM_get", A, 15);
    /* (0x00000118 bits 14) field FLTRSSM of register PMC_RRMP_REG_RXD_SSM */
    reg_value = rrmp_reg_RXD_SSM_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RXD_SSM_BIT_FLTRSSM_MSK) >> RRMP_REG_RXD_SSM_BIT_FLTRSSM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_FLTRSSM_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 rrmp_field_TIP_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_TIP_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_TIP_get", A, 15);
    /* (0x00000100 bits 15) field TIP of register PMC_RRMP_REG_CFG */
    reg_value = rrmp_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_CFG_BIT_TIP_MSK) >> RRMP_REG_CFG_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rrmp_field_TIP_poll( rrmp_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rrmp_field_TIP_poll( rrmp_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "rrmp_field_TIP_poll", A, value );

    /* (0x00000100 bits 15) field TIP of register PMC_RRMP_REG_CFG */
    return rrmp_reg_CFG_poll( b_ptr,
                              h_ptr,
                              A,
                              RRMP_REG_CFG_BIT_TIP_MSK,
                              (value<<RRMP_REG_CFG_BIT_TIP_OFF),
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 rrmp_field_APSBF_V_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_APSBF_V_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSBF_V_get", A, 15);
    /* (0x00000108 bits 5) field APSBF_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_APSBF_V_MSK) >> RRMP_REG_STAT_BIT_APSBF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_APSBF_V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_LRDI_V_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LRDI_V_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI_V_get", A, 15);
    /* (0x00000108 bits 4) field LRDI_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_LRDI_V_MSK) >> RRMP_REG_STAT_BIT_LRDI_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LRDI_V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_LAIS_V_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LAIS_V_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS_V_get", A, 15);
    /* (0x00000108 bits 3) field LAIS_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_LAIS_V_MSK) >> RRMP_REG_STAT_BIT_LAIS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LAIS_V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_LOS_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOS_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOS_V_get", A, 15);
    /* (0x00000108 bits 2) field LOS_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_LOS_V_MSK) >> RRMP_REG_STAT_BIT_LOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOS_V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_LOF_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOF_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOF_V_get", A, 15);
    /* (0x00000108 bits 1) field LOF_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_LOF_V_MSK) >> RRMP_REG_STAT_BIT_LOF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOF_V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_OOF_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_OOF_V_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_OOF_V_get", A, 15);
    /* (0x00000108 bits 0) field OOF_V of register PMC_RRMP_REG_STAT */
    reg_value = rrmp_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_STAT_BIT_OOF_V_MSK) >> RRMP_REG_STAT_BIT_OOF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_OOF_V_get", A, value );

    return value;
}
static INLINE void rrmp_field_COK2_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COK2_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COK2_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COK2_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COK2_I_set", A, value );

    /* (0x00000110 bits 11) field COK2_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_COK2_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_COK2_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_COK2_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COK2_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COK2_I_get", A, 15);
    /* (0x00000110 bits 11) field COK2_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_COK2_I_MSK) >> RRMP_REG_INT_STAT_BIT_COK2_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COK2_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LREIE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LREIE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIE_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LREIE_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LREIE_I_set", A, value );

    /* (0x00000110 bits 10) field LREIE_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LREIE_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LREIE_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LREIE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LREIE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIE_I_get", A, 15);
    /* (0x00000110 bits 10) field LREIE_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LREIE_I_MSK) >> RRMP_REG_INT_STAT_BIT_LREIE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LREIE_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LBIPE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LBIPE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPE_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LBIPE_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LBIPE_I_set", A, value );

    /* (0x00000110 bits 9) field LBIPE_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LBIPE_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LBIPE_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LBIPE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPE_I_get", A, 15);
    /* (0x00000110 bits 9) field LBIPE_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LBIPE_I_MSK) >> RRMP_REG_INT_STAT_BIT_LBIPE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPE_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_SBIPE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_SBIPE_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPE_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_SBIPE_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_SBIPE_I_set", A, value );

    /* (0x00000110 bits 8) field SBIPE_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_SBIPE_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_SBIPE_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_SBIPE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_SBIPE_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPE_I_get", A, 15);
    /* (0x00000110 bits 8) field SBIPE_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_SBIPE_I_MSK) >> RRMP_REG_INT_STAT_BIT_SBIPE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_SBIPE_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_COSSM_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COSSM_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COSSM_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COSSM_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COSSM_I_set", A, value );

    /* (0x00000110 bits 7) field COSSM_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_COSSM_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_COSSM_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_COSSM_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COSSM_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COSSM_I_get", A, 15);
    /* (0x00000110 bits 7) field COSSM_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_COSSM_I_MSK) >> RRMP_REG_INT_STAT_BIT_COSSM_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COSSM_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_COAPS_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_COAPS_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COAPS_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_COAPS_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_COAPS_I_set", A, value );

    /* (0x00000110 bits 6) field COAPS_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_COAPS_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_COAPS_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_COAPS_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_COAPS_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_COAPS_I_get", A, 15);
    /* (0x00000110 bits 6) field COAPS_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_COAPS_I_MSK) >> RRMP_REG_INT_STAT_BIT_COAPS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_COAPS_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_APSBF_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_APSBF_I_set( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSBF_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_APSBF_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_APSBF_I_set", A, value );

    /* (0x00000110 bits 5) field APSBF_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_APSBF_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_APSBF_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_APSBF_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_APSBF_I_get( rrmp_buffer_t *b_ptr,
                                             sdh_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_APSBF_I_get", A, 15);
    /* (0x00000110 bits 5) field APSBF_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_APSBF_I_MSK) >> RRMP_REG_INT_STAT_BIT_APSBF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_APSBF_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LRDI_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LRDI_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LRDI_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LRDI_I_set", A, value );

    /* (0x00000110 bits 4) field LRDI_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LRDI_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LRDI_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LRDI_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LRDI_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LRDI_I_get", A, 15);
    /* (0x00000110 bits 4) field LRDI_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LRDI_I_MSK) >> RRMP_REG_INT_STAT_BIT_LRDI_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LRDI_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LAIS_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LAIS_I_set( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LAIS_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LAIS_I_set", A, value );

    /* (0x00000110 bits 3) field LAIS_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LAIS_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LAIS_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LAIS_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LAIS_I_get( rrmp_buffer_t *b_ptr,
                                            sdh_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LAIS_I_get", A, 15);
    /* (0x00000110 bits 3) field LAIS_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LAIS_I_MSK) >> RRMP_REG_INT_STAT_BIT_LAIS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LAIS_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LOS_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LOS_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOS_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LOS_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LOS_I_set", A, value );

    /* (0x00000110 bits 2) field LOS_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LOS_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LOS_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LOS_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOS_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOS_I_get", A, 15);
    /* (0x00000110 bits 2) field LOS_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LOS_I_MSK) >> RRMP_REG_INT_STAT_BIT_LOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOS_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_LOF_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_LOF_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOF_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_LOF_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_LOF_I_set", A, value );

    /* (0x00000110 bits 1) field LOF_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_LOF_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_LOF_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_LOF_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LOF_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LOF_I_get", A, 15);
    /* (0x00000110 bits 1) field LOF_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_LOF_I_MSK) >> RRMP_REG_INT_STAT_BIT_LOF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LOF_I_get", A, value );

    return value;
}
static INLINE void rrmp_field_OOF_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rrmp_field_OOF_I_set( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_OOF_I_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rrmp_field_OOF_I_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "rrmp_field_OOF_I_set", A, value );

    /* (0x00000110 bits 0) field OOF_I of register PMC_RRMP_REG_INT_STAT */
    rrmp_reg_INT_STAT_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 RRMP_REG_INT_STAT_BIT_OOF_I_MSK,
                                 RRMP_REG_INT_STAT_BIT_OOF_I_OFF,
                                 value);
}

static INLINE UINT32 rrmp_field_OOF_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_OOF_I_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_OOF_I_get", A, 15);
    /* (0x00000110 bits 0) field OOF_I of register PMC_RRMP_REG_INT_STAT */
    reg_value = rrmp_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_INT_STAT_BIT_OOF_I_MSK) >> RRMP_REG_INT_STAT_BIT_OOF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_OOF_I_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_K1V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_K1V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_K1V_get", A, 15);
    /* (0x00000114 bits 15:8) bits 0:7 use field K1V of register PMC_RRMP_REG_RX_APS */
    reg_value = rrmp_reg_RX_APS_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RX_APS_BIT_K1V_MSK) >> RRMP_REG_RX_APS_BIT_K1V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_K1V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_K1V_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_K1V_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_K1V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_K1V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_K1V_get", stop_bit, 7 );
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
    /* (0x00000114 bits 15:8) bits 0:7 use field K1V of register PMC_RRMP_REG_RX_APS */
    reg_value = rrmp_reg_RX_APS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RRMP_REG_RX_APS_BIT_K1V_MSK)
                  >> RRMP_REG_RX_APS_BIT_K1V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_RX_APS_BIT_K1V_MSK, RRMP_REG_RX_APS_BIT_K1V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_K1V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_K2V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_K2V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_K2V_get", A, 15);
    /* (0x00000114 bits 7:0) bits 0:7 use field K2V of register PMC_RRMP_REG_RX_APS */
    reg_value = rrmp_reg_RX_APS_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RX_APS_BIT_K2V_MSK) >> RRMP_REG_RX_APS_BIT_K2V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_K2V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_K2V_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_K2V_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_K2V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_K2V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_K2V_get", stop_bit, 7 );
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
    /* (0x00000114 bits 7:0) bits 0:7 use field K2V of register PMC_RRMP_REG_RX_APS */
    reg_value = rrmp_reg_RX_APS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RRMP_REG_RX_APS_BIT_K2V_MSK)
                  >> RRMP_REG_RX_APS_BIT_K2V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_RX_APS_BIT_K2V_MSK, RRMP_REG_RX_APS_BIT_K2V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_K2V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_SSMV_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_SSMV_get( rrmp_buffer_t *b_ptr,
                                          sdh_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SSMV_get", A, 15);
    /* (0x00000118 bits 7:0) bits 0:7 use field SSMV of register PMC_RRMP_REG_RXD_SSM */
    reg_value = rrmp_reg_RXD_SSM_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RXD_SSM_BIT_SSMV_MSK) >> RRMP_REG_RXD_SSM_BIT_SSMV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_SSMV_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_SSMV_get( rrmp_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_SSMV_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_SSMV_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_SSMV_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_SSMV_get", stop_bit, 7 );
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
    /* (0x00000118 bits 7:0) bits 0:7 use field SSMV of register PMC_RRMP_REG_RXD_SSM */
    reg_value = rrmp_reg_RXD_SSM_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RRMP_REG_RXD_SSM_BIT_SSMV_MSK)
                  >> RRMP_REG_RXD_SSM_BIT_SSMV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_RXD_SSM_BIT_SSMV_MSK, RRMP_REG_RXD_SSM_BIT_SSMV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_SSMV_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_SBIPE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_SBIPE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_SBIPE_get", A, 15);
    /* (0x00000120 bits 15:0) bits 0:15 use field SBIPE of register PMC_RRMP_REG_SECTION_BIP_ERR_CNT */
    reg_value = rrmp_reg_SECTION_BIP_ERR_CNT_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_MSK) >> RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_SBIPE_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_SBIPE_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_SBIPE_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_SBIPE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_SBIPE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_SBIPE_get", stop_bit, 15 );
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
    /* (0x00000120 bits 15:0) bits 0:15 use field SBIPE of register PMC_RRMP_REG_SECTION_BIP_ERR_CNT */
    reg_value = rrmp_reg_SECTION_BIP_ERR_CNT_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_MSK)
                  >> RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_MSK, RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_SBIPE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_LBIPE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LBIPE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LBIPE_get", A, 15);
    /* (0x00000124 bits 15:0) bits 0:15 use field LBIPE of register PMC_RRMP_REG_LINE_BIP_ERR_CNT_LSB */
    reg_value = rrmp_reg_LINE_BIP_ERR_CNT_LSB_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_MSK) >> RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_OFF;
    value |= field_value;

    /* (0x00000128 bits 9:0) bits 16:25 use field LBIPE of register PMC_RRMP_REG_LINE_BIP_ERR_CNT_MSB */
    reg_value = rrmp_reg_LINE_BIP_ERR_CNT_MSB_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_MSK) >> RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_OFF;
    value |= field_value;

    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LBIPE_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_LBIPE_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_LBIPE_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_LBIPE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_LBIPE_get", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_LBIPE_get", stop_bit, 25 );
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
    /* (0x00000124 bits 15:0) bits 0:15 use field LBIPE of register PMC_RRMP_REG_LINE_BIP_ERR_CNT_LSB */
        reg_value = rrmp_reg_LINE_BIP_ERR_CNT_LSB_read( b_ptr,
                                                        h_ptr,
                                                        A);
        field_value = (reg_value & RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_MSK)
                      >> RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_MSK, RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 25) && (stop_bit >= 16)) {
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
        if (stop_bit < 25) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 25;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000128 bits 9:0) bits 16:25 use field LBIPE of register PMC_RRMP_REG_LINE_BIP_ERR_CNT_MSB */
        reg_value = rrmp_reg_LINE_BIP_ERR_CNT_MSB_read( b_ptr,
                                                        h_ptr,
                                                        A);
        field_value = (reg_value & RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_MSK)
                      >> RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_MSK, RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_LBIPE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_LREIE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_LREIE_get( rrmp_buffer_t *b_ptr,
                                           sdh_pmg_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_LREIE_get", A, 15);
    /* (0x0000012c bits 15:0) bits 0:15 use field LREIE of register PMC_RRMP_REG_LINE_REI_ERR_CNT_LSB */
    reg_value = rrmp_reg_LINE_REI_ERR_CNT_LSB_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_MSK) >> RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_OFF;
    value |= field_value;

    /* (0x00000130 bits 9:0) bits 16:25 use field LREIE of register PMC_RRMP_REG_LINE_REI_ERR_CNT_MSB */
    reg_value = rrmp_reg_LINE_REI_ERR_CNT_MSB_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_MSK) >> RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_OFF;
    value |= field_value;

    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_LREIE_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_LREIE_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_LREIE_get( rrmp_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_LREIE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_LREIE_get", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_LREIE_get", stop_bit, 25 );
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
    /* (0x0000012c bits 15:0) bits 0:15 use field LREIE of register PMC_RRMP_REG_LINE_REI_ERR_CNT_LSB */
        reg_value = rrmp_reg_LINE_REI_ERR_CNT_LSB_read( b_ptr,
                                                        h_ptr,
                                                        A);
        field_value = (reg_value & RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_MSK)
                      >> RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_MSK, RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 25) && (stop_bit >= 16)) {
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
        if (stop_bit < 25) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 25;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000130 bits 9:0) bits 16:25 use field LREIE of register PMC_RRMP_REG_LINE_REI_ERR_CNT_MSB */
        reg_value = rrmp_reg_LINE_REI_ERR_CNT_MSB_read( b_ptr,
                                                        h_ptr,
                                                        A);
        field_value = (reg_value & RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_MSK)
                      >> RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_MSK, RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_LREIE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rrmp_field_J0V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_J0V_get( rrmp_buffer_t *b_ptr,
                                         sdh_pmg_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_J0V_get", A, 15);
    /* (0x00000134 bits 7:0) bits 0:7 use field J0V of register PMC_RRMP_REG_RXD_J0_BYTE */
    reg_value = rrmp_reg_RXD_J0_BYTE_read(  b_ptr, h_ptr, A);
    value = (reg_value & RRMP_REG_RXD_J0_BYTE_BIT_J0V_MSK) >> RRMP_REG_RXD_J0_BYTE_BIT_J0V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "rrmp_field_J0V_get", A, value );

    return value;
}
static INLINE UINT32 rrmp_field_range_J0V_get( rrmp_buffer_t *b_ptr,
                                               sdh_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rrmp_field_range_J0V_get( rrmp_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "rrmp_field_range_J0V_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rrmp_field_range_J0V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rrmp_field_range_J0V_get", stop_bit, 7 );
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
    /* (0x00000134 bits 7:0) bits 0:7 use field J0V of register PMC_RRMP_REG_RXD_J0_BYTE */
    reg_value = rrmp_reg_RXD_J0_BYTE_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & RRMP_REG_RXD_J0_BYTE_BIT_J0V_MSK)
                  >> RRMP_REG_RXD_J0_BYTE_BIT_J0V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RRMP_REG_RXD_J0_BYTE_BIT_J0V_MSK, RRMP_REG_RXD_J0_BYTE_BIT_J0V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "rrmp_field_range_J0V_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RRMP_IO_INLINE_H */
