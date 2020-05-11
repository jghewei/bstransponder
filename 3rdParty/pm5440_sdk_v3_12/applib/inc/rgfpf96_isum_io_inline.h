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
 *     and register accessor functions for the rgfpf96_isum block
 *****************************************************************************/
#ifndef _RGFPF96_ISUM_IO_INLINE_H
#define _RGFPF96_ISUM_IO_INLINE_H

#include "mapotn_loc.h"
#include "rgfpf96_isum_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RGFPF96_ISUM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rgfpf96_isum
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
    pmc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} rgfpf96_isum_buffer_t;
static INLINE void rgfpf96_isum_buffer_init( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_buffer_init( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "rgfpf96_isum_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rgfpf96_isum_buffer_flush( rgfpf96_isum_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_buffer_flush( rgfpf96_isum_buffer_t *b_ptr )
{
    IOLOG( "rgfpf96_isum_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rgfpf96_isum_reg_read( rgfpf96_isum_buffer_t *b_ptr,
                                            pmc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_read( rgfpf96_isum_buffer_t *b_ptr,
                                            pmc_handle_t *h_ptr,
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
static INLINE void rgfpf96_isum_reg_write( rgfpf96_isum_buffer_t *b_ptr,
                                           pmc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_reg_write( rgfpf96_isum_buffer_t *b_ptr,
                                           pmc_handle_t *h_ptr,
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

static INLINE void rgfpf96_isum_field_set( rgfpf96_isum_buffer_t *b_ptr,
                                           pmc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_field_set( rgfpf96_isum_buffer_t *b_ptr,
                                           pmc_handle_t *h_ptr,
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

static INLINE void rgfpf96_isum_action_on_write_field_set( rgfpf96_isum_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_action_on_write_field_set( rgfpf96_isum_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

static INLINE void rgfpf96_isum_burst_read( rgfpf96_isum_buffer_t *b_ptr,
                                            pmc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_burst_read( rgfpf96_isum_buffer_t *b_ptr,
                                            pmc_handle_t *h_ptr,
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

static INLINE void rgfpf96_isum_burst_write( rgfpf96_isum_buffer_t *b_ptr,
                                             pmc_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgfpf96_isum_burst_write( rgfpf96_isum_buffer_t *b_ptr,
                                             pmc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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
 *  register access functions for rgfpf96_isum
 * ==================================================================================
 */

static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_0_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_0_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rgfpf96_isum_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0);

    IOLOG( "%s -> 0x%08x;", "rgfpf96_isum_reg_INT_SUMMARY_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgfpf96_isum_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_1_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_1_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rgfpf96_isum_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1);

    IOLOG( "%s -> 0x%08x;", "rgfpf96_isum_reg_INT_SUMMARY_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgfpf96_isum_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_2_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_2_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rgfpf96_isum_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2);

    IOLOG( "%s -> 0x%08x;", "rgfpf96_isum_reg_INT_SUMMARY_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgfpf96_isum_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_3_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_3_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rgfpf96_isum_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3);

    IOLOG( "%s -> 0x%08x;", "rgfpf96_isum_reg_INT_SUMMARY_REG_3_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgfpf96_isum_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_4_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_reg_INT_SUMMARY_REG_4_read( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rgfpf96_isum_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4);

    IOLOG( "%s -> 0x%08x;", "rgfpf96_isum_reg_INT_SUMMARY_REG_4_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgfpf96_isum_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_0_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_0_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 31:0) field INT_SUMMARY_0 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_0_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK) >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF;
    IOLOG( "%s -> 0x%08x", "rgfpf96_isum_field_INT_SUMMARY_0_get", value );

    return value;
}
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_0_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_0_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_0_get", stop_bit, 31 );
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
    /* (0x00000000 bits 31:0) field INT_SUMMARY_0 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK)
                  >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK, RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_0_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_0_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_0_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 31:0) field INT_SUMMARY_0 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000000 bits 31:0) field INT_SUMMARY_0 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 */
        return rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF + subfield_offset),
                                                        value << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_0_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "rgfpf96_isum_field_INT_SUMMARY_0_poll", value );

    /* (0x00000000 bits 31:0) field INT_SUMMARY_0 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 */
    return rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll( b_ptr,
                                                    h_ptr,
                                                    RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK,
                                                    (value<<RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_1_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_1_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 31:0) field INT_SUMMARY_1 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK) >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF;
    IOLOG( "%s -> 0x%08x", "rgfpf96_isum_field_INT_SUMMARY_1_get", value );

    return value;
}
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_1_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_1_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_1_get", stop_bit, 31 );
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
    /* (0x00000004 bits 31:0) field INT_SUMMARY_1 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK)
                  >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK, RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_1_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_1_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_1_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 31:0) field INT_SUMMARY_1 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000004 bits 31:0) field INT_SUMMARY_1 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 */
        return rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF + subfield_offset),
                                                        value << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_1_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "rgfpf96_isum_field_INT_SUMMARY_1_poll", value );

    /* (0x00000004 bits 31:0) field INT_SUMMARY_1 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 */
    return rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll( b_ptr,
                                                    h_ptr,
                                                    RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK,
                                                    (value<<RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_2_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_2_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 31:0) field INT_SUMMARY_2 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_MSK) >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF;
    IOLOG( "%s -> 0x%08x", "rgfpf96_isum_field_INT_SUMMARY_2_get", value );

    return value;
}
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_2_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_2_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_2_get", stop_bit, 31 );
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
    /* (0x00000008 bits 31:0) field INT_SUMMARY_2 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_MSK)
                  >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_MSK, RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_2_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_2_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_2_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 31:0) field INT_SUMMARY_2 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000008 bits 31:0) field INT_SUMMARY_2 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 */
        return rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF + subfield_offset),
                                                        value << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_2_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "rgfpf96_isum_field_INT_SUMMARY_2_poll", value );

    /* (0x00000008 bits 31:0) field INT_SUMMARY_2 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 */
    return rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll( b_ptr,
                                                    h_ptr,
                                                    RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_MSK,
                                                    (value<<RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_3_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_3_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 31:0) field INT_SUMMARY_3 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_3_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_MSK) >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF;
    IOLOG( "%s -> 0x%08x", "rgfpf96_isum_field_INT_SUMMARY_3_get", value );

    return value;
}
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_3_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_3_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_3_get", stop_bit, 31 );
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
    /* (0x0000000c bits 31:0) field INT_SUMMARY_3 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_3_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_MSK)
                  >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_MSK, RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_3_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_3_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_3_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 31:0) field INT_SUMMARY_3 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000000c bits 31:0) field INT_SUMMARY_3 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 */
        return rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF + subfield_offset),
                                                        value << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_3_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "rgfpf96_isum_field_INT_SUMMARY_3_poll", value );

    /* (0x0000000c bits 31:0) field INT_SUMMARY_3 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 */
    return rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll( b_ptr,
                                                    h_ptr,
                                                    RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_MSK,
                                                    (value<<RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_4_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_INT_SUMMARY_4_get( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 14:0) field INT_SUMMARY_4 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_4_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_MSK) >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF;
    IOLOG( "%s -> 0x%08x", "rgfpf96_isum_field_INT_SUMMARY_4_get", value );

    return value;
}
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_4_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgfpf96_isum_field_range_INT_SUMMARY_4_get( rgfpf96_isum_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_4_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_4_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000010 bits 14:0) field INT_SUMMARY_4 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 */
    reg_value = rgfpf96_isum_reg_INT_SUMMARY_REG_4_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_MSK)
                  >> RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_MSK, RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rgfpf96_isum_field_range_INT_SUMMARY_4_poll", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rgfpf96_isum_field_range_INT_SUMMARY_4_poll", stop_bit, 14 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rgfpf96_isum_field_range_INT_SUMMARY_4_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 14:0) field INT_SUMMARY_4 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000010 bits 14:0) field INT_SUMMARY_4 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 */
        return rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF + subfield_offset),
                                                        value << (RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_4_poll( rgfpf96_isum_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "rgfpf96_isum_field_INT_SUMMARY_4_poll", value );

    /* (0x00000010 bits 14:0) field INT_SUMMARY_4 of register PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 */
    return rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll( b_ptr,
                                                    h_ptr,
                                                    RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_MSK,
                                                    (value<<RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RGFPF96_ISUM_IO_INLINE_H */
